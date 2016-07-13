//-----------------------------------------------------------------------------
// メインフレーム
//-----------------------------------------------------------------------------
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "mainFrm.h"
#include "kjmlib-20130512/kjmlib.h"
#include <process.h>
#include "resource.h"

#include "cron_core.h"

#define DEF_DAEMON			0
#define DEF_SERVER			_T("")		// "ntp.nict.jp"
#define DEF_INTERVAL		300		// 5分(300秒)
#define DEF_NOADJUST		0
#define DEF_ALLOWABLEOFFSET	1		// 1秒
#define DEF_ALLOWABLEMAXOFFSET

#define WM_MYNOTIFYICON	(WM_APP + 1)

static HANDLE s_hThread = NULL;

//-----------------------------------------------------------------------------
// sntpのスレッドを起動する
//-----------------------------------------------------------------------------
static void start_crond_thread() {
	TCHAR iniFile[MAX_PATH];
	GetModuleFileName(NULL, iniFile, MAX_PATH);
	PathRenameExtension(iniFile, _T(".ini"));

	static cron_options opt;
	opt.m_debug_notrun = GetPrivateProfileInt(_T("debug"), _T("NotRun"), 0, iniFile);

	kjm::GetLog().write(_T("[debug] NotRun = %d (if not zero, don't run program)"), opt.m_debug_notrun);
	/*
	opt.m_daemon_mode = GetPrivateProfileInt(_T("option"), _T("daemon"), DEF_DAEMON, iniFile);
	GetPrivateProfileString(_T("option"), _T("server"), DEF_SERVER, opt.m_ntp_server, sizeof(opt.m_ntp_server), iniFile);
	opt.m_noadjust = GetPrivateProfileInt(_T("option"), _T("noadjust"), DEF_NOADJUST, iniFile);
	opt.m_interval = GetPrivateProfileInt(_T("option"), _T("interval"), DEF_INTERVAL, iniFile);
	opt.m_allowable_offset = GetPrivateProfileInt(_T("option"), _T("AllowableOffset"), 1, iniFile);
	opt.m_allowable_delay = GetPrivateProfileInt(_T("option"), _T("AllowableDelay"), 1, iniFile);
	opt.m_allowable_max_offset = GetPrivateProfileInt(_T("option"), _T("AllowableMaxOffset"), 3600, iniFile);

	kjm::GetLog().write(_T("option read from '%s'"), iniFile);
	kjm::GetLog().write(_T("[option] daemon             = %d"), opt.m_daemon_mode);
	kjm::GetLog().write(_T("[option] server             = %s"), opt.m_ntp_server);
	kjm::GetLog().write(_T("[option] noadjust           = %d"), opt.m_noadjust);
	kjm::GetLog().write(_T("[option] interval           = %d (sec)"), opt.m_interval);
	kjm::GetLog().write(_T("[option] AllowableOffset    = %d (sec)"), opt.m_allowable_offset);
	kjm::GetLog().write(_T("[option] AllowableDelay     = %d (sec)"), opt.m_allowable_delay);
	*/
	unsigned thrdid = 0;
	s_hThread = (HANDLE)_beginthreadex(NULL, 0, crond_thread, &opt, 0, &thrdid);
	kjm::GetLog().write(_T("crond thread start(%04x)"), thrdid);
}

//-----------------------------------------------------------------------------
// crond のスレッドを終了する
//-----------------------------------------------------------------------------
static void stop_crond_thread() {
	if (s_hThread) {
		crond_thread_end();
		kjm::GetLog().write(_T("crond thread stop."));
		WaitForSingleObject(s_hThread, 10 * 1000);
		kjm::GetLog().write(_T("crond thread stopped."));

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
		//
	} else if (wID == IDM_POPUP_OPENLOG) {
		kjm::GetLog().write(_T("ログフォルダを開きます。"));

		_tstring szPath = kjm::GetLog().get_log_path();
		ShellExecute(NULL, _T("open"), szPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
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
	lstrcpy(nid.szTip, _T("cronw"));
	Shell_NotifyIcon(NIM_ADD, &nid);

	// sntpスレッドを実行
	start_crond_thread();

	kjm::GetLog().write(_T("スレッド起動: 1 sec wait"));
	Sleep(1000);

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
	stop_crond_thread();

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
	TCHAR iniFile[MAX_PATH];
	TCHAR debugCmd[1024];

	GetModuleFileName(NULL, iniFile, MAX_PATH);
	PathRenameExtension(iniFile, _T(".ini"));

	GetPrivateProfileString(_T("debug"), _T("command"), _T(""), debugCmd, 1024, iniFile);

	if (lstrcmp(debugCmd, _T("restart")) == 0) {
		// スレッドの再起動
		kjm::GetLog().write(_T("[debug] command=%s 検知：スレッド再起動"), debugCmd);
		WritePrivateProfileString(_T("debug"), _T("command"), NULL, iniFile);

		stop_crond_thread();
		start_crond_thread();
	} else if (lstrcmp(debugCmd, _T("exit")) == 0) {
		// プログラムの終了
		kjm::GetLog().write(_T("[debug] command=%s 検知：アプリケーション終了"), debugCmd);
		WritePrivateProfileString(_T("debug"), _T("command"), NULL, iniFile);

		DestroyWindow(hwnd);
	} else if (lstrcmp(debugCmd, _T("update")) == 0) {
		kjm::GetLog().write(_T("[debug] command=%s 検知：アプリケーション更新"), debugCmd);
		WritePrivateProfileString(_T("debug"), _T("command"), NULL, iniFile);

		TCHAR updater[_MAX_PATH];
		GetModuleFileName(NULL, updater, MAX_PATH);
		PathRemoveFileSpec(updater);
		PathAppend(updater, _T("update.exe"));

		STARTUPINFO startupInfo = {sizeof(startupInfo)};
		PROCESS_INFORMATION procInfo = {NULL};

		startupInfo.dwFlags = STARTF_USESHOWWINDOW;
		startupInfo.wShowWindow = SW_SHOW;

		kjm::GetLog().write(_T("'%s' を起動します。"), updater);

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
			kjm::GetLog().write(_T("起動に成功しました。"));
			CloseHandle(procInfo.hThread);
			CloseHandle(procInfo.hProcess);
		} else {
			kjm::GetLog().write(_T("起動に失敗しました: error %d"), GetLastError());
		}

		DestroyWindow(hwnd);
	} else {
		// 何もないときは、スレッドの状態監視
		DWORD exitCode = 0;
		if ((s_hThread != NULL) && (GetExitCodeThread(s_hThread, &exitCode) != FALSE)) {
			if (exitCode != STILL_ACTIVE) {
				// スレッドの再起動
				kjm::GetLog().write(_T("なぜか、スレッドが止まっています(終了コード %d)。再起動を試みます。"), exitCode);

				stop_crond_thread();
				start_crond_thread();
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

				/*
				{
					TCHAR work[1024];
					TCHAR iniFile[MAX_PATH];
					GetModuleFileName(NULL, iniFile, MAX_PATH);
					PathRenameExtension(iniFile, _T(".ini"));
					GetPrivateProfileString(_T("option"), _T("server"), DEF_SERVER, work, sizeof(work), iniFile);
					if (lstrlen(work) == 0) {
						EnableMenuItem(hMenuPopup, 0, MF_BYPOSITION | MF_GRAYED);
					}
				}
				*/

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
	HWND hwnd = CreateWindow(_T("SNTPW_MAINFRM"), _T("cronw"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		return FALSE;
	}

	//ShowWindow(hwnd, nCmdShow);

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
	wc.lpszClassName = _T("SNTPW_MAINFRM");

	return RegisterClass(&wc);
}
