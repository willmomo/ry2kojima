//---------------------------------------------------------------------
// kjm::time class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <string>
#include <time.h>
#include "kjm_util.h"


namespace kjm {

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

		// 保持する日付を、ctime 関数で文字列にして返す。
		std::string ctime() {
			return kjm::util::chomp( std::string( ::ctime( &m_time ) ) );
		};

	public:

		// 現在日時を保持するインスタンスを返す。
		static kjm::time GetCurrentTime() {
			return kjm::time( ::time( NULL ) );
		};
	};

}
