//---------------------------------------------------------------------
// ���[�U���\���v���O����

#pragma warning(disable:4786)

#include <windows.h>
#include <lmaccess.h>
#include <lmapibuf.h>
#include <lmerr.h>

#include <time.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <list>

#include "getopt.h"
#include "regex.h"
#include "sub.h"

//---------------------------------------------------------------------
// �}�N��/�萔

// VC6�Ŗ���`��flags�B
#define UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED	0x000080
#define UF_NOT_DELEGATED					0x100000
#define UF_SMARTCARD_REQUIRED				0x040000
#define UF_USE_DES_KEY_ONLY					0x200000
#define UF_DONT_REQUIRE_PREAUTH				0x400000
#define UF_TRUSTED_FOR_DELEGATION			0x080000

#define SEC_1MIN	60				// 1����60�b
#define SEC_1HOUR	(60*SEC_1MIN)	// 1���Ԃ�60��
#define SEC_1DAY	(24*SEC_1HOUR)	// 1����24����

typedef map<int, const char*>	PRIV_MAP, FLAGS_MAP, AFLAGS_MAP;

//---------------------------------------------------------------------
// �O���[�o���ϐ�

PRIV_MAP priv_map;
FLAGS_MAP flags_map;
AFLAGS_MAP aflags_map;
wstring wstrServer;
list<regex_t> regex_list;

//---------------------------------------------------------------------
//[�T�v]
//	�g�p�@��\�����ďI��

void usage(int status)
{
	cout << "userdump ver.1.00 (build:" << __DATE__ << ")" << endl;
	cout << "usage: userdump [-s server] [user [...]]" << endl;

	exit(status);
}

//---------------------------------------------------------------------
//[�T�v]
//	���[�J���O���[�v�̃_���v

void dump_local_group(const wchar_t* server, const wchar_t* name)
{
	DWORD dwRead, dwIndex;
	NET_API_STATUS ret;
	LOCALGROUP_USERS_INFO_0* plui0;

	// ���[�U�[������O���[�v���擾
	ret = NetUserGetLocalGroups(server, name,
		0, 0, (LPBYTE*)&plui0,
		4096, &dwRead, &dwIndex);

	cout << setw(16) << left << "local group" << " : ";
	wstring wstrTemp;
	for (int i = 0; i < (int)dwRead; i++) {
		if (i != 0)
			cout << ",";

		cout << WStrToStr(plui0[i].lgrui0_name);
	}
	cout << endl;

	NetApiBufferFree(plui0);
}

//---------------------------------------------------------------------
//[�T�v]
//	���[�U���̕\��

void userdump(const USER_INFO_3* pui3)
{
	cout << setw(16) << left << "name" << " : " << WStrToStr(pui3->usri3_name) << endl;
	cout << setw(16) << left << "password" << " : " << "<unknown>" << endl;

	cout << setw(16) << left << "password age" << " : ";
	DWORD password_age = pui3->usri3_password_age;
	if (password_age / SEC_1DAY) {
		cout << (password_age / SEC_1DAY) << "�� ";
		password_age %= SEC_1DAY;
	}
	if (password_age / SEC_1HOUR) {
		cout << (password_age / SEC_1HOUR) << "���� ";
		password_age %= SEC_1HOUR;
	}
	if (password_age / SEC_1MIN) {
		cout << (password_age / SEC_1MIN) << "�� ";
		password_age %= SEC_1MIN;
	}
	cout << password_age << "�b " << endl;

	PRIV_MAP::iterator ci = priv_map.find(pui3->usri3_priv);
	cout << setw(16) << left << "priv" << " : " << (ci != priv_map.end() ? (*ci).second:"(unknown priv)") << endl;

	cout << setw(16) << left << "home dir" << " : " << WStrToStr(pui3->usri3_home_dir) << endl;
	cout << setw(16) << left << "comment" << " : " << WStrToStr(pui3->usri3_comment) << endl;

	DWORD flags = pui3->usri3_flags;
	string title = "flags";
	FLAGS_MAP::iterator i;
	for (i = flags_map.begin(); i != flags_map.end(); ++i) {
		if (flags & (*i).first) {
			cout << setw(16) << left << title << " : " << (*i).second << endl;
			title = "";
			flags &= ~(*i).first;
		}
	}
	if (flags) {
		cout << setw(16) << left << title << " : " << hex << uppercase << flags << dec << " (unknown flags)" << endl;
	}

	cout << setw(16) << left << "script path" << " : " << WStrToStr(pui3->usri3_script_path) << endl;

	AFLAGS_MAP::iterator ci2 = aflags_map.find(pui3->usri3_auth_flags);
	cout << setw(16) << left << "auth flags" << " : " << (ci2 != aflags_map.end() ? (*ci2).second:"(unknown)") << endl;

	cout << setw(16) << left << "full name" << " : " << WStrToStr(pui3->usri3_full_name) << endl;
	cout << setw(16) << left << "usr comment" << " : " << WStrToStr(pui3->usri3_usr_comment) << endl;
	cout << setw(16) << left << "parms" << " : " << WStrToStr(pui3->usri3_parms) << endl;
	cout << setw(16) << left << "workstations" << " : " << WStrToStr(pui3->usri3_workstations) << endl;

	if (pui3->usri3_last_logon == 0) {
		cout << setw(16) << left << "last logon" << " : " << "(no data)" << endl;
	} else {
		struct tm* ptm = localtime((time_t*)&pui3->usri3_last_logon);;
		cout << setw(16) << left << "last logon" << " : " << asctime(ptm);
	}

	if (pui3->usri3_last_logoff == 0) {
		cout << setw(16) << left << "last logoff" << " : " << "(no data)" << endl;
	} else {
		struct tm* ptm = localtime((time_t*)&pui3->usri3_last_logoff);
		cout << setw(16) << left << "last logoff" << " : " << asctime(ptm);
	}

	cout << setw(16) << left << "acct expires" << " : ";
	if (pui3->usri3_acct_expires == TIMEQ_FOREVER)
		cout << "TIMEQ_FOREVER" << endl;
	else
		cout << pui3->usri3_acct_expires << endl;
	
	cout << setw(16) << left << "max storage" << " : "; 
	if (pui3->usri3_max_storage == USER_MAXSTORAGE_UNLIMITED)
		cout << "USER_MAXSTORAGE_UNLIMITED" << endl;
	else
		cout << pui3->usri3_max_storage << endl;

	cout << setw(16) << left << "units per week" << " : " << pui3->usri3_units_per_week << endl;
	
	{
		int i;
		cout << setw(16) << left << "logon hours" << " : ";
		cout << hex << uppercase;
		for (i = 0; i < 21; i++)
			cout << setw(3) << left << (u_int)pui3->usri3_logon_hours[i];
		cout << dec << endl;
	}
	
	cout << setw(16) << left << "bad pw count" << " : " << pui3->usri3_bad_pw_count << endl;
	cout << setw(16) << left << "num logons" << " : " << pui3->usri3_num_logons << endl;
	cout << setw(16) << left << "logon server" << " : " << WStrToStr(pui3->usri3_logon_server) << endl;
	cout << setw(16) << left << "country code" << " : " << pui3->usri3_country_code << endl;
	cout << setw(16) << left << "code page" << " : " << pui3->usri3_code_page << endl;
	cout << setw(16) << left << "user id" << " : " << pui3->usri3_user_id << endl;
	cout << setw(16) << left << "primary group id" << " : " << pui3->usri3_primary_group_id << endl;
	cout << setw(16) << left << "profile" << " : " << WStrToStr(pui3->usri3_profile) << endl;
	cout << setw(16) << left << "home dir drive" << " : " << WStrToStr(pui3->usri3_home_dir_drive) << endl;

	cout << setw(16) << left << "password expired" << " : ";
	if (pui3->usri3_password_expired == 0)
		cout << "not expired" << endl;
	else
		cout << pui3->usri3_password_expired << endl;

	dump_local_group(wstrServer.c_str(), pui3->usri3_name);

	cout << endl;
}

//---------------------------------------------------------------------
//[�T�v]
//	�e��e�[�u���̏�����

int init()
{
	// priv_map�̏�����
	priv_map[USER_PRIV_GUEST] = "Guest";
	priv_map[USER_PRIV_USER] = "User";
	priv_map[USER_PRIV_ADMIN] = "Administrator";

	// flags_map�̏�����
	flags_map[UF_SCRIPT] = "UF_SCRIPT";
	flags_map[UF_ACCOUNTDISABLE] = "UF_ACCOUNTDISABLE";
	flags_map[UF_HOMEDIR_REQUIRED] = "UF_HOMEDIR_REQUIRED";
	flags_map[UF_PASSWD_NOTREQD] = "UF_PASSWD_NOTREQD";
	flags_map[UF_PASSWD_CANT_CHANGE] = "UF_PASSWD_CANT_CHANGE";
	flags_map[UF_LOCKOUT] = "UF_LOCKOUT";
	flags_map[UF_DONT_EXPIRE_PASSWD] = "UF_DONT_EXPIRE_PASSWD";
	flags_map[UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED] = "UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED";
	flags_map[UF_NOT_DELEGATED] = "UF_NOT_DELEGATED";
	flags_map[UF_SMARTCARD_REQUIRED] = "UF_SMARTCARD_REQUIRED";
	flags_map[UF_USE_DES_KEY_ONLY] = "UF_USE_DES_KEY_ONLY";
	flags_map[UF_DONT_REQUIRE_PREAUTH] = "UF_DONT_REQUIRE_PREAUTH";
	flags_map[UF_TRUSTED_FOR_DELEGATION] = "UF_TRUSTED_FOR_DELEGATION";

	// �ȉ��̓�́AXP only�ł���炵���̂����A�l���s���B
//	flags_map[UF_PASSWORD_EXPIRED] = "UF_PASSWORD_EXPIRED";
//	flags_map[UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION] = "UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION";

	flags_map[UF_NORMAL_ACCOUNT] = "UF_NORMAL_ACCOUNT";
	flags_map[UF_TEMP_DUPLICATE_ACCOUNT] = "UF_TEMP_DUPLICATE_ACCOUNT";
	flags_map[UF_WORKSTATION_TRUST_ACCOUNT] = "UF_WORKSTATION_TRUST_ACCOUNT";
	flags_map[UF_SERVER_TRUST_ACCOUNT] = "UF_SERVER_TRUST_ACCOUNT";
	flags_map[UF_INTERDOMAIN_TRUST_ACCOUNT] = "UF_INTERDOMAIN_TRUST_ACCOUNT";

	// aflags_map�̏�����
	aflags_map[0] = "(no flags)";
	aflags_map[AF_OP_PRINT] = "AF_OP_PRINT";
	aflags_map[AF_OP_COMM] = "AF_OP_COMM";
	aflags_map[AF_OP_SERVER] = "AF_OP_SERVER";
	aflags_map[AF_OP_ACCOUNTS] = "AF_OP_ACCOUNTS";

	return 0;
}

//---------------------------------------------------------------------
//[�T�v]
//	�㏈��

void term()
{
	list<regex_t>::iterator i;
	
	for (i = regex_list.begin(); i != regex_list.end(); i++)
		regfree(&(*i));
}

//---------------------------------------------------------------------
//[�T�v]
//	�v���O�����X�^�[�g

int main(int argc, char** argv)
{
	init();		// �e��e�[�u���̏�����

	// �I�v�V�����̊m�F
	int c;
	while ((c = getopt(argc, argv, "s:h?")) != -1) {
		switch (c) {
		case 's':
			wstrServer = StrToWStr(optarg);
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

	// �c��̃p�����[�^�𐳋K�\��������
	regex_t reg;
	if (!(optind < argc)) {
		regcomp(&reg, "^.*$", REG_ICASE);
		regex_list.push_back(reg);
	} else {
		for (;optind < argc; optind++) {
			string str = WildCardToRegExp(argv[optind]);
			if (regcomp(&reg, str.c_str(), REG_ICASE) != 0) {
				term();
				return -1;
			}
			regex_list.push_back(reg);
		}
	}

	USER_INFO_3* pui;
	DWORD read = 0, total = 0, handle = 0;
	NET_API_STATUS napi_ret;
	do {
		// ���[�U�̗�
		napi_ret = NetUserEnum(wstrServer.c_str(), 3, 0, (LPBYTE*)&pui, 4096, &read, &total, &handle);
		if (napi_ret != NERR_Success && napi_ret != ERROR_MORE_DATA) {
			cerr << GetSystemMessage(napi_ret) << endl;
			return -1;
		}

		// �ǂݍ��߂�����������
		for (DWORD i = 0; i < read; i++) {
			string name = WStrToStr(pui[i].usri3_name);

			// ���[�U�������K�\���̂ǂꂩ�ƃ}�b�`���邩�`�F�b�N
			list<regex_t>::iterator j;
			for (j = regex_list.begin(); j != regex_list.end(); ++j) {
				regmatch_t match[8];
				if (regexec(&(*j), name.c_str(), 8, match, 0) == 0) {
					userdump(&pui[i]);	// �}�b�`����Ώ��\��
					break;
				}
			}
		}

		NetApiBufferFree(pui);

		// �f�[�^���������J��Ԃ�
	} while (napi_ret == ERROR_MORE_DATA);

	term();	// �㏈��(�o�b�t�@�j���Ȃ�)

	return 0;
}
