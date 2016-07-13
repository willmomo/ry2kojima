//---------------------------------------------------------------------
// kjm::mutex class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <string>


namespace kjm {

	//-----------------------------------------------------------------
	// class mutex の投げる例外用の型
	//-----------------------------------------------------------------
	struct mutex_error {
		enum error_code {
			not_initialized = 1,	// mutex が未初期化
			already_initialized		// mutex が初期化済み
		};

		error_code error;
		mutex_error(error_code e) : error(e) {};
	};


	//-----------------------------------------------------------------
	// mutex 管理クラス
	//
	// note: 作成したハンドルは、デストラクタで破棄される。
	//       また、lock(WaitForSingleObject)したままのハンドルは、
	//       必要回数unlock(ReleaseMutex)してから、破棄される。
	//-----------------------------------------------------------------
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
		//-------------------------------------------------------------
		// コンストラクタとデストラクタ
		//-------------------------------------------------------------
		mutex() : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {};
		mutex(const std::string& name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
			create(name);	// 例外は起こりえない。チェックは、is_openでチェック
		};
		virtual ~mutex() { close(); };


		// mutex の作成
		bool create( const std::string& name );

		// mutex を閉じる
		void close();

		// 指定時間シグナル状態になるのを待つ
		DWORD lock( DWORD dwMillisec );

		// mutex を開放する
		BOOL unlock();
	};
}
