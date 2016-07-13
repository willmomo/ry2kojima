//---------------------------------------------------------------------
// ドライブの情報を保持するクラス
//---------------------------------------------------------------------

#include "kjm_drive.h"

//---------------------------------------------------------------------
// ドライブレターを指定するコンストラクタ
//---------------------------------------------------------------------
kjm::drive::drive(const _tstring& driveLetter) {
	m_driveNo = (int)CharUpper((LPTSTR)driveLetter[0]) - 'A';
}

//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
kjm::drive& kjm::drive::operator =(const kjm::drive& rhs) {
	if (this == &rhs) return *this;

	m_driveNo = rhs.m_driveNo;

	return *this;
}

//---------------------------------------------------------------------
// ドライブタイプを文字列で取得
//---------------------------------------------------------------------
kjm::_tstring kjm::drive::get_DriveTypeString() {
	_tstring result;

	switch (get_DriveType()) {
	case DRIVE_UNKNOWN:
		result = _T("不明なタイプ");
		break;

	case DRIVE_NO_ROOT_DIR:
		result = _T("存在しないドライブ");
		break;

	case DRIVE_REMOVABLE:
		result = _T("リムーバブルドライブ");
		break;
		
	case DRIVE_FIXED:
		result = _T("固定ドライブ");
		break;

	case DRIVE_REMOTE:
		result = _T("ネットワークドライブ");
		break;

	case DRIVE_CDROM:
		result = _T("CD-ROMドライブ");
		break;

	case DRIVE_RAMDISK:
		result = _T("RAMディスク");
		break;

	default:
		result = _T("未対応のタイプ");
		break;
	}

	return result;
}


//----------------------------------------------------------------------------
// ドライブにメディアが挿入されているかどうか調べる
//---------------------------------------------------------------------
bool kjm::drive::isDiskInDrive() {
	kjm::_tstring volume = kjm::_tstring(_T("\\\\.\\")) + this->get_DriveLetter().substr(0, 2);

	HANDLE h = CreateFile(
		volume.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if (h == INVALID_HANDLE_VALUE) {
		DWORD lastError = GetLastError();
		// 2 means "no disk", anything else means by inference "disk
		// in drive, but you do not have admin privs to do a
		// CreateFile on that volume".
		return lastError != 2;
	}

	DWORD bytesReturned; // ignored
	BOOL devSuccess = DeviceIoControl(h, FSCTL_IS_VOLUME_MOUNTED, NULL, 0, NULL, 0, &bytesReturned, NULL);

	if(devSuccess == FALSE) {
		DWORD lastError = GetLastError();
		(void)lastError; // For debugging.
		// Presumably, any error means "no disk in drive", or more
		// accurately, "no volume mounted on that drive letter".
		CloseHandle(h);
		return false;
	}

	CloseHandle(h);
	return true;
}
