//---------------------------------------------------------------------------
// System.IO like
//---------------------------------------------------------------------------

#pragma once

#include <afxtempl.h>	// CArray 用

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
// CArray, CStringArray にコピーコンストラクタ、代入演算子がないため、
// 派生クラスで実装。
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
	
	// コンピューター上のすべての論理ドライブのドライブ名を取得します。
	static CDriveInfoArray GetDrives();
	
	// __int64 AvailableFreeSpace() const {};
	// CString DriveFormat() const {};

	// ドライブ タイプを取得します。
	CDriveType DriveType() const;

	// bool IsReady() const {};

	// ドライブの名前を取得します。
	CString Name() const { return m_driveName; };

	// CDirectoryInfo RootDirectory() const {};
	// __int64 TotalFreeSpace() const {};
	// __int64 TotalSize() const {};
	// CString get_VolumeLabel() const;
	// void set_VolumeLabel(const CString& value);
	
private:
	CString m_driveName;
};
