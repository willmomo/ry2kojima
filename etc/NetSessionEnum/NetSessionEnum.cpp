#pragma warning(disable:4786)

#include <windows.h>
#include <lm.h>

#include <iostream>
#include <iomanip>
#include <strstream>

#include <time.h>

#include "getopt.h"
#include "sub.h"

//---------------------------------------------------------------------
//	グローバル変数

string g_strServerName;
string g_strClientName;
string g_strUserName;
DWORD g_dwInterval;

//---------------------------------------------------------------------
//[概要]
//	使用法を表示して終了

void usage(int status)
{
	cout <<
		"NetSessionEnum ver.0.01 build(" << __DATE__ << ")\n"
		"usage: NetSessionEnum [-s servername] [-c clientname] [-u username] [-i interval]" <<
		endl;

	exit(status);
}

//---------------------------------------------------------------------
//[概要]
//	累積秒をH:M:Sに分解

string SecToHMS(DWORD sec)
{
	ostrstream ss;

	ss << setw(2) << (sec / 3600) << ":";	// 時
	sec %= 3600;
	ss << setfill('0');						// non-zero-suppress
	ss << setw(2) << (sec / 60) << ":";		// 分
	sec %= 60;
	ss << setw(2) << sec << ends;			// 秒

	return string(ss.str());
}

//---------------------------------------------------------------------
//[概要]
//	SESSION_INFO_2のタイトル表示

void PrintSessionInfo2Title()
{
	cout << setw(8) << left << "ユーザー" << " ";
	cout << setw(15) << left << "コンピュータ" << " ";
	cout << setw(4) << right << "ﾌｧｲﾙ" << " ";
	cout << setw(8) << right << "接続時間" << " ";
	cout << setw(8) << right << "アイドル" << " ";
	cout << setw(5) << right << "Guest" << " ";
	cout << setw(6) << right << "非暗号" << " ";
	cout << left << "タイプ";
	cout << endl;
}

//---------------------------------------------------------------------
//[概要]
//	SESSION_INFO_2の表示

void PrintSessionInfo2(SESSION_INFO_2* psi2)
{
	cout << setw(8) << left << WStrToStr((wchar_t*)psi2->sesi2_username) << " ";
	cout << setw(15) << left << WStrToStr((wchar_t*)psi2->sesi2_cname) << " ";
	cout << setw(4) << right << psi2->sesi2_num_opens << " ";
	cout << setw(8) << right << SecToHMS(psi2->sesi2_time) << " ";
	cout << setw(8) << right << SecToHMS(psi2->sesi2_idle_time) << " ";
	cout << setw(5) << right << ((psi2->sesi2_user_flags & SESS_GUEST) ? "Yes" : "") << " ";
	cout << setw(6) << right << ((psi2->sesi2_user_flags & SESS_NOENCRYPTION) ? "Yes" : "") << " ";
	cout << left << WStrToStr((wchar_t*)psi2->sesi2_cltype_name);
	cout << endl;
}

//---------------------------------------------------------------------
//[概要]
//	プログラム･スタート

int main(int argc, char** argv)
{
	// オプションの確認
	int c;
	while ((c = getopt(argc, argv, "s:c:u:i:h?")) != -1) {
		switch (c) {
		case 's':
			g_strServerName.assign(optarg);
			break;
		case 'c':
			g_strClientName.assign(optarg);
			break;
		case 'u':
			g_strUserName.assign(optarg);
			break;
		case 'i':
			g_dwInterval = strtoul(optarg, NULL, 0);
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

	do {
		if (g_dwInterval) {
			time_t t = time(NULL);
			cout << ctime(&t);
		}

		PrintSessionInfo2Title();

		SESSION_INFO_2* pbuf;
		NET_API_STATUS ret;
		DWORD dwRead, dwTotal, dwHandle = 0, dwTotalSession = 0;
		do {
#if _MSC_VER <= 1200	// VC++6以下
			// ヘッダの宣言の関係で、char* にキャストする必要があった
			ret = NetSessionEnum(
				(char*)(g_strServerName.empty() ? NULL : StrToWStr(g_strServerName.c_str()).c_str()),
				(char*)(g_strClientName.empty() ? NULL : StrToWStr(g_strClientName.c_str()).c_str()),
				(char*)(g_strUserName.empty() ? NULL : StrToWStr(g_strUserName.c_str()).c_str()),
				2, (u_char**)&pbuf, MAX_PREFERRED_LENGTH,
				&dwRead, &dwTotal, &dwHandle);
#else
			ret = NetSessionEnum(
				(LPWSTR)(g_strServerName.empty() ? NULL : StrToWStr(g_strServerName.c_str()).c_str()),
				(LPWSTR)(g_strClientName.empty() ? NULL : StrToWStr(g_strClientName.c_str()).c_str()),
				(LPWSTR)(g_strUserName.empty() ? NULL : StrToWStr(g_strUserName.c_str()).c_str()),
				2, (u_char**)&pbuf, MAX_PREFERRED_LENGTH,
				&dwRead, &dwTotal, &dwHandle);
#endif
			// データが取得できた？
			if (ret == NERR_Success || ret == ERROR_MORE_DATA) {
				for (DWORD n = 0; n < dwRead; n++)
					PrintSessionInfo2(&pbuf[n]);

				NetApiBufferFree(pbuf);
			} else {
				cerr << GetSystemMessage(ret) << endl;
				return -1;
			}

			dwTotalSession += dwRead;
		} while (ret == ERROR_MORE_DATA);

		cout << "全セッション数: " << dwTotalSession << "\n" << endl;

		if (g_dwInterval != 0)
			Sleep(g_dwInterval * 1000);

	} while (g_dwInterval);

	return 0;
}
