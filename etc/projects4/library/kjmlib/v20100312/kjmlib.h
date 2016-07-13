//---------------------------------------------------------------------
// ���x������Ă� codesnipet
//---------------------------------------------------------------------
#pragma once

#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>

#if defined(UNICODE)
typedef std::wstring	_tstring;
#else
typedef std::string		_tstring;
#endif

// ���S�� sprintf 
int sprintf_vec(std::vector<char>& buf, const char* format, ...);

namespace kjm {
	//-----------------------------------------------------------------
	// ���ԃN���X
	//-----------------------------------------------------------------
	class time {
	public:
		// �f�t�H���g�R���X�g���N�^
		time();

		// �f�X�g���N�^
		~time();

		// �������P���� YYYY/MM/DD HH:MM:SS ������ɕϊ����܂��B
		_tstring to_string();

		// ���ݎ��Ԃ�100ns�P�ʂŕԂ�
		__int64 get_time() { return *(__int64*)&m_ft; };

		// GetLocalTime
		// ���݂̃��[�J���������擾���܂��B
		static kjm::time get_local_time();
	private:
		FILETIME m_ft;
	};
};
