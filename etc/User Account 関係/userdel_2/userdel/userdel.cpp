#include <windows.h>
#include <lmaccess.h>
#include <lmerr.h>
#include <iostream>
#include <string>
#include <vector>
#include "..\lib\opt\opt.h"

using namespace std;

//---------------------------------------------------------------------
//[概要]	文字列終端の'\n'を削除

char* chomp(char* str)
{
	for (int i = strlen(str) - 1; i > 0; i--) {
		if (str[i] == '\n')
			str[i] = '\0';
		else
			break;
	}

	return str;
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
//[概要]	エラー文字列を取得

string GetErrString(int code)
{
	static string msg;
	char* pmsg;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL,
		code,
		0,
		(char*)&pmsg,
		256,
		NULL);
	chomp(pmsg);
	msg = string(pmsg);
	LocalFree(pmsg);

	return msg;
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
