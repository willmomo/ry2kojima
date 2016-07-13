#pragma once


#include <windows.h>
#include <tchar.h>
#include <string>
#include <sstream>

#ifdef UNICODE
typedef std::wstring		_tstring;
typedef std::wostringstream	_tostringstream;
#else
typedef std::string			_tstring;
typedef std::ostringstream	_tostringstream;
#endif


namespace kjmutil {
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
		process(kjmutil::log* pLog) : m_hProc(NULL), m_pLog(pLog) {};

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
				m_pLog->writeLog(_T(">> kjmutil::process::openProcess(%u, %d, %u)"), dwDesiredAccess, bInheritHandle, dwProcessId);
				writePropertyLog();
			}

			m_hProc = ::OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);

			if (m_pLog) {
				if (m_hProc == NULL)	m_pLog->writeLog(_T("   OpenProcess(%u, %d, %u) error(%u)"), dwDesiredAccess, bInheritHandle, dwProcessId, ::GetLastError());
				m_pLog->writeLog(_T("<< kjmutil::process::openProcess() bool(%p != NULL)."), m_hProc);
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
				m_pLog->writeLog(_T(">> kjmutil::process::closeHandle()"));
				writePropertyLog();
			}

			BOOL bRet = TRUE;
			if (m_hProc != NULL) {
				bRet = ::CloseHandle(m_hProc);
				m_hProc = NULL;
			}

			if (m_pLog)	m_pLog->writeLog(_T("<< kjmutil::process::closeHandele() BOOL(%d)."), bRet);
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
				m_pLog->writeLog(_T(">> kjmutil::process::terminateProcess(%u)"), uExitCode);
				writePropertyLog();
			}

			BOOL bRet = ::TerminateProcess(m_hProc, uExitCode);

			if (m_pLog) {
				if (bRet == FALSE) {
					m_pLog->writeLog(_T("   TerminateProcess(%p, %u) error(%d)"), m_hProc, uExitCode, ::GetLastError());
				}
				m_pLog->writeLog(_T("<< kjmutil::process::terminateProcess() BOOL(%d)"), bRet);
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
		static BOOL terminateProcess(DWORD dwProcessId, UINT uExitCode, kjmutil::log* pLog = NULL) {
			if (pLog)	pLog->writeLog(_T(">> static kjmutil::process::terminateProcess(%u, %u, %p)"), dwProcessId, uExitCode, pLog);

			BOOL bRet = FALSE;
			kjmutil::process proc(pLog);
			if (proc.openProcess(PROCESS_TERMINATE, FALSE, dwProcessId)) {
				bRet = proc.terminateProcess(uExitCode);
			}

			if (pLog)	pLog->writeLog(_T("<< static kjmutil::process::terminateProcess() BOOL(%d)."), bRet);
			return bRet;
		};

	private:
		HANDLE m_hProc;		// オープンしているプロセスのハンドル
		kjmutil::log* m_pLog;	// ログオブジェクトへのポインタ

		// プロパティ(内部変数)を可能ならログに出力する
		void writePropertyLog() {
			if (m_pLog) {
				m_pLog->writeLog(_T("   this->m_hProc = %p"), m_hProc);
			}
		};
	};

	
	/* エラー情報を扱うクラス

	   [使用例]
	   kjmutil::err lastError;
	   _tprintf(_T("error %d: %s\n"), lastError.get_number(), lastError.get_description().c_str());
	*/
	class err {
	public:
		/* デフォルトコンストラクタ
		   オブジェクト作成時の GetLastError の値で初期化される。
		*/
		err() : m_dwErrCode(GetLastError()) {};

		/* コンストラクタ(エラー番号初期化付き) */
		err(DWORD dwErrCode) : m_dwErrCode(dwErrCode) {};

		/* デストラクタ */
		~err() {};

		/* エラー番号の取得 */
		DWORD get_number() { return m_dwErrCode; };

		/* エラー番号の設定 */
		void set_number(DWORD dwErrCode) { m_dwErrCode = dwErrCode; };

		/* エラーの説明を取得
		   FormatMessage 関数で取得できるエラーメッセージです。
		   通常、最後に付く改行を削除してあります。
		*/
		_tstring get_description();

	private:
		DWORD m_dwErrCode;
	};

	
	/* RECT を扱うクラス */
	class rect : public RECT {
	public:
		/* デフォルトコンストラクタ */
		rect() { SetRectEmpty(this); };

		/* デストラクタ */
		~rect() {};

		/* スクリーン座標をクライアント座標に変換 */
		rect& screen_to_client(HWND hwnd);

		/* 矩形をオフセットする */
		rect& offset(int dx, int dy);

		/* 幅 */
		int get_width() const { return right - left; };

		/* 高さ */
		int get_height() const { return bottom - top; };

		/* ウィンドウのクライアント座標を取得します */
		static rect get_client_rect(HWND hwnd);

		/* ウィンドウの座標を取得します */
		static rect get_window_rect(HWND hwnd);

		/* 指定された 2 つの長方形を完全に含む、最小の長方形を作成します */
		static rect union_rect(const RECT* lprcSrc1, const RECT* lprcSrc2);
	};

	
	/* 文字列の最後の改行を取り除く */
	void chomp(_tstring& str);

	/* kjmutil::rectを引数にとるMoveWindow関数 */
	BOOL move_window(HWND hWnd, const kjmutil::rect& rc, BOOL bRepaint);

	/****************************************************************************
	  lpExistingFileNameのファイルをlpNewFileNameにコピーします。コピー失敗時には
	  指定回数分リトライを行います。

	  <パラメータ>
	  lpExistingFileName  コピー元
	  lpNewFileName       コピー先
	  bFailIfExists       上書きを行うか(FALSEで上書きを行う)
	  retry               再試行を何回行うか
	  wait                再試行を行うまで何秒待つのか

	  <戻り値>
	  コピーに成功したときTRUE、コピーに失敗したときFALSEを返す。
	****************************************************************************/
	BOOL IOUtil__CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, int retry, int wait);
};
