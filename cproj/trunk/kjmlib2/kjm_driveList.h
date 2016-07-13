//---------------------------------------------------------------------
// ドライブの一覧を保持するクラス
//---------------------------------------------------------------------

#pragma once

#include "kjm_drive.h"

namespace kjm {
	class driveList {
	public:
		// デフォルトコンストラクタ
		driveList();

		// コピーコンストラクタ
		driveList(const driveList& src) : m_driveList(src.m_driveList) {};

		// デストラクタ
		virtual ~driveList() {};

		// 代入演算子
		driveList& operator=(const driveList& rhs);

		// ドライブ一覧の数を取得
		int get_Size() { return m_driveList.size(); };

		// ドライブオブジェクトを取得
		drive get_Drive(int index) { return m_driveList[index]; };

	private:
		std::vector<drive> m_driveList;	// PCにつながっているドライブ一覧
	};
};
