#pragma once

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <windows.h>
#include <tchar.h>

#include "kjmlib.h"

#include "resource.h"

extern kjm::log g_log;	// �O���[�o���ȃ��O�o�̓N���X

class ssthread : public kjm::thread {
public:
	// �R���X�g���N�^
	ssthread() {};

	// �f�X�g���N�^
	virtual ~ssthread() {};

	// ���[�J�[�X���b�h
	virtual unsigned worker();
};
