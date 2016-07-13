//
// プロセス実行に関するコード
//
// process : Version 0.2015.12.23
//
// r.kojima
//
#pragma once

#include "kjmbase.h"

namespace kjm {
	//
	//
	//
	class process {
	public:
		process();
		virtual ~process() { this->Close(); }

	public:
		// スタートアップ情報をクリアする
		void ClearSI() { memset(&m_si, 0, sizeof(m_si)); }

		// スタートアップ情報(wShowWindow)を取得する
		WORD get_ShowWindow() const { return m_si.wShowWindow; }

		// スタートアップ情報(wShowWindow)を設定する
		// ShowWindow API の引数と同じ
		// SW_SHOWDEFAULT, SW_HIDE など
		void set_ShowWindow(WORD newShowWindow);

		// スタートアップ情報(wShowWindow)をクリア(未使用に)する
		void clear_ShowWindow();

		// プロセスID取得
		DWORD GetProcessId() const { return m_pi.dwProcessId; }

		// 実行ファイルのコマンドライン取得
		kjm::_tstring GetCommandLine() const { return m_szCommandLine; }

		// プロセスの終了コードを取得
		DWORD getExitCode();

		// プロセスハンドルが有効かどうか調べる
		bool IsHandleAvailable() { return (m_pi.hThread != NULL && m_pi.hProcess != NULL) ? true : false; }

		// プロセスが動作中かどうか調べる
		bool IsStillActive() { return (IsHandleAvailable() && (getExitCode() == STILL_ACTIVE)) ? true : false; }

	public:
		// プロセスの実行
		virtual BOOL createProcess(const kjm::_tstring& commandLine);

		// OpenProcess APIのラッパー
		//
		// [引数]
		// dwDesiredAccess  アクセスフラグ
		// bInheritHandle   継承フラグ
		// dwProcessId      プロセスID
		//
		// [戻り値]
		// 関数が成功すると、true が返る。
		// 関数が失敗すると、false が返る。
		bool openProcess(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) {
			memset(&m_pi, 0, sizeof(m_pi));

			m_pi.dwProcessId = dwProcessId;
			m_pi.hProcess = ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);

			return m_pi.hProcess != NULL;
		};

		// プロセスハンドルを閉じる。プロセスの終了ではないので注意。
		// プロセスの終了は、プロセス間の取り決めによる。
		void Close();

		// プロセスの終了を待つ
		virtual DWORD waitForTermination(DWORD dwMilliseconds = INFINITE) { return ::WaitForSingleObject(m_pi.hProcess, dwMilliseconds); }

		// プロセスの起動完了を待つ
		virtual DWORD waitForInputIdle(DWORD dwMilliseconds = INFINITE) { return ::WaitForInputIdle(m_pi.hProcess, dwMilliseconds); }

		// プロセスに関連付けられたウィンドウを返す
		HWND GetProcessWindow();

		// プロセスを強制終了
		virtual BOOL kill(UINT uExitCode) {
			return ::TerminateProcess(m_pi.hProcess, uExitCode);
		}

	public:
		// プロセスIDからプロセスを終了させるユーティリティ関数
		//
		// [引数]
		// dwProcessId  プロセスID
		// uExitCode    exit code for the process
		// pLog         ログオブジェクトへのポインタ。!NULL のとき、これを使用してログを残す。
		//
		// [戻り値]
		// 関数が成功すると、TRUE が返る。
		// 関数が失敗すると、FALSE が返る。
		static BOOL terminateProcess(DWORD dwProcessId, UINT uExitCode);

		// 簡易プロセス実行
		static int run(const kjm::_tstring& strCommand, int intWindowStyle=SW_SHOWNORMAL, bool bWaitOnReturn=false);

	private:
		HWND m_hwnd;				// GetProcessWindow関数で使用

		STARTUPINFO m_si;					// スタートアップ情報
		PROCESS_INFORMATION m_pi;			// プロセス情報
		TCHAR m_szCommandLine[MAX_PATH];	// 実行コマンドライン記録用
											// Create関数実行後有効
		
		static BOOL CALLBACK process_EnumWindowsProc(HWND hwnd, LPARAM lParam);
	};

	//
	//
	//
	class Process {
	public:
		//-------------------------------------------------------------
		// デフォルト コンストラクタ
		//-------------------------------------------------------------
		Process() {
			memset(&m_startupInfo, 0, sizeof(m_startupInfo));
			memset(&m_processInformation, 0, sizeof(m_processInformation));
		};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~Process() { Close(); };

		//-------------------------------------------------------------
		// プロセス生成
		//-------------------------------------------------------------
		BOOL Create(const kjm::_tstring& commandLine) {
			m_startupInfo.cb = sizeof(m_startupInfo);

			// CreateProcess の二番目の引数が、LPTSTR のため、ローカル変数に代入して使用する。
			std::vector<TCHAR> cmd(commandLine.begin(), commandLine.end());
			cmd.push_back('\0');

			return CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE, 0, NULL, NULL, &m_startupInfo, &m_processInformation);
		}

		//-------------------------------------------------------------
		// プロセスハンドルクローズ
		//-------------------------------------------------------------
		BOOL Close() {
			BOOL bRet = TRUE;
			if (m_processInformation.hThread) {
				if (!CloseHandle(m_processInformation.hThread)) {
					bRet = FALSE;
				}
			}
			if (m_processInformation.hProcess) {
				if (!CloseHandle(m_processInformation.hProcess)) {
					bRet = FALSE;
				}
			}
			memset(&m_startupInfo, 0, sizeof(m_startupInfo));
			memset(&m_processInformation, 0, sizeof(m_processInformation));
			return bRet;
		};

		//-------------------------------------------------------------
		// プロセスを強制終了
		//-------------------------------------------------------------
		BOOL Terminate(UINT uExitCode) { return ::TerminateProcess(m_processInformation.hProcess, uExitCode); };

		//-------------------------------------------------------------
		// プロセス終了を待つ
		//-------------------------------------------------------------
		DWORD Wait(DWORD dwMilliseconds = INFINITE) { return WaitForSingleObject(m_processInformation.hProcess, dwMilliseconds); };

		//-------------------------------------------------------------
		// 終了コードを取得する
		//-------------------------------------------------------------
		DWORD GetExitCodeProcess(LPDWORD lpExitCode) { return ::GetExitCodeProcess(m_processInformation.hProcess, lpExitCode); };

	private:
		//-------------------------------------------------------------
		// コピーと代入を禁止
		//-------------------------------------------------------------
		Process(const Process& src) {};
		void operator=(const Process& rhs) {};

	public:
		//-------------------------------------------------------------
		// STARTUPINFO::wShowWindow へのアクセサ
		//-------------------------------------------------------------
		WORD get_ShowWindow() { return m_startupInfo.wShowWindow; };
		void set_ShowWindow(WORD value) {
			m_startupInfo.dwFlags |= STARTF_USESHOWWINDOW;
			m_startupInfo.wShowWindow = value;
		};

	private:
		STARTUPINFO m_startupInfo;
		PROCESS_INFORMATION m_processInformation;
	};
};
