#pragma once

#define _WIN32_WINNT 0x400

#include <windows.h>
#include <vector>
#include <ctime>

#include "smart.h"

#pragma comment( lib, "shlwapi.lib" )

#define countof(A)	(sizeof((A))/sizeof((A)[0]))

namespace kjm {


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

	//-----------------------------------------------------------------
	// time_t型を扱うクラス
	//-----------------------------------------------------------------
	class time {
	private:
		time_t m_time;

	public:
		time() : m_time( 0 ) {}; 
		time( time_t t ) : m_time( t ) {};
		virtual ~time() {};

		std::string ctime();

	public:
		static kjm::time GetCurrentTime();
	};


	//-----------------------------------------------------------------
	// ライブラリモジュールを管理するクラス
	//-----------------------------------------------------------------
	class library {
	private:
		HINSTANCE m_hLibModule;

	public:
		// アクセサ
		HINSTANCE getInstance() { return m_hLibModule; };

	public:
		library();
		virtual ~library();

	public:
		BOOL LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags );
		BOOL FreeLibrary();
	};


	//-----------------------------------------------------------------
	// レジストリを管理するクラス
	//-----------------------------------------------------------------
	class registry {
	private:
		HKEY m_hKey;

	public:
		// アクセサ
		HKEY getKey() { return m_hKey; };

	public:
		registry();
		virtual ~registry();

	public:
		LONG RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired );
		LONG RegCloseKey();
		LONG RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data );
	};


	//-----------------------------------------------------------------
	// イベントログの1レコード分を管理するクラス
	//-----------------------------------------------------------------
	class eventlog_record {
	private:
		std::string m_source_name;		// イベントソース
		std::vector<BYTE> m_raw_record;	// 生情報
		
		EVENTLOGRECORD* getRaw() { return (EVENTLOGRECORD*)m_raw_record.begin(); };
		std::string DispMessage( const char *SourceName, const char *EntryName, const char **Args, DWORD MessageId);
		int GetArgs(const EVENTLOGRECORD *pBuf, std::vector<char*>& args );
		BOOL GetModuleNameFromSourceName( const char *SourceName, const char *EntryName, std::vector<TCHAR>& ExpandedName);

	public:
		// アクセサ
		DWORD RecordNumber() { return getRaw()->RecordNumber; };
		kjm::time TimeGenerated() { return kjm::time( getRaw()->TimeGenerated ); };
		kjm::time TimeWritten() { return kjm::time( getRaw()->TimeWritten ); };
		DWORD EventID() { return getRaw()->EventID; };
		WORD EventType() { return getRaw()->EventType; };
		WORD EventCategory() { return getRaw()->EventCategory; };
		std::string EventCategoryText() { return DispMessage( SourceName().c_str(), "CategoryMessageFile", NULL, EventCategory()); };
		std::string EventIDText() {
			std::vector<char*> Args;
			GetArgs(getRaw(), Args);

			return DispMessage(SourceName().c_str(), "EventMessageFile", (const char **)Args.begin(), EventID());
		};
		DWORD DataLength() { return getRaw()->DataLength; };
		DWORD DataOffset() { return getRaw()->DataOffset; };
		std::string SourceName();
		std::string ComputerName();
		std::vector<BYTE> Data();

	public:
		eventlog_record();
		eventlog_record( const kjm::eventlog_record& src );
		eventlog_record( LPCTSTR lpSourceName, const EVENTLOGRECORD* src );
		virtual ~eventlog_record();

		kjm::eventlog_record& operator= ( const kjm::eventlog_record& src );
	};


	//-----------------------------------------------------------------
	// イベントログを管理するクラス
	//-----------------------------------------------------------------
	class eventlog {
	private:
		HANDLE m_hEventLog;
		std::string m_unc_server_name;
		std::string m_source_name;

	public:
		// アクセサ
		HANDLE getHandle() { return m_hEventLog; };

	public:
		eventlog();
		virtual ~eventlog();

	public:
		BOOL OpenEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName );
		BOOL OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName );
		BOOL CloseEventLog();
		BOOL ReadEventLog( DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventlog_record& record );
		BOOL BackupEventLog( LPCTSTR lpBackupFileName );
	};



	//-----------------------------------------------------------------
	// 物理ドライブ(SMART)を処理するクラス
	//-----------------------------------------------------------------
	ATTRVALUEANDNAME attrValueAndNames[];
	#define	NUM_ATTRIBUTE_STRUCTS	30

	class physical_drive {
	private:
		HANDLE m_disk_handle;

	public:
		// アクセサ
		HANDLE getHandle() { return m_disk_handle; };
		int getTemperature();
		int getDrivePowerCycleCount();
		int getPowerOnHours();
		int getStartStopCount();

	public:
		physical_drive();
		virtual ~physical_drive();

		BOOL OpenDrive( LPCTSTR driveName );
		BOOL CloseDrive();

		bool isSupportedSMART();
		int identifyDevice(IDENTIFYDATA* identifyData);
		int smartDisable();
		int smartEnable();
		int smartEnableDisableAutoSave(char enable);
		int smartReturnStatus();
		int smartReadAttributeValues(SMARTATTRIBUTESDATA* attrData);
		int smartReadAttributeThresholds(SMARTHRESHOLDSDATA* thData);

		std::string toCSVString();
	};


	//-----------------------------------------------------------------
	// パス用の処理を追加した string クラス
	//-----------------------------------------------------------------
	class path_string : public std::string {
	private:

	public:
		path_string();
		path_string( const path_string& rhs );
		path_string( std::string::const_pointer s );
		virtual ~path_string();

		BOOL FileExists();
		path_string& RemoveFileSpec();
		path_string& Append( LPCTSTR pMore );
		path_string& RenameExtension( LPCTSTR pszExt );

	public:
		static path_string GetModuleFileName( HMODULE hModule = NULL );
	};


	//-----------------------------------------------------------------
	// 他言語 like な便利関数
	//-----------------------------------------------------------------
	std::string& chomp( std::string& s );


	//-----------------------------------------------------------------
	// APIを安全に扱う関数群
	//-----------------------------------------------------------------
	DWORD ExpandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst );
	DWORD FormatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments );
	DWORD GetModuleFileName( HMODULE hModule, std::vector<TCHAR>& rFilename );
	DWORD GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::vector<TCHAR>& returnedString, LPCTSTR lpFileName );
	std::string kjm_LoadString( HINSTANCE hInstance, UINT uID );


	//-----------------------------------------------------------------
	// ただ単に便利な関数
	//-----------------------------------------------------------------
	BOOL MakeDirectory( LPCTSTR lpDir );	// 多階層対応のディレクトリ作成

};
