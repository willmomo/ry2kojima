//---------------------------------------------------------------------
// kjm::memfile class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <string>
#include "kjm_mutex.h"


namespace kjm {

	//-----------------------------------------------------------------
	// class memfile の投げる例外用の型
	//-----------------------------------------------------------------
	struct memfile_error {
		enum error_code {
			not_initialized = 1,	// memfile が未初期化
			already_initialized		// memfile が初期化済み
		};
		error_code error;

		memfile_error(error_code e) : error(e) {};
	};


	//-----------------------------------------------------------------
	// メモリマップドファイル管理クラス
	//
	// note: 作成したハンドルは、デストラクタで破棄される。
	//-----------------------------------------------------------------
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
		//-------------------------------------------------------------
		// コンストラクタとデストラクタ
		//-------------------------------------------------------------
		memfile<T>() : m_hFile( NULL ), m_create_status( 0 ) {};
		memfile<T>( const std::string& name, DWORD size = 0 ) : m_hFile( NULL ), m_create_status( 0 ) {
			// どちらも例外は起こりえない。初期化の成否は、is_openで確認。
			if ( size == 0 )	open( name );
			else				create( name, size );
		};
		virtual ~memfile<T>() { close(); };

		// メモリマップドファイルの作成
		bool create(const std::string& name, DWORD size);

		// メモリマップドファイルを開く
		bool open( const std::string& name );

		// メモリマップドファイルを閉じる
		void close();

		// メモリマップドファイルからデータを読み込む
		int read( T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout );

		// メモリマップドファイルへデータを書き込む
		int write( T* buffer, size_t offset, size_t size, mutex* pmtx, DWORD timeout );
	};
}
