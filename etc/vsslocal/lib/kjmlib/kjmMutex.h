//-----------------------------------------------------------------------------
// ミューテックスハンドルを管理するクラス
//-----------------------------------------------------------------------------
#pragma once

#include "kjmBase.h"
#include "kjmErrInfo.h"

namespace kjm {
	class mutex {
	public:
		// デフォルトコンストラクタ
		mutex() : m_hMutex(NULL) {}

		// デストラクタ
		virtual ~mutex() {}

		// ミューテックスオブジェクトの作成(Unicode)
		bool create(const std::wstring& strName);

		// ミューテックスオブジェクトの作成(SJIS)
		bool create(const std::string& strName) { return create(kjm::util::toUnicode(strName)); }

		// ミューテックスオブジェクトを閉じる
		bool close();

		// ロックする
		DWORD lock(DWORD dwMilliseconds);

		// アンロックする
		void unlock();

	private:
		HANDLE m_hMutex;	// ミューテックスへのハンドル
	};
};
