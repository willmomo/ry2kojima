#include <windows.h>
#include <windowsx.h>
#include <lm.h>
#include <iostream>
#include <string>

using namespace std;


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

wstring StrToWStr(const char* psz)
{
	wstring wstr;

	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, psz, -1, NULL, 0);
	if (nSizeNeeded == 0)
		return wstr;

	// バッファを確保
	wchar_t* pwsz = new wchar_t[nSizeNeeded];
	if (pwsz == NULL)
		return wstr;

	// 変換
	int ret = MultiByteToWideChar(CP_ACP, 0, psz, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		delete[] pwsz;
		return wstr;
	}

	wstr.assign(pwsz);
	delete[] pwsz;

	return wstr;
}

string WStrToStr(const wchar_t* pwsz)
{
	string str;

	// 変換に必要なバッファサイズ(文字数(\0含む))を取得
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0)
		return str;

	// バッファを確保
	char* psz = new char[nSizeNeeded];
	if (psz == NULL)
		return str;

	// 変換
	int ret = WideCharToMultiByte(CP_ACP, 0, pwsz, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		delete[] psz;
		return str;
	}

	str.assign(psz);
	delete[] psz;

	return str;
}

string GetSystemMessage(DWORD code)
{
	string s;
	LPTSTR p;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL, code, 0, (LPTSTR)&p, 256, NULL);
	s.assign(p);
	LocalFree(p);

	return s;
}

int AddUser()
{
	char szTemp[4096];
	USER_INFO_3 ui3;
	ZeroMemory(&ui3, sizeof(ui3));

	// usri3_name ...
//	GetPrivateProfileString(p, "name", "", szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "wssadmin");
	wstring wstrName = StringToWString(string(szTemp));
	ui3.usri3_name = (LPWSTR)wstrName.c_str();

	// usri3_password ... 常にユーザー名と同じ
	wstring wstrPassword = StringToWString(string(szTemp));
	ui3.usri3_password = (LPWSTR)wstrPassword.c_str();

	// usri3_password_age ... NetUserAdd function ignore this member

	// usri3_priv ... NetUserAdd function ignore this member

	// usri3_home_dir ...
//	GetPrivateProfileString(p, "home_dir", "", szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "");
	wstring wstrHomeDir = StringToWString(string(szTemp));
	ui3.usri3_home_dir = (LPWSTR)wstrHomeDir.c_str();

	// usri3_comment ...
//	GetPrivateProfileString(p, "comment", "", szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "");
	wstring wstrComment = StringToWString(string(szTemp));
	ui3.usri3_comment = (LPWSTR)wstrComment.c_str();

	// usri3_flags ...
//	ui3.usri3_flags = GetPrivateProfileInt(p, "flags", 0, g_szConfig);
	ui3.usri3_flags = UF_DONT_EXPIRE_PASSWD;

	// usri3_script_path ...
//	GetPrivateProfileString(p, "script_path", "", szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "");
	wstring wstrScriptPath = StringToWString(string(szTemp));
	ui3.usri3_script_path = (LPWSTR)wstrScriptPath.c_str();

	// usri3_auth_flags ... NetUserAdd function ignore this member

	// usri3_full_name ...
//	GetPrivateProfileString(p, "full_name", "",szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "");
	wstring wstrFullName = StringToWString(string(szTemp));
	ui3.usri3_full_name = (LPWSTR)wstrFullName.c_str();

	// usri3_usr_comment ...
//	GetPrivateProfileString(p, "usr_comment", "", szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "");
	wstring wstrUsrComment = StringToWString(string(szTemp));
	ui3.usri3_usr_comment = (LPWSTR)wstrUsrComment.c_str();

	// usri3_parms ...
	char bParms[4096];
//	GetPrivateProfileStruct(p, "parms", bParms,
//		GetPrivateProfileString(p, "parms", "", szTemp, sizeof(szTemp), g_szConfig) / 2 - 1,
//		g_szConfig);
	ZeroMemory(bParms, sizeof(bParms));
	ui3.usri3_parms = (LPWSTR)bParms;
	
	// usri3_workstations ...
//	GetPrivateProfileString(p, "workstations", "", szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "");
	wstring wstrWorkstations = StringToWString(string(szTemp));
	ui3.usri3_workstations = (LPWSTR)wstrWorkstations.c_str();

	// usri3_last_logon ... NetUserAdd function ignore this member
//	ui3.usri3_last_logoff = GetPrivateProfileInt(p, "last_logoff", 0, g_szConfig);
	ui3.usri3_last_logoff = 0;

	// usri3_acct_expires ...
//	ui3.usri3_acct_expires = GetPrivateProfileInt(p, "acct_expires", -1, g_szConfig);
	ui3.usri3_acct_expires = -1;

	// usri3_max_storage ...
//	ui3.usri3_max_storage = GetPrivateProfileInt(p, "max_storage", -1, g_szConfig);
	ui3.usri3_max_storage = -1;

	// usri3_units_per_week ... NetUserAdd function ignore this member

	// usri3_logon_hours ...
	char bLogonHours[21];
//	GetPrivateProfileStruct(p, "logon_hours", bLogonHours,
//		GetPrivateProfileString(p, "logon_hours", "", szTemp, sizeof(szTemp), g_szConfig) / 2 - 1,
//		g_szConfig);
	FillMemory(bLogonHours, sizeof(bLogonHours), 0xff);
	ui3.usri3_logon_hours = (PBYTE)bLogonHours;

	// usri3_bad_pw_count ... NuetUserAdd function ignore this member

	// usri3_num_logons ... NetUserAdd function ignore this member

	// usri3_logon_server ... NetUserAdd ignore this member

	// usri3_country_code ...
//	ui3.usri3_country_code = GetPrivateProfileInt(p, "country_code", 0, g_szConfig);
	ui3.usri3_country_code = 0;

	// usri3_code_page ...
//	ui3.usri3_code_page = GetPrivateProfileInt(p, "code_page", 0, g_szConfig);
	ui3.usri3_code_page = 0;

	// usri3_user_id ... NetUserAdd function ignore this member
	
	// usri3_primary_group_id ...
//	ui3.usri3_primary_group_id = GetPrivateProfileInt(p, "primary_group_id", 0, g_szConfig);
	ui3.usri3_primary_group_id = 0;

	// usri3_profile ...
//	GetPrivateProfileString(p, "profile", "", szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "");
	wstring wstrProfile = StringToWString(string(szTemp));
	ui3.usri3_profile = (LPWSTR)wstrProfile.c_str();

	// usri3_home_dir_drive ...
//	GetPrivateProfileString(p, "home_dir_drive", "", szTemp, sizeof(szTemp), g_szConfig);
	lstrcpy(szTemp, "");
	wstring wstrHomeDirDrive = StringToWString(string(szTemp));
	ui3.usri3_home_dir_drive = (LPWSTR)wstrHomeDirDrive.c_str();

	// usri3_password_expired ...
//	ui3.usri3_password_expired = GetPrivateProfileInt(p, "password_expired", 0, g_szConfig);
	ui3.usri3_password_expired = 0;

	cout << "add user " << "wssadmin" << " ... ";
	DWORD parm_err;
	NET_API_STATUS ret;
	ret = NetUserAdd(NULL, 3, (LPBYTE)&ui3, &parm_err);
	if (ret == 0) {
		// ローカルグループに追加
//		AddGroup(wstrMachine, p);
		cout << endl;
	} else {
//		PrintNERR(ret);
	}


	return 0;
}

int main(int argc, char** argv)
{
	AddUser();

	return 0;
}
