/*
	VSSINFO
	$Header: /tools/kjmtools/difffolder/difffolder.cpp 4     07/08/07 11:36 Kojima $
*/
/*
	フォルダを比較するプログラム

	フォルダを再帰的に比較して、以下の記号で結果を出力する。

	L  左側に指定したフォルダにしか、ないファイル
	R  右側に指定したフォルダにしか、ないファイル
	DS サイズが不一致なファイル
	DC サイズは一致するが、内容が不一致なファイル
*/

// 抑止「'identifier' : デバッグ情報で識別子が 'number' 文字に切り捨てられました。」
#pragma warning(disable: 4786)

#include <windows.h>
#include <shlwapi.h>
#include <iostream>
#include <string>
#include <map>
#include <utility>
#include <locale>
#include <vector>
#include <fstream>
#include "resource.h"

#pragma comment(lib, "shlwapi.lib")

using namespace std;


struct file_info {
	string m_path;
	WIN32_FIND_DATA m_data;

	file_info() { m_path = ""; memset(&m_data, 0, sizeof(m_data)); };
	file_info(const char* path, const WIN32_FIND_DATA& findData) : m_path(path) { m_data = findData; };
};


//---------------------------------------------------------------------
// 文字列を小文字化する
//---------------------------------------------------------------------
string& to_lower(string& str) {
	vector<char> work(str.begin(), str.end());
	work.push_back('\0');
	CharLower(&work[0]);
	str = string(&work[0]);

	return str;
}


//---------------------------------------------------------------------
// ファイルの比較
//---------------------------------------------------------------------
bool compare_file(const char* f1, const char* f2) {
	
	FILE* fp1 = fopen(f1, "rb");
	FILE* fp2 = fopen(f2, "rb");

	if (fp1 == NULL) {
		cerr << "'" << f1 << "' が、オープンできません。" << endl;
		cerr << errno << endl;
		return false;
	}

	if (fp2 == NULL) {
		cerr << "'" << f2 << "' が、オープンできません。" << endl;
		cerr << errno << endl;
		fclose(fp1);
		return false;
	}

	vector<char> buf1(8192), buf2(8192);
	
	while (!feof(fp1) && !feof(fp2)) {
		size_t r1 = fread(&buf1[0], sizeof(char), buf1.size(), fp1);
		size_t r2 = fread(&buf2[0], sizeof(char), buf2.size(), fp2);

		if (r1 != r2) {
			// サイズが同じはずなのに、読み込んだ量が違う
			fclose(fp1);
			fclose(fp2);
			return false;
		}

		if (memcmp(&buf1[0], &buf2[0], r1) != 0) {
			fclose(fp1);
			fclose(fp2);
			return false;
		}
	}

	fclose(fp1);
	fclose(fp2);

	return true;
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
void base_listing(const char* base_path, const char* find_path, map<string, pair<file_info, file_info> >& file_pair) {

	// base_pathをリストアップして、file_pairに追加
	char wc[MAX_PATH];
	strcpy(wc, find_path);
	PathAppend(wc, "*.*");

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(wc, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0) {
					char full_name[MAX_PATH];
					strcpy(full_name, find_path);
					PathAppend(full_name, data.cFileName);
					PathAddBackslash(full_name);

					file_pair[to_lower( string(&full_name[strlen(base_path)]) )] = pair<file_info, file_info>(file_info(full_name, data), file_info());

					base_listing(base_path, full_name, file_pair);
				}
			} else {
				char full_name[MAX_PATH];
				strcpy(full_name, find_path);
				PathAppend(full_name, data.cFileName);

				file_pair[to_lower( string(&full_name[strlen(base_path)]) )] = pair<file_info, file_info>(file_info(full_name, data), file_info());
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);
	}
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
void pair_listing(const char* base_path, const char* find_path, map<string, pair<file_info, file_info> >& file_pair) {

	// base_pathをリストアップして、file_pairに追加
	char wc[MAX_PATH];
	strcpy(wc, find_path);
	PathAppend(wc, "*.*");

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(wc, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if (strcmp(data.cFileName, ".") != 0 && strcmp(data.cFileName, "..") != 0) {
					char full_name[MAX_PATH];
					strcpy(full_name, find_path);
					PathAppend(full_name, data.cFileName);
					PathAddBackslash(full_name);

					map<string, pair<file_info, file_info> >::iterator i = file_pair.find(to_lower( string(&full_name[strlen(base_path)]) ));
					if (i == file_pair.end()) {
						file_pair[to_lower( string(&full_name[strlen(base_path)]) )] = pair<file_info, file_info>(file_info(), file_info(full_name, data));
					} else {
						file_pair[to_lower( string(&full_name[strlen(base_path)]) )].second = file_info(full_name, data);
					}

					pair_listing(base_path, full_name, file_pair);
				}
			} else {
				char full_name[MAX_PATH];
				strcpy(full_name, find_path);
				PathAppend(full_name, data.cFileName);

				map<string, pair<file_info, file_info> >::iterator i = file_pair.find( to_lower( string(&full_name[strlen(base_path)]) ));
				if (i == file_pair.end()) {
					file_pair[to_lower( string(&full_name[strlen(base_path)]) )] = pair<file_info, file_info>(file_info(), file_info(full_name, data));
				} else {
					file_pair[to_lower( string(&full_name[strlen(base_path)]) )].second = file_info(full_name, data);
				}
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);
	}
}


//---------------------------------------------------------------------
// プログラム・スタート
//---------------------------------------------------------------------
int main(int argc, char** argv) {

	if (argc != 3) {
		char myname[MAX_PATH];
		GetModuleFileName(NULL, myname, _countof(myname));
		printf("%s\n", myname);
		puts("使用法：difffolder <folder1> <folder2>");
		return 1;
	}

	map<string, pair<file_info, file_info> > file_pair;

	base_listing(argv[1], argv[1], file_pair);
	pair_listing(argv[2], argv[2], file_pair);

	cout << "左: " << argv[1] << "\n";
	cout << "右: " << argv[2] << "\n";
	cout << "\n";
	cout << "F,NAME,S1,S2\n";

	map<string, pair<file_info, file_info> >::iterator i;
	for (i = file_pair.begin(); i != file_pair.end(); i++) {
		//cout << (*i).first << "," << (*i).second.first.m_path << "," << (*i).second.second.m_path;
		if (!(*i).second.first.m_path.empty() && !(*i).second.second.m_path.empty()) {
			if ((*i).second.first.m_data.nFileSizeLow == (*i).second.second.m_data.nFileSizeLow) {
				// サイズが一致している場合、中身を比較する
				if ((*i).second.first.m_data.nFileSizeLow > 0) {
					if (compare_file((*i).second.first.m_path.c_str(), (*i).second.second.m_path.c_str())) {
						cout << ",";
					} else {
						cout << "DC,";
					}
				} else {
					cout << ",";
				}
			} else {
				cout << "DS,";
			}
			cout << (*i).first << "," << (*i).second.first.m_data.nFileSizeLow << "," << (*i).second.second.m_data.nFileSizeLow;
		} else if (!(*i).second.first.m_path.empty()) {
			cout << "L," << (*i).first << "," << (*i).second.first.m_data.nFileSizeLow << ",";
		} else {
			cout << "R," << (*i).first << ",," << (*i).second.second.m_data.nFileSizeLow;
		}
		cout << "\n";
	}

	return 0;
}
