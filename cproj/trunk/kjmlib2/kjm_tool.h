#pragma once

#include "kjm_base.h"

namespace kjm {
	class tool {
	public:
		// �s���̉��s����菜��
		static _tstring chomp(const _tstring& str);
		
		// �G���[�R�[�h�Ń��b�Z�[�W�̍쐬
		static _tstring formatMessage(DWORD dwError);

	private:
		tool() {}
		~tool() {}
	};
};
