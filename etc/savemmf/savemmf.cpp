// savemmf.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "kjmsub.h"

#include <iostream>

#include "getopt.h"
#include "sub.h"

using namespace std;

#define MAX_WRITE_SIZE	8192

char g_fname[_MAX_PATH];
char g_oname[_MAX_PATH];
size_t g_size;


//-----------------------------------------------------------------------------
// 使用法を表示して終了
//-----------------------------------------------------------------------------
void usage(int status) {
	cout << "savemmf ver.1.20 (build:" << __DATE__ << ")" << endl;
	cout << "savemmf -f <メモリマップドファイル名> -s <サイズ> -o <出力ファイル名>" << endl;
	exit(status);
}


//-----------------------------------------------------------------------------
// プログラム スタート
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
	// オプションの確認
	int c;
	while ((c = getopt(argc, argv, "f:s:o:h?")) != -1) {
		switch (c) {
		case 'f':
			strcpy(g_fname, optarg);
			break;
		case 's':
			g_size = atoi(optarg);
			break;
		case 'o':
			strcpy(g_oname, optarg);
			break;
		case 'h':
		case '?':
			usage(EXIT_SUCCESS);
			break;
		default:
			usage(EXIT_FAILURE);
			break;
		}
	}

	// オプションの正当性の確認
	if (strlen(g_fname) == 0 || strlen(g_oname) == 0 || g_size == 0) {
		cerr << "全てのオプションを指定してください。" << endl;
		usage(EXIT_FAILURE);
	}

	// 余分なオプションの確認
	if (optind < argc) {
		cerr << "オプション以外のパラメータは存在しません。" << endl;
		usage(EXIT_FAILURE);
	}

	int ret = 0;
	char* p = NULL;
	FILE* fp = NULL;
	size_t size_written = 0;

	kjm::memfile<char> anyMemFile;

	if (anyMemFile.open(g_fname) == false) {
		cerr << g_fname << ": " << GetSystemMessage(GetLastError()) << endl;
		ret = -1;
		goto end_main;
	}

	p = anyMemFile.mapViewOfFile();
	if (p == NULL) {
		cerr << GetSystemMessage(GetLastError()) << endl;
		ret = -2;
		goto end_main;
	}

	fp = fopen(g_oname, "wb");
	if (fp == NULL) {
		cerr << g_oname << ": " << GetSystemMessage(GetLastError()) << endl;
		ret = -3;
		goto end_main;
	}

	while (g_size > 0) {
		size_t write_size = (g_size > MAX_WRITE_SIZE) ? MAX_WRITE_SIZE : g_size;

		fwrite(&p[size_written], sizeof(char), write_size, fp);
		if (errno) {
			cerr << GetSystemMessage(GetLastError()) << endl;
			goto end_main;
		}
		size_written += write_size;
		g_size -= write_size;
	}
	cout << "メモリマップドファイル ( " << g_fname << " ) の内容を ファイル ( "
		<< g_oname << " ) に " << size_written << " バイト書き込みました(error=" 
		<< errno << ")。\n" << endl;

end_main:
	if (fp)
		fclose(fp);

	return ret;
}
