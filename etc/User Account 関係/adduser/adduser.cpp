// adduser.cpp : コンソール アプリケーション用のエントリ ポイントの定義
//

#include "stdafx.h"
#include <windows.h>
#include <lmaccess.h>
#include <lmapibuf.h>
#include <lmerr.h>
#include <stdio.h>
#include "etc.h"

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
	}

	printf("%s (%d)\n", pszMsg, code);
}

int main(int argc, char* argv[])
{
	char sections[61];
	DWORD dwRet;
	dwRet = GetPrivateProfileSectionNames(sections, sizeof(sections), ".\\pc1users.ini");

	NET_API_STATUS ret;
	USER_INFO_1 ui1;
	DWORD parm_err;

	ZeroMemory(&ui1, sizeof(ui1));

	ui1.usri1_name = L"test2";
	ui1.usri1_priv = USER_PRIV_USER;

	dwRet = WritePrivateProfileStruct("wpps", "ui1", &ui1, sizeof(ui1), ".\\pc1users.ini");

	printf("USER_PRIV_GUEST => %d\n", USER_PRIV_GUEST);
	printf("USER_PRIV_USER => %d\n", USER_PRIV_USER);
	printf("USER_PRIV_ADMIN => %d\n", USER_PRIV_ADMIN);

	ret = NetUserAdd(
		L"",
		1,
		(u_char*)&ui1,
		&parm_err);

	PrintNERR(ret);
	printf("parm_err = %d\n", parm_err);

	GROUP_USERS_INFO_0 gui0;

	ZeroMemory(&gui0, sizeof(gui0));
	gui0.grui0_name = L"Users";

	ret = NetUserSetGroups(
		L"",
		L"test2",
		0,
		(u_char*)&gui0,
		1);

	PrintNERR(ret);

	DWORD read = 0;
	DWORD start = 0;
	GROUP_USERS_INFO_0* pgui0;

	ret = NetUserGetGroups(
		L"",
		L"Administrator",
		0,
		(u_char**)&pgui0,
		4096,
		&read,
		&start);

	PrintNERR(ret);

	for (int i = 0; i < (int)read; i++) {
		char* p = UnicodeToAscii(pgui0[i].grui0_name);
		printf("%d: %s\n", i, p);
		free(p);
	}

	NetApiBufferFree(pgui0);

	NetUserGetLocalGroups(
		L"",
		L"test2",
		0,
		0,
		(u_char**)&pgui0,
		4096,
		&read,
		&start);

	PrintNERR(ret);

	for (i = 0; i < (int)read; i++) {
		char* p = UnicodeToAscii(pgui0[i].grui0_name);
		printf("%d: %s\n", i, p);
		free(p);
	}

	NetApiBufferFree(pgui0);

	ret = NetGroupAddUser(
		L"",
		L"PC1\\Users",
		L"test2");

	PrintNERR(ret);

	gui0.grui0_name = L"test2";

	ret = NetGroupSetUsers(
		NULL,
		L"PC1\\Users",
		0,
		(u_char*)&gui0,
		1);

	PrintNERR(ret);

	LOCALGROUP_MEMBERS_INFO_3 lmi3;
	ZeroMemory(&lmi3, sizeof(lmi3));
	lmi3.lgrmi3_domainandname = L"test2";
	ret = NetLocalGroupAddMembers(
		L"",
		L"Users",
		3,
		(LPBYTE)&lmi3,
		1);

	PrintNERR(ret);

	return 0;
}
