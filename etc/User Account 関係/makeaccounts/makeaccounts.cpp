#include <windows.h>
#include <lmaccess.h>
#include <lmapibuf.h>
#include <lmerr.h>
#include <iostream>
#include <stdio.h>
#include <mbstring.h>
#include "etc.h"

using namespace std;

//---------------------------------------------------------------------
// デファイン

#define GROW_SIZE	512
#define DEFAULT_OUTPUT	".\\users.ini"

//---------------------------------------------------------------------
// グローバル変数

char g_szMachine[256];		// 情報を取得するサーバ(PC)の名前
char g_szPrefix[UNLEN];		// ユーザ検索に使うprefix
char g_szConfig[_MAX_PATH];	// configファイル名

//---------------------------------------------------------------------
//[概要]	エラーメッセージの表示

void PrintNERR(NET_API_STATUS code)
{
	char* pszMsg = "(unknown error)";

	switch (code) {
	case ERROR_ACCESS_DENIED:
		pszMsg = "The user does not have access to the requested information.";
		break;
	case ERROR_INVALID_LEVEL:
		pszMsg = "The value specified for the Level parameter is invalid.";
		break;
	case ERROR_NO_MORE_ITEMS:
		pszMsg = "There were no more items on which to operate.";
		break;
	case NERR_InvalidComputer:
		pszMsg = "The computer name is invalid.";
		break;
	case NERR_UserNotFound:
		pszMsg = "The user name could not be found.";
		break;
	case RPC_S_SERVER_UNAVAILABLE:
		pszMsg = "The RPC server is unavailable.";
		break;
	case NERR_NotPrimary:
		pszMsg = "The operation is allowed only on the primary domain controller of the domain.";
		break;
	case NERR_GroupExists:
		pszMsg = "The group already exists.";
		break;
	case NERR_UserExists:
		pszMsg = "The user account already exists.";
		break;
	case NERR_PasswordTooShort:
		pszMsg = "The password is shorter than required. The password could also have been too long, too recent in its change history, not enough unique characters, or some other faulty indication of password restrictions.";
		break;
	case ERROR_INVALID_PARAMETER:
		pszMsg = "The parameter is incorrect.";
		break;
	case NERR_GroupNotFound:
		pszMsg = "The local group specified by the groupname parameter does not exist.";
		break;
	case ERROR_MEMBER_IN_ALIAS:
		pszMsg = "The specified account name is already a member of the local group.";
		break;
	case ERROR_INVALID_MEMBER:
		pszMsg = "A new member could not be added to a local group because the member has the"
				 " wrong account type.";
		break;
	}

	cout << pszMsg << "(" << code << ")" << endl;
}

//---------------------------------------------------------------------
//[概要]	ヘルプ

void help(int status)
{
	printf("Syntax: makeaccounts <config file> [machine]\n");
	
	exit(status);
}

//---------------------------------------------------------------------
//[概要]	オプションのチェック

void chkarg(int argc, char **argv)
{
	if (argc == 2) {		// バックアップファイルのみ指定
		if (argv[1][0] == '/' || argv[1][0] == '-') {
			help(EXIT_FAILURE);
		} else {
			strcpy(g_szConfig, argv[1]);
		}
	} else if (argc == 3) {	// バックアップファイル＋書き込みサーバ指定
		if (argv[1][0] == '/' || argv[1][0] == '-') {
			help(EXIT_FAILURE);
		} else {
			strcpy(g_szConfig, argv[1]);
		}

		if (argv[2][0] == '/' || argv[2][0] == '-') {
			help(EXIT_FAILURE);
		} else {
			strcpy(g_szMachine, argv[2]);
		}
	} else {
		help(EXIT_FAILURE);
	}

	// configファイル名の確認
	if (g_szConfig[0] == '\0') {	// 指定されていない
		strcpy(g_szConfig, DEFAULT_OUTPUT);
	} else {
		char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
		_splitpath(g_szConfig, drive, dir, fname, ext);
		if (drive[0] == '\0' && dir[0] == '\0') {	// ドライブ、パスとも未指定
			sprintf(g_szConfig, ".\\%s%s", fname, ext);
		}
	}
}

//---------------------------------------------------------------------
//[概要]	configファイルの解析

int ProcessFile()
{
	static char szLine[4096];
	FILE* fp;
	
	fp = fopen(g_szConfig, "r");
	if (fp) {
		while (fgets(szLine, sizeof(szLine), fp)) {
			chomp(szLine);
			if (szLine[0] == '[' && szLine[strlen(szLine)-1] == ']') {
				char szName[UNLEN];
				mid(szLine, 1, strlen(szLine) - 2, szName);
			}
		}

		fclose(fp);
	} else {
		printf("Unable to open %s\n", g_szConfig);
	}

	return 0;
}

//---------------------------------------------------------------------
//[概要]	ユーザをローカルグループに追加

int AddGroup(const wstring wstrMachine, const char* pszUser)
{
	char szLocalGroups[4096];
	GetPrivateProfileString(pszUser, "local_groups", "", szLocalGroups, sizeof(szLocalGroups), g_szConfig);

	cout << "add local group ... ";
	char* p = strtok(szLocalGroups, ",");
	while (p) {
		cout << p << " ";

		LOCALGROUP_MEMBERS_INFO_3 lmi3;
		ZeroMemory(&lmi3, sizeof(lmi3));
		lmi3.lgrmi3_domainandname = (LPWSTR)StringToWString(string(pszUser)).c_str();
		NET_API_STATUS ret = NetLocalGroupAddMembers(
			wstrMachine.c_str(),
			StringToWString(string(p)).c_str(),
			3,
			(LPBYTE)&lmi3,
			1);

		if (ret != NERR_Success) {
			PrintNERR(ret);
			return -1;
		}

		p = strtok(NULL, ",");
	}

	return 0;
}

//---------------------------------------------------------------------
//[概要]	ユーザの追加

int AddUser(const wstring wstrMachine)
{
	// すべてのセクションの名前を取得
	int nRet = 0;
	int nSize = 0;
	char* pSections = NULL;
	do {
		nSize += GROW_SIZE;
		pSections= (char*)realloc(pSections, nSize);
		nRet = GetPrivateProfileSectionNames(pSections, nSize, g_szConfig);
	} while (nRet == (nSize - 2));
	
	char szTemp[4096];
	for (char* p = pSections; *p != '\0'; p += (strlen(p) + 1)) {
		USER_INFO_3 ui3;
		ZeroMemory(&ui3, sizeof(ui3));

		// usri3_name ...
		GetPrivateProfileString(p, "name", "", szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrName = StringToWString(string(szTemp));
		ui3.usri3_name = (LPWSTR)wstrName.c_str();

		// usri3_password ... 常にユーザー名と同じ
		wstring wstrPassword = StringToWString(string(szTemp));
		ui3.usri3_password = (LPWSTR)wstrPassword.c_str();

		// usri3_password_age ... NetUserAdd function ignore this member

		// usri3_priv ... NetUserAdd function ignore this member

		// usri3_home_dir ...
		GetPrivateProfileString(p, "home_dir", "", szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrHomeDir = StringToWString(string(szTemp));
		ui3.usri3_home_dir = (LPWSTR)wstrHomeDir.c_str();

		// usri3_comment ...
		GetPrivateProfileString(p, "comment", "", szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrComment = StringToWString(string(szTemp));
		ui3.usri3_comment = (LPWSTR)wstrComment.c_str();

		// usri3_flags ...
		ui3.usri3_flags = GetPrivateProfileInt(p, "flags", 0, g_szConfig);

		// usri3_script_path ...
		GetPrivateProfileString(p, "script_path", "", szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrScriptPath = StringToWString(string(szTemp));
		ui3.usri3_script_path = (LPWSTR)wstrScriptPath.c_str();

		// usri3_auth_flags ... NetUserAdd function ignore this member

		// usri3_full_name ...
		GetPrivateProfileString(p, "full_name", "",szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrFullName = StringToWString(string(szTemp));
		ui3.usri3_full_name = (LPWSTR)wstrFullName.c_str();

		// usri3_usr_comment ...
		GetPrivateProfileString(p, "usr_comment", "", szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrUsrComment = StringToWString(string(szTemp));
		ui3.usri3_usr_comment = (LPWSTR)wstrUsrComment.c_str();

		// usri3_parms ...
		char bParms[4096];
		GetPrivateProfileStruct(p, "parms", bParms,
			GetPrivateProfileString(p, "parms", "", szTemp, sizeof(szTemp), g_szConfig) / 2 - 1,
			g_szConfig);
		ui3.usri3_parms = (LPWSTR)bParms;
		
		// usri3_workstations ...
		GetPrivateProfileString(p, "workstations", "", szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrWorkstations = StringToWString(string(szTemp));
		ui3.usri3_workstations = (LPWSTR)wstrWorkstations.c_str();

		// usri3_last_logon ... NetUserAdd function ignore this member
		ui3.usri3_last_logoff = GetPrivateProfileInt(p, "last_logoff", 0, g_szConfig);

		// usri3_acct_expires ...
		ui3.usri3_acct_expires = GetPrivateProfileInt(p, "acct_expires", -1, g_szConfig);

		// usri3_max_storage ...
		ui3.usri3_max_storage = GetPrivateProfileInt(p, "max_storage", -1, g_szConfig);

		// usri3_units_per_week ... NetUserAdd function ignore this member

		// usri3_logon_hours ...
		char bLogonHours[21];
		GetPrivateProfileStruct(p, "logon_hours", bLogonHours,
			GetPrivateProfileString(p, "logon_hours", "", szTemp, sizeof(szTemp), g_szConfig) / 2 - 1,
			g_szConfig);
		ui3.usri3_logon_hours = (PBYTE)bLogonHours;

		// usri3_bad_pw_count ... NuetUserAdd function ignore this member

		// usri3_num_logons ... NetUserAdd function ignore this member

		// usri3_logon_server ... NetUserAdd ignore this member

		// usri3_country_code ...
		ui3.usri3_country_code = GetPrivateProfileInt(p, "country_code", 0, g_szConfig);

		// usri3_code_page ...
		ui3.usri3_code_page = GetPrivateProfileInt(p, "code_page", 0, g_szConfig);

		// usri3_user_id ... NetUserAdd function ignore this member
		
		// usri3_primary_group_id ...
		ui3.usri3_primary_group_id = GetPrivateProfileInt(p, "primary_group_id", 0, g_szConfig);

		// usri3_profile ...
		GetPrivateProfileString(p, "profile", "", szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrProfile = StringToWString(string(szTemp));
		ui3.usri3_profile = (LPWSTR)wstrProfile.c_str();

		// usri3_home_dir_drive ...
		GetPrivateProfileString(p, "home_dir_drive", "", szTemp, sizeof(szTemp), g_szConfig);
		wstring wstrHomeDirDrive = StringToWString(string(szTemp));
		ui3.usri3_home_dir_drive = (LPWSTR)wstrHomeDirDrive.c_str();

		// usri3_password_expired ...
		ui3.usri3_password_expired = GetPrivateProfileInt(p, "password_expired", 0, g_szConfig);

		cout << "add user " << p << " ... ";
		DWORD parm_err;
		NET_API_STATUS ret;
		ret = NetUserAdd(wstrMachine.c_str(), 3, (LPBYTE)&ui3, &parm_err);
		if (ret == 0) {
			// ローカルグループに追加
			AddGroup(wstrMachine, p);
			cout << endl;
		} else {
			PrintNERR(ret);
		}
	}

	free(pSections);

	return 0;
}

//---------------------------------------------------------------------
//[概要]	プログラム･スタート

int main(int argc, char** argv)
{
	wstring wstrMachine;

	chkarg(argc, argv);	// オプションのチェック

	wstrMachine = StringToWString(string(g_szMachine));	// サーバ名をUnicode化

	AddUser(wstrMachine);	// ユーザの追加

	return 0;
}
