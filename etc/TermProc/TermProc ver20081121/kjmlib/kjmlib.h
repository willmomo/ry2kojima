//---------------------------------------------------------------------
// 小島用ライブラリ
//
// すべてのクラスは、namespace kjmで宣言される。
//---------------------------------------------------------------------
#pragma once


#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <string>
#include <vector>
#include <share.h>

#include "getopt.h"


namespace kjm {

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
		void writeLog(const char* format, ...);

	private:
		std::string m_strLogFile;	// ログファイルの名前
	};

	//---------------------------------------------------------------------
	// WIN32関数の簡易呼び出しクラス
	//
	// [説明]
	// static 関数のみで構成される。インスタンス化できないクラス。
	//---------------------------------------------------------------------
	class win32 {
	public:
		// GetEnvironmentVariableのラッパー
		//
		// [引数]
		// lpName  環境変数の名前へのポインタ
		//
		// [戻り値]
		// 指定された環境変数が持つ値を返す。
		static std::string getEnvironmentVariable(LPCTSTR lpName);

		// GetModuleFileNameのラッパー
		//
		// [引数]
		// hModule  モジュールのハンドル
		//
		// [戻り値]
		// 実行ファイルの名前を返す。
		static std::string getModuleFileName(HMODULE hModule = NULL);

		// PathAppend のラッパー
		//
		// [引数]
		// strPath  元になるフルパスのファイル名
		// strMore  結合するパスの名前
		//
		// [戻り値]
		// 結合された新しいファイル名文字列
		static std::string pathAppend(const std::string& strPath, const std::string& strMore);

		// PathFindFileName のラッパー
		//
		// [引数]
		// strPath  ファイル名を検索するフルパスのファイル名
		//
		// [戻り値]
		// ファイル名だけの文字列
		static std::string pathFindFileName(const std::string& strPath);

		// PathRemoveExtension のラッパー
		//
		// [引数]
		// strPath  拡張子を取り除きたいファイル名
		//
		// [戻り値]
		// 拡張子を取り除いた新しい文字列
		static std::string pathRemoveExtension(const std::string& strPath);

		// ファイル名部分を取り除いて新しいファイル名をくっつける
		// PathRemoveFileSpec と PathAppend をくっつけたような関数
		//
		// [引数]
		// strPath  元になるフルパスのファイル名
		// strMore  変更する FileSpec
		//
		// [戻り値]
		// 新しいフルパスのファイル名。
		static std::string pathRenameFileSpec(const std::string& strPath, const std::string& strMore);

		// PathRenameExtension のラッパー
		//
		// [引数]
		// strPath  元にふぁるフルパスのファイル名
		// strExt   置き換える拡張子
		//
		// [戻り値]
		// 拡張子を置き換えた新しいファイル名文字列
		static std::string pathRenameExtension(const std::string& strPath, const std::string& strExt);

	private:
		win32() {};
		~win32() {};
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
		std::string m_strIniFile;	// iniファイルの名前
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

};	// namespace kjm

