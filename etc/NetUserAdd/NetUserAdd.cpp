#include <windows.h>
#include <lmaccess.h>
#include <lmerr.h>
#include <stdio.h>

typedef struct {
	char name[96];		// /N:name
	char pass[96];		// /P:password
	char group[96];		// /G:group
	bool dont_expire;	// /E
} OPTIONS;

OPTIONS g_opt;

void usage(int stat)
{
	puts("使用法：NetUserAdd /N<名前> [/P<パスワード>][/G<グループ>][/E]");
	exit(stat);
}

void chkarg(int argc, char** argv)
{
	int i;
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '/' || argv[i][0] == '-') {
			int c = toupper(argv[i][1]);

			if (c == 'N') {
				lstrcpy(g_opt.name, &argv[i][2]);
			} else if (c == 'P') {
				lstrcpy(g_opt.pass, &argv[i][2]);
			} else if (c == 'G') {
				lstrcpy(g_opt.group, &argv[i][2]);
			} else if (c == 'E') {
				g_opt.dont_expire = true;
			} else if (c == '?') {
				usage(EXIT_SUCCESS);
			} else {
				usage(EXIT_FAILURE);
			}
		} else {
			usage(EXIT_FAILURE);
		}
	}

	// /Nが未指定のときはエラー
	if (lstrlen(g_opt.name) == 0) {
		usage(EXIT_FAILURE);
	}

	// /Gが未指定のときはUsersにしておく
	if (lstrlen(g_opt.group) == 0) {
		lstrcpy(g_opt.group, "Guests");
	}
}

bool CharToWChar(const char* src, wchar_t* dst, int len)
{
	if (MultiByteToWideChar(CP_ACP, 0, src, -1, dst, len) == 0) {
		return false;
	}

	return true;
}

bool WCharToChar(const wchar_t* src, char* dst, int len)
{
	if (WideCharToMultiByte(CP_ACP, 0, src, -1, dst, len, NULL, NULL) == 0) {
		return false;
	}

	return true;
}

int main(int argc, char** argv)
{
	// ---- オプション解析 ----
	chkarg(argc, argv);

	// ---- ユーザー追加 ----
	wchar_t wname[96];
	wchar_t wpass[96];
	USER_INFO_1 ui;

	CharToWChar(g_opt.name, wname, sizeof(wname));
	CharToWChar(g_opt.pass, wpass, sizeof(wpass));
	memset(&ui, 0, sizeof(ui));
	ui.usri1_name = wname;
	ui.usri1_password = wpass;
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_flags = (g_opt.dont_expire) ? UF_DONT_EXPIRE_PASSWD : 0;

	NET_API_STATUS ret;
	ret = NetUserAdd(NULL, 1, (LPBYTE)&ui, NULL);
	if (ret != NERR_Success) {
		printf("NetUserAdd error.\n");
		return -1;
	}

	// ---- ユーザーアカウントのSIDを取得 ----
	char sid_buf[96];
	PSID psid = (PSID)sid_buf;
	char dom[96];
	DWORD dwSid = sizeof(sid_buf);
	DWORD dwDom = sizeof(dom);
	SID_NAME_USE snu;
	if (LookupAccountName(NULL, g_opt.name, psid, &dwSid, dom, &dwDom, &snu) == FALSE) {
		printf("LookupAccountName error.\n");
		return -1;
	}

	// ---- ユーザーを指定グループに追加 ----
	wchar_t wgroup[96];
	LOCALGROUP_MEMBERS_INFO_0 lmi;

	CharToWChar(g_opt.group, wgroup, sizeof(wgroup));
	lmi.lgrmi0_sid = psid;
	if (NetLocalGroupAddMembers(NULL, wgroup, 0, (LPBYTE)&lmi, 1) != NERR_Success) {
		printf("NetLocalGroupAddMembers error.\n");
		return -1;
	}

	return 0;
}
