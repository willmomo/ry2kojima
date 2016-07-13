//---------------------------------------------------------------------
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

#if !defined(__KJMLIB_H)
#define __KJMLIB_H

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <string>
#include <string>
#include <vector>

#include <assert.h>
#include <process.h>

#if defined(USE_WSSLOG)
#include "wsslog.h"
#endif


namespace kjm {
	// OutputDebugString を簡単に使うための関数
	void ods(LPCTSTR format, ...);

	void log_info(const char* signature, const char* format, ...);	// 情報ログを残す関数
	void log_error(const char* signature, const char* format, ...);	// エラーログを残す関数

	BOOL create_directory(LPCTSTR lpPathName);	// 再帰的にディレクトリを作成できる CreateDirectory 関数
	
	// パス名を扱う文字列
	//
	class PathString
	{
	private:
		std::string m_path;

	public:
		// コンストラクタ
		PathString() : m_path() {};
		PathString( const PathString& src ) : m_path( src.m_path ) {};
		PathString( const char* src ) : m_path( src ) {};

		// デストラクタ
		virtual ~PathString() {};

	public:
		// 文字列への const pointer を戻す
		const char* c_str() const { return m_path.c_str(); };

		// 文字列にパスを結合する
		PathString& Append( const PathString& more ) {
			std::vector<char> buf( MAX_PATH );
			
			///::lstrcpy( buf.begin(), m_path.c_str() );
			::lstrcpy( &buf.at(0), m_path.c_str() );
			///::PathAppend( buf.begin(), more.c_str() );
			::PathAppend( &buf.at(0), more.c_str() );

			///m_path = buf.begin();
			m_path = &buf.at(0);

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

	// API関数のラッパークラス
	//
	class win32 {
	private:
		win32() {};		// デフォルトコンストラクタ
		~win32() {};	// デストラクタ

	public:
		// ExpandEnvironmentStrings のラッパー
		//
		static std::string expand_environment_strings(LPCTSTR lpSrc) {
			DWORD dwSizeNeed;

			// 必要なバッファサイズを取得
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, NULL, 0);
			if (dwSizeNeed == 0) {
				return std::string();	// エラー
			}

			// 必要なバッファを確保して、再実行
			std::vector<char> buf(dwSizeNeed);
			///dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, buf.begin(), buf.size());
			dwSizeNeed = ::ExpandEnvironmentStrings(lpSrc, &buf.at(0), buf.size());
			if (dwSizeNeed != buf.size() - 1) {
				return std::string();	// エラー
			}

			///return std::string(buf.begin());	// 展開された文字列
			return std::string(&buf.at(0));	// 展開された文字列
		};

		static std::string format_message(DWORD dwErrorCode);	// エラーコードからシステムエラーメッセージを取得する。
		static std::string str_replace(const std::string& src, LPCTSTR pFind, LPCTSTR pReplace);	// 文字列を置換する。VBのStrReplaceのような関数。
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
		std::string get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const;

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
		static std::string GetModuleName() {
			return std::string(::PathFindFileName(kjm::util::getModuleFileName().c_str()));
		}

		// ini ファイルから文字列を取得する(GetPrivateProfileString 関数のラッパー)
		static std::basic_string<TCHAR> getPrivateProfileString(LPCTSTR section, LPCTSTR key, LPCTSTR defValue, LPCTSTR iniFile);

		static DWORD get_ipaddr(LPCSTR lpName);	// ホスト名または、IP文字列からIPアドレスを取得

		//-------------------------------------------------------------
		// ディレクトリを再帰的に削除する
		//-------------------------------------------------------------
		static bool remove_folder(LPCTSTR pszPath);

		// パス文字列の拡張子を変更する
		static std::basic_string<TCHAR> renameExtension(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& ext);

		// パス文字列のファイル名を変更する
		static std::basic_string<TCHAR> renameFileSpec(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& fname);
	};


	//=========================================================================
	// ディレクトリやサブディレクトリを通じて、作成、移動、および列挙するための
	// 静的メソッドを公開します。
	//=========================================================================
	class directory {
	public:
		// ファイルまたはディレクトリ、およびその内容を新しい場所にコピーします。
		static BOOL copy(LPCTSTR sourceDirName, LPCTSTR destDirName);

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
		BOOL Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, const char* pszTip);

		// タスクトレイからアイコンを削除する
		BOOL Delete();

		// タスクトレイアイコンの情報を変更する。
		BOOL Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, char* pszTip);
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
		mutex(const std::string& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
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
		bool create(const std::string& name) {
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
		std::string m_fullname;	// ロックファイルへのフルパス名

	public:
		// コンストラクタ・デストラクタ
		//
		lockfile(const std::string& fullname) : m_hFile(INVALID_HANDLE_VALUE), m_fullname(fullname) {};
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
		DWORD wait(DWORD timeout = INFINITE) {
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

	public:
		// プロセスの実行
		virtual BOOL createProcess(const std::basic_string<TCHAR>& commandLine);

		// プロセスハンドルを閉じる。プロセスの終了ではないので注意。
		// プロセスの終了は、プロセス間の取り決めによる。
		void Close();

		// プロセスの終了を待つ
		virtual DWORD WaitForTermination(DWORD dwMilliseconds = INFINITE) { return ::WaitForSingleObject(m_pi.hProcess, dwMilliseconds); }

		// プロセスの起動完了を待つ
		virtual DWORD waitForInputIdle(DWORD dwMilliseconds = INFINITE) { return ::WaitForInputIdle(m_pi.hProcess, dwMilliseconds); }

		// プロセスに関連付けられたウィンドウを返す
		HWND GetProcessWindow();

	private:
		HWND m_hwnd;				// GetProcessWindow関数で使用

		STARTUPINFO m_si;					// スタートアップ情報
		PROCESS_INFORMATION m_pi;			// プロセス情報
		TCHAR m_szCommandLine[MAX_PATH];	// 実行コマンドライン記録用
											// Create関数実行後有効
		
		static BOOL CALLBACK process_EnumWindowsProc(HWND hwnd, LPARAM lParam);
	};
}

#endif
