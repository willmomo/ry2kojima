#include <windows.h>
#include <lmaccess.h>
#include <lmerr.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

typedef struct {
	int option;
	string parm;
} OPTION;

class COpt
{
private:
	vector<OPTION> m_options;
	vector<string> m_nonoptions;

public:
	COpt(){};
	COpt(int argc, char** argv, char* optstring) {
		Parse(argc, argv, optstring);
	};
	virtual ~COpt(){};

	int Parse(int argc, char** argv, char* optstring);
	bool Get(int option);
	bool Get(int option, string& parm);
	bool GetNonOption(int index, string& parm);
};

//---------------------------------------------------------------------
//[概要]	オプションの取得
//[戻り値]	true: オプションが指定されている
//			false: オプションは指定されていない

bool COpt::Get(int option)
{
	vector<OPTION>::iterator i;
	for (i = m_options.begin(); i != m_options.end(); i++) {
		if (i->option == option) {
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------
//[概要]	オプションの取得(パラメータ取得付き)
//[戻り値]	true: オプションが指定されている
//			false: オプションは指定されていない

bool COpt::Get(int option, string& parm)
{
	vector<OPTION>::iterator i;
	for (i = m_options.begin(); i != m_options.end(); i++) {
		if (i->option == option) {
			parm = i->parm;
			return true;
		}
	}

	parm.erase();
	return false;
}

//---------------------------------------------------------------------
//[概要]	nonオプションの取得

bool COpt::GetNonOption(int index, string& parm)
{
	if (index >= m_nonoptions.size())
		return false;

	parm = m_nonoptions[index];
	return true;
}

//---------------------------------------------------------------------
//[概要]	オプションの解析

int COpt::Parse(int argc, char** argv, char* optstring)
{
	OPTION temp;
	int option;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-' || argv[i][0] == '/') {
			// optionの処理
			for (int j = 1; argv[i][j] != '\0'; j++) {
				option = argv[i][j];
				char* p = strchr(optstring, option);

				if (p == NULL) {
					cerr << "オプション -" << (char*)option << " は、無効です。" << endl;
					return -1;
				}

				if (p[1] == '=') {
					if (argv[i][j+1] == '\0') {
						++i;
						if (i < argc) {
							temp.option = option;
							temp.parm = string(argv[i]);
							m_options.push_back(temp);
							break;
						} else {
							cerr << "オプション -"<< (char)option << " には、パラメータが必要です。" << endl;
							return -1;
						}
					} else if (argv[i][j+1] == '=') {
						temp.option = option;
						temp.parm = string(&argv[i][j+2]);
						m_options.push_back(temp);
						break;
					} else {
						temp.option = option;
						temp.parm = string(&argv[i][j+1]);
						m_options.push_back(temp);
						break;
					}
				} else {
					temp.option = option;
					temp.parm.erase();
					m_options.push_back(temp);
				}
			}
		} else {
			// non-optionの処理
			m_nonoptions.push_back(string(argv[i]));
		}
	}

	return 0;
}

//---------------------------------------------------------------------
//[概要]	stringをwstringに変換

wstring StringToWString(const string& str)
{
	wstring wstr;	// 戻り文字列
	wstr.empty();	// まず空にする

	// 変換に必要なバッファサイズ(文字数(含む\0))を取得
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	if (nSizeNeeded == 0)
		return wstr;

	// バッファを確保
	wchar_t* pwsz = (wchar_t*)malloc(sizeof(wchar_t) * nSizeNeeded);
	if (pwsz == NULL)
		return wstr;

	// 変換
	int ret = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return wstr;
	}

	wstr.assign(pwsz);
	free(pwsz);

	return wstr;
}

//---------------------------------------------------------------------
//[概要]	wstringをstringに変換

string WStringToString(const wstring& wstr)
{
	string str;		// 戻り文字列
	str.empty();	// まず空にする

	// 変換に必要なバッファサイズ(文字数(含む\0))を取得
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return str;

	// バッファを確保
	char* psz = (char*)malloc(sizeof(char) * nSizeNeeded);
	if (psz == NULL)
		return str;

	// 変換
	int ret = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return str;
	}

	str.assign(psz);
	free(psz);

	return str;
}

//---------------------------------------------------------------------
//[概要]	エラー文字列群

typedef struct {
	int code;
	char* errstr;
} ERR_STR_LIST;

ERR_STR_LIST errList[] = {
	{ERROR_ACCESS_DENIED, "The user does not have access to the requested information."},
	{NERR_InvalidComputer, "The computer name is invalid."},
	{NERR_NotPrimary, "The operation is allowed only on the primary domain controller of the domain."},
	{NERR_UserNotFound, "The user name could not be found."},
};

//---------------------------------------------------------------------
//[概要]	エラー文字列を取得

char* GetErrString(int code)
{
	for (int i = 0; i < sizeof(errList) / sizeof(ERR_STR_LIST); i++) {
		if (errList[i].code == code) {
			return errList[i].errstr;
		}
	}

	return "(unknown error)";
}

//---------------------------------------------------------------------
//[概要]	使用法の表示 and 終了

void usage(int status)
{
	cout << "使用法：userdel [-m server] <user [...]>" << endl;
	exit(status);
}

//---------------------------------------------------------------------
//[概要]	ユーザーの削除
//[説明]	machineで指定されたPCからusernameで指定されたユーザを削除

int UserDel(string& machine, string& username)
{
	wstring wstrMachine = StringToWString(machine);
	wstring wstrUsername = StringToWString(username);

	NET_API_STATUS ret;
	ret = NetUserDel(wstrMachine.c_str(), wstrUsername.c_str());
	if (ret != NERR_Success) {
		cerr << "error " << ret << ":" << GetErrString(ret) << endl;
	}

	return 0;
}

//---------------------------------------------------------------------
//[概要]	プログラム・スタート

int main(int argc, char** argv)
{
	COpt opt;

	if (opt.Parse(argc, argv, "hm=") != 0) {
		usage(EXIT_FAILURE);
	}

	if (opt.Get('h')) {
		usage(EXIT_SUCCESS);
	}

	string machine;
	string username;

	opt.Get('m', machine);
	for (int i = 0; opt.GetNonOption(i, username); i++) {
		UserDel(machine, username);
	}

	return 0;
}
