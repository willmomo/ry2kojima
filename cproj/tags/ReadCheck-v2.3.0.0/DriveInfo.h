//---------------------------------------------------------------------------
// System.IO like
//---------------------------------------------------------------------------

#pragma once

#include <afxtempl.h>	// CArray �p

enum CDriveType {
	Unknown,
	NoRootDirectory,
	Removable,
	Fixed,
	Network,
	CDRom,
	Ram
};

//---------------------------------------------------------------------------
// CArray, CStringArray �ɃR�s�[�R���X�g���N�^�A������Z�q���Ȃ����߁A
// �h���N���X�Ŏ����B
//---------------------------------------------------------------------------
class CDriveInfo;

class CDriveInfoArray : public CArray<CDriveInfo, CDriveInfo&> {
public:
	CDriveInfoArray() {};
	CDriveInfoArray(CDriveInfoArray& src) { *this = src; };
	virtual ~CDriveInfoArray() {};

	CDriveInfoArray& operator=(CDriveInfoArray& rhs);
};

class CDriveInfo {
public:
	CDriveInfo() : m_driveName() {};
	CDriveInfo(const CString& driveName) : m_driveName(driveName) {};
	~CDriveInfo() {};
	
	// �R���s���[�^�[��̂��ׂĂ̘_���h���C�u�̃h���C�u�����擾���܂��B
	static CDriveInfoArray GetDrives();
	
	// __int64 AvailableFreeSpace() const {};
	// CString DriveFormat() const {};

	// �h���C�u �^�C�v���擾���܂��B
	CDriveType DriveType() const;

	// bool IsReady() const {};

	// �h���C�u�̖��O���擾���܂��B
	CString Name() const { return m_driveName; };

	// CDirectoryInfo RootDirectory() const {};
	// __int64 TotalFreeSpace() const {};
	// __int64 TotalSize() const {};
	// CString get_VolumeLabel() const;
	// void set_VolumeLabel(const CString& value);
	
private:
	CString m_driveName;
};
