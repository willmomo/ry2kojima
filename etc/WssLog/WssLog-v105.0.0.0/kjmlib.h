//---------------------------------------------------------------------
// kjmlib : Version 0.2011.6.7
//
// kjmlib.h : ユーティリティ的に使えるライブラリ
//
// ほかのコードとバッティングしないように、namespace kjm で、作成してあります。
// stlベースの処理ばかりなので、dll と exe をまたいで使用することは、できません。
//
// USE_WSSLOG を define しておくと、wsslog.dll を使って、ログを残す。
//
// kjmlib.cpp とセットで、複製して使用してください。
// shlwapi.lib をリンクしてください。
//
// 2007.09.05
//---------------------------------------------------------------------

#pragma once

#pragma warning(disable : 4786)
#pragma warning(disable : 4996)

#if !defined(__KJMLIB_H)
#define __KJMLIB_H

#if !defined(_WINSOCKAPI_)
// winsock.h がインクルードされているときは、winsock2.h は、インクルードしません。
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#endif

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <lm.h>
#pragma comment(lib, "netapi32.lib")

#include <shlobj.h>
// kjm::util::createShortcutで使用する.

#include <string>
#include <vector>
#include <map>
#include <fstream>

#include <assert.h>
#include <process.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
// #pragma comment(lib, "wsslog.lib")
#endif

#if !defined(countof)
#define countof(A)	(sizeof((A)) / sizeof((A)[0]))
#endif

namespace kjm {
	//-------------------------------------------------------------------------
	// kjmlib で使う汎用例外
	//-------------------------------------------------------------------------
	class kjmException {
	public:
		kjmException(DWORD errorCode, const std::basic_string<TCHAR>& msg) : m_errorCode(errorCode), m_msg(msg) {}
		kjmException(const kjmException& src) : m_errorCode(src.m_errorCode), m_msg(src.m_msg) {}
		virtual ~kjmException() {}

	public:
		DWORD m_errorCode;
		std::basic_string<TCHAR> m_msg;

	private:
		kjmException() {}
	};

	void setVerboseMode(bool mode);
	bool getVerboseMode();

	// OutputDebugString を簡単に使うための関数
	void ods(LPCTSTR format, ...);

	void log_info(LPCTSTR signature, LPCTSTR format, ...);	// 情報ログを残す関数
	void log_error(LPCTSTR signature, LPCTSTR format, ...);	// エラーログを残す関数

	BOOL create_directory(LPCTSTR lpPathName);	// 再帰的にディレクトリを作成できる CreateDirectory 関数
	
	//-------------------------------------------------------------------------
	// CSV ファイルを扱うクラス
	//-------------------------------------------------------------------------
	class csvFile {
	public:
		// コンストラクタ
		csvFile() {}

		// デストラクタ
		virtual ~csvFile() { this->close(); }

		// CSV ファイルを開く
		bool open(LPCTSTR pszFileName);

		// CSV ファイルを閉じる
		void csvFile::close();

		// CSV ファイルから 1 行読み込む
		int parseCsvCol(std::vector<std::basic_string<TCHAR> >& cols);

	private:
		std::basic_ifstream<TCHAR> m_fr;
	};

	//-------------------------------------------------------------------------
	// パス名を扱う文字列
	//-------------------------------------------------------------------------
	class PathString {
	private:
		std::basic_string<TCHAR> m_path;

	public:
		// コンストラクタ
		PathString() : m_path() {};
		PathString( const PathString& src ) : m_path( src.m_path ) {};
		PathString( LPCTSTR src ) : m_path( src ) {};
		PathString(const std::basic_string<TCHAR>& src) : m_path(src) {};

		// デストラクタ
		virtual ~PathString() {};

	public:
		// 文字列への const pointer を戻す
		LPCTSTR c_str() const { return m_path.c_str(); };

		// 内部文字列を返す
		std::basic_string<TCHAR> getString() const { return m_path; }

		// 文字列にパスを結合する
		PathString& Append( const PathString& more ) {
			std::vector<TCHAR> buf( MAX_PATH );
			
			::lstrcpy(&buf[0], m_path.c_str() );
			::PathAppend(&buf[0], more.c_str() );

			m_path = &buf[0];

			return *this;
		};

	public:
		// 代入演算子
		PathString& operator= ( const PathString& src ) {
			if (this == &src)	return *this;	// 同一オブジェクト代入チェック
			m_path = src.c_str();
			return *this;
		};
	};

	//-------------------------------------------------------------------------
	// netapi32.lib で提供される関数のラッパー
	// lm.h のインクルードと、netapi32.lib をリンク
	//-------------------------------------------------------------------------
	class netapi32 {
	private:
		netapi32() {}
		~netapi32() {}

	public:
		// ワークグループ名を取得する
		static std::basic_string<TCHAR> getWorkgroupName();
	};

	// API関数のラッパークラス
	//
	class win32 {
	private:
		win32() {};		// デフォルトコンストラクタ
		~win32() {};	// デストラクタ

	public:
		// ExpandEnvironmentStrings のラッパー
		//
		static std::basic_string<TCHAR> expand_environment_strings(LPCTSTR lpSrc) {
			DWORD dwSizeNeed;

			// 必要なバッファサイズを取得
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, NULL, 0);
			if (dwSizeNeed == 0) {
				return std::basic_string<TCHAR>();	// エラー
			}

			// 必要なバッファを確保して、再実行
			std::vector<TCHAR> buf(dwSizeNeed);
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, &buf[0], buf.size());
			if (dwSizeNeed != buf.size() - 1) {
				return std::basic_string<TCHAR>();	// エラー
			}

			return std::basic_string<TCHAR>(&buf[0]);	// 展開された文字列
		};

		static std::basic_string<TCHAR> format_message(DWORD dwErrorCode);	// エラーコードからシステムエラーメッセージを取得する。
		static std::basic_string<TCHAR> str_replace(const std::basic_string<TCHAR>& src, LPCTSTR pFind, LPCTSTR pReplace);	// 文字列を置換する。VBのStrReplaceのような関数。
	};

	//-------------------------------------------------------------------------
	// レジストリを扱うクラス
	//-------------------------------------------------------------------------
	class registry {
	public:
		// デフォルトコンストラクタ
		registry() : m_hkey(NULL) {}

		// コピーコンストラクタ
		registry(const registry& src);

		// デストラクタ
		virtual ~registry() { reg_close_key(); }

		// RegOpenKeyEx のラッパー
		//
		// hkey        開くキーへのハンドル
		// lpSubKey    開くべきサブキーの名前へのアドレス
		// samDesired  セキュリティアクセスマスク
		LONG reg_open_key_ex(HKEY hkey, LPCTSTR lpSubKey, REGSAM samDesired);

		// RegCloseKey のラッパー
		LONG reg_close_key();

		// RegQueryValueEx のラッパー
		//
		// lpValueName  検索すべき値の名前へのアドレス
		// lpType       値のタイプのためのバッファへのアドレス
		// lpData       データのバッファへのアドレス
		// lpcbData     データのバッファのサイズへのアドレス
		LONG reg_query_value_ex(LPTSTR lpValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) const;


		// RegSetValueEx のラッパー
		//
		// lpValueName  検索すべき値の名前へのアドレス
		// lpType       値のタイプのためのバッファへのアドレス
		// lpData       データのバッファへのアドレス
		// lpcbData     データのバッファのサイズへのアドレス
		LONG reg_set_value_ex(LPCTSTR lpValueName, DWORD dwType, CONST BYTE *lpData, DWORD cbData) {
			assert(m_hkey != NULL);
			return ::RegSetValueEx(m_hkey, lpValueName, 0, dwType, lpData, cbData);
		}
		
		// RegSetValueEx のラッパー
		//
		// lpValueName  検索すべき値の名前へのアドレス
		// dwData       データのバッファへのアドレス
		LONG reg_set_value_ex(LPCTSTR lpValueName, DWORD dwData) {
			return reg_set_value_ex(lpValueName, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));
		}
		
		// 文字列を取得する関数
		//
		// 値のタイプが
		//   REG_SZ のときは、そのまま返す。
		//   REG_EXPAND_SZ のときは、ExpandEnvironmentStrings を通してから返す。
		//   REG_DWORD のときは、_ultoa を通してから返す。
		std::basic_string<TCHAR> get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const;

	public:
		// 代入演算子
		registry& operator =(const registry& rhs);

	private:
		HKEY m_hkey;	// オープン中のレジストリへのハンドル
	};


	//-----------------------------------------------------------------
	// ユーティリティクラス
	//
	// [概要]
	// スタティックメンバーのみなので、インスタンス化できない。
	//-----------------------------------------------------------------
	class util {
	private:
		util() {};
		~util() {};

	public:
		//---------------------------------------------------------------------
		// Unicode 文字列を ANSI 文字列に変換
		//---------------------------------------------------------------------
		static std::string toAnsi(const std::wstring& src);

		// ANSI 文字列を Unicode 文字列に変換
		static std::wstring toUnicode(const std::string& src);

		//-------------------------------------------------------------
		// コピー元の直下にあるすべてのファイルをコピー先にコピーする
		//
		// [引数]
		// pszSrcPath  コピー元フォルダの名前
		// pszDstPath  コピー先フォルダの名前
		//
		// [戻り値]
		// 正常終了: NO_ERROR
		// エラー終了: Win32エラーコード
		//-------------------------------------------------------------
		static DWORD CopyFiles(LPCTSTR pszSrcPath, LPCTSTR pszDstPath);

		// プロセスのモジュール名を取得(GetModuleFileName 関数のラッパー)
		static std::basic_string<TCHAR> getModuleFileName();

		// プロセス名をstringで取得する関数
		static std::basic_string<TCHAR> GetModuleName() {
			return std::basic_string<TCHAR>(::PathFindFileName(kjm::util::getModuleFileName().c_str()));
		}

		// ini ファイルから文字列を取得する(GetPrivateProfileString 関数のラッパー)
		static std::basic_string<TCHAR> getPrivateProfileString(const std::basic_string<TCHAR>& section,
			const std::basic_string<TCHAR>& key, const std::basic_string<TCHAR>& defValue, const std::basic_string<TCHAR>& iniFile);

		static DWORD get_ipaddr(LPCSTR lpName);	// ホスト名または、IP文字列からIPアドレスを取得

		// ディレクトリを再帰的に削除する
		static bool remove_folder(LPCTSTR pszPath);

		// 文字列を返す安全な sprintf 関数
		static std::basic_string<TCHAR> sprintf_str(LPCTSTR format, ...);

		// ファイル名部分を取得
		static std::basic_string<TCHAR> findFileName(const std::basic_string<TCHAR>& path);

		// パス文字列にパスを追加
		static std::basic_string<TCHAR> append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more);

		// パス文字列の終端 \ を削除する
		static std::basic_string<TCHAR> removeBackslash(const std::basic_string<TCHAR>& path);

		// パス文字列の拡張子を変更する
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& ext);

		// パス文字列のファイル名を変更する
		static std::basic_string<TCHAR> renameFileSpec(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& fname);

		// パス文字列のファイル名部分を削除する
		static std::basic_string<TCHAR> removeFileSpec(const std::basic_string<TCHAR>& path);

		// ファイルの有無を確認する
		static bool fileExists(const std::basic_string<TCHAR>& path);

		// カレントディレクトリを変更する
		static bool setCurrentDirectory(const std::basic_string<TCHAR>& pathName);
		
		// ショートカットの作成
		//
		// strFileName : リンクファイルの名前
		// strFile : リンクを作成するファイルの名前
		// strArgs : リンクに渡す引数
		// strDir : 作業ディレクトリ
		//
		static bool createShortcut(
			const std::basic_string<TCHAR>& strFileName,
			const std::basic_string<TCHAR>& strFile,
			const std::basic_string<TCHAR>& strArgs, 
			const std::basic_string<TCHAR>& strDir);

		// スペシャルフォルダを取得する
		// 少なくとも、vs2008から有効な関数
#if _WIN32_IE >= 0x0700
		static std::basic_string<TCHAR> getSpecialFolderLocation(int nFolder);
#endif

		// 整数を文字列に変換
		static std::basic_string<TCHAR> to_a(int value, int radix = 10);

		// 整数を文字列に変換(printf相当のフォーマットが指定できる)
		static std::basic_string<TCHAR> to_a(int value, const std::basic_string<TCHAR>& format);

		// 複数ファイルを削除出る DeleteFile 
		static bool deleteFiles(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildcard);

		// %TEMP%フォルダを取得する
		static std::basic_string<TCHAR> getTempPath();

		// 指定フォルダ以下の読み込み専用属性を外す
		static bool UnsetReadOnly(const std::basic_string<TCHAR>& path);

		// 指定の共有フォルダと共有パスの存在を保証する
		static int guaranteeSharedFolder(LPCTSTR pszSharedFolder, LPCTSTR pszSharedPath);
	};


	//=========================================================================
	// ディレクトリやサブディレクトリを通じて、作成、移動、および列挙するための
	// 静的メソッドを公開します。
	//=========================================================================
	class directory {
	public:
		// ファイルまたはディレクトリ、およびその内容を新しい場所にコピーします。
		static BOOL copy(LPCTSTR sourceDirName, LPCTSTR destDirName);

		// ファイルを列挙します。
		static std::vector<std::basic_string<TCHAR> > getFiles(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

		// ディレクトリを列挙します。
		static std::vector<std::basic_string<TCHAR> > getSubFolders(
			const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard = _T("*.*"));

	private:
		directory() {};
		~directory() {};
	};


	/**
	 *	@brief	NOTIFYICONDATA をラップするクラス
	 */
	class BiNotifyIcon : private NOTIFYICONDATA {
	public:

		// デフォルト・コンストラクタ
		BiNotifyIcon();

		// デストラクタ
		virtual ~BiNotifyIcon();

	public:
		// タスクトレイにアイコンを追加する。
		BOOL Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip);

		// タスクトレイからアイコンを削除する
		BOOL Delete();

		// タスクトレイアイコンの情報を変更する。
		BOOL Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip);
	};

	
	// class mutex の投げる例外用の型
	//
	struct mutex_error {
		enum error_code {
			not_initialized = 1,	// mutex が未初期化
			already_initialized		// mutex が初期化済み
		};

		error_code error;
		mutex_error(error_code e) : error(e) {};
	};

	// mutex 管理クラス
	//
	// note: 作成したハンドルは、デストラクタで破棄される。
	//       また、lock(WaitForSingleObject)したままのハンドルは、
	//       必要回数unlock(ReleaseMutex)してから、破棄される。
	//
	class mutex {
	private:
		HANDLE m_hMutex;		// 管理する mutex へのハンドル
		DWORD m_create_status;	// mutex 作成時の状態変数
		DWORD m_last_error;		// API使用時のエラー保持
		int m_lock_count;		// lockした回数

		// 面倒なので、インスタンスの複製は不許可。
		mutex(const mutex &src) {};
		mutex& operator=(const mutex &src) {};

	public:
		// mutex 作成時に同名の mutex が存在したときは、ture を返す
		//
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// mutex作成済みかどうかのテスト
		//
		bool is_open() { return (m_hMutex != NULL); };

	public:
		// コンストラクタとデストラクタ
		//
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {};
		mutex(const std::basic_string<TCHAR>& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
			create(name);	// 例外は起こりえない。チェックは、is_openでチェック
		};
		virtual ~mutex() { close(); };

		// mutex の作成
		// 
		// arg : name = mutex の名前
		// ret : true = 正常終了、false = 以上終了
		// exp : mutex_error
		// note:
		//
		bool create(const std::basic_string<TCHAR>& name) {
			// 初期化済み instance の時は、例外を throw
			if (m_hMutex)	throw mutex_error(mutex_error::already_initialized);

			// mutex の作成
			m_hMutex = ::CreateMutex(NULL, FALSE, name.c_str());
			m_create_status = ::GetLastError();
			return (m_hMutex != 0);
		};
		
		// mutex を閉じる
		//
		// arg : なし
		// ret : なし
		// exp : なし
		// note: 未初期化のインスタンスに使用してもok
		//       lock中のオブジェクトに対しては、unlockしてからクローズする。
		//
		void close() {
			if (m_hMutex) {
				while (m_lock_count)
					if (!unlock())
						break;

				::CloseHandle(m_hMutex);
				m_hMutex = NULL;
			}
		};

		// 指定時間シグナル状態になるのを待つ
		//
		// arg : dwMillisec = タイムアウト時間(ms)
		// ret : WaitForSingleObject API 参照
		// exp : mutex_error
		// note: mutexが初期化済みの場合は、WaitForSingleObjectを呼ぶだけ。
		//
		DWORD lock(DWORD dwMillisec) {
			if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
			
			DWORD dw = ::WaitForSingleObject(m_hMutex, dwMillisec);
			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				++m_lock_count;
			}

			return dw;
		};

		// mutexを開放する
		//
		// arg : なし
		// ret : ReleaseMutex API 参照
		// exp : mutex_error
		// note: mutexが初期化済みの場合は、ReleaseMutex を呼ぶだけ。
		//
		BOOL unlock() {
			if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
			
			BOOL ret = ::ReleaseMutex(m_hMutex);
			if (ret) {
				--m_lock_count;
			}

			return ret;
		};
	};

	// class memfile の投げる例外用の型
	//
	struct memfile_error {
		enum error_code {
			not_initialized = 1,	// memfile が未初期化
			already_initialized		// memfile が初期化済み
		};
		error_code error;

		memfile_error(error_code e) : error(e) {};
	};

	// メモリマップドファイル管理クラス
	//
	// note: 作成したハンドルは、デストラクタで破棄される。
	//
	template<class T> class memfile {
	private:
		HANDLE m_hFile;
		DWORD m_create_status;

		// 面倒なので、インスタンスの複製は不許可。
		memfile<T>(const memfile<T> &src) {};
		memfile<T>& operator=(const memfile<T> &src) {};

	public:
		// memfile作成時に同名のmemfileが存在したときは、ture を返す
		//
		bool is_exists() { return (m_create_status == ERROR_ALREADY_EXISTS); };

		// memfile作成済みかどうかのテスト
		//
		bool is_open() { return (m_hFile != NULL); };

	public:
		// コンストラクタとデストラクタ
		//
		memfile<T>() : m_hFile(NULL), m_create_status(0) {};
		memfile<T>(const std::string& name, DWORD size = 0) : m_hFile(NULL), m_create_status(0) {
			// どちらも例外は起こりえない。初期化の成否は、is_openで確認。
			if (size == 0)	open(name);
			else			create(name, size);
		};
		virtual ~memfile<T>() { close(); };

		// メモリマップドファイルの作成
		//
		// arg : name = メモリマップドファイルの名前
		//       size = サイズ(要素数)
		// ret : false = 失敗。true = 成功
		// exp : memfile_error
		// note:
		//
		bool create(const std::string& name, DWORD size) {
			// 初期化済みinstanceのときは、例外をthrow
			if (m_hFile) throw memfile_error(memfile_error::already_initialized);

			// メモリマップドファイルの作成
			m_hFile = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(T) * size, name.c_str());
			m_create_status = ::GetLastError();
			return (m_hFile != NULL);
		};

		// メモリマップドファイルを開く
		//
		// arg : name = メモリマップドファイルの名前
		// ret : false = 失敗。true = 成功
		// exp : memfile_error
		// note:
		//
		bool open(const std::string& name) {
			// 初期化済みinstanceの時は、例外をthrow
			if (m_hFile) throw memfile_error(memfile_error::already_initialized);

			// メモリマップドファイルを開く
			m_hFile = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, TRUE, name.c_str());
			return (m_hFile != NULL);
		};

		// メモリマップドファイルを閉じる
		//
		// arg : なし
		// ret : なし
		// exp : なし
		// note: 未初期化のinstanceで使用してもok
		//
		void close() {
			if (m_hFile) {
				::CloseHandle(m_hFile);
				m_hFile = NULL;
			}
		};

		// メモリマップドファイルからデータを読み込む
		//
		// arg : buffer = 読み込むバッファへのポインタ
		//       offset = 先頭位置へのオフセット
		//       size = 読み込む要素数
		//       pmtx = 排他制御に使用するmutexへのポインタ(NULL可)
		//       timeout = 排他タイムアウト時間
		//
		int read(T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout) {
			// 未初期化instanceの時は、例外をthrow
			if (!m_hFile)	throw memfile_error(memfile_error::not_initialized);

			int ret = 0;
			DWORD dw = WAIT_OBJECT_0;

			// mutexへのポインタが与えられているときは、まずlock
			if (pmtx)	dw = pmtx->lock(timeout);

			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				
				T* p = (T*)::MapViewOfFile(m_hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (p) {
					memmove(buffer, &p[offset], sizeof(T) * size);
					::UnmapViewOfFile(p);
				} else {
					ret = -1;
				}

				// mutexへのポインタが与えられているときは、unlock
				if (pmtx)	pmtx->unlock();
			} else {
				ret = -2;
			}

			return ret;
		}

		// メモリマップドファイルへデータを書き込む
		//
		// arg : buffer = 読み込むバッファへのポインタ
		//       offset = 先頭位置へのオフセット
		//       size = 読み込む要素数
		//       pmtx = 排他制御に使用するmutexへのポインタ(NULL可)
		//       timeout = 排他タイムアウト時間
		//
		template <class T> int write(T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout) {
			// 未初期化instanceの時は、例外をthrow
			if (!m_hFile)	throw memfile_error(memfile_error::not_initialized);

			int ret = 0;
			DWORD dw = WAIT_OBJECT_0;

			// mutexへのポインタが与えられているときは、まずlock
			if (pmtx)	dw = pmtx->lock(timeout);

			if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
				
				T* p = (T*)::MapViewOfFile(m_hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0);
				if (p) {
					memmove(&p[offset], buffer, sizeof(T) * size);
					::UnmapViewOfFile(p);
				} else {
					ret = -1;
				}

				// mutexへのポインタが与えられているときは、unlock
				if (pmtx)	pmtx->unlock();
			} else {
				ret = -2;
			}

			return ret;
		}
	};

	// class lockfile の投げる例外用の型
	//
	struct lockfile_error {
		enum error_code {
			not_initialized = 1	// lockfile が未初期化
		};
		error_code error;

		lockfile_error(error_code e) : error(e) {};
	};

	// lockfileを簡単に扱うクラス
	//
	class lockfile {
	private:
		HANDLE m_hFile;			// ロックファイルへのハンドル
		std::basic_string<TCHAR> m_fullname;	// ロックファイルへのフルパス名

	public:
		// コンストラクタ・デストラクタ
		//
		lockfile(const std::basic_string<TCHAR>& fullname) : m_hFile(INVALID_HANDLE_VALUE), m_fullname(fullname) {};
		virtual ~lockfile() { unlock(); };

		// ロックする
		//
		bool lock() {
			// すでに自分でロックしているときは、trueを返す
			if (m_hFile != INVALID_HANDLE_VALUE)	return true;

			m_hFile = ::CreateFile(m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL);
			if (m_hFile == INVALID_HANDLE_VALUE) {

				// lockに失敗しても、ゴミの可能性があるので削除を試みる。
				::DeleteFile(m_fullname.c_str());
				m_hFile = ::CreateFile(m_fullname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY|FILE_FLAG_DELETE_ON_CLOSE, NULL);
			}
			
			return (m_hFile != INVALID_HANDLE_VALUE);
		};

		// ロックを解除する
		//
		void unlock() {
			if (m_hFile != INVALID_HANDLE_VALUE) {
				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;
			}
		};
	};

#if 0
	//-----------------------------------------------------------------
	// 簡易SOCKETクラス
	//-----------------------------------------------------------------
	class in_socket {
	private:
		SOCKET m_sock;

	public:
		in_socket() : m_sock(INVALID_SOCKET) {};	// コンストラクタ
		virtual ~in_socket() { close_socket(); };	// デストラクタ

		// ソケットを開く
		bool open_socket(int type) {
			assert(m_sock == INVALID_SOCKET);
			return (m_sock = socket(AF_INET, type, 0)) != INVALID_SOCKET;
		};

		// ソケットを閉じる
		int close_socket() {
			int ret = 0;
			if (m_sock != INVALID_SOCKET) {
				ret = ::closesocket(m_sock);
				m_sock = INVALID_SOCKET;
			}
			return ret;
		};

		// 伝聞送信
		int send_to(const char* to_addr, short port, const char* buf, int len) {
			assert(m_sock != INVALID_SOCKET);

			sockaddr_in to;
			memset(&to, 0, sizeof(to));
			to.sin_family = AF_INET;
			to.sin_addr.S_un.S_addr = kjm::util::get_ipaddr(to_addr);
			to.sin_port = htons(port);

			return sendto(m_sock, buf, len, 0, (sockaddr*)&to, sizeof(to));
		};
	};
#endif

	//-------------------------------------------------------------------------
	// イベントログにアクセスするためのクラス
	//
	// 現在のところ、バックアップのためのメソッドしかないため、インスタンス化
	// できるようになっていません。
	//-------------------------------------------------------------------------
	class event_log {
	private:
		// デフォルトコンストラクタ
		event_log() {}

		// デストラクタ
		~event_log() {}

	public:
		// イベントログをバックアップする
		//
		// バックアップ先のフォルダがない場合、エラーになります。
		// バックアップ先に同名ファイルがある場合、エラーになります。
		static BOOL backup(LPCTSTR sourceName, LPCTSTR backupFileName);

	};

	//-------------------------------------------------------------------------
	// プロセスの起動に関するクラス
	//-------------------------------------------------------------------------
	class process {
	public:
		// デフォルト コンストラクタ
		process();

		// デストラクタ
		virtual ~process() { this->Close(); }

	public:
		// スタートアップ情報をクリアする
		void ClearSI() { memset(&m_si, 0, sizeof(m_si)); }

		// スタートアップ情報(wShowWindow)を取得する
		WORD GetSIShowWindow() const { return m_si.wShowWindow; }

		// スタートアップ情報(wShowWindow)を設定する
		void SetSIShowWindow(WORD newShowWindow);

		// スタートアップ情報(wShowWindow)をクリア(未使用に)する
		void ClearSIShowWindow();

		// プロセスID取得
		DWORD GetProcessId() const { return m_pi.dwProcessId; }

		// 実行ファイルのコマンドライン取得
		LPCTSTR GetCommandLine() const { return m_szCommandLine; }

		// プロセスの終了コードを取得
		DWORD getExitCode();

		// プロセスハンドルが有効かどうか調べる
		bool IsHandleAvailable() { return (m_pi.hThread != NULL && m_pi.hProcess != NULL) ? true : false; }

		// プロセスが動作中かどうか調べる
		bool IsStillActive() { return (IsHandleAvailable() && (getExitCode() == STILL_ACTIVE)) ? true : false; }

	public:
		// プロセスの実行
		virtual BOOL createProcess(const std::basic_string<TCHAR>& commandLine);

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

	private:
		HWND m_hwnd;				// GetProcessWindow関数で使用

		STARTUPINFO m_si;					// スタートアップ情報
		PROCESS_INFORMATION m_pi;			// プロセス情報
		TCHAR m_szCommandLine[MAX_PATH];	// 実行コマンドライン記録用
											// Create関数実行後有効
		
		static BOOL CALLBACK process_EnumWindowsProc(HWND hwnd, LPARAM lParam);
	};

	// 基本ソケットクラス
	class baseSocket {
	public:
		// コンストラクタ
		baseSocket() : m_sock(INVALID_SOCKET) {}

		// デストラクタ
		virtual ~baseSocket() { closeSocket(); }

		// ソケットを開く
		bool openSocket(int af, int type, int protocol);

		// 受信タイムアウトの設定
		bool setRecvTimeout(int msTimeout);

		// バインドする(ポート番号を指定する簡単バージョンだけ準備)
		bool bindSocket(unsigned short port);

		// 受信する
		int recvfrom(std::vector<char>& buf, sockaddr_in& from);

		// 送信する
		int sendto(const std::vector<char>& buf, const sockaddr_in& to);

		// ソケットを閉じる
		bool closeSocket();

	public:
		// Winsock初期化
		static int startup();

		// Winsock後始末
		static int cleanup() { return ::WSACleanup(); }

	private:
		SOCKET m_sock;
	};

	// UDPソケットクラス
	class udpSocket : public kjm::baseSocket {
	public:
		// コンストラクタ
		udpSocket() {}

		// デストラクタ
		virtual ~udpSocket() {}

		// ソケットを開く
		bool openSocket() { return baseSocket::openSocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); }
	};

	//-------------------------------------------------------------------------
	// コマンドライン解析クラス
	//-------------------------------------------------------------------------
	class cmdLine {
	public:
		// デフォルトコンストラクタ
		cmdLine() {}

		// デストラクタ
		virtual ~cmdLine() {}

		// コマンドライン解析
		virtual int parse(int argc, TCHAR** argv);

	public:
		// オプションがあるかどうか調べる
		bool hasOption(const std::basic_string<TCHAR>& key) { return (m_options.find(key) != m_options.end()) ? true : false; }

		// オプションの引数を取得する
		std::basic_string<TCHAR> getOptArgument(const std::basic_string<TCHAR>& key);

		// オプション以外の引数の数を取得する
		int get_argCount() { return this->m_arguments.size(); }

		// N 番目のオプション以外の引数を取得する
		std::basic_string<TCHAR> get_argument(int n) { return this->m_arguments.at(n); }

	private:
		// オプションとその引数
		std::map<std::basic_string<TCHAR>, std::basic_string<TCHAR> > m_options;

		// オプション以外の引数
		std::vector<std::basic_string<TCHAR> > m_arguments;
	};

#if defined(_MT)
	/*!
	 *	スレッド管理クラス
	 */

	/*
		-- 使い方 --

		1. thread を派生する

		class my_thread : public thread { ... }

		2. 純粋仮想関数 worker() と end() を定義する
		   スレッドを終了するための具体的な処理は、派生クラスが行う。
		   使用する側は、 my_thread::end() を呼べば、スレッドが終了する。

		class my_thread : public thread {
		private:
			bool m_loop;	// 終了するためのフラグ(worker内で見る)
		public:
			my_thread() : m_loop(true) {};	// コンストラクタ
			virtual ~my_thread() {};		// デストラクタ
			unsigned worker() {
				while (m_loop) {
					Sleep(200);
					// TODO: 処理を書く
				}
				return 0;
			};
			void end() { m_loop = false; };
		};

		3. スレッドの開始と終了

		// 開始
		my_thread myThrd;	// インスタンス化
		myThrd.run();		// スレッドの実行

		// 終了
		myThrd.end();		// スレッドの終了
		myThrd.wait(5000);	// スレッド終了まで待機(5秒)

		wait() メソッドでスレッドハンドルを閉じるので、永続的なインスタンスを使用して、
		基本的にこの順番で呼び出す。
	*/

	class thread {
	private:
		HANDLE m_hThread;
		CRITICAL_SECTION m_cs;

	public:
		thread() : m_hThread( NULL ) {
			InitializeCriticalSection( &m_cs );
		};
		virtual ~thread() {
			DeleteCriticalSection( &m_cs );
		};

		void lock() { EnterCriticalSection( &m_cs ); };
		void unlock() { LeaveCriticalSection( &m_cs ); };

		//! スレッドが動作中かどうか調べる
		bool still_active() { 
			DWORD code;
			GetExitCodeThread( m_hThread, &code );
			return ( code == STILL_ACTIVE );
		};

		//! スレッドを起動する。
		void run() {
			m_hThread = ( HANDLE )_beginthreadex( NULL, 0, thread_func, this, 0, NULL );
		};

		/*!
		 * スレッドを終了させる。派生クラスで定義する。
		 * 終了方法は、worker method の実装に依存する。
		 */
		virtual void end() = 0;

		//! スレッドの一時停止
		DWORD suspend() {
			return SuspendThread( m_hThread );
		};

		//! スレッドの再開
		DWORD resume() {
			return ResumeThread( m_hThread );
		};

		/*!
		 * スレッドに呼び出される関数。派生クラスで定義する。
		 * end methodの呼び出しで終了できるように実装する。
		 */
		virtual unsigned worker() = 0;

		//---------------------------------------------------------------------
		// スレッドの終了を待つ。
		//---------------------------------------------------------------------
		DWORD wait( DWORD timeout = INFINITE ) {
			DWORD ret = 0;
			if (m_hThread != NULL) {
				ret = WaitForSingleObject(m_hThread, timeout);
				CloseHandle(m_hThread);
				m_hThread = NULL;
			}
			return ret;
		}

		/*!
		 * 共通のスレッド関数。
		 * argsには、threadクラスから派生したクラスオブジェクトへの
		 * ポインタを受け取る。p->worker() を実行するだけ。
		 */
		static unsigned __stdcall thread_func( void* args ) {
			return ( ( thread* )args )->worker();
		};
	};
#endif
}

#endif
