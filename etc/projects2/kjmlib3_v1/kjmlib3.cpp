#include "kjmlib3.h"

//---------------------------------------------------------------------
// ���̃��C�u���������O���o�͂��邽�߂Ɏg�p����O���[�o���ȕϐ�
//---------------------------------------------------------------------
static kl3::log* s_pLog = NULL;

//---------------------------------------------------------------------
// ���C�u���������O�o�͂Ɏg�p���� kl3::log �I�u�W�F�N�g��ݒ�
//---------------------------------------------------------------------
void kl3::set_log_object(kl3::log* pLog) {
	s_pLog = pLog;
}

_tostream& operator<<(_tostream& os, __int64 i) {
	TCHAR buf[20];
	_stprintf(buf, _T("%I64d"), i);
	os << buf;
	return os;
}

_tstring kl3::util::get_module_file_name() {
	TCHAR szFileName[MAX_PATH];

	GetModuleFileName(NULL, szFileName, sizeof(szFileName));

	return szFileName;
}

_tstring kl3::util::get_file_version(const _tstring &strFileName) {
	DWORD dwHandle = 0;
	DWORD dwVersionInfoSize = 0;

	TCHAR work[MAX_PATH];
	dwVersionInfoSize = GetFileVersionInfoSize(lstrcpy(work, strFileName.c_str()), &dwHandle);
	if (dwVersionInfoSize == 0) {
		return _T("");
	}

	std::vector<char> versionInfo(dwVersionInfoSize);

	if (GetFileVersionInfo(work, dwHandle, versionInfo.size(), versionInfo.begin()) == FALSE) {
		return _T("");
	}

	void* pBuf;
	UINT uLen;

	if (VerQueryValue(versionInfo.begin(), _T("\\"), &pBuf, &uLen) == FALSE) {
		return _T("");
	}

	VS_FIXEDFILEINFO* pffi = (VS_FIXEDFILEINFO*)pBuf;

	_tostringstream oss;
	oss << ((pffi->dwFileVersionMS & 0xffff0000) >> 16) << _T(".")
		<< ((pffi->dwFileVersionMS & 0x0000ffff) >>  0) << _T(".")
		<< ((pffi->dwFileVersionLS & 0xffff0000) >> 16) << _T(".")
		<< ((pffi->dwFileVersionLS & 0x0000ffff) >>  0);

	return oss.str();
}

_tstring kl3::util::get_error_message(DWORD dwError) {
	_tstring result;
	LPTSTR pszMsg;
	DWORD dwRet;

	dwRet = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		NULL,
		dwError,
		0,
		(LPTSTR)&pszMsg,
		0,
		NULL);

	if (dwRet == 0) {
		_tostringstream oss;
		oss << _T("�G���[ ") << dwError << _T(" �̃��b�Z�[�W���擾���AFormatMessage �֐��ŃG���[ ") << GetLastError() << _T(" ���������܂����B");
		result = oss.str();
	} else {
		result = kl3::util::chonp(pszMsg);

		LocalFree(pszMsg);
	}

	return result;
}

//---------------------------------------------------------------------
// �o�b�N�A�b�v�p�̃��j�[�N�ȃt�@�C�������쐬����
//
// strOriginalFilename �Ŏw�肳�ꂽ�t�@�C������ "Copy #n of " ��ǉ�����
// �t�@�C������Ԃ��B
//
// ���łɓ����̃t�@�C�������݂���Ƃ��́A#n �̒l�������Ă����B
//---------------------------------------------------------------------
_tstring kl3::util::create_backup_filename(const _tstring &strOriginalFilename) {
	TCHAR szNewFilename[MAX_PATH];
	_tstring strResult;

	for (int i = 1; i < 1000; i++) {
		_stprintf(szNewFilename, _T("Copy #%d of %s"), i, kl3::path::find_file_name(strOriginalFilename).c_str());
		strResult = kl3::path::rename_file_spec(strOriginalFilename, szNewFilename);
		
		if (kl3::path::file_exists(strResult) == false) {
			break;
		}
	}

	return strResult;
}

_tstring kl3::util::chonp(const _tstring &str) {
	std::vector<TCHAR> work(str.begin(), str.end());

	std::vector<TCHAR>::reverse_iterator ri = work.rbegin();
	while (*ri == 0x0d || *ri == 0x0a) {
		*ri = 0;
		ri++;
	}

	return work.begin();
}

//---------------------------------------------------------------------
// �ċA�I�Ƀt�H���_���쐬�\�� CreateDirectory �֐�
//---------------------------------------------------------------------
bool kl3::util::create_directory(const _tstring& strPathName) {
	
	if (PathIsRoot(strPathName.c_str()))		return true;
	if (kl3::path::file_exists(strPathName))	return true;

	if (s_pLog) s_pLog->write(_T("kl3::util::create_directory() : [ %s ] ���쐬���܂��B"), strPathName.c_str());

	if (kl3::util::create_directory(kl3::path::remove_file_spec(strPathName))) {
		if (CreateDirectory(strPathName.c_str(), NULL)) {
			return true;
		} else {
			if (s_pLog) s_pLog->write(_T("kl3::util::create_directory() : [ %s ] �̍쐬�Ɏ��s���܂����B(error %d)"), strPathName.c_str(), GetLastError());
		}
	}

	return false;
}

//---------------------------------------------------------------------
// �ċA�I�Ƀt�H���_���폜�\�� RemoveDirectory �֐�
//---------------------------------------------------------------------
bool kl3::util::remove_directory(const _tstring& strPathName) {
	bool bRet = true;
	kl3::find_file ff;

	if (ff.find_first_file(kl3::path::append(strPathName, _T("*.*")))) {
		do {
			if (ff.is_directory()) {
				if (!ff.is_dots()) {
					if (!kl3::util::remove_directory(ff.get_full_name())) {
						bRet = false;
						break;
					}
				}
			} else {
				if (!DeleteFile(ff.get_full_name().c_str())) {
					bRet = false;
					break;
				}
			}
		} while (ff.find_next_file());

		ff.find_close();
	}

	if (bRet) {
		if (!RemoveDirectory(strPathName.c_str())) {
			bRet = false;
		}
	}

	return bRet;
}

//---------------------------------------------------------------------
// �e���|�����t�@�C���p�̃f�B���N�g���̃p�X���擾���܂��B
//---------------------------------------------------------------------
_tstring kl3::util::get_temp_path() {
	std::vector<TCHAR> work(MAX_PATH);

	GetTempPath(work.size(), work.begin());

	return work.begin();
}

//---------------------------------------------------------------------
// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
//---------------------------------------------------------------------
_tstring kl3::util::str_replace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace) {
	_tstring result = src;
	_tstring::size_type pos = 0;
	while ((pos = result.find(strFind, pos)) != _tstring::npos) {
		result.replace(pos, lstrlen(strFind.c_str()), strReplace);
	}
	return result;
}

_tstring kl3::path::append(const _tstring& strPath, const _tstring& strMore) {
	TCHAR szTemp[MAX_PATH];

	PathAppend(lstrcpy(szTemp, strPath.c_str()), strMore.c_str());

	return szTemp;
}

_tstring kl3::path::remove_file_spec(const _tstring& strPath) {
	TCHAR szTemp[MAX_PATH];

	PathRemoveFileSpec(lstrcpy(szTemp, strPath.c_str()));

	return szTemp;
}

_tstring kl3::path::rename_file_spec(const _tstring& strPath, const _tstring& strFileSpec) {
	return kl3::path::append(kl3::path::remove_file_spec(strPath), strFileSpec);
}

bool kl3::path::file_exists(const _tstring& strPath) {
	return (PathFileExists(strPath.c_str()) != FALSE);
}

_tstring kl3::path::find_file_name(const _tstring &strPath) {
	return PathFindFileName(strPath.c_str());
}

//---------------------------------------------------------------------
// PathRenameExtension �֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kl3::path::rename_extension(const _tstring& strPath, const _tstring& strExt) {
	TCHAR szTemp[MAX_PATH];

	PathRenameExtension(lstrcpy(szTemp, strPath.c_str()), strExt.c_str());

	return szTemp;
}

//---------------------------------------------------------------------
// PathRemoveExtension �֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kl3::path::remove_extension(const _tstring& strPath) {
	TCHAR szTemp[MAX_PATH];

	PathRemoveExtension(lstrcpy(szTemp, strPath.c_str()));

	return szTemp;
}

kl3::tool_help::~tool_help() {
	this->close_handle();
}

bool kl3::tool_help::create_snapshot() {
	this->m_hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	return (m_hSnapshot != INVALID_HANDLE_VALUE);
}

void kl3::tool_help::close_handle() {
	if (this->m_hSnapshot != INVALID_HANDLE_VALUE) {
		CloseHandle(this->m_hSnapshot);
		this->m_hSnapshot = INVALID_HANDLE_VALUE;
	}
}

bool kl3::tool_help::process_first() {
	memset(&m_pe, 0, sizeof(m_pe));
	m_pe.dwSize = sizeof(m_pe);

	return (Process32First(m_hSnapshot, &m_pe) != FALSE);
}

bool kl3::tool_help::process_next() {
	return (Process32Next(m_hSnapshot, &m_pe) != FALSE);
}

kl3::process::process() {
	memset(&m_processInformation, 0, sizeof(m_processInformation));
}

kl3::process::~process() {
	close_handle();
}

bool kl3::process::create_process(const _tstring& strCommandLine) {
	TCHAR szCommandLine[MAX_PATH];
	STARTUPINFO startupInfo;

	memset(&startupInfo, 0, sizeof(startupInfo));
	startupInfo.cb = sizeof(startupInfo);

	return (CreateProcess(NULL, lstrcpy(szCommandLine, strCommandLine.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &m_processInformation) != FALSE);
}

//---------------------------------------------------------------------
// [���l]
// PROCESS_INFORMATION �̃v���Z�X�Ɋւ��鏊�����ɒl������܂��B
//---------------------------------------------------------------------
bool kl3::process::open_process(DWORD dwDesiredAccess, BOOL bInheritHandle, DWORD dwProcessId) {
	m_processInformation.hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, m_processInformation.dwProcessId = dwProcessId);

	return (m_processInformation.hProcess != NULL);
}

void kl3::process::close_handle() {
	if (m_processInformation.hThread != NULL) {
		CloseHandle(m_processInformation.hThread);
	}

	if (m_processInformation.hProcess != NULL) {
		CloseHandle(m_processInformation.hProcess);
	}

	memset(&m_processInformation, 0, sizeof(m_processInformation));
}

bool kl3::process::terminate_process(UINT uExitCode) {
	return (TerminateProcess(m_processInformation.hProcess, uExitCode) != FALSE);
}

bool kl3::process::terminate_process(DWORD dwProcessId, UINT uExitCode) {
	bool bRet = false;
	kl3::process proc;

	if (proc.open_process(PROCESS_TERMINATE, FALSE, dwProcessId)) {
		bRet = proc.terminate_process(uExitCode);
		proc.close_handle();
	}

	return bRet;
}

//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------
kl3::log::log() {

	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �e���|�����p�X���擾
	_tstring strTempPath = kl3::util::get_temp_path();

	// ���O�t�@�C���̊�{�ɂȂ�t�@�C�������������s�t�@�C��������擾
	_tstring strFileSpec = kl3::path::find_file_name( kl3::util::get_module_file_name() );

	// �t�@�C�����𐶐�����
	m_strBaseFileName = kl3::path::append(
		strTempPath,
		kl3::path::remove_extension(strFileSpec) + _T("{YMD}.log"));
}

//---------------------------------------------------------------------
// ���O�t�@�C���̃v���t�B�N�X��ݒ肷��
//---------------------------------------------------------------------
void kl3::log::set_prefix(const _tstring& strPrefix) {
	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �e���|�����p�X���擾
	_tstring strTempPath = kl3::util::get_temp_path();

	// �t�@�C�����𐶐�����
	m_strBaseFileName = kl3::path::append(
		strTempPath,
		strPrefix + _T("{YMD}.log"));
}

//---------------------------------------------------------------------
// ���O�̏o�͊֐�
//---------------------------------------------------------------------
void kl3::log::write(LPCTSTR format, ...) {
#ifdef USE_WSSLOG
	va_list marker;
	std::vector<char> work;
	int n;

	va_start(marker, format);

	do {
		work.resize(work.size() + MAX_PATH);
		n = _vsnprintf(work.begin(), work.size(), format, marker);
	} while (n == -1 || n == work.size());

	va_end(marker);

	Winssa_Log(m_strIdent.c_str(), LOGLV_INFO, "%s", work.begin());
#else
	va_list marker;
	va_start(marker, format);

	// ��{�t�@�C��������A���ۂ̃��O�t�@�C�����𐶐�����
	_tstring fname = kl3::util::str_replace( m_strBaseFileName, _T("{YMD}"), kl3::time::get_local_time().to_string_yyyymmdd() );

	FILE* fp = NULL;
	for (int i = 0; i < 100; i++) {
		fp = _tfsopen(fname.c_str(), _T("a+"), _SH_DENYWR);
		if (fp) break;
		Sleep(0);
	}

	if (fp) {
		_ftprintf(fp, _T("%s "), kl3::time::get_local_time().to_string().c_str());
		_vftprintf(fp, format, marker);
		_ftprintf(fp, _T("\n"));

		fclose(fp);
	}

	va_end(marker);
#endif
}

//---------------------------------------------------------------------
// ���O�̏o�͊֐�(��ʏo�͂���)
//---------------------------------------------------------------------
void kl3::log::writee(LPCTSTR format, ...) {

	va_list marker;
	std::vector<TCHAR> work;
	int n;

	va_start(marker, format);

	do {
		work.resize(work.size() + MAX_PATH);
		n = _vsntprintf(work.begin(), work.size(), format, marker);
	} while (n == -1 || n == work.size());

	va_end(marker);

	write(_T("%s"), work.begin());

	// �W���o�͂ɂ��o��
	_tprintf(_T("%s\n"), work.begin());
}

kl3::find_file::~find_file() {
	find_close();
}

bool kl3::find_file::find_first_file(const _tstring& strFileName) {
	m_strWildcard = strFileName;

	m_hFind = FindFirstFile(m_strWildcard.c_str(), &m_findData);
	return (m_hFind != INVALID_HANDLE_VALUE);
}

bool kl3::find_file::find_next_file() {
	return (FindNextFile(m_hFind, &m_findData) != FALSE);
}

void kl3::find_file::find_close() {
	if (m_hFind != INVALID_HANDLE_VALUE) {
		FindClose(m_hFind);
		m_hFind = INVALID_HANDLE_VALUE;
	}
}

const __int64 kl3::time::FT_1USEC = 10;
const __int64 kl3::time::FT_1MSEC = kl3::time::FT_1USEC * 1000;
const __int64 kl3::time::FT_1SEC  = kl3::time::FT_1MSEC * 1000;

//---------------------------------------------------------------------
// SYSTEMTIME���󂯎��R���X�g���N�^
//---------------------------------------------------------------------
kl3::time::time(const SYSTEMTIME& st) {
	SystemTimeToFileTime(&st, &m_ft);
}

//---------------------------------------------------------------------
// �������Ԃ����[�J�����Ԃɕϊ�
//---------------------------------------------------------------------
kl3::time &kl3::time::to_local_time() {
	FileTimeToLocalFileTime(&m_ft, &m_ft);
	return *this;
}

//---------------------------------------------------------------------
// ���Ԃ𕶎���(yyyy/mm/dd hh:mm:ss)�ɕϊ�
//---------------------------------------------------------------------
_tstring kl3::time::to_string() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&m_ft, &st);

	_tostringstream oss;
	oss << std::setfill(_T('0'))
		<< st.wYear << _T("/")
		<< std::setw(2) << st.wMonth << _T("/")
		<< std::setw(2) << st.wDay << _T(" ") 
		<< std::setw(2) << st.wHour << _T(":") 
		<< std::setw(2) << st.wMinute << _T(":") 
		<< std::setw(2) << st.wSecond;
	return oss.str();
}

//---------------------------------------------------------------------
// ���Ԃ𕶎���(YYYYMMDD)�ɕϊ�
//---------------------------------------------------------------------
_tstring kl3::time::to_string_yyyymmdd() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&m_ft, &st);

	_tostringstream oss;
	oss << std::setfill(_T('0'))
		<< st.wYear
		<< std::setw(2) << st.wMonth
		<< std::setw(2) << st.wDay;
	return oss.str();
}

//---------------------------------------------------------------------
// ���Ԃ��������ǂ�����r����
//---------------------------------------------------------------------
bool kl3::time::operator==(const kl3::time& rhs) const {
	return (memcmp(&this->m_ft, &rhs.m_ft, sizeof(this->m_ft)) == 0);
}

//---------------------------------------------------------------------
// ���Ԃ̈����Z������
//---------------------------------------------------------------------
__int64 kl3::time::operator -(const kl3::time& rhs) const {
	return this->to_int64() - rhs.to_int64();
}

//---------------------------------------------------------------------
// ������FILETIME��__int64�̒l�Ƃ��ĕԂ�
//---------------------------------------------------------------------
__int64 kl3::time::to_int64() const {
	return *(__int64*)&m_ft;
}

//---------------------------------------------------------------------
// ���݂̃��[�J���������擾����
//---------------------------------------------------------------------
kl3::time kl3::time::get_local_time() {
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return kl3::time(st);
}

//---------------------------------------------------------------------
// �h���C�u�̏���ێ�����N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �h���C�u���^�[���w�肷��R���X�g���N�^
//---------------------------------------------------------------------
kl3::drive::drive(const _tstring& driveLetter) {
	m_driveNo = (int)CharUpper((LPTSTR)driveLetter[0]) - 'A';
}

//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kl3::drive& kl3::drive::operator =(const kl3::drive& rhs) {
	if (this == &rhs) return *this;

	m_driveNo = rhs.m_driveNo;

	return *this;
}

//---------------------------------------------------------------------
// �h���C�u�^�C�v�𕶎���Ŏ擾
//---------------------------------------------------------------------
_tstring kl3::drive::get_DriveTypeString() {
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

//---------------------------------------------------------------------
// �h���C�u�̈ꗗ��ێ�����N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------
kl3::drive_list::drive_list() {
	DWORD dwDrives = GetLogicalDrives();

	for (int i = 'A'; i <= 'Z'; i++) {
		if (dwDrives & (1 << (i - 'A'))) {
			m_driveList.push_back(kl3::drive(i - 'A'));
		}
	}
}

//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kl3::drive_list& kl3::drive_list::operator =(const kl3::drive_list& rhs) {
	if (this == &rhs) return *this;

	m_driveList = rhs.m_driveList;

	return *this;
}

//---------------------------------------------------------------------
// folder �I�u�W�F�N�g
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------
kl3::folder::folder() : m_path(), m_files(), m_sub_folders() {
}

//---------------------------------------------------------------------
// �R�s�[�R���X�g���N�^
//---------------------------------------------------------------------
kl3::folder::folder(const kl3::folder &src) {
	m_path = src.m_path;
	m_files = src.m_files;
	m_sub_folders = src.m_sub_folders;
}

//---------------------------------------------------------------------
// �t�H���_���������ɂƂ�R���X�g���N�^
//---------------------------------------------------------------------
kl3::folder::folder(const _tstring &strPath) {
	m_path = strPath;

	kl3::find_file ff;
	
	if (ff.find_first_file(kl3::path::append(m_path, _T("*.*")))) {
		do {
			if (ff.is_directory()) {
				if (!ff.is_dots()) {
					m_sub_folders.push_back(ff.get_full_name());
				}
			} else {
				m_files.push_back(ff.get_full_name());
			}
		} while (ff.find_next_file());

		ff.find_close();
	}
}

//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kl3::folder::~folder() {
}

//---------------------------------------------------------------------
// �t�H���_���̂��ׂẴt�@�C����Ԃ�
//---------------------------------------------------------------------
std::vector<_tstring> kl3::folder::get_files() {
	return m_files;
}

//---------------------------------------------------------------------
// �t�H���_���̂��ׂẴT�u�t�H���_��Ԃ�
//---------------------------------------------------------------------
std::vector<_tstring> kl3::folder::get_sub_folders() {
	return m_sub_folders;
}

//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kl3::folder &kl3::folder::operator =(const kl3::folder &rhs) {
	if (this == &rhs) return *this;

	m_path = rhs.m_path;
	m_files = rhs.m_files;
	m_sub_folders = rhs.m_sub_folders;

	return *this;
}
