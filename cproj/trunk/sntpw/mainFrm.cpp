//-----------------------------------------------------------------------------
// メインフレーム
//-----------------------------------------------------------------------------
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "mainFrm.h"
#include "sntpd.h"
#include "kjmlog.h"
#include <process.h>
#include "resource.h"

#define DEF_DAEMON			0
#define DEF_SERVER			""		// "ntp.nict.jp"
#define DEF_INTERVAL		300		// 5分(300秒)
#define DEF_NOADJUST		0
#define DEF_ALLOWABLEOFFSET	1		// 1秒
#define DEF_ALLOWABLEMAXOFFSET

#define WM_MYNOTIFYICON	(WM_APP + 1)

static HANDLE s_hThread = NULL;

//-----------------------------------------------------------------------------
// sntpのスレッドを起動する
//-----------------------------------------------------------------------------
static void start_sntp_thread() {
	char iniFile[MAX_PATH];
	GetModuleFileName(NULL, iniFile, MAX_PATH);
	PathRenameExtension(iniFile, ".ini");

	static ntp_options opt;
	opt.m_daemon_mode = GetPrivateProfileInt("option", "daemon", DEF_DAEMON, iniFile);
	GetPrivateProfileString("option", "server", DEF_SERVER, opt.m_ntp_server, sizeof(opt.m_ntp_server), iniFile);
	opt.m_noadjust = GetPrivateProfileInt("option", "noadjust", DEF_NOADJUST, iniFile);
	opt.m_interval = GetPrivateProfileInt("option", "interval", DEF_INTERVAL, iniFile);
	opt.m_allowable_offset = GetPrivateProfileInt("option", "AllowableOffset", 1, iniFile);
	opt.m_allowable_delay = GetPrivateProfileInt("option", "AllowableDelay", 1, iniFile);
	opt.m_allowable_max_offset = GetPrivateProfileInt("option", "AllowableMaxOffset", 3600, iniFile);

	KLogWrite("option read from '%s'", iniFile);
	KLogWrite("[option] daemon             = %d", opt.m_daemon_mode);
	KLogWrite("[option] server             = %s", opt.m_ntp_server);
	KLogWrite("[option] noadjust           = %d", opt.m_noadjust);
	KLogWrite("[option] interval           = %d (sec)", opt.m_interval);
	KLogWrite("[option] AllowableOffset    = %d (sec)", opt.m_allowable_offset);
	KLogWrite("[option] AllowableDelay     = %d (sec)", opt.m_allowable_delay);
	KLogWrite("[option] AllowableMaxOffset = %d (sec)", opt.m_allowable_max_offset);

	unsigned thrdid = 0;
	if (opt.m_daemon_mode != 0) {
		KLogWrite("sntpd として起動");
		s_hThread = (HANDLE)_beginthreadex(NULL, 0, sntpd_thread, &opt, 0, &thrdid);
	} else {
		KLogWrite("sntp として起動");
		s_hThread = (HANDLE)_beginthreadex(NULL, 0, sntp_thread, &opt, 0, &thrdid);
	}
	KLogWriteE("sntp thread start(%04x)", thrdid);
}

//-----------------------------------------------------------------------------
// sntpのスレッドを終了する
//-----------------------------------------------------------------------------
static void stop_sntp_thread() {
	if (s_hThread) {
		sntp_thread_end();
		KLogWriteE("sntp thread stop.");
		WaitForSingleObject(s_hThread, 10 * 1000);
		KLogWriteE("sntp thread stopped.");

		CloseHandle(s_hThread);
		s_hThread = NULL;
	}
}

//-----------------------------------------------------------------------------
// WM_COMMAND メッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wNotifyCode = HIWORD(wParam);
	WORD wID = LOWORD(wParam);
	HWND hwndCtl = (HWND)lParam;

	LRESULT nRet = 0;

	if (wID == IDM_POPUP_EXIT) {
		DestroyWindow(hwnd);
	} else if (wID == IDM_POPUP_ADJUST) {
		KLogWrite("ただちに、時計合わせを実行させます。");
		sntp_thread_adjust();
	} else if (wID == IDM_POPUP_OPENLOG) {
		KLogWrite("ログフォルダを開きます。");

		char szPath[MAX_PATH];
		KLogGetLogPath(szPath);
		ShellExecute(NULL, "open", szPath, NULL, NULL, SW_SHOWNORMAL);
	} else {
		nRet = DefWindowProc(hwnd, WM_COMMAND, wParam, lParam);
	}

	return nRet;
}

//-----------------------------------------------------------------------------
// WM_CREATE メッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	// タスクトレイアイコンの追加
	NOTIFYICONDATA nid = {sizeof(nid), hwnd, 1};
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nid.hIcon = (HICON)LoadImage(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDI_APPL), IMAGE_ICON, 16, 16, 0);
	nid.uCallbackMessage = WM_MYNOTIFYICON;
	strcpy(nid.szTip, "sntpw");
	Shell_NotifyIcon(NIM_ADD, &nid);

	// sntpスレッドを実行
	start_sntp_thread();

	// iniファイル監視タイマを起動
	SetTimer(hwnd, 1, 10 * 1000, NULL);

	return 0;
}

//-----------------------------------------------------------------------------
// WM_DESTROY メッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnDestroy(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	// iniファイル監視タイマを停止
	KillTimer(hwnd, 1);

	// スレッド終了
	stop_sntp_thread();

	// タスクトレイアイコンの削除
	NOTIFYICONDATA nid = {sizeof(nid), hwnd, 1};
	Shell_NotifyIcon(NIM_DELETE, &nid);

	PostQuitMessage(0);
	return 0;
}

//-----------------------------------------------------------------------------
// WM_TIMER メッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnTimer(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	char iniFile[MAX_PATH];
	char debugCmd[1024];

	GetModuleFileName(NULL, iniFile, MAX_PATH);
	PathRenameExtension(iniFile, ".ini");

	GetPrivateProfileString("debug", "command", "", debugCmd, 1024, iniFile);

	if (strcmp(debugCmd, "restart") == 0) {
		// スレッドの再起動
		KLogWrite("[debug] command=%s 検知：スレッド再起動", debugCmd);
		WritePrivateProfileString("debug", "command", NULL, iniFile);

		stop_sntp_thread();
		start_sntp_thread();
	} else if (strcmp(debugCmd, "exit") == 0) {
		// プログラムの終了
		KLogWrite("[debug] command=%s 検知：アプリケーション終了", debugCmd);
		WritePrivateProfileString("debug", "command", NULL, iniFile);

		DestroyWindow(hwnd);
	} else if (strcmp(debugCmd, "update") == 0) {
		KLogWrite("[debug] command=%s 検知：アプリケーション更新", debugCmd);
		WritePrivateProfileString("debug", "command", NULL, iniFile);

		char updater[_MAX_PATH];
		GetModuleFileName(NULL, updater, MAX_PATH);
		PathRemoveFileSpec(updater);
		PathAppend(updater, "update.exe");

		STARTUPINFO startupInfo = {sizeof(startupInfo)};
		PROCESS_INFORMATION procInfo = {NULL};

		startupInfo.dwFlags = STARTF_USESHOWWINDOW;
		startupInfo.wShowWindow = SW_SHOW;

		KLogWrite("'%s' を起動します。", updater);

		BOOL bRet = CreateProcess(
			NULL,
			updater,
			NULL,
			NULL,
			TRUE,
			0,
			NULL,
			NULL,
			&startupInfo,
			&procInfo);
		if (bRet) {
			KLogWrite("起動に成功しました。");
			CloseHandle(procInfo.hThread);
			CloseHandle(procInfo.hProcess);
		} else {
			KLogWrite("起動に失敗しました: error %d", GetLastError());
		}

		DestroyWindow(hwnd);
	} else {
		// 何もないときは、スレッドの状態監視
		DWORD exitCode = 0;
		if ((s_hThread != NULL) && (GetExitCodeThread(s_hThread, &exitCode) != FALSE)) {
			if (exitCode != STILL_ACTIVE) {
				// スレッドの再起動
				KLogWrite("なぜか、スレッドが止まっています(終了コード %d)。再起動を試みます。", exitCode);

				stop_sntp_thread();
				start_sntp_thread();
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// WM_MYNOTIFYICON メッセージ処理
//-----------------------------------------------------------------------------
static LRESULT OnMyNotifyIcon(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	UINT uID = (UINT)wParam;
	UINT uMouseMsg = (UINT)lParam;

	if (uMouseMsg == WM_RBUTTONUP) {
		HMENU hMenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUPMENU));
		if (hMenu) {
			HMENU hMenuPopup = GetSubMenu(hMenu, 0);
			if (hMenuPopup) {
				POINT pt;
				GetCursorPos(&pt);

				{
					char work[1024];
					char iniFile[MAX_PATH];
					GetModuleFileName(NULL, iniFile, MAX_PATH);
					PathRenameExtension(iniFile, ".ini");
					GetPrivateProfileString("option", "server", DEF_SERVER, work, sizeof(work), iniFile);
					if (strlen(work) == 0) {
						EnableMenuItem(hMenuPopup, 0, MF_BYPOSITION | MF_GRAYED);
					}
				}

				SetForegroundWindow(hwnd);
				TrackPopupMenu(hMenuPopup, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
				PostMessage(hwnd, WM_NULL, 0, 0);
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// ウィンドウプロシージャ
//-----------------------------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		return OnCommand(hwnd, wParam, lParam);

	case WM_CREATE:
		return OnCreate(hwnd, wParam, lParam);

	case WM_DESTROY:
		return OnDestroy(hwnd, wParam, lParam);

	case WM_TIMER:
		return OnTimer(hwnd, wParam, lParam);

	case WM_MYNOTIFYICON:
		return OnMyNotifyIcon(hwnd, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

//-----------------------------------------------------------------------------
// メインフレームの作成
//-----------------------------------------------------------------------------
BOOL MainFrm_CreateWindow(HINSTANCE hInstance, int nCmdShow) {
	HWND hwnd = CreateWindow("SNTPW_MAINFRM", "sntpw", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return FALSE;
	}

	/// ShowWindow(hwnd, nCmdShow);

	return TRUE;
}

//-----------------------------------------------------------------------------
// メインフレームの登録
//-----------------------------------------------------------------------------
BOOL MainFrm_RegisterClass(HINSTANCE hInstance) {
	WNDCLASS wc;
	wc.style = 0;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPL));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "SNTPW_MAINFRM";

	return RegisterClass(&wc);
}
