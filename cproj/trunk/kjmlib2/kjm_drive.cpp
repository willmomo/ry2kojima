//---------------------------------------------------------------------
// �h���C�u�̏���ێ�����N���X
//---------------------------------------------------------------------

#include "kjm_drive.h"

//---------------------------------------------------------------------
// �h���C�u���^�[���w�肷��R���X�g���N�^
//---------------------------------------------------------------------
kjm::drive::drive(const _tstring& driveLetter) {
	m_driveNo = (int)CharUpper((LPTSTR)driveLetter[0]) - 'A';
}

//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::drive& kjm::drive::operator =(const kjm::drive& rhs) {
	if (this == &rhs) return *this;

	m_driveNo = rhs.m_driveNo;

	return *this;
}

//---------------------------------------------------------------------
// �h���C�u�^�C�v�𕶎���Ŏ擾
//---------------------------------------------------------------------
kjm::_tstring kjm::drive::get_DriveTypeString() {
	_tstring result;

	switch (get_DriveType()) {
	case DRIVE_UNKNOWN:
		result = _T("�s���ȃ^�C�v");
		break;

	case DRIVE_NO_ROOT_DIR:
		result = _T("���݂��Ȃ��h���C�u");
		break;

	case DRIVE_REMOVABLE:
		result = _T("�����[�o�u���h���C�u");
		break;
		
	case DRIVE_FIXED:
		result = _T("�Œ�h���C�u");
		break;

	case DRIVE_REMOTE:
		result = _T("�l�b�g���[�N�h���C�u");
		break;

	case DRIVE_CDROM:
		result = _T("CD-ROM�h���C�u");
		break;

	case DRIVE_RAMDISK:
		result = _T("RAM�f�B�X�N");
		break;

	default:
		result = _T("���Ή��̃^�C�v");
		break;
	}

	return result;
}


//----------------------------------------------------------------------------
// �h���C�u�Ƀ��f�B�A���}������Ă��邩�ǂ������ׂ�
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
