#pragma once

#include "kjm_base.h"

namespace kjm {
	class tool {
	public:
		// 行末の改行を取り除く
		static _tstring chomp(const _tstring& str);
		
		// エラーコードでメッセージの作成
		static _tstring formatMessage(DWORD dwError);

	private:
		tool() {}
		~tool() {}
	};
};
