//---------------------------------------------------------------------
// wsslog が使用できるときは、USE_WSSLOG を定義してください。
//---------------------------------------------------------------------
#pragma once


//---------------------------------------------------------------------
// ヘッダファイル
//---------------------------------------------------------------------
#pragma warning(disable : 4786)

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <string>
#include <assert.h>
#include <vector>
#include <map>
#include <strstream>
#pragma comment(lib, "version.lib")

#if defined(USE_WSSLOG)
#include "wsslog.h"
#pragma comment(lib, "wsslog.lib")
#endif


namespace kjm {
	//-----------------------------------------------------------------
	// 関数内で発生したWin32エラーを例外として流すクラス
	//-----------------------------------------------------------------
	class Win32Exception {
	public:
		Win32Exception(DWORD dwError) : m_dwError(dwError) {};
		virtual ~Win32Exception() {};

	public:
		DWORD m_dwError;
	};

	//-----------------------------------------------------------------
	// パス文字列
	//-----------------------------------------------------------------
	class PathString {
	public:
		//-------------------------------------------------------------
		// デフォルトコンストラクタ
		//-------------------------------------------------------------
		PathString() : m_str("") {};

		//-------------------------------------------------------------
		// コピー コンストラクタ
		//-------------------------------------------------------------
		PathString(const PathString& src) : m_str(src.m_str) {};

		//-------------------------------------------------------------
		// std::stringを受け取るコンストラクタ
		//-------------------------------------------------------------
		PathString(const std::string& src) : m_str(src) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~PathString() {};

		//-------------------------------------------------------------
		// 代入演算子
		//-------------------------------------------------------------
		PathString& operator=(const PathString& rhs) {
			if (this == &rhs) return *this;
			m_str = rhs.m_str;
			return *this;
		};

		//-------------------------------------------------------------
		// c_str() へのアクセサ
		//-------------------------------------------------------------
		const char* c_str() { return m_str.c_str(); };

		//-------------------------------------------------------------
		// const char* へのキャスト
		//-------------------------------------------------------------
		operator const char *() { return m_str.c_str(); };

		//-------------------------------------------------------------
		// PathAppend のラッパー
		//-------------------------------------------------------------
		PathString& Append(const std::string& strMore) {
			std::vector<char> work(MAX_PATH);
			PathAppend(lstrcpy(work.begin(), m_str.c_str()), strMore.c_str());
			m_str = work.begin();
			return *this;
		};

		//-------------------------------------------------------------
		// PathRemoveFileSpec のラッパー
		//-------------------------------------------------------------
		PathString& RemoveFileSpec() {
			std::vector<char> work(MAX_PATH);
			::PathRemoveFileSpec(lstrcpy(work.begin(), m_str.c_str()));
			m_str = work.begin();
			return *this;
		};

		//-------------------------------------------------------------
		// ルートディレクトリを取得する
		//-------------------------------------------------------------
		std::string GetRoot() {
			std::vector<char> work(MAX_PATH);
			lstrcpy(work.begin(), this->m_str.c_str());

			while (PathIsRoot(work.begin()) == FALSE) {
				if (!PathRemoveFileSpec(work.begin())) {
					return "";
				}
			}

			return work.begin();
		};

	private:
		std::string m_str;
	};


	class Mutex {
	public:
		//-------------------------------------------------------------
		// デフォルト コンストラクタ
		//-------------------------------------------------------------
		Mutex() : m_hMutex(NULL), m_alreadyExists(false) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~Mutex() { Close(); };

		//-------------------------------------------------------------
		// ミューテックスの作成
		//-------------------------------------------------------------
		bool Create(const std::string& name) {
			m_hMutex = ::CreateMutex(NULL, FALSE, name.c_str());
			if (GetLastError() == ERROR_ALREADY_EXISTS) {
				m_alreadyExists = true;
			}
			return (m_hMutex != NULL);
		};

		//-------------------------------------------------------------
		// ミューテックスの破棄
		//-------------------------------------------------------------
		BOOL Close() {
			BOOL bRet = TRUE;
			if (m_hMutex) {
				bRet = CloseHandle(m_hMutex);
				m_hMutex = NULL;
			}
			return bRet;
		};

	private:
		//-------------------------------------------------------------
		// コピーと代入を禁止
		//-------------------------------------------------------------
		Mutex(const Mutex& src) {};
		void operator=(const Mutex& rhs) {};

	public:
		//-------------------------------------------------------------
		// ミューテックスが既に存在していたかどうかを調べる
		//-------------------------------------------------------------
		bool IsAlreadyExists() { return m_alreadyExists; };

	private:
		HANDLE m_hMutex;
		bool m_alreadyExists;
	};


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
		BOOL Create(const std::string& commandLine) {
			m_startupInfo.cb = sizeof(m_startupInfo);

			TCHAR cmd[MAX_PATH];
			lstrcpy(cmd, commandLine.c_str());

			return CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &m_startupInfo, &m_processInformation);
		};

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


	class FindFile {
	public:
		//-------------------------------------------------------------
		// デフォルト コンストラクタ
		//-------------------------------------------------------------
		FindFile() : m_hFindFile(INVALID_HANDLE_VALUE) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~FindFile() { Close(); };

		//-------------------------------------------------------------
		// FindFirstFile関数のラッパー
		//-------------------------------------------------------------
		bool FindFirst(const std::string& lpPathName, const std::string& lpWildcard) {
			assert(m_hFindFile == INVALID_HANDLE_VALUE);

			::PathAppend(::lstrcpy(m_fileName, lpPathName.c_str()), lpWildcard.c_str());
			//SetLastError(ERROR_SUCCESS);
			m_hFindFile = ::FindFirstFile(m_fileName, &m_findFileData);
			return (m_hFindFile != INVALID_HANDLE_VALUE);
		};

		//-------------------------------------------------------------
		// FindNextFile関数のラッパー
		//-------------------------------------------------------------
		bool FindNext() { return (::FindNextFile(m_hFindFile, &m_findFileData) != FALSE); };

		//-------------------------------------------------------------
		// FindClose関数のラッパー
		//-------------------------------------------------------------
		bool Close() {
			bool bResult = true;
			if (m_hFindFile != INVALID_HANDLE_VALUE) {
				bResult = (::FindClose(m_hFindFile) != FALSE);
				m_hFindFile = INVALID_HANDLE_VALUE;
			}
			return bResult;
		};

	private:
		//-------------------------------------------------------------
		// コピーと代入を禁止
		//-------------------------------------------------------------
		FindFile(const FindFile& src) {};
		void operator=(const FindFile& rhs) {};

	public:
		//-------------------------------------------------------------
		// 検索に使うフルパス名を取得
		//-------------------------------------------------------------
		std::string GetFindFileName() { return m_fileName; };

		//-------------------------------------------------------------
		// ファイルがディレクトリかどうか調べる
		//-------------------------------------------------------------
		bool IsDirectory() { return ((m_findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0); };

		//-------------------------------------------------------------
		// ファイルが"."または".."かどうか調べる
		//-------------------------------------------------------------
		bool IsDots() { return (::lstrcmp(m_findFileData.cFileName, _T(".")) == 0) || (::lstrcmp(m_findFileData.cFileName, _T("..")) == 0); };

		//-------------------------------------------------------------
		// ファイル名を取得
		//-------------------------------------------------------------
		std::string GetFileName() { return m_findFileData.cFileName; };

	private:
		TCHAR m_fileName[MAX_PATH];
		HANDLE m_hFindFile;
		WIN32_FIND_DATA m_findFileData;
	};

	//-----------------------------------------------------------------
	// ロックファイルを安全に扱うためのクラス
	//-----------------------------------------------------------------
	class Lockfile {
	public:
		//-------------------------------------------------------------
		// コンストラクタ
		//-------------------------------------------------------------
		Lockfile() : m_hFile(INVALID_HANDLE_VALUE) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~Lockfile() { Unlock(); };

		//-------------------------------------------------------------
		// ロックする
		//-------------------------------------------------------------
		BOOL Lock(const std::string& strLockfile);

		//-------------------------------------------------------------
		// アンロックする
		//-------------------------------------------------------------
		BOOL Unlock();

		// ロック中かどうか調べる
		bool IsLocked() { return (m_hFile != INVALID_HANDLE_VALUE); };

	private:
		//-------------------------------------------------------------
		// コピーと代入を認めない
		//-------------------------------------------------------------
		Lockfile(const Lockfile& src) {};
		void operator=(const Lockfile& lhs) {};

		//-------------------------------------------------------------
		// メンバー変数
		//-------------------------------------------------------------
		HANDLE m_hFile;
	};


	//-----------------------------------------------------------------
	// INIファイルを扱うクラス
	//-----------------------------------------------------------------
	class IniFile {
	public:
		//-------------------------------------------------------------
		// デフォルト コンストラクタ
		//-------------------------------------------------------------
		IniFile() {
			char buf[MAX_PATH];
			::GetModuleFileName(NULL, buf, sizeof(buf));
			::PathRenameExtension(buf, ".ini");
			m_iniFile = buf;
		};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~IniFile() {};

		//-------------------------------------------------------------
		// 文字列を取得する
		//-------------------------------------------------------------
		std::string GetString(const std::string& strSection, const std::string& strKey, const std::string& strDefault) const {
			char buf[8192];
			::GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(), buf, sizeof(buf), m_iniFile.c_str());
			return buf;
		};

		//-------------------------------------------------------------
		// 値を取得する
		//-------------------------------------------------------------
		UINT GetUInt(const std::string& strSection, const std::string& strKey, UINT uDefault) const {
			return ::GetPrivateProfileInt(strSection.c_str(), strKey.c_str(), uDefault, m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// 文字列を書き込む
		//-------------------------------------------------------------
		void WriteString(const std::string& strSection, const std::string& strKey, const std::string& strValue) {
			::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// 値(UINT)を書き込む
		//-------------------------------------------------------------
		void WriteUInt(const std::string& strSection, const std::string& strKey, UINT uValue) {
			char work[16];	// 4294967295(0xffffffff)を書き込むのに十分なバッファ
			wsprintf(work, "%u", uValue);
			::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), work, m_iniFile.c_str());
		};

		//-------------------------------------------------------------
		// INI ファイルの名前を設定する
		//-------------------------------------------------------------
		void set_IniFile(const std::string& strIniFile) { m_iniFile = strIniFile; };

		//-------------------------------------------------------------
		// INI ファイルの名前を取得する
		//-------------------------------------------------------------
		std::string get_IniFile() { return m_iniFile; };

	private:
		std::string m_iniFile;
	};


	//-----------------------------------------------------------------
	//-----------------------------------------------------------------
	class util {
	public:
		//-------------------------------------------------------------
		// 固定ファイル情報からファイルバージョンを取得
		//-------------------------------------------------------------
		static std::string get_file_version() {
			DWORD handle = 0;
			std::vector<char> fname(MAX_PATH);
			
			// バージョン情報リソースを取得
			GetModuleFileName(NULL, fname.begin(), fname.size());
			std::vector<char> buf(GetFileVersionInfoSize(fname.begin(), &handle));
			GetFileVersionInfo(fname.begin(), handle, buf.size(), buf.begin());

			VS_FIXEDFILEINFO* p;
			UINT size;
			VerQueryValue(buf.begin(), "\\", (LPVOID*)&p, &size);

			std::ostrstream os;
			os	<< ((p->dwFileVersionMS & 0xffff0000) >> 16) << "."
				<< (p->dwFileVersionMS & 0x0000ffff) << "."
				<< ((p->dwFileVersionLS & 0xffff0000) >> 16) << "."
				<< (p->dwFileVersionLS & 0x0000ffff);

			return std::string(os.str(), os.pcount());
		};

		//-------------------------------------------------------------
		// 文字列を置換する。VBのStrReplaceのような関数。
		//-------------------------------------------------------------
		static std::string str_replace(const std::string& src, LPCTSTR pFind, LPCTSTR pReplace);

		//-------------------------------------------------------------
		// フォルダの作成
		//-------------------------------------------------------------
		static BOOL MakeDirectory(LPCTSTR lpPathName);

		//-------------------------------------------------------------
		// GetModuleFileName 関数のラッパー
		//-------------------------------------------------------------
		static std::string get_module_file_name(HMODULE hModule) {
			std::vector<char> filename(MAX_PATH);
			if (::GetModuleFileName(hModule, filename.begin(), filename.size()) == FALSE) {
				DWORD dwErr = ::GetLastError();
				throw kjm::Win32Exception(::GetLastError());
			}
			return filename.begin();
		};

		//-------------------------------------------------------------
		// SHFileOperation の FO_COPY を使ったファイルのコピー
		//-------------------------------------------------------------
		static int FOCopy(LPCTSTR lpFrom, LPCTSTR lpTo) {
			// SHFileOperationの文字列は、\0\0で終わる必要があるっぽい
			char szFrom[MAX_PATH], szTo[MAX_PATH];
			szFrom[::lstrlen(::lstrcpy(szFrom, lpFrom)) + 1] = '\0';
			szTo  [::lstrlen(::lstrcpy(szTo  , lpTo  )) + 1] = '\0';

			SHFILEOPSTRUCT fileOp;
			memset(&fileOp, 0, sizeof(fileOp));
			fileOp.wFunc = FO_COPY;
			fileOp.pFrom = szFrom;
			fileOp.pTo = szTo;
			fileOp.fFlags = FOF_SILENT;

			return SHFileOperation(&fileOp);
		}

		//-------------------------------------------------------------
		// SHFileOperation の FO_DELETE を使ったファイルの削除
		//-------------------------------------------------------------
		static int FODelete(LPCTSTR lpPathName) {
			// SHFileOperationの文字列は、\0\0で終わる必要があるっぽい
			char szFrom[MAX_PATH];
			szFrom[::lstrlen(::lstrcpy(szFrom, lpPathName)) + 1] = '\0';

			SHFILEOPSTRUCT fileOp;
			memset(&fileOp, 0, sizeof(fileOp));
			fileOp.wFunc = FO_DELETE;
			fileOp.pFrom = szFrom;
			fileOp.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;

			return ::SHFileOperation(&fileOp);
		};

		//-------------------------------------------------------------
		// ExitWindowsEx のラッパー
		//-------------------------------------------------------------
		static BOOL exit_windows_ex(UINT uFlags) {
			HANDLE hProcess = GetCurrentProcess();

			HANDLE hToken = NULL;
			if (OpenProcessToken(hProcess, TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken) == FALSE) {
#if defined(USE_WSSLOG)
				Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): OpenProcessToken() error: %d", GetLastError());
#endif
				return FALSE;
			}

			LUID luid;
			if (LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid) == FALSE) {
#if defined(USE_WSSLOG)
				Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): LookupPrivilegeValue() error: %d", GetLastError());
#endif
				return FALSE;
			}

			TOKEN_PRIVILEGES token, preToken;
			token.PrivilegeCount = 1;
			token.Privileges[0].Luid = luid;
			token.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

			DWORD dwRet;
			if (AdjustTokenPrivileges(hToken, FALSE, &token, sizeof(preToken), &preToken, &dwRet) == FALSE) {
#if defined(USE_WSSLOG)
				Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): AdjustTokenPrivileges() error: %d", GetLastError());
#endif
				return FALSE;
			}

#if defined(USE_WSSLOG)
			Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): 次のコマンドでOSをシャットダウンします: ExitWindowsEx(%d)", uFlags);
#endif
			BOOL bRet = ExitWindowsEx(uFlags, 0);
			if (bRet == FALSE) {
#if defined(USE_WSSLOG)
				Winssa_Log("kjmlib", LOGLV_INFO, "kjm::exit_windows_ex(): ExitWindowsEx(%d) error: %d", uFlags, GetLastError());
#endif
			}

			return bRet;
		};

	private:
		// コンストラクタとデストラクタをプライベートにして、
		// インスタンスかを不許可とする
		util() {};
		~util() {};
	};

	//-----------------------------------------------------------------
	// RECT クラス
	//-----------------------------------------------------------------
	class Rect : public RECT {
	public:
		//-------------------------------------------------------------
		// デフォルトコンストラクタ
		//-------------------------------------------------------------
		Rect() {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		~Rect() {};

		//-------------------------------------------------------------
		// OffsetRect のラッパー
		//-------------------------------------------------------------
		BOOL OffsetRect(int dx, int dy) { return ::OffsetRect(this, dx, dy); };

		//-------------------------------------------------------------
		// 幅を返す
		//-------------------------------------------------------------
		int Width() { return this->right - this->left; };

		//-------------------------------------------------------------
		// 高さを返す
		//-------------------------------------------------------------
		int Height() { return this->bottom - this->top; };
	};

	//-----------------------------------------------------------------
	// ウィンドウクラス
	//-----------------------------------------------------------------
	class Window {
	public:
		//-------------------------------------------------------------
		// デフォルトコンストラクタ
		//-------------------------------------------------------------
		Window() : m_hwnd(NULL) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~Window() {};

		//-------------------------------------------------------------
		// アタッチ
		//-------------------------------------------------------------
		void Attach(HWND hwnd) { m_hwnd = hwnd; };

		//-------------------------------------------------------------
		// デタッチ
		//-------------------------------------------------------------
		void Detach() { m_hwnd = NULL; };

	private:
		HWND m_hwnd;	// 管理するウィンドウのハンドル
	};

	//-----------------------------------------------------------------
	// ダイアログクラス
	//-----------------------------------------------------------------
	class Dialog {
	public:
		//-------------------------------------------------------------
		// デフォルトコンストラクタ
		//-------------------------------------------------------------
		Dialog() : m_hwnd(NULL) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~Dialog() {};

		//-------------------------------------------------------------
		// モードレスダイアログの作成
		//-------------------------------------------------------------
		bool ShowModelessDialog(HINSTANCE hInstance, UINT uID, HWND hwndParent) {
			assert(this->m_hwnd == NULL);
			m_bModal = false;

#if defined(USE_WSSLOG)
			Winssa_Log("kjmlib", LOGLV_INFO, "kjm::Dialog::ShowModelessDialog(): hwndParent = %p, uID = %d", hwndParent, uID);
#endif

			m_hwnd = ::CreateDialogParam(hInstance, MAKEINTRESOURCE(uID), hwndParent, Dialog::DialogProc, (LPARAM)this);
			return (m_hwnd != NULL);
		};

		//-------------------------------------------------------------
		// モーダルダイアログの作成
		//-------------------------------------------------------------
		BOOL ShowModalDialog(HINSTANCE hInstance, UINT uID, HWND hwndParent) {
			assert(this->m_hwnd == NULL);
			m_bModal = true;

#if defined(USE_WSSLOG)
			Winssa_Log("kjmlib", LOGLV_INFO, "kjm::Dialog::ShowModalDialog(): hwndParent = %p, uID = %d", hwndParent, uID);
#endif

			return ::DialogBoxParam(hInstance, MAKEINTRESOURCE(uID), hwndParent, Dialog::DialogProc, (LPARAM)this);
		};

		//-------------------------------------------------------------
		// クラス固有のダイアログプロシージャ
		//-------------------------------------------------------------
		virtual BOOL DlgProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch (uMsg) {
			case WM_COMMAND:
				return OnCommand(HIWORD(wParam), LOWORD(wParam), (HWND)lParam);

			case WM_DESTROY:
				return OnDestroy();

			case WM_INITDIALOG:
				return OnInitDialog((HWND)wParam, lParam);

			case WM_TIMER:
				return OnTimer(wParam, (TIMERPROC*)lParam);
			}

			return FALSE;
		};

		//-------------------------------------------------------------
		// 良く使うメッセージは、virtual関数として定義しておく。
		//
		// WM_COMMAND メッセージ処理
		// WM_DESTROY メッセージ処理
		// WM_INITDIALOG メッセージ処理
		// WM_TIMER メッセージ処理
		//-------------------------------------------------------------
		virtual BOOL OnCommand(WORD wNotifyCode, WORD wID, HWND hwndCtl) { return FALSE; };
		virtual BOOL OnDestroy() { return FALSE; };
		virtual BOOL OnInitDialog(HWND hwndFocus, LPARAM lInitParam) { return FALSE; };
		virtual BOOL OnTimer(WPARAM wTimerID, TIMERPROC* tmprc) { return FALSE; };

	public:
		//-------------------------------------------------------------
		// ウィンドウのインスタンスを取得
		//-------------------------------------------------------------
		HINSTANCE get_Instance() { return GetWindowInstance(m_hwnd); };

		//-------------------------------------------------------------
		// ウィンドウのタイトルを設定する
		//-------------------------------------------------------------
		BOOL set_WindowText(LPCTSTR lpString) { return ::SetWindowText(m_hwnd, lpString); };

		//-------------------------------------------------------------
		// ダイアログ上のコントロールにテキストを設定する
		//-------------------------------------------------------------
		BOOL SetDlgItemText(int nIDDlgItem, LPCTSTR lpString) {
			assert(this->m_hwnd != NULL);
			assert(lpString != NULL);
			return ::SetDlgItemText(this->m_hwnd, nIDDlgItem, lpString);
		};

		//-------------------------------------------------------------
		// 指定された ID を持つコントロールのウィンドウハンドルを取得します。
		//-------------------------------------------------------------
		HWND get_dlg_item(int nIDDlgItem) {
			assert(this->m_hwnd != NULL);
			return ::GetDlgItem(this->m_hwnd, nIDDlgItem);
		};

		//-------------------------------------------------------------
		// 指定された整数値を文字列に変換して、ダイアログボックス内の
		// コントロールにテキストとして設定します。
		//-------------------------------------------------------------
		BOOL set_dlg_item_int(int nIDDlgItem, INT nValue) {
			assert(this->m_hwnd != NULL);
			return ::SetDlgItemInt(this->m_hwnd, nIDDlgItem, nValue, TRUE);
		};

		//-------------------------------------------------------------
		// 指定された整数値を文字列に変換して、ダイアログボックス内の
		// コントロールにテキストとして設定します。
		//-------------------------------------------------------------
		BOOL set_dlg_item_int(int nIDDlgItem, UINT uValue) {
			assert(this->m_hwnd != NULL);
			return ::SetDlgItemInt(this->m_hwnd, nIDDlgItem, uValue, FALSE);
		};

		//-------------------------------------------------------------
		// ダイアログボックス内の指定されたコントロールのテキストを、
		// 整数値に変換します。
		//-------------------------------------------------------------
		UINT get_dlg_item_int(int nIDDlgItem, BOOL bSigned) {
			assert(this->m_hwnd != NULL);
			return ::GetDlgItemInt(this->m_hwnd, nIDDlgItem, NULL, bSigned);
		};

		//-------------------------------------------------------------
		// タイマーを設定する
		//-------------------------------------------------------------
		UINT SetTimer(UINT nIDEvent, UINT uElapse) { return ::SetTimer(m_hwnd, nIDEvent, uElapse, NULL); };

		//-------------------------------------------------------------
		// タイマーを破棄する
		//-------------------------------------------------------------
		BOOL KillTimer(UINT uIDEvent) { return ::KillTimer(m_hwnd, uIDEvent); };

		//-------------------------------------------------------------
		// ダイアログを閉じる
		//-------------------------------------------------------------
		BOOL EndDialog(int nResult) {
			assert(m_hwnd != NULL);
			assert(m_bModal != false);
			return ::EndDialog(m_hwnd, nResult);
		};

		//-------------------------------------------------------------
		// DestroyWindow のラッパー
		//-------------------------------------------------------------
		BOOL DestroyWindow() {
			assert(m_hwnd != NULL);
			return ::DestroyWindow(this->m_hwnd);
		};

		//-------------------------------------------------------------
		// ウィンドウの拡張属性を取得
		//-------------------------------------------------------------
		LONG GetExStyle() {
			assert(this->m_hwnd != NULL);
			return ::GetWindowLong(this->m_hwnd, GWL_EXSTYLE);
		};

		//-------------------------------------------------------------
		// ウィンドウの拡張属性を設定
		//-------------------------------------------------------------
		LONG SetExStyle(LONG dwNewExStyle) {
			assert(this->m_hwnd != NULL);
			return ::SetWindowLong(this->m_hwnd, GWL_EXSTYLE, dwNewExStyle);
		};

		//-------------------------------------------------------------
		// ウィンドウの位置情報などを設定
		//-------------------------------------------------------------
		BOOL SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags) {
			assert(this->m_hwnd != NULL);
			return ::SetWindowPos(this->m_hwnd, hWndInsertAfter, X, Y, cx, cy, uFlags);
		};

		//-------------------------------------------------------------
		// ウィンドウを常に最前面(top-most)にする
		//-------------------------------------------------------------
		BOOL SetTopMost() {
			assert(this->m_hwnd != NULL);
			this->SetExStyle(this->GetExStyle() | WS_EX_TOPMOST);
			return this->SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);
		};

		//-------------------------------------------------------------
		// GetWindowRect 関数のラッパー
		//-------------------------------------------------------------
		BOOL GetWindowRect(kjm::Rect& rect) {
			assert(this->m_hwnd != NULL);
			return ::GetWindowRect(this->m_hwnd, &rect);
		};

		//-------------------------------------------------------------
		// MoveWindow のラッパー
		//-------------------------------------------------------------
		BOOL MoveWindow(int X, int Y, int nWidth, int nHeight, BOOL bRepaint) {
			assert(this->m_hwnd != NULL);
			return ::MoveWindow(this->m_hwnd, X, Y, nWidth, nHeight, bRepaint);
		};

		//-------------------------------------------------------------
		// ウィンドウが有効かどうか調べる
		//-------------------------------------------------------------
		bool IsAvailable() { return (this->m_hwnd != NULL); };

	private:
		//-------------------------------------------------------------
		// 標準のダイアログプロシージャ
		//-------------------------------------------------------------
		static BOOL CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			static std::map<HWND, kjm::Dialog*> s_dialogMap;
		
			kjm::Dialog* pDialog = NULL;

			if (uMsg == WM_INITDIALOG) {
				pDialog = (kjm::Dialog*)lParam;
				pDialog->m_hwnd = hwnd;
				s_dialogMap[hwnd] = pDialog;
			} else {
				pDialog = s_dialogMap[hwnd];
			}

			BOOL bRet = FALSE;
			if (pDialog != NULL) {
				bRet = pDialog->DlgProc(uMsg, wParam, lParam);
			}

			if (uMsg == WM_NCDESTROY) {
				if (pDialog != NULL) {
					pDialog->m_hwnd = NULL;
				}
				s_dialogMap.erase(hwnd);
			}
			return bRet;
		};

	protected:
		HWND m_hwnd;	// ダイアログへのハンドル

	private:
		bool m_bModal;	// モーダルダイアログのとき、true 
	};

	//-----------------------------------------------------------------
	// NOTIFYICONDATA クラス
	//-----------------------------------------------------------------
	class NotifyIcon {
	public:
		//-------------------------------------------------------------
		// デフォルトコンストラクタ
		//-------------------------------------------------------------
		NotifyIcon() { memset(&m_nid, 0, sizeof(m_nid)); };

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~NotifyIcon() {};
		
		//-------------------------------------------------------------
		// アイコンの追加
		//-------------------------------------------------------------
		BOOL Add(HWND hwnd, UINT uID, HICON hIcon, UINT uCallbackMessage, LPCTSTR pszTip) {
			memset(&m_nid, 0, sizeof(m_nid));

			m_nid.cbSize = sizeof(m_nid);
			m_nid.hWnd = hwnd;
			m_nid.uID = uID;

			if (hIcon != NULL) {
				m_nid.hIcon = hIcon;
				m_nid.uFlags |= NIF_ICON;
			}

			if (uCallbackMessage != 0) {
				m_nid.uCallbackMessage = uCallbackMessage;
				m_nid.uFlags |= NIF_MESSAGE;
			}

			if (pszTip != NULL) {
				lstrcpy(m_nid.szTip, pszTip);
				m_nid.uFlags |= NIF_TIP;
			}

			return Shell_NotifyIcon(NIM_ADD, &m_nid);
		};

		//-------------------------------------------------------------
		// アイコンの削除
		//-------------------------------------------------------------
		BOOL Delete() { return ::Shell_NotifyIcon(NIM_DELETE, &m_nid); };

		//-------------------------------------------------------------
		// アイコンの変更
		//-------------------------------------------------------------
		BOOL Modify(HICON hIcon, UINT uCallbackMessage, LPCTSTR pszTip) {
			m_nid.uFlags = 0;

			if (hIcon != NULL) {
				m_nid.hIcon = hIcon;
				m_nid.uFlags |= NIF_ICON;
			}

			if (uCallbackMessage != 0) {
				m_nid.uCallbackMessage = uCallbackMessage;
				m_nid.uFlags |= NIF_MESSAGE;
			}

			if (pszTip != NULL) {
				lstrcpy(m_nid.szTip, pszTip);
				m_nid.uFlags |= NIF_TIP;
			}

			return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
		};

	public:
		NOTIFYICONDATA m_nid;	// アイコン情報
	};

	//-----------------------------------------------------------------
	// クリティカルセクションを管理するクラス
	//-----------------------------------------------------------------
	class CriticalSection : public CRITICAL_SECTION {
	public:
		//-------------------------------------------------------------
		// コンストラクタ(クリティカルセクションの初期化も兼ねる)
		//-------------------------------------------------------------
		CriticalSection() {
			::InitializeCriticalSection(this);
		};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		~CriticalSection() {
			::DeleteCriticalSection(this);
		};

		//-------------------------------------------------------------
		// 所有権の取得
		//-------------------------------------------------------------
		VOID Enter() {
			::EnterCriticalSection(this);
		};

		//-------------------------------------------------------------
		// 所有権の破棄
		//-------------------------------------------------------------
		VOID Leave() {
			::LeaveCriticalSection(this);
		};
	};
};
