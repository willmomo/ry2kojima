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
	// time_t�^�������N���X
	//-----------------------------------------------------------------
	class time {
	private:
		time_t m_time;

	public:
		time() : m_time( 0 ) {}; 
		time( time_t t ) : m_time( t ) {};
		virtual ~time() {};

		// �ێ�������t���Actime �֐��ŕ�����ɂ��ĕԂ��B
		std::string ctime() {
			return kjm::util::chomp( std::string( ::ctime( &m_time ) ) );
		};

	public:

		// ���ݓ�����ێ�����C���X�^���X��Ԃ��B
		static kjm::time GetCurrentTime() {
			return kjm::time( ::time( NULL ) );
		};
	};

}
