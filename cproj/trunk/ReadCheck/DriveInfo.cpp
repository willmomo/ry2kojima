//---------------------------------------------------------------------------
// System.IO like
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "DriveInfo.h"

CDriveInfoArray& CDriveInfoArray::operator =(CDriveInfoArray& rhs) {
	this->RemoveAll();
	for (int i = 0; i < rhs.GetSize(); i++) {
		this->Add(rhs[i]);
	}
	return *this;
}

//---------------------------------------------------------------------------
// コンピューター上のすべての論理ドライブのドライブ名を取得します。
//---------------------------------------------------------------------------
CDriveInfoArray CDriveInfo::GetDrives() {
	CDriveInfoArray result;
	
	DWORD dwDrives = ::GetLogicalDrives();
	for (TCHAR i = 'A'; i <= 'Z'; i++) {
		if (dwDrives & (1 << (i - 'A'))) {
			result.Add(CDriveInfo(CString(i)));
		}
	}

	return result;
}

//---------------------------------------------------------------------------
// ドライブ タイプを取得します。
//---------------------------------------------------------------------------
CDriveType CDriveInfo::DriveType() const {
	CDriveType result = Unknown;
	CString msg;
	msg.Format(_T("%c:\\"), m_driveName[0]);

	switch (::GetDriveType(msg)) {
	case DRIVE_RAMDISK:
		result = Ram;
		break;
	case DRIVE_CDROM:
		result = CDRom;
		break;
	case DRIVE_REMOTE:
		result = Network;
		break;
	case DRIVE_FIXED:
		result = Fixed;
		break;
	case DRIVE_REMOVABLE:
		result = Removable;
		break;
	case DRIVE_NO_ROOT_DIR:
		result = NoRootDirectory;
		break;
	}

	return result;
}


void hoge() {
	CDriveInfoArray a;
	CDriveInfoArray b = a;
	CDriveInfoArray c;
	CDriveInfoArray d;
	
	c = a;

	d = CDriveInfo::GetDrives();
}
