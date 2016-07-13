//---------------------------------------------------------------------
// $Header: /GLw3.0.0.x/WssLog/syslogc.h 2     07/09/07 18:23 Kojima $ 
//---------------------------------------------------------------------
#pragma once


#include "kjmlib.h"


// syslog client �� thread �N���X
//
class syslog_client : public kjm::thread {
private:
	bool m_loop;	// �I�����邽�߂̃t���O(worker���Ō���)

public:
	syslog_client() : m_loop(true) {};	// �R���X�g���N�^
	virtual ~syslog_client() {};		// �f�X�g���N�^

	unsigned worker();					// ���[�J�X���b�h(�����́AWssLog.cpp�ɂ���)
	void end() { m_loop = false; };		// �I���w���֐�
};


/**

$History: syslogc.h $ 
 * 
 * *****************  Version 2  *****************
 * User: Kojima       Date: 07/09/07   Time: 18:23
 * Updated in $/GLw3.0.0.x/WssLog
 * Embedded�Ή��B�X���b�h�𗘗p�����x���������݌^�ɕύX�B

**/
