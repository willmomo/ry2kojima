//---------------------------------------------------------------------
// ドライブの情報を保持するクラス
//---------------------------------------------------------------------

#pragma once

#include "kjm_base.h"

namespace kjm {
	class drive {
	public:
		// デフォルトコンストラクタ
		drive() : m_driveNo(-1) {};

		// コピーコンストラクタ
		drive(const drive& src) : m_driveNo(src.m_driveNo) {};

		// ドライブ番号を指定するコンストラクタ
		drive(int driveNo) : m_driveNo(driveNo) {};

		// ドライブレターを指定するコンストラクタ
		drive(const _tstring& driveLetter);

		// デストラクタ
		virtual ~drive() {};

		// 代入演算子
		drive& operator=(const drive& rhs);

		// ドライブ番号を取得
		int get_DriveNo() { return m_driveNo; };

		// ドライブレターを取得
		_tstring get_DriveLetter() { return _tstring(1, (m_driveNo + 'A')) + _T(":"); };

		// ドライブタイプを取得
		UINT get_DriveType() { return GetDriveType(get_DriveLetter().c_str()); };

		// ドライブタイプを文字列で取得
		_tstring get_DriveTypeString();

		// ドライブにメディアが挿入されているかどうか調べる
		// どのドライブタイプにも使えるが、
		// get_DriveType() == DRIVE_REMOVABLE のとき、使ってください。
		bool isDiskInDrive();

	private:
		int m_driveNo;	// ドライブ番号(-1=未選択 0=A 1=B...)
	};
};
