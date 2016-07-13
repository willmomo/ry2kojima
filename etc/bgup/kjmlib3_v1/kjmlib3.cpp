#include "kjmlib3.h"

//---------------------------------------------------------------------
// このライブラリがログを出力するために使用するグローバルな変数
//---------------------------------------------------------------------
static kl3::log* s_pLog = NULL;

//---------------------------------------------------------------------
// ライブラリがログ出力に使用する kl3::log オブジェクトを設定
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
		oss << _T("エラー ") << dwError << _T(" のメッセージを取得中、FormatMessage 関数でエラー ") << GetLastError() << _T(" が発生しました。");
		result = oss.str();
	} else {
		result = kl3::util::chonp(pszMsg);

		LocalFree(pszMsg);
	}

	return result;
}

//---------------------------------------------------------------------
// バックアップ用のユニークなファイル名を作成する
//
// strOriginalFilename で指定されたファイル名に "Copy #n of " を追加した
// ファイル名を返す。
//
// すでに同名のファイルが存在するときは、#n の値が増えていく。
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
// 再帰的にフォルダを作成可能な CreateDirectory 関数
//---------------------------------------------------------------------
bool kl3::util::create_directory(const _tstring& strPathName) {
	
	if (PathIsRoot(strPathName.c_str()))		return true;
	if (kl3::path::file_exists(strPathName))	return true;

	if (s_pLog) s_pLog->write(_T("kl3::util::create_directory() : [ %s ] を作成します。"), strPathName.c_str());

	if (kl3::util::create_directory(kl3::path::remove_file_spec(strPathName))) {
		if (CreateDirectory(strPathName.c_str(), NULL)) {
			return true;
		} else {
			if (s_pLog) s_pLog->write(_T("kl3::util::create_directory() : [ %s ] の作成に失敗しました。(error %d)"), strPathName.c_str(), GetLastError());
		}
	}

	return false;
}

//---------------------------------------------------------------------
// 再帰的にフォルダを削除可能な RemoveDirectory 関数
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
// テンポラリファイル用のディレクトリのパスを取得します。
//---------------------------------------------------------------------
_tstring kl3::util::get_temp_path() {
	std::vector<TCHAR> work(MAX_PATH);

	GetTempPath(work.size(), work.begin());

	return work.begin();
}

//---------------------------------------------------------------------
// 文字列を置換する。VBのStrReplaceのような関数。
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
// PathRenameExtension 関数のラッパー
//---------------------------------------------------------------------
_tstring kl3::path::rename_extension(const _tstring& strPath, const _tstring& strExt) {
	TCHAR szTemp[MAX_PATH];

	PathRenameExtension(lstrcpy(szTemp, strPath.c_str()), strExt.c_str());

	return szTemp;
}

//---------------------------------------------------------------------
// PathRemoveExtension 関数のラッパー
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
// [備考]
// PROCESS_INFORMATION のプロセスに関する所だけに値が入ります。
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
// デフォルトコンストラクタ
//---------------------------------------------------------------------
kl3::log::log() {

	// 識別子に空文字列を設定
	m_strIdent = _T("");

	// テンポラリパスを取得
	_tstring strTempPath = kl3::util::get_temp_path();

	// ログファイルの基本になるファイル名部分を実行ファイル名から取得
	_tstring strFileSpec = kl3::path::find_file_name( kl3::util::get_module_file_name() );

	// ファイル名を生成する
	m_strBaseFileName = kl3::path::append(
		strTempPath,
		kl3::path::remove_extension(strFileSpec) + _T("{YMD}.log"));
}

//---------------------------------------------------------------------
// ログファイルのプレフィクスを設定する
//---------------------------------------------------------------------
void kl3::log::set_prefix(const _tstring& strPrefix) {
	// 識別子に空文字列を設定
	m_strIdent = _T("");

	// テンポラリパスを取得
	_tstring strTempPath = kl3::util::get_temp_path();

	// ファイル名を生成する
	m_strBaseFileName = kl3::path::append(
		strTempPath,
		strPrefix + _T("{YMD}.log"));
}

//---------------------------------------------------------------------
// ログの出力関数
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

	// 基本ファイル名から、実際のログファイル名を生成する
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
// ログの出力関数(画面出力あり)
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

	// 標準出力にも出力
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
// SYSTEMTIMEを受け取るコンストラクタ
//---------------------------------------------------------------------
kl3::time::time(const SYSTEMTIME& st) {
	SystemTimeToFileTime(&st, &m_ft);
}

//---------------------------------------------------------------------
// 内部時間をローカル時間に変換
//---------------------------------------------------------------------
kl3::time &kl3::time::to_local_time() {
	FileTimeToLocalFileTime(&m_ft, &m_ft);
	return *this;
}

//---------------------------------------------------------------------
// 時間を文字列(yyyy/mm/dd hh:mm:ss)に変換
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
// 時間を文字列(YYYYMMDD)に変換
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
// 時間が同じかどうか比較する
//---------------------------------------------------------------------
bool kl3::time::operator==(const kl3::time& rhs) const {
	return (memcmp(&this->m_ft, &rhs.m_ft, sizeof(this->m_ft)) == 0);
}

//---------------------------------------------------------------------
// 時間の引き算をする
//---------------------------------------------------------------------
__int64 kl3::time::operator -(const kl3::time& rhs) const {
	return this->to_int64() - rhs.to_int64();
}

//---------------------------------------------------------------------
// 内部のFILETIMEを__int64の値として返す
//---------------------------------------------------------------------
__int64 kl3::time::to_int64() const {
	return *(__int64*)&m_ft;
}

//---------------------------------------------------------------------
// 現在のローカル日時を取得する
//---------------------------------------------------------------------
kl3::time kl3::time::get_local_time() {
	SYSTEMTIME st;
	::GetLocalTime(&st);
	return kl3::time(st);
}

//---------------------------------------------------------------------
// ドライブの情報を保持するクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// ドライブレターを指定するコンストラクタ
//---------------------------------------------------------------------
kl3::drive::drive(const _tstring& driveLetter) {
	m_driveNo = (int)CharUpper((LPTSTR)driveLetter[0]) - 'A';
}

//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
kl3::drive& kl3::drive::operator =(const kl3::drive& rhs) {
	if (this == &rhs) return *this;

	m_driveNo = rhs.m_driveNo;

	return *this;
}

//---------------------------------------------------------------------
// ドライブタイプを文字列で取得
//---------------------------------------------------------------------
_tstring kl3::drive::get_DriveTypeString() {
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

//---------------------------------------------------------------------
// ドライブの一覧を保持するクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// デフォルトコンストラクタ
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
// 代入演算子
//---------------------------------------------------------------------
kl3::drive_list& kl3::drive_list::operator =(const kl3::drive_list& rhs) {
	if (this == &rhs) return *this;

	m_driveList = rhs.m_driveList;

	return *this;
}

//---------------------------------------------------------------------
// folder オブジェクト
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// デフォルトコンストラクタ
//---------------------------------------------------------------------
kl3::folder::folder() : m_path(), m_files(), m_sub_folders() {
}

//---------------------------------------------------------------------
// コピーコンストラクタ
//---------------------------------------------------------------------
kl3::folder::folder(const kl3::folder &src) {
	m_path = src.m_path;
	m_files = src.m_files;
	m_sub_folders = src.m_sub_folders;
}

//---------------------------------------------------------------------
// フォルダ名を引数にとるコンストラクタ
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
// デストラクタ
//---------------------------------------------------------------------
kl3::folder::~folder() {
}

//---------------------------------------------------------------------
// フォルダ内のすべてのファイルを返す
//---------------------------------------------------------------------
std::vector<_tstring> kl3::folder::get_files() {
	return m_files;
}

//---------------------------------------------------------------------
// フォルダ内のすべてのサブフォルダを返す
//---------------------------------------------------------------------
std::vector<_tstring> kl3::folder::get_sub_folders() {
	return m_sub_folders;
}

//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
kl3::folder &kl3::folder::operator =(const kl3::folder &rhs) {
	if (this == &rhs) return *this;

	m_path = rhs.m_path;
	m_files = rhs.m_files;
	m_sub_folders = rhs.m_sub_folders;

	return *this;
}
