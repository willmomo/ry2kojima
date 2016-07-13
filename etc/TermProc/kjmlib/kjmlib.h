//---------------------------------------------------------------------
// 小島用ライブラリ
//
// すべてのクラスは、namespace kjmで宣言される。
//---------------------------------------------------------------------
#pragma once

#if defined(UNICODE)
#define _UNICODE
#endif

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <share.h>

#include "getopt.h"


namespace kjm {
	typedef std::basic_string<TCHAR>	_tstring;

	//---------------------------------------------------------------------
	// 簡易ログクラス
	//
	// [説明]
	// %TEMP%\<実行ファイル名>YYYYMMDD.log というファイルに、どんどんログを
	// 追記するクラス。
	// ファイルの削除処理もなく、排他もある程度のリトライがあるだけ、ユー
	// ティリティ的クラス、
	//---------------------------------------------------------------------
	class log {
	public:
		// デフォルトコンストラクタ
		log() {};

		// デストラクタ
		~log() {};

		// ログを書く
		//
		// [引数]
		// format  書式制御文字列
		void writeLog(LPCTSTR format, ...);

	private:
		kjm::_tstring m_strLogFile;	// ログファイルの名前
	};


	//---------------------------------------------------------------------
	// ちょっとした判定関数をおさめたクラス
	//---------------------------------------------------------------------
	class util {
	public:
		static bool is_archive(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) != 0);
		};

		static bool is_system(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != 0);
		};

		static bool is_readonly(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_READONLY) != 0);
		};

		static bool is_directory(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0);
		};

		static bool is_hidden(const WIN32_FIND_DATA& data) {
			return ((data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
		};

		static bool is_dots(const WIN32_FIND_DATA& data) {
			return ((::lstrcmp(data.cFileName, _T(".")) == 0) || (::lstrcmp(data.cFileName, _T("..")) == 0));
		};

	private:
		util() {};
		~util() {};
	};

	//---------------------------------------------------------------------
	// WIN32関数の簡易呼び出しクラス
	//
	// [説明]
	// static 関数のみで構成される。インスタンス化できないクラス。
	//---------------------------------------------------------------------
	class win32 {
	public:
		// 再帰的にフォルダを作成できる関数
		//
		static BOOL makeDirectory(LPCTSTR pszFolder);

		// GetEnvironmentVariableのラッパー
		//
		// [引数]
		// lpName  環境変数の名前へのポインタ
		//
		// [戻り値]
		// 指定された環境変数が持つ値を返す。
		static kjm::_tstring getEnvironmentVariable(LPCTSTR lpName);

		// GetModuleFileNameのラッパー
		//
		// [引数]
		// hModule  モジュールのハンドル
		//
		// [戻り値]
		// 実行ファイルの名前を返す。
		static kjm::_tstring getModuleFileName(HMODULE hModule = NULL);

		// PathAppend のラッパー
		//
		// [引数]
		// strPath  元になるフルパスのファイル名
		// strMore  結合するパスの名前
		//
		// [戻り値]
		// 結合された新しいファイル名文字列
		static kjm::_tstring pathAppend(const kjm::_tstring& strPath, const kjm::_tstring& strMore);

		// PathFindFileName のラッパー
		//
		// [引数]
		// strPath  ファイル名を検索するフルパスのファイル名
		//
		// [戻り値]
		// ファイル名だけの文字列
		static kjm::_tstring pathFindFileName(const kjm::_tstring& strPath);

		// PathRemoveExtension のラッパー
		//
		// [引数]
		// strPath  拡張子を取り除きたいファイル名
		//
		// [戻り値]
		// 拡張子を取り除いた新しい文字列
		static kjm::_tstring pathRemoveExtension(const kjm::_tstring& strPath);

		// ファイル名部分を取り除いて新しいファイル名をくっつける
		// PathRemoveFileSpec と PathAppend をくっつけたような関数
		//
		// [引数]
		// strPath  元になるフルパスのファイル名
		// strMore  変更する FileSpec
		//
		// [戻り値]
		// 新しいフルパスのファイル名。
		static kjm::_tstring pathRenameFileSpec(const kjm::_tstring& strPath, const kjm::_tstring& strMore);

		// PathRenameExtension のラッパー
		//
		// [引数]
		// strPath  元にふぁるフルパスのファイル名
		// strExt   置き換える拡張子
		//
		// [戻り値]
		// 拡張子を置き換えた新しいファイル名文字列
		static kjm::_tstring pathRenameExtension(const kjm::_tstring& strPath, const kjm::_tstring& strExt);

	private:
		win32() {};
		~win32() {};
	};


	//-----------------------------------------------------------------
	// MAX_PATH を超える文字列も扱えるクラス
	//-----------------------------------------------------------------
	class pathString {
	public:
		//-------------------------------------------------------------
		// デフォルト コンストラクタ
		//-------------------------------------------------------------
		pathString() : m_str() {};

		//-------------------------------------------------------------
		// コピー コンストラクタ
		//-------------------------------------------------------------
		pathString(const pathString& src) : m_str(src.m_str) {};

		//-------------------------------------------------------------
		// _tstring を受け取るコンストラクタ
		//-------------------------------------------------------------
		pathString(const _tstring& src) : m_str(src) {};

		//-------------------------------------------------------------
		// デストラクタ
		//-------------------------------------------------------------
		virtual ~pathString() {};

		//-------------------------------------------------------------
		// 代入演算子
		//-------------------------------------------------------------
		pathString& operator=(const pathString& rhs) {
			if (this == &rhs) return *this;
			m_str = rhs.m_str;
			return *this;
		};

		//-------------------------------------------------------------
		// 文字列の長さを返す
		//-------------------------------------------------------------
		int length() { return m_str.length(); };

		//-------------------------------------------------------------
		// 内部文字列へのアクセス
		//-------------------------------------------------------------
		LPCTSTR c_str() { return m_str.c_str(); };

		//-------------------------------------------------------------
		// LPCTSTR へのキャスト
		//-------------------------------------------------------------
		operator LPCTSTR() { return m_str.c_str(); };

		//-------------------------------------------------------------
		// \\?\ \\?\UNC を追加した文字列を返す
		//
		// CopyFile などは、MAX_PATH を超えたファイル名を正常に処理するためには、
		// 頭に \\?\ or \\?\UNC をつけたパス文字列を引数として渡す必要がある。
		//-------------------------------------------------------------
		_tstring GetOverMaxPath();

		//-------------------------------------------------------------
		// パス文字列の後ろに '\' を追加する
		//-------------------------------------------------------------
		pathString& AddBackslash();

		//-------------------------------------------------------------
		// パス文字列の結合
		//
		// param: pMore: 追加する文字列
		//-------------------------------------------------------------
		pathString& Append(const _tstring& pMore);

	private:
		_tstring m_str;	// 管理文字列
	};

	
	//---------------------------------------------------------------------
	// iniファイルアクセスクラス
	//
	// [説明]
	// クラスを宣言するだけで、実行ファイル名.ini にアクセスできるようにな
	// る便利クラス。
	//---------------------------------------------------------------------
	class iniFile {
	public:
		// デフォルトコンストラクタ
		//
		// [説明]
		// 実行ファイル名.ini をデフォルトでアクセスするように調整される。
		iniFile();

		// コピーコンストラクタ
		iniFile(const iniFile& src) : m_strIniFile(src.m_strIniFile) {};

		// デストラクタ
		~iniFile() {};

		// 代入演算子
		iniFile& operator =(const iniFile& rhs);

	private:
		kjm::_tstring m_strIniFile;	// iniファイルの名前
	};


	//---------------------------------------------------------------------
	// プロセス関係クラス
	//---------------------------------------------------------------------
	class process {
	public:
		// コンストラクタ
		process() : m_hProc(NULL), m_pLog(NULL) {};

		// ログオブジェクトを指定するコンストラクタ
		process(kjm::log* pLog) : m_hProc(NULL), m_pLog(pLog) {};

		// デストラクタ
		virtual ~process() { closeHandle(); };

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
			if (m_pLog) {
				m_pLog->writeLog(_T(">> kjm::process::openProcess(%u, %d, %u)"), dwDesiredAccess, bInheritHandle, dwProcessId);
				writePropertyLog();
			}

			m_hProc = ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);

			if (m_pLog) {
				if (m_hProc == NULL)	m_pLog->writeLog(_T("   OpenProcess(%u, %d, %u) error(%u)"), dwDesiredAccess, bInheritHandle, dwProcessId, ::GetLastError());
				m_pLog->writeLog(_T("<< kjm::process::openProcess() bool(%p != NULL)."), m_hProc);
			}
			return m_hProc != NULL;
		};

		// CloseHandle APIのラッパー
		//
		// [戻り値]
		// 関数が成功すると、TRUE が返る。
		// 関数が失敗すると、FALSE が返る。
		//
		// [説明]
		// 関数の成功、失敗にかかわらず、ハンドルはNULLに初期化される。
		BOOL closeHandle() {
			if (m_pLog) {
				m_pLog->writeLog(_T(">> kjm::process::closeHandle()"));
				writePropertyLog();
			}

			BOOL bRet = TRUE;
			if (m_hProc != NULL) {
				bRet = ::CloseHandle(m_hProc);
				m_hProc = NULL;
			}

			if (m_pLog)	m_pLog->writeLog(_T("<< kjm::process::closeHandele() BOOL(%d)."), bRet);
			return bRet;
		};

		// TerminateProcess APIのラッパー
		//
		// [引数]
		// uExitCode  exit code for the process
		//
		// [戻り値]
		// 関数が成功すると、TRUE が返る。
		// 関数が失敗すると、FALSE が返る。
		BOOL terminateProcess(UINT uExitCode) {
			if (m_pLog) {
				m_pLog->writeLog(_T(">> kjm::process::terminateProcess(%u)"), uExitCode);
				writePropertyLog();
			}

			BOOL bRet = ::TerminateProcess(m_hProc, uExitCode);

			if (m_pLog) {
				if (bRet == FALSE) {
					m_pLog->writeLog(_T("   TerminateProcess(%p, %u) error(%d)"), m_hProc, uExitCode, ::GetLastError());
				}
				m_pLog->writeLog(_T("<< kjm::process::terminateProcess() BOOL(%d)"), bRet);
			}
			return bRet;
		};

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
		static BOOL terminateProcess(DWORD dwProcessId, UINT uExitCode, kjm::log* pLog = NULL) {
			if (pLog)	pLog->writeLog(_T(">> static kjm::process::terminateProcess(%u, %u, %p)"), dwProcessId, uExitCode, pLog);

			BOOL bRet = FALSE;
			kjm::process proc(pLog);
			if (proc.openProcess(PROCESS_TERMINATE, FALSE, dwProcessId)) {
				bRet = proc.terminateProcess(uExitCode);
			}

			if (pLog)	pLog->writeLog(_T("<< static kjm::process::terminateProcess() BOOL(%d)."), bRet);
			return bRet;
		};

	private:
		HANDLE m_hProc;		// オープンしているプロセスのハンドル
		kjm::log* m_pLog;	// ログオブジェクトへのポインタ

		// プロパティ(内部変数)を可能ならログに出力する
		void writePropertyLog() {
			if (m_pLog) {
				m_pLog->writeLog(_T("   this->m_hProc = %p"), m_hProc);
			}
		};
	};

	// toolhelpのサポートクラス
	class toolhelp {
	public:
		toolhelp() : m_pLog(NULL) {};
		toolhelp(kjm::log* pLog) : m_pLog(pLog) {};
		virtual ~toolhelp() {};

		bool createSnapProcess();
		int snapProcessSize() { return m_peList.size(); };
		PROCESSENTRY32& getSnapProcess(int n) { return m_peList.at(n); };

	private:
		std::vector<PROCESSENTRY32> m_peList;
		kjm::log* m_pLog;
	};


	//---------------------------------------------------------------------
	// FindFile系関数のラップクラス
	//---------------------------------------------------------------------
	class find_file {
	public:
		find_file() : m_hFind(INVALID_HANDLE_VALUE), m_path(_T(".")), m_pattern(_T("*.*")) {};

		virtual ~find_file() { find_close(); };

		bool find_files() {
			bool bRet = false;
			if (m_hFind == INVALID_HANDLE_VALUE) {
				kjm::pathString(m_path).Append(m_pattern);
				m_hFind = ::FindFirstFile(kjm::pathString(m_path).Append(m_pattern), &m_data);
				bRet = (m_hFind != INVALID_HANDLE_VALUE);
				m_last_error = ::GetLastError();
			} else {
				bRet = (::FindNextFile(m_hFind, &m_data) != FALSE);
				m_last_error = ::GetLastError();
			}
			return bRet;
		};

		void find_close() {
			if (m_hFind != INVALID_HANDLE_VALUE) {
				::FindClose(m_hFind);
				m_hFind = INVALID_HANDLE_VALUE;
			}
		};

		bool is_archive() const { return kjm::util::is_archive(m_data); };

		bool is_system() { return kjm::util::is_system(m_data); };

		bool is_readonly() { return kjm::util::is_readonly(m_data); };

		bool is_directory() { return kjm::util::is_directory(m_data); };

		bool is_hidden() { return kjm::util::is_hidden(m_data); };

		bool is_dots() { return kjm::util::is_dots(m_data); };

		_tstring get_filename() { return m_data.cFileName; };

		FILETIME get_last_write_time() const { return m_data.ftLastWriteTime; };

		__int64 get_file_size() {
			LARGE_INTEGER ui64 = {m_data.nFileSizeLow, m_data.nFileSizeHigh};
			return ui64.QuadPart;
		};

		void set_path(LPCTSTR value) { m_path = value; };

		DWORD get_last_error() { return m_last_error; };
	private:
		DWORD m_last_error;
		HANDLE m_hFind;
		WIN32_FIND_DATA m_data;
		_tstring m_path;
		_tstring m_pattern;
	};

};	// namespace kjm

