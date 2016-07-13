//////////////////////////////////////////////////////////////////////
//	[概要]
//	プロセス管理クラス
//
//	[作成者]
//	Ryota Kojima
//
//	[作成日
//	2003/10/15

#if !defined(__RKPROCESS_H)
#define __RKPROCESS_H

//////////////////////////////////////////////////////////////////////
//	ヘッダ

#include <windows.h>
#include <windowsx.h>

//////////////////////////////////////////////////////////////////////
//	[概要]
//	プロセス管理クラス

class rkProcess
{
private:
	HWND m_hwnd;				// GetProcessWindow関数で使用
	static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

	STARTUPINFO m_si;					// スタートアップ情報
	PROCESS_INFORMATION m_pi;			// プロセス情報
	TCHAR m_szCommandLine[MAX_PATH];	// 実行コマンドライン記録用
										// Create関数実行後有効

public:
	// デフォルト コンストラクタ
	rkProcess()
	{
		ZeroMemory(&m_si, sizeof(m_si));
		ZeroMemory(&m_pi, sizeof(m_pi));
		ZeroMemory(&m_szCommandLine, sizeof(m_szCommandLine));
	};
	// デストラクタ
	virtual ~rkProcess() { Close(); };

public:
	// スタートアップ情報をクリアする
	VOID ClearSI() { ZeroMemory(&m_si, sizeof(m_si)); };
	// スタートアップ情報(wShowWindow)を取得する
	WORD GetSIShowWindow() const { return m_si.wShowWindow; };
	// スタートアップ情報(wShowWindow)を設定する
	VOID SetSIShowWindow(WORD newShowWindow)
	{
		m_si.wShowWindow = newShowWindow;
		m_si.dwFlags |= STARTF_USESHOWWINDOW;
	};
	// スタートアップ情報(wShowWindow)をクリア(未使用に)する
	VOID ClearSIShowWindow()
	{
		m_si.wShowWindow = 0;
		m_si.dwFlags &= ~STARTF_USESHOWWINDOW;
	};

	// プロセスID取得
	DWORD GetProcessId() const { return m_pi.dwProcessId; };
	// 実行ファイルのコマンドライン取得
	LPCTSTR GetCommandLine() const { return m_szCommandLine; };

public:
	// プロセスの実行
	virtual BOOL Create(LPTSTR lpCommandLine)
	{
		if (m_pi.hProcess)	return FALSE;	// すでに何か実行している
		lstrcpy(m_szCommandLine, lpCommandLine);
		m_si.cb = sizeof(m_si);
		return CreateProcess(NULL, lpCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);
	};
	// プロセスハンドルを閉じる。プロセスの終了ではないので注意。
	// プロセスの終了は、プロセス間の取り決めによる。
	VOID Close()
	{
		if (m_pi.hThread)	CloseHandle(m_pi.hThread);
		if (m_pi.hProcess)	CloseHandle(m_pi.hProcess);
		ZeroMemory(&m_pi, sizeof(m_pi));
	};
	// プロセスの終了を待つ
	virtual DWORD WaitForTermination(DWORD dwMilliseconds = INFINITE)
	{
		return ::WaitForSingleObject(m_pi.hProcess, dwMilliseconds);
	};
	// プロセスの起動完了を待つ
	virtual DWORD WaitForInputIdle(DWORD dwMilliseconds = INFINITE)
	{
		return ::WaitForInputIdle(m_pi.hProcess, dwMilliseconds);
	};
	// プロセスに関連付けられたウィンドウを返す
	HWND GetProcessWindow()
	{
		if (!m_pi.hProcess)	return NULL;
		m_hwnd = NULL;
		EnumWindows(rkProcess::EnumWindowsProc, reinterpret_cast<LPARAM>(this));
		return m_hwnd;
	};
};

#endif
