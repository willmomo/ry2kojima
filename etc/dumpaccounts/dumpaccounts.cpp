#include <windows.h>
#include <lmaccess.h>
#include <lmapibuf.h>
#include <lmwksta.h>
#include <lmerr.h>
#include <iostream>
#include "etc.h"

using namespace std;

//---------------------------------------------------------------------
// �f�t�@�C��

#define DEFAULT_OUTPUT	".\\users.ini"

//---------------------------------------------------------------------
// �O���[�o���ϐ�

char g_szMachine[256];	// �����擾����T�[�o(PC)�̖��O
char g_szPrefix[UNLEN];	// ���[�U�����Ɏg��prefix
char g_szOutput[256];	// �o�̓t�@�C����

//---------------------------------------------------------------------
//[�T�v]	�G���[���b�Z�[�W�̕\��

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

	cout << pszMsg << "(" << code << ")" << endl;
}

//---------------------------------------------------------------------
//[�T�v]	�w���v

void help(int status)
{
	cout <<	"usage: dumpaccounts [-m machine][-p prefix][-o savefile]\n"
			"  -m    �f�[�^���擾����R���s���[�^��(default=local)\n"
			"  -p    prefix�Ŏn�܂郆�[�U�[�̂ݏo��\n"
			"  -o    �o�̓t�@�C��(default=.\\users.ini)\n";

	exit(status);
}

//---------------------------------------------------------------------
//[�T�v]	�I�v�V�����̃`�F�b�N

void chkarg(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		char* p = argv[i];
		if (p[0] == '-' || p[0] == '/') {

			// -m:�����擾����T�[�o�[�̖��O
			if (p[1] == 'm' || p[1] == 'M') {
				if (p[2] == '=') {
					lstrcpy(g_szMachine, &p[3]);
				} else if (p[2] != '\0') {
					lstrcpy(g_szMachine, &p[2]);
				} else {
					if (++i < argc) {
						lstrcpy(g_szMachine, argv[i]);
					} else {
						help(EXIT_FAILURE);
					}
				}

			// -p:���[�U�����Ɏg��prefix
			} else if (p[1] == 'p' || p[1] == 'P') {
				if (p[2] == '=') {
					lstrcpy(g_szPrefix, &p[3]);
				} else if (p[2] != '\0') {
					lstrcpy(g_szPrefix, &p[2]);
				} else {
					if (++i < argc) {
						lstrcpy(g_szPrefix, argv[i]);
					} else {
						help(EXIT_FAILURE);
					}
				}

			// -o:�o�̓t�@�C����
			} else if (p[1] == 'o' || p[1] == 'O') {
				if (p[2] == '=') {
					lstrcpy(g_szOutput, &p[3]);
				} else if (p[2] != '\0') {
					lstrcpy(g_szOutput, &p[2]);
				} else {
					if (++i < argc) {
						lstrcpy(g_szOutput, argv[i]);
					} else {
						help(EXIT_FAILURE);
					}
				}

			// -?:�w���v�\��
			} else if (p[1] == '?') {
				help(EXIT_SUCCESS);
			} else {
				help(EXIT_FAILURE);
			}
		} else {
			help(EXIT_FAILURE);
		}
	}

	// �o�̓t�@�C�����̊m�F
	if (g_szOutput[0] == '\0') {	// �w�肳��Ă��Ȃ�
		lstrcpy(g_szOutput, DEFAULT_OUTPUT);
	} else {
		char drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
		_splitpath(g_szOutput, drive, dir, fname, ext);
		if (drive[0] == '\0' && dir[0] == '\0') {	// �h���C�u�A�p�X�Ƃ����w��
			wsprintf(g_szOutput, ".\\%s%s", fname, ext);
		}
	}
}

//---------------------------------------------------------------------
//[�T�v]	�A�J�E���g���̏����o��
//[����]	strName	�A�J�E���g��(�Z�N�V�����ɂȂ�܂�)
//			pui3	�A�J�E���g���

int DumpAccount(const string& strName, const USER_INFO_3 *pui3)
{
	static char szBuf[4096];
	wstring wstrTemp;

	// usri3_name ...
	wstrTemp = pui3->usri3_name ? pui3->usri3_name : L"";
	WritePrivateProfileString(strName.c_str(), "name",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_password ...
	wstrTemp = pui3->usri3_password ? pui3->usri3_password : L"";
	WritePrivateProfileString(strName.c_str(), "password",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_password_age ...
	wsprintf(szBuf, "%d", pui3->usri3_password_age);
	WritePrivateProfileString(strName.c_str(), "password_age", szBuf, g_szOutput);

	// usri3_priv ...
	wsprintf(szBuf, "%d", pui3->usri3_priv);
	WritePrivateProfileString(strName.c_str(), "priv", szBuf, g_szOutput);

	// usri3_home_dir ...
	wstrTemp = pui3->usri3_home_dir ? pui3->usri3_home_dir : L"";
	WritePrivateProfileString(strName.c_str(), "home_dir",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_comment ...
	wstrTemp = pui3->usri3_comment ? pui3->usri3_comment : L"";
	WritePrivateProfileString(strName.c_str(), "comment",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_flags ...
	wsprintf(szBuf, "%d", pui3->usri3_flags);
	WritePrivateProfileString(strName.c_str(), "flags", szBuf, g_szOutput);

	// usri3_script_path ...
	wstrTemp = pui3->usri3_script_path ? pui3->usri3_script_path : L"";
	WritePrivateProfileString(strName.c_str(), "script_path",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_auth_flags ...
	wsprintf(szBuf, "%d", pui3->usri3_auth_flags);
	WritePrivateProfileString(strName.c_str(), "auth_flags", szBuf, g_szOutput);

	// usri3_full_name ...
	wstrTemp = pui3->usri3_full_name ? pui3->usri3_full_name : L"";
	WritePrivateProfileString(strName.c_str(), "full_name",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_usr_comment ...
	wstrTemp = pui3->usri3_usr_comment ? pui3->usri3_usr_comment : L"";
	WritePrivateProfileString(strName.c_str(), "usr_comment",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_parms ...
	WritePrivateProfileStruct(strName.c_str(), "parms",
		pui3->usri3_parms, (lstrlenW(pui3->usri3_parms) + 1) * 2, g_szOutput);

	// usri3_workstations ...
	wstrTemp = pui3->usri3_workstations ? pui3->usri3_workstations : L"";
	WritePrivateProfileString(strName.c_str(), "workstations",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_last_logon ...
	wsprintf(szBuf, "%d", pui3->usri3_last_logon);
	WritePrivateProfileString(strName.c_str(), "last_logon", szBuf, g_szOutput);

	// usri3_last_logoff ...
	wsprintf(szBuf, "%d", pui3->usri3_last_logoff);
	WritePrivateProfileString(strName.c_str(), "last_logoff", szBuf, g_szOutput);

	// usri3_acct_expires ...
	wsprintf(szBuf, "%d", pui3->usri3_acct_expires);
	WritePrivateProfileString(strName.c_str(), "acct_expires", szBuf, g_szOutput);

	// usri3_max_storage ...
	wsprintf(szBuf, "%d", pui3->usri3_max_storage);
	WritePrivateProfileString(strName.c_str(), "max_storage", szBuf, g_szOutput);

	// usri3_units_per_week ...
	wsprintf(szBuf, "%d", pui3->usri3_units_per_week);
	WritePrivateProfileString(strName.c_str(), "units_per_week", szBuf, g_szOutput);

	// usri3_logon_hours ...
	WritePrivateProfileStruct(strName.c_str(), "logon_hours",
		pui3->usri3_logon_hours, 21, g_szOutput);

	// usri3_bad_pw_count ...
	wsprintf(szBuf, "%d", pui3->usri3_bad_pw_count);
	WritePrivateProfileString(strName.c_str(), "bad_pw_count", szBuf, g_szOutput);

	// usri3_num_logons ...
	wsprintf(szBuf, "%d", pui3->usri3_num_logons);
	WritePrivateProfileString(strName.c_str(), "num_logons", szBuf, g_szOutput);

	// usri3_logon_server ...
	wstrTemp = pui3->usri3_logon_server ? pui3->usri3_logon_server : L"";
	WritePrivateProfileString(strName.c_str(), "logon_server",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_country_code ...
	wsprintf(szBuf, "%d", pui3->usri3_country_code);
	WritePrivateProfileString(strName.c_str(), "country_code", szBuf, g_szOutput);

	// usri3_code_page ...
	wsprintf(szBuf, "%d", pui3->usri3_code_page);
	WritePrivateProfileString(strName.c_str(), "code_page", szBuf, g_szOutput);

	// usri3_user_id ...
	wsprintf(szBuf, "%d", pui3->usri3_user_id);
	WritePrivateProfileString(strName.c_str(), "user_id", szBuf, g_szOutput);

	// usri3_primary_group_id ...
	wsprintf(szBuf, "%d", pui3->usri3_primary_group_id);
	WritePrivateProfileString(strName.c_str(), "primary_group_id", szBuf, g_szOutput);

	// usri3_profile ...
	wstrTemp = pui3->usri3_profile ? pui3->usri3_profile : L"";
	WritePrivateProfileString(strName.c_str(), "profile",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_home_dir_drive ...
	wstrTemp = pui3->usri3_home_dir_drive ? pui3->usri3_home_dir_drive : L"";
	WritePrivateProfileString(strName.c_str(), "home_dir_drive",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	// usri3_password_expired ...
	wsprintf(szBuf, "%d", pui3->usri3_password_expired);
	WritePrivateProfileString(strName.c_str(), "password_expired", szBuf, g_szOutput);

	return 0;
}

//---------------------------------------------------------------------

int DumpLocalGroup(const string& strName)
{
	DWORD dwRead, dwIndex;
	NET_API_STATUS ret;
	LOCALGROUP_USERS_INFO_0* plui0;
	wstring wstrMachine = StringToWString(g_szMachine);	// �T�[�o����Unicode��

	// ���[�U�[������O���[�v���擾
	ret = NetUserGetLocalGroups(wstrMachine.c_str(), 
		StringToWString(strName).c_str(), 0, 0, (LPBYTE*)&plui0,
		4096, &dwRead, &dwIndex);

	wstring wstrTemp;
	for (int i = 0; i < (int)dwRead; i++) {
		wstrTemp += wstring(plui0[i].lgrui0_name);
		wstrTemp += wstring(L",");
	}

	NetApiBufferFree(plui0);

	// local_groups ...
	WritePrivateProfileString(strName.c_str(), "local_groups",
		WStringToString(wstrTemp).c_str(), g_szOutput);

	return 0;
}

//---------------------------------------------------------------------
//[�T�v]	�v���O������X�^�[�g

int main(int argc, char** argv)
{
	chkarg(argc, argv);	// �I�v�V�����̃`�F�b�N

	remove(g_szOutput);	// �t�@�C�����c���Ă���Ƃ����Ȃ��̂ō폜

	NET_API_STATUS ret;
	NET_API_STATUS retQuery;
	DWORD dwIndex = 4;
	wstring wstrMachine = StringToWString(g_szMachine);	// �T�[�o����Unicode��

	// NetQueryDisplayInformation���߂̐擪���擾
	ret = NetGetDisplayInformationIndex(wstrMachine.c_str(), 1, 
		StringToWString(g_szPrefix).c_str(), &dwIndex);
	if (ret != NERR_Success) {
		PrintNERR(ret);
		return 1;
	}

	do {
		NET_DISPLAY_USER *ndu;
		DWORD dwEntryCount;

		// ���[�U��񋓂���
		retQuery = NetQueryDisplayInformation(wstrMachine.c_str(), 1, dwIndex, 8, 0xffffffff, &dwEntryCount, (void**)&ndu);
		if (retQuery != NERR_Success && retQuery != ERROR_MORE_DATA) {
			PrintNERR(retQuery);
			return 2;
		}

		for (int i = 0; i < (int)dwEntryCount; i++) {

			// ���[�U�����Z�N�V�����ɂ���
			string strName = WStringToString(wstring(ndu[i].usri1_name));

			// ���[�U�[����prefix�Ɉ�v���邩�ǂ����m�F
			if (_strnicmp(g_szPrefix, strName.c_str(), strlen(g_szPrefix)) != 0) {
				retQuery = NERR_Success;	// �s��v�Ȃ�Έȍ~���݂��Ȃ��̂ŏI��������
				break;
			}

			cout << "Saving " << strName << " ... " << endl;	// ���

			USER_INFO_3 *ui3;
			ret = NetUserGetInfo(wstrMachine.c_str(), ndu[i].usri1_name, 3, (u_char**)&ui3);
			if (ret == NERR_Success) {

				DumpAccount(strName, ui3);	// dump account
				DumpLocalGroup(strName);	// ���[�U�̏�������O���[�v��dump

				NetApiBufferFree(ui3);
			} else {
				PrintNERR(ret);
			}

			dwIndex = ndu[i].usri1_next_index;
		}

		NetApiBufferFree(ndu);
	
	} while(retQuery == ERROR_MORE_DATA);

	return 0;
}
