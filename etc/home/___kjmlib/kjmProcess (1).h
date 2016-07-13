//---------------------------------------------------------------------
// �v���Z�X�������N���X
//
// 2009/01/24 ����  �V�K�쐬
//---------------------------------------------------------------------
#pragma once

#include <windows.h>

namespace kjm {
	// �v���Z�X�������N���X
	class Process {
	private:
		PROCESS_INFORMATION m_pi;
		STARTUPINFO m_si;

	public:
		Process();
		virtual ~Process() { CloseProcessHandle(); };

		bool IsStillActive();		// �v���Z�X�����쒆���ǂ������ׂ�
		bool Run(LPCTSTR cmdLine);	// �v���Z�X�����s����
		void CloseProcessHandle();
	};
};
