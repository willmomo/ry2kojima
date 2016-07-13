//---------------------------------------------------------------------
// kjm::lockfile class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <string>


namespace kjm {

	//-----------------------------------------------------------------
	// class lockfile の投げる例外用の型
	//-----------------------------------------------------------------
	struct lockfile_error {
		enum error_code {
			not_initialized = 1	// lockfile が未初期化
		};
		error_code error;

		lockfile_error(error_code e) : error(e) {};
	};


	//-----------------------------------------------------------------
	// lockfileを簡単に扱うクラス
	//-----------------------------------------------------------------
	class lockfile {
	private:
		HANDLE m_hFile;			// ロックファイルへのハンドル
		std::string m_fullname;	// ロックファイルへのフルパス名


	private:
		//-------------------------------------------------------------
		// デフォルトコンストラクタは封印
		//-------------------------------------------------------------
		lockfile() : m_hFile( INVALID_HANDLE_VALUE ), m_fullname() {};

	public:
		//-------------------------------------------------------------
		// コンストラクタ・デストラクタ
		//-------------------------------------------------------------
		lockfile(const std::string& fullname) : m_hFile(INVALID_HANDLE_VALUE), m_fullname(fullname) {};
		virtual ~lockfile() { unlock(); };

		// ロックする
		bool lock();

		// ロックを解除する
		void unlock();
	};

}
