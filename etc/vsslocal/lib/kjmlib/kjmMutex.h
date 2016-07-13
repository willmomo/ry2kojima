//-----------------------------------------------------------------------------
// �~���[�e�b�N�X�n���h�����Ǘ�����N���X
//-----------------------------------------------------------------------------
#pragma once

#include "kjmBase.h"
#include "kjmErrInfo.h"

namespace kjm {
	class mutex {
	public:
		// �f�t�H���g�R���X�g���N�^
		mutex() : m_hMutex(NULL) {}

		// �f�X�g���N�^
		virtual ~mutex() {}

		// �~���[�e�b�N�X�I�u�W�F�N�g�̍쐬(Unicode)
		bool create(const std::wstring& strName);

		// �~���[�e�b�N�X�I�u�W�F�N�g�̍쐬(SJIS)
		bool create(const std::string& strName) { return create(kjm::util::toUnicode(strName)); }

		// �~���[�e�b�N�X�I�u�W�F�N�g�����
		bool close();

		// ���b�N����
		DWORD lock(DWORD dwMilliseconds);

		// �A�����b�N����
		void unlock();

	private:
		HANDLE m_hMutex;	// �~���[�e�b�N�X�ւ̃n���h��
	};
};
