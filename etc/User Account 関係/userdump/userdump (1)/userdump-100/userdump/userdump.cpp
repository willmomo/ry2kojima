//---------------------------------------------------------------------
// ユーザ情報表示プログラム

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
// マクロ/定数

// VC6で未定義のflags達
#define UF_ENCRYPTED_TEXT_PASSWORD_ALLOWED	0x000080
#define UF_NOT_DELEGATED					0x100000
#define UF_SMARTCARD_REQUIRED				0x040000
#define UF_USE_DES_KEY_ONLY					0x200000
#define UF_DONT_REQUIRE_PREAUTH				0x400000
#define UF_TRUSTED_FOR_DELEGATION			0x080000

#define SEC_1MIN	60				// 1分は60秒
#define SEC_1HOUR	(60*SEC_1MIN)	// 1時間は60分
#define SEC_1DAY	(24*SEC_1HOUR)	// 1日は24時間

typedef map<int, const char*>	PRIV_MAP, FLAGS_MAP, AFLAGS_MAP;

//---------------------------------------------------------------------
// グローバル変数

PRIV_MAP priv_map;
FLAGS_MAP flags_map;
AFLAGS_MAP aflags_map;
wstring wstrServer;
list<regex_t> regex_list;

//---------------------------------------------------------------------
//[概要]
//	使用法を表示して終了

void usage(int status)
{
	cout << "userdump ver.1.00 (build:" << __DATE__ << ")" << endl;
	cout << "usage: userdump [-s server] [user [...]]" << endl;

	exit(status);
}

//---------------------------------------------------------------------
//[概要]
//	ローカルグループのダンプ

void dump_local_group(const wchar_t* server, const wchar_t* name)
{
	DWORD dwRead, dwIndex;
	NET_API_STATUS ret;
	LOCALGROUP_USERS_INFO_0* plui0;

	// ユーザー名からグループを取得
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
//[概要]
//	ユーザ情報の表示

void userdump(const USER_INFO_3* pui3)
{
	cout << setw(16) << left << "name" << " : " << WStrToStr(pui3->usri3_name) << endl;
	cout << setw(16) << left << "password" << " : " << "<unknown>" << endl;

	cout << setw(16) << left << "password age" << " : ";
	DWORD password_age = pui3->usri3_password_age;
	if (password_age / SEC_1DAY) {
		cout << (password_age / SEC_1DAY) << "日 ";
		password_age %= SEC_1DAY;
	}
	if (password_age / SEC_1HOUR) {
		cout << (password_age / SEC_1HOUR) << "時間 ";
		password_age %= SEC_1HOUR;
	}
	if (password_age / SEC_1MIN) {
		cout << (password_age / SEC_1MIN) << "分 ";
		password_age %= SEC_1MIN;
	}
	cout << password_age << "秒 " << endl;

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
//[概要]
//	各種テーブルの初期化

int init()
{
	// priv_mapの初期化
	priv_map[USER_PRIV_GUEST] = "Guest";
	priv_map[USER_PRIV_USER] = "User";
	priv_map[USER_PRIV_ADMIN] = "Administrator";

	// flags_mapの初期化
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

	// 以下の二つは、XP onlyであるらしいのだが、値が不明。
//	flags_map[UF_PASSWORD_EXPIRED] = "UF_PASSWORD_EXPIRED";
//	flags_map[UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION] = "UF_TRUSTED_TO_AUTHENTICATE_FOR_DELEGATION";

	flags_map[UF_NORMAL_ACCOUNT] = "UF_NORMAL_ACCOUNT";
	flags_map[UF_TEMP_DUPLICATE_ACCOUNT] = "UF_TEMP_DUPLICATE_ACCOUNT";
	flags_map[UF_WORKSTATION_TRUST_ACCOUNT] = "UF_WORKSTATION_TRUST_ACCOUNT";
	flags_map[UF_SERVER_TRUST_ACCOUNT] = "UF_SERVER_TRUST_ACCOUNT";
	flags_map[UF_INTERDOMAIN_TRUST_ACCOUNT] = "UF_INTERDOMAIN_TRUST_ACCOUNT";

	// aflags_mapの初期化
	aflags_map[0] = "(no flags)";
	aflags_map[AF_OP_PRINT] = "AF_OP_PRINT";
	aflags_map[AF_OP_COMM] = "AF_OP_COMM";
	aflags_map[AF_OP_SERVER] = "AF_OP_SERVER";
	aflags_map[AF_OP_ACCOUNTS] = "AF_OP_ACCOUNTS";

	return 0;
}

//---------------------------------------------------------------------
//[概要]
//	後処理

void term()
{
	list<regex_t>::iterator i;
	
	for (i = regex_list.begin(); i != regex_list.end(); i++)
		regfree(&(*i));
}

//---------------------------------------------------------------------
//[概要]
//	プログラムスタート

int main(int argc, char** argv)
{
	init();		// 各種テーブルの初期化

	// オプションの確認
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

	// 残りのパラメータを正規表現化する
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
		// ユーザの列挙
		napi_ret = NetUserEnum(wstrServer.c_str(), 3, 0, (LPBYTE*)&pui, 4096, &read, &total, &handle);
		if (napi_ret != NERR_Success && napi_ret != ERROR_MORE_DATA) {
			cerr << GetSystemMessage(napi_ret) << endl;
			return -1;
		}

		// 読み込めた分処理する
		for (DWORD i = 0; i < read; i++) {
			string name = WStrToStr(pui[i].usri3_name);

			// ユーザ名が正規表現のどれかとマッチするかチェック
			list<regex_t>::iterator j;
			for (j = regex_list.begin(); j != regex_list.end(); ++j) {
				regmatch_t match[8];
				if (regexec(&(*j), name.c_str(), 8, match, 0) == 0) {
					userdump(&pui[i]);	// マッチすれば情報表示
					break;
				}
			}
		}

		NetApiBufferFree(pui);

		// データがある限り繰り返す
	} while (napi_ret == ERROR_MORE_DATA);

	term();	// 後処理(バッファ破棄など)

	return 0;
}
