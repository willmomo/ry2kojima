//-----------------------------------------------------------------------------
// �G���[����ێ�����N���X
//-----------------------------------------------------------------------------
#pragma once

#include "kjmBase.h"

namespace kjm {
	class errInfo {
	public:
		// �f�t�H���g�R���X�g���N�^
		errInfo() : m_dwInnerErr(0), m_dwErr(0), m_strMsg() {}

		// �G���[�ԍ��ƃ��b�Z�[�W���󂯂�R���X�g���N�^(Unicode)
		errInfo(DWORD dwErr, const std::wstring& strMsg) : m_dwInnerErr(0), m_dwErr(dwErr), m_strMsg(strMsg) {}

		// �G���[�ԍ��ƃ��b�Z�[�W���󂯂�R���X�g���N�^(Ascii)
		errInfo(DWORD dwErr, const std::string& strMsg) : m_dwInnerErr(0), m_dwErr(dwErr), m_strMsg(kjm::util::toUnicode(strMsg)) {}

		// �G���[�ԍ��������󂯎��R���X�g���N�^
		errInfo(DWORD dwErr);

		// �f�X�g���N�^
		virtual ~errInfo() {}

	public:
		// FormatError ���s���̃G���[�R�[�h
		DWORD get_innerErrNumber() { return m_dwInnerErr; }

		// �G���[�ԍ���Ԃ�
		DWORD get_errNumber() { return m_dwErr; }

		// �G���[�������Ԃ�(Unicode��)
		std::wstring get_errMessageW() { return m_strMsg; }

		// �G���[�������Ԃ�(SJIS��)
		std::string get_errMessageA() { return kjm::util::toSJIS(m_strMsg); }

#if defined(UNICODE) || defined(_UNICODE)
#define get_errMessage	get_errMessageW
#else
#define get_errMessage	get_errMessageA
#endif

	private:
		DWORD m_dwInnerErr;
		DWORD m_dwErr;
		std::wstring m_strMsg;
	};
};
