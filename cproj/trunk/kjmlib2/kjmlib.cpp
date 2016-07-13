//---------------------------------------------------------------------
// kjmlib : Version 0.2015.10.29
//
// �v���R���p�C���ς݃w�b�_���g�p���Ȃ��悤�ɐݒ肵�Ă��������B
//
// r.kojima
//---------------------------------------------------------------------
#include "kjmlib.h"
#include <list>

//---------------------------------------------------------------------
// __int64 �� iostream �ɏo�͂��邽�߂̊֐�
//---------------------------------------------------------------------
#if _MSC_VER < 1500		// ���Ȃ��Ƃ��Avs2008����́A__int64���󂯎��ostream�����݂���̂ŁA�����ɂ���B
_otstream& operator<<(_otstream& os, __int64 i) {
	TCHAR buf[20];
	_stprintf(buf, _T("%I64d"), i);
	os << buf;
	return os;
}
#endif

#if _MSC_VER <= 1200	// VC++6�ȉ�
// �����̂��߂����̊֐��I�u�W�F�N�g���K�v�ɂȂ��Ă��܂��B
// �{���̌���d�l�ł́A���̂悤�ȃN���X�͕s�v�B
//-------------------------------------------------------------------------
// vector<optionInfo> ���� �����O�I�v�V�������������邽�߂̊֐��I�u�W�F�N�g
//-------------------------------------------------------------------------
class optionInfoLongNameFinder {
public:
	std::basic_string<TCHAR> m_targetName;
	optionInfoLongNameFinder(const std::basic_string<TCHAR>& targetName) : m_targetName(targetName) {}
	bool operator()(const kjm::optionInfo& rhs) const { return rhs.m_longOption == m_targetName; }
};

//-------------------------------------------------------------------------
// vector<optionInfo> ���� �V���[�g�I�v�V�������������邽�߂̊֐��I�u�W�F�N�g
//-------------------------------------------------------------------------
class optionInfoShortNameFinder {
public:
	std::basic_string<TCHAR> m_targetName;
	optionInfoShortNameFinder(const std::basic_string<TCHAR>& targetName) : m_targetName(targetName) {}
	bool operator()(const kjm::optionInfo& rhs) const { return rhs.m_shortOption == m_targetName; }
};
#endif

//---------------------------------------------------------------------
// �ȈՃ��O�I�u�W�F�N�g
static kjm::log s_log;
kjm::log& kjm::GetLog() { return s_log; }
//---------------------------------------------------------------------

static bool s_verboseMode = false;

void kjm::setVerboseMode(bool mode) { s_verboseMode = mode; }
bool kjm::getVerboseMode() { return s_verboseMode; }

#if !defined(USE_WSSLOG)
void Winssa_Log(LPCTSTR ident, int level, LPCTSTR format, ...) {
	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, format);
	int result = kjm::util::vsprintf_vec(buf, format, marker);
	va_end(marker);

	kjm::GetLog().setLogPath_App(_T("log"));
	kjm::GetLog().writeAll(_T("%s"), &buf[0]);
}
void WLDeleteOldLog() {
	// no implement
}
#endif

//---------------------------------------------------------------------
// kjm::PathString ���X�g���[���ɓn���֐�
//---------------------------------------------------------------------
std::basic_ostream<TCHAR>& operator<<(std::basic_ostream<TCHAR>& lhs, kjm::PathString& p) {
	lhs << p.c_str();
	return lhs;
}

//---------------------------------------------------------------------
// OutputDebugString ���ȒP�Ɏg�����߂̊֐�
//---------------------------------------------------------------------
void kjm::ods(LPCTSTR format, ...) {
	std::vector<TCHAR> work(512);
	va_list args;
	va_start(args, format);
	while (_vsntprintf(&work.at(0), work.size(), format, args) == -1) {
		work.resize(work.size() + 512);
	}
	va_end(args);
	OutputDebugString(&work.at(0));
}

// Unicode��ascii�ɕϊ�
// !defined(UNICODE)�̎��Ɏg����֐�
std::string kjm::encoding::to_mbs(const std::string& src) {
	return src;
}

// Unicode��ascii�ɕϊ�
// defined(UNICODE)�̎��Ɏg����֐�
std::string kjm::encoding::to_mbs(const std::wstring& src) {
	std::vector<char> mbstr;
	size_t returnValue = 0;
	errno_t ret = wcstombs_s(&returnValue, NULL, 0, src.c_str(), src.length());
	if (ret == 0) {
		mbstr.resize(returnValue);
		ret = wcstombs_s(&returnValue, &mbstr[0], mbstr.size(), src.c_str(), src.length());
	}
	return &mbstr[0];
}

//=============================================================================
// class base64 �̎���
//=============================================================================
//---------------------------------------------------------------------
// �G���R�[�h
//---------------------------------------------------------------------
std::string kjm::base64::encode(const std::vector<char>& data) {
	static const char s_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::string str;

	unsigned int nData = 0;
	int nBits = 0;

	for (std::vector<char>::const_iterator it = data.begin(); it != data.end(); ++it) {
		nData = (nData << 8) | ((*it) & 0xff);
		nBits += 8;

		while (nBits >= 6) {
			nBits -= 6;
			const int nIndex = (nData >> nBits) & 0x3f;
			str += s_map[nIndex];
		}
	}

	if (nBits > 0) {
		const int nIndex = (nData << (6 - nBits)) & 0x3f;
		str += s_map[nIndex];
	}

	while (str.size() % 4) {
		str += '=';
	}

	return str;
}

//---------------------------------------------------------------------
// �f�R�[�h
//---------------------------------------------------------------------
std::vector<char> kjm::base64::decode(const std::string& str) {
	static const char s_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	std::vector<char> data;

	unsigned int nData = 0;
	int nBits = 0;

	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		const char c = *it;
		if (c == '=') {
			break;
		}

		const char* p = ::strchr(s_map, c);
		if (!p) {
			continue;
		}
		const size_t nIndex = p - s_map;

		nData = (nData << 6) | (nIndex & 0x3f);
		nBits += 6;

		while (nBits >= 8) {
			nBits -= 8;
			data.push_back(static_cast<char>((nData >> nBits) & 0xff));
		}
	}

	return data;
}

//=============================================================================
// class crypt �̎���
//=============================================================================
//---------------------------------------------------------------------
// ������𕜍���
//---------------------------------------------------------------------
std::string kjm::crypt::decrypt(const std::string& str) {
	// BASE64-�f�R�[�h
	std::vector<char> data = kjm::base64::decode(str);
	if (!data.size()) {
		// �󂾂���
		return std::string();
	}

	// �f�R�[�h (type:0)
	std::string ret;
	std::vector<char>::const_iterator it = data.begin();

	const char enctype = *it++;
	if (enctype == 0) {
		if (it != data.end()) {
			unsigned char seed = static_cast<unsigned char>(*it++);
			char last = '\0';
			while (it != data.end()) {
				const char c = *it++;
				last += (c ^ seed);
				ret += static_cast<char>(last);
				seed = (seed << 1) | (seed >> 7);
			}
		}
	} else {
		// �m��Ȃ��G���R�[�h�^�C�v
	}
	return ret;
}

//---------------------------------------------------------------------
// ������𕄍���
//---------------------------------------------------------------------
std::string kjm::crypt::encrypt(const std::string& str) {
	unsigned char seed = rand();

	std::vector<char> data;
	data.push_back(0);
	data.push_back(static_cast<char>(seed));

	char back = '\0';
	for (std::string::const_iterator it = str.begin(); it != str.end(); it++) {
		const char c = *it;
		data.push_back((c - back) ^ seed);
		back = c;
		seed = (seed << 1) | (seed >> 7);
	}

	// BASE64�G���R�[�h�������̂���������
	return kjm::base64::encode(data);
}

//=============================================================================
// class util �̎���
//=============================================================================
//---------------------------------------------------------------------
// GetEnvironmentVariable�֐��̃��b�p�[
//---------------------------------------------------------------------
kjm::_tstring kjm::util::get_environment_variable(const kjm::_tstring& lpName) {
	// �K�v�ȕ����������߂�
	DWORD dwRet = GetEnvironmentVariable(lpName.c_str(), NULL, 0);
	if (dwRet == 0) {
		return _tstring();	// ���ϐ��Ȃ��̎��́A�󕶎����Ԃ�
	}

	std::vector<TCHAR> buffer(dwRet);	// �K�v�ȃo�b�t�@���m��

	GetEnvironmentVariable(lpName.c_str(), &buffer[0], buffer.size());
	return &buffer[0];
}

//---------------------------------------------------------------------
// PathRenameExtension�̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::path_rename_extension(const _tstring& pszPath, const _tstring& pszExt) {
	TCHAR buffer[MAX_PATH];
	if (PathRenameExtension(lstrcpy(buffer, pszPath.c_str()), pszExt.c_str()) == FALSE) {
		return _tstring();	// �֐����s�̎��́A�󕶎����Ԃ�
	}

	return buffer;
}

//---------------------------------------------------------------------
// GetModuleFileName�֐��̃��b�p�[
//---------------------------------------------------------------------
kjm::_tstring kjm::util::get_module_filename(HMODULE hModule) {
	TCHAR buffer[MAX_PATH];
	GetModuleFileName(hModule, buffer, countof(buffer));
	return buffer;
}

//---------------------------------------------------------------------
// pszPath�Ŏw�肵���f�B���N�g�����́A���ݓ�������nDay�ȏ�Â��t�@�C��
// ���폜���܂��B
//
// �폜�Ώۂ́ApszPath�����̃t�@�C�������ł��B
// �폜�̊�́A(���ݓ��� - �ŏI�X�V����)�̒l�Ȃ̂ŁA���Ԃ܂ōl�����āA
// nDay�Ɣ�r���܂��B
//---------------------------------------------------------------------
void kjm::util::cleanup_dir(const kjm::_tstring& pszPath, int nDay) {
	kjm::_tstring findPath = kjm::util::path_append(pszPath, _T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(findPath.c_str(), &data);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			__int64 diff = (kjm::time::get_system_time() - kjm::time::time(data.ftLastWriteTime)) / FT_1DAY;
			if (diff >= nDay) {
				_tstring delFile = kjm::util::path_append(pszPath, data.cFileName);
				DeleteFile(delFile.c_str());
			}
		}
	} while (FindNextFile(hFind, &data));

	FindClose(hFind);
}

//---------------------------------------------------------------------
// CopyFileEx �̊ȒP�����B
//---------------------------------------------------------------------
BOOL kjm::util::copyFileEx(LPCTSTR lpExistingFileName,
						  LPCTSTR lpNewFileName,
						  LPPROGRESS_ROUTINE lpProgressRoutine,
						  LPVOID lpData,
						  LPBOOL pbCancel,
						  DWORD dwCopyFlags)
{
	LARGE_INTEGER TotalFileSize;
	LARGE_INTEGER TotalBytesTransferred;
	LARGE_INTEGER StreamSize;
	LARGE_INTEGER StreamBytesTransferred;
	DWORD dwStreamNumber = 0;
	DWORD dwCallbackReason = CALLBACK_STREAM_SWITCH;
	HANDLE hSourceFile = INVALID_HANDLE_VALUE;
	HANDLE hDestinationFile = INVALID_HANDLE_VALUE;

	// �R�s�[���t�@�C���̃t�@�C�������擾���Ă���
	kjm::fileInfo fi(lpExistingFileName);

	hSourceFile = CreateFile(
		lpExistingFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	if (hSourceFile == INVALID_HANDLE_VALUE)
		return FALSE;

	hDestinationFile = CreateFile(
		lpNewFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_ARCHIVE,
		NULL);
	if (hDestinationFile == INVALID_HANDLE_VALUE) {
		CloseHandle(hSourceFile);
		return FALSE;
	}


	GetFileSizeEx(hSourceFile, &TotalFileSize);
	StreamSize = TotalFileSize;
	TotalBytesTransferred.QuadPart = StreamBytesTransferred.QuadPart = 0;
	dwStreamNumber = 1;
	dwCallbackReason = CALLBACK_STREAM_SWITCH;
	if (lpProgressRoutine)
		(*lpProgressRoutine)(TotalFileSize, TotalBytesTransferred, StreamSize, StreamBytesTransferred, dwStreamNumber, dwCallbackReason, hSourceFile, hDestinationFile, lpData);

	BOOL result = TRUE;
	//DWORD dwBufferSize = 1 * 1024 * 1024;
	DWORD dwBufferSize = 8192;
	LPVOID lpvBuffer = (LPVOID)GlobalAlloc(GMEM_FIXED, dwBufferSize);
	while (1) {
		DWORD dwBytesRead;
		if (!ReadFile(hSourceFile, lpvBuffer, dwBufferSize, &dwBytesRead, NULL)) {
			result = FALSE;
			break;
		}
		if (dwBytesRead == 0)
			break;

		DWORD dwWritten = 0;
		if (!WriteFile(hDestinationFile, lpvBuffer, dwBytesRead, &dwWritten, NULL)) {
			result = FALSE;
			break;
		}

		TotalBytesTransferred.QuadPart += dwBytesRead;
		StreamBytesTransferred = TotalBytesTransferred;
		dwCallbackReason = CALLBACK_CHUNK_FINISHED;

		if (lpProgressRoutine)
			(*lpProgressRoutine)(TotalFileSize, TotalBytesTransferred, StreamSize, StreamBytesTransferred, dwStreamNumber, dwCallbackReason, hSourceFile, hDestinationFile, lpData);
	}

	if (result) {
		/*
		{
			FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;
			BOOL ret = GetFileTime(hSourceFile, &ftCreationTime, &ftLastAccessTime, &ftLastWriteTime);
			kjm::_tstring creationTime = kjm::dateTime(ftCreationTime).toLocalTime().toString();
			kjm::_tstring lastAccessTime = kjm::dateTime(ftLastAccessTime).toLocalTime().toString();
			kjm::_tstring lastWriteTime = kjm::dateTime(ftLastWriteTime).toLocalTime().toString();

			_tprintf(_T("\n%s\n%s\n%s\n"), creationTime.c_str(), lastAccessTime.c_str(), lastWriteTime.c_str());

			_tprintf(_T("\n%s\n%s\n%s\n"), 
				fi.creationTime().toString().c_str(),
				fi.lastAccessTime().toString().c_str(),
				fi.lastWriteTime().toString().c_str());
		}
		*/
		FILETIME ft = fi.lastWriteTimeUTC().get_FILETIME_Value();
		SetFileTime(hDestinationFile, NULL, NULL, &ft);
	}

	GlobalFree(lpvBuffer);
	CloseHandle(hSourceFile);
	CloseHandle(hDestinationFile);

	if (result == FALSE) {
		DeleteFile(lpNewFileName);
	}

	return result;
}

//---------------------------------------------------------------------
// ������̑O������A�󔒁A�^�u�A���s��������菜��
//---------------------------------------------------------------------
kjm::_tstring kjm::util::trimLeft(const kjm::_tstring &s) {
	if (s.empty())
		return s;
	int b = s.find_first_not_of(_T(" \t\r\n"));
	if (b == -1)
		return _T("");
	//return kjm::_tstring(s, b, s.size() - b + 1);
	return kjm::_tstring(s, b);
}

//---------------------------------------------------------------------
// ������O��́A�󔒁A�^�u�A���s��������菜��
//---------------------------------------------------------------------
kjm::_tstring kjm::util::trim(const kjm::_tstring &s) {
	if (s.empty())
		return s;
	int b = s.find_first_not_of(_T(" \t\r\n"));
	int e = s.find_last_not_of(_T(" \t\r\n"));
	if (b == -1)
		return _T("");
	return kjm::_tstring(s, b, e - b + 1);
}

//---------------------------------------------------------------------
// �t�@�C������1�s�Ǎ��ށB
//
// [�߂�l]
//  1 eof �ŏI��
//  0 ���s�R�[�h�ŏI��
// -1 error �ŏI��
//
// [���l]
// '\x0d'���������ꍇ�A�ǂݔ�΂��B
// '\x0a'���������ꍇ�A�����œǍ��ނ̂���߂�B
// �o�C�i�����[�h���g�p�����ꍇ�ł��Amac�^�̉��s�R�[�h�ȊO�́A�����������ł���B
//---------------------------------------------------------------------
int kjm::util::getline(FILE *fp, kjm::_tstring &str) {
	_TINT c;

	// eof or error �܂ŁA���[�v����B
	str = _T("");
	while ((c = _fgettc(fp)) != _TEOF) {
		if (c == _T('\x0d')) {
			// �ǂݔ�΂��B
		} else if (c == _T('\x0a')) {
			// ���[�v�I��
			break;
		}

		str += c;
	}

	int result = 0;
	if (feof(fp) != 0)			result = 1;
	else if (ferror(fp) != 0)	result = -1;

	return result;
}

//---------------------------------------------------------------------
// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
//---------------------------------------------------------------------
kjm::_tstring kjm::util::strReplace(const kjm::_tstring& src, const kjm::_tstring& strFind, const kjm::_tstring& strReplace) {
	kjm::_tstring result = src;
	kjm::_tstring::size_type pos = 0;
	while ((pos = result.find(strFind, pos)) != kjm::_tstring::npos) {
		result.replace(pos, lstrlen(strFind.c_str()), strReplace);
	}
	return result;
}

//---------------------------------------------------------------------
// �o�b�t�@����CRC16���v�Z����
//---------------------------------------------------------------------
unsigned short kjm::util::calcCRC16(unsigned char *p, unsigned int n) {
	unsigned short crc = 0xffff;

	for (unsigned int i = 0; i < n; i++) {
		unsigned char  dat = p[i];

		for (int b = 0; b < 8; b++) {
			int w = (crc ^ dat) & 0x0001;
			crc = crc >> 1;
			if (w) crc = crc ^ 0x8408;
			dat = dat >> 1;
		}
	}

	return crc;
}

//---------------------------------------------------------------------
// �t�@�C������CRC16���v�Z����
//---------------------------------------------------------------------
unsigned short kjm::util::calcCRC16(FILE* fp) {
	unsigned char buf[8192];		// ���œǍ��ރo�C�g��
	unsigned short crc = 0xffff;

	while (1) {
		int nRead = fread(buf, sizeof(char), sizeof(buf), fp);
		for (int i = 0; i < nRead; i++) {
			unsigned char  dat = buf[i];

			for (int b = 0; b < 8; b++) {
				int w = (crc ^ dat) & 0x0001;
				crc = crc >> 1;
				if (w) crc = crc ^ 0x8408;
				dat = dat >> 1;
			}
		}
		
		if (nRead != sizeof(buf)) {
			break;
		}
	}

	return crc;
}

//---------------------------------------------------------------------
// �t�@�C������CRC16���v�Z����
//---------------------------------------------------------------------
unsigned short kjm::util::calcCRC16(const kjm::_tstring& pfname) {
	unsigned short crc = 0xffff;
	FILE* fp = NULL;

#if _MSC_VER > 1200	// VC++6����
	if (_tfopen_s(&fp, pfname.c_str(), _T("rb")) == 0) {
		crc = kjm::util::calcCRC16(fp);
		fclose(fp);
	}
#else
	if ((fp = _tfopen(pfname.c_str(), _T("rb"))) != NULL) {
		crc = kjm::util::calcCRC16(fp);
		fclose(fp);
	}
#endif

	return crc;
}

//---------------------------------------------------------------------
// �R�s�[���̒����ɂ��邷�ׂẴt�@�C�����R�s�[��ɃR�s�[����
//---------------------------------------------------------------------
DWORD kjm::util::CopyFiles(LPCTSTR pszSrcPath, LPCTSTR pszDstPath) {

	assert(pszSrcPath != NULL);
	assert(pszSrcPath != NULL);

	std::basic_string<TCHAR> module_name = kjm::util::GetModuleName();
	kjm::log_info(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles(%s, %s) enter"), module_name.c_str(), pszSrcPath, pszDstPath);

	DWORD dwRet = NO_ERROR;

	// �����p���C���h�J�[�h�쐬
	kjm::PathString wc = kjm::PathString(pszSrcPath).Append(_T("*.*"));

	// �t�@�C��������
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(wc.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// �t�H���_�ȊO�̃t�@�C�����R�s�[����B
			if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {

				// lockfile�ȊO�̃t�@�C�����R�s�[����B
				// lockfile�́A�R�s�[���悤�Ƃ��Ă���ɃG���[�ɂȂ�B
				if (lstrcmpi(data.cFileName, _T("lockfile")) != 0) {
					kjm::PathString srcFile = kjm::PathString(pszSrcPath).Append(data.cFileName);
					kjm::PathString dstFile = kjm::PathString(pszDstPath).Append(data.cFileName);

					kjm::log_info(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles(): CopyFile(%s, %s, FALSE)"), module_name.c_str(), srcFile.c_str(), dstFile.c_str());
					int retryCnt = 0;
retry:
					BOOL ret = CopyFile(srcFile.c_str(), dstFile.c_str(), FALSE);
					if (ret == FALSE) {
						dwRet = GetLastError();
						kjm::log_info(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles(): CopyFile error: %u"), module_name.c_str(), dwRet);

						if (retryCnt < 10) {
							Sleep(1000);
							++retryCnt;
							goto retry;
						} else {
							break;
						}
					} else {
						dwRet = NO_ERROR;
					}
				}
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);
	} else {
		kjm::log_error(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles(): FindFirstFile(%s, -)"), module_name.c_str(), wc.c_str());
	}

	kjm::log_info(_T("KJMLIB"), _T("<%s> kjm::util::CopyFiles() leave with %u"), module_name.c_str(), dwRet);

	return dwRet;
}

//---------------------------------------------------------------------
// �V���[�g�J�b�g�̍쐬
//
// CoInitialize(NULL)/CoUninitialize()�́A�A�v���P�[�V�������s���Ă��������B
//---------------------------------------------------------------------
bool kjm::util::createShortcut(const std::basic_string<TCHAR>& strFileName,
							   const std::basic_string<TCHAR>& strFile,
							   const std::basic_string<TCHAR>& strArgs,
							   const std::basic_string<TCHAR>& strDir) {
	HRESULT hRes;
	IShellLink *isLink = NULL; 
	IPersistFile *ipFile = NULL;

	//IShellLink���擾
	hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&isLink);
	if (SUCCEEDED(hRes)) {
		//�p�X�̐ݒ�
		isLink->SetPath(strFile.c_str());
		//���s�������i�X�C�b�`�j�̐ݒ�
		isLink->SetArguments(strArgs.c_str());
		//��ƃt�H���_�̐ݒ�
		isLink->SetWorkingDirectory(strDir.c_str());

		//IPersistFile���擾
		hRes = isLink->QueryInterface(IID_IPersistFile, (void**)&ipFile);
		if (SUCCEEDED(hRes)) {
			//�f�B�X�N�ɕۑ�����
#if defined(UNICODE)
			hRes = ipFile->Save(strFileName.c_str(), TRUE);
#else
			hRes = ipFile->Save(kjm::util::toUnicode(strFileName).c_str(), TRUE);
#endif
			//IPersistFile�̔j��
			ipFile->Release();
		}

		//IShellLink��j��
		isLink->Release();
	}else{
		//���s
	}

	return (SUCCEEDED(hRes) ? true : false);
}

//---------------------------------------------------------------------
// �����t�@�C�����폜�o�� DeleteFile 
//---------------------------------------------------------------------
bool kjm::util::deleteFiles(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildcard, int nDays/* = 0*/) {
	std::basic_string<TCHAR> module_name = kjm::util::GetModuleName();

	std::basic_string<TCHAR> findPath = kjm::util::append(path, wildcard);
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(findPath.c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		DWORD dwErr = ::GetLastError();
		kjm::log_error(_T("KJMLIB"), _T("<%s> kjm::util::deleteFiles(): FindFirstFile(%s, -) error %d"),
			module_name.c_str(), findPath.c_str(), dwErr);
		return (dwErr == ERROR_FILE_NOT_FOUND) ? true : false;
	}

	bool result = true;
	do {
		// �f�B���N�g���ȊO�̃t�@�C�����폜�Ώ�
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			std::basic_string<TCHAR> delFile = kjm::util::append(path, findData.cFileName);

			kjm::timeSpan ts = kjm::dateTime::now() - kjm::dateTime(findData.ftLastWriteTime);
			if ((int)ts.totalDays() > nDays) {
				if (::DeleteFile(delFile.c_str()) == FALSE) {
					DWORD dwErr = ::GetLastError();
					if (dwErr != ERROR_FILE_NOT_FOUND) {
						kjm::log_error(_T("KJMLIB"), _T("<%s> kjm::util::deleteFiles(): DeleteFile(%s) error %d"),
							module_name.c_str(), delFile.c_str(), dwErr);
						result = false;
						break;
					}
				}
			}
		}
	} while (::FindNextFile(hFind, &findData));

	::FindClose(hFind);

	return result;
}

//---------------------------------------------------------------------
// ������̔�r(�啶���E�������𖳎����Ĕ�r)
//---------------------------------------------------------------------
bool kjm::util::equalsIgnoreCase(const std::basic_string<TCHAR>& lhs, const std::basic_string<TCHAR>& rhs) {
	return (::lstrcmpi(lhs.c_str(), rhs.c_str()) == 0 ? true : false);
}

//---------------------------------------------------------------------
// Win32API ExpandEnvironmentStrings�����S�Ɏg���֐�
//---------------------------------------------------------------------
DWORD kjm::util::expandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst ) {
	DWORD dwRet;

	if (rDst.empty()) {
		rDst.resize(1);
	}

	while ((dwRet = ::ExpandEnvironmentStrings(lpSrc, &rDst[0], rDst.size())) > rDst.size()) {
		if (dwRet == 0) {
			break;
		} else {
			rDst.resize(dwRet);
		}
	}

	return dwRet;
}

//---------------------------------------------------------------------
// GetEnvironmentVariable�֐��̃��b�p�[
//---------------------------------------------------------------------
kjm::_tstring kjm::util::getEnvironmentVariable(LPCTSTR lpName) {
	// �K�v�ȕ����������߂�
	DWORD dwRet = GetEnvironmentVariable(lpName, NULL, 0);
	if (dwRet == 0) {
		return _tstring();	// ���ϐ��Ȃ��̎��́A�󕶎����Ԃ�
	}

	std::vector<TCHAR> buffer(dwRet);	// �K�v�ȃo�b�t�@���m��

	GetEnvironmentVariable(lpName, &buffer[0], buffer.size());
	return &buffer[0];
}

//---------------------------------------------------------------------
// �t�@�C���̃o�[�W�������\�[�X���擾
//---------------------------------------------------------------------
kjm::_tstring kjm::util::getFileVersion(const kjm::_tstring &strFileName) {
	DWORD dwHandle = 0;
	DWORD dwVersionInfoSize = 0;

	TCHAR work[MAX_PATH];
	dwVersionInfoSize = GetFileVersionInfoSize(lstrcpy(work, strFileName.c_str()), &dwHandle);
	if (dwVersionInfoSize == 0) {
		return _T("");
	}

	std::vector<TCHAR> versionInfo(dwVersionInfoSize);

	if (GetFileVersionInfo(work, dwHandle, versionInfo.size(), &versionInfo[0]) == FALSE) {
		return _T("");
	}

	void* pBuf;
	UINT uLen;

	if (VerQueryValue(&versionInfo[0], _T("\\"), &pBuf, &uLen) == FALSE) {
		return _T("");
	}

	VS_FIXEDFILEINFO* pffi = (VS_FIXEDFILEINFO*)pBuf;

	_otstringstream oss;
	oss << ((pffi->dwFileVersionMS & 0xffff0000) >> 16) << _T(".")
		<< ((pffi->dwFileVersionMS & 0x0000ffff) >>  0) << _T(".")
		<< ((pffi->dwFileVersionLS & 0xffff0000) >> 16) << _T(".")
		<< ((pffi->dwFileVersionLS & 0x0000ffff) >>  0);

	return oss.str();
}

//---------------------------------------------------------------------
// ����t�H���_�ւ̃p�X���擾����
//
// nFolder �̒l�́Ashlobj.h �ɒ�`���Ă���ACSIDL_�Ŏn�܂�萔���w��B
// ��\�I�Ȃ��̂Ƃ��āA
// CSIDL_STARTUP ... ���[�U�[���Ƃ̃X�^�[�g�A�b�v
// CSIDL_COMMON_STARTUP ... All Users�̃X�^�[�g�A�b�v
//
// ���Ȃ��Ƃ��Avs2008����L���Ȋ֐�
//---------------------------------------------------------------------
#if _WIN32_IE >= 0x0700
std::basic_string<TCHAR> kjm::util::getSpecialFolderLocation(int nFolder) {
	std::vector<TCHAR> path(MAX_PATH);
	SHGetFolderPath(NULL, nFolder, NULL, SHGFP_TYPE_CURRENT, &path[0]);
	return &path[0];
}
#endif

//---------------------------------------------------------------------
// %TEMP%�t�H���_���擾����
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getTempPath() {
	std::vector<TCHAR> path(MAX_PATH);
	::GetTempPath(path.size(), &path[0]);
	return &path[0];
}

//---------------------------------------------------------------------
// �w��̋��L�t�H���_�Ƌ��L�p�X�̑��݂�ۏ؂���
//---------------------------------------------------------------------
int kjm::util::guaranteeSharedFolder(LPCTSTR pszSharedFolder, LPCTSTR pszSharedPath) {
	int nRet = 0;

	// ���L�t�H���_����Unicode�ɕϊ�
#if defined(_MBCS)
	std::wstring wstrSharedFolder = kjm::util::toUnicode(pszSharedFolder);
#else
	std::wstring wstrSharedFolder = pszSharedFolder;
#endif
	if (wstrSharedFolder.empty()) {
		// ���L�t�H���_�̎w�肪�Ԉ���Ă���Ƃ��̏���
		kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: ���L�t�H���_���̎w�肪�ُ�ł��B"));
		return -1;
	}

	// ���L�p�X����Unicode�ɕϊ�
#if defined(_MBCS)
	std::wstring wstrSharedPath = kjm::util::toUnicode(pszSharedPath);
#else
	std::wstring wstrSharedPath = pszSharedPath;
#endif
	if (wstrSharedPath.empty()) {
		// ���L�p�X�̎w�肪�Ԉ���Ă���Ƃ��̏���
		kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: �����p�X���̎w�肪�ُ�ł��B"));
		return -1;
	}

	SHARE_INFO_2* pInfo;
	NET_API_STATUS ret;
	int tryCount = 0;

	// ���[�J���ɗL��A�w��̋��L�t�H���_�����擾
retry:
	++tryCount;

	// ���Ȃ��Ƃ��Avs2008����LPWSTR �Ő錾�����悤�ɂȂ���
#if _WIN32_IE >= 0x0700
	ret = NetShareGetInfo(NULL, (LPWSTR)wstrSharedFolder.c_str(), 2, (LPBYTE*)&pInfo);
#else
	ret = NetShareGetInfo(NULL, (LPWSTR)wstrSharedFolder.c_str(), 2, (LPBYTE*)&pInfo);
#endif

	if (ret == NERR_Success) {
	
		// ���L�t�H���_���w��̋��L�p�X���w���Ă��邩�m�F����
#if defined(_MBCS)
		if (lstrcmpi(kjm::util::toAnsi((LPCWSTR)pInfo->shi2_path).c_str(), pszSharedPath) != 0) {
			// ���L�t�H���_�����L�p�X�ƈႤ���Ƃ��x��
			kjm::log_info(_T("KJMLIB"), 
				_T("kjm::util::guaranteeSharedFolder: ���L�t�H���_�͂���܂����A'%s' ���w���Ă��܂��B"), 
				kjm::util::toAnsi((LPCWSTR)pInfo->shi2_path).c_str());
		}
#else
		if (lstrcmpi(std::wstring((LPCWSTR)pInfo->shi2_path).c_str(), pszSharedPath) != 0) {
			// ���L�t�H���_�����L�p�X�ƈႤ���Ƃ��x��
			kjm::log_info(_T("KJMLIB"), 
				_T("kjm::util::guaranteeSharedFolder: ���L�t�H���_�͂���܂����A'%s' ���w���Ă��܂��B"), pInfo->shi2_path);
		}
#endif

		NetApiBufferFree(pInfo);

		nRet = 0;	// ����I��
	} else if (ret == NERR_ServerNotStarted) {
		// The Server service is not started.
		// 1 �b�҂��ă��g���C�A�ő� 60 �񎎂��܂��B
		if (tryCount < 60) {
			Sleep(1000);
			goto retry;
		} else {
			kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: The Server service is not started.(NetShareGetInfo error %d)�B"), ret); 
			nRet = -1;
		}
	} else if (ret == NERR_NetNameNotFound) {
		// ���L�t�H���_�����݂��Ȃ��Ƃ��̏���
		kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: ���L�t�H���_������܂���B�쐬�����݂܂��B"));

		// �w��̋��L�p�X��(�������)�쐬����
		if (::PathFileExists(pszSharedPath) == FALSE) {
			if (kjm::create_directory(pszSharedPath) == FALSE) {
				// �����p�X���쐬�ł��Ȃ����̏���
				kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: �����p�X���쐬�ł��܂���B"));

				return -1;
			}
		}

		SHARE_INFO_2 info;
	// ���Ȃ��Ƃ��Avs2008����LPWSTR �Ő錾�����悤�ɂȂ���
#if _WIN32_IE >= 0x0700
		info.shi2_netname = (LPWSTR)wstrSharedFolder.c_str();
		info.shi2_type = STYPE_DISKTREE;
		info.shi2_remark = (LPWSTR)L"";
		info.shi2_permissions = ACCESS_ALL;
		info.shi2_max_uses = -1;
		info.shi2_current_uses = 0;
		info.shi2_path = (LPWSTR)wstrSharedPath.c_str();
		info.shi2_passwd = (LPWSTR)L"";
#else
		info.shi2_netname = (LPWSTR)wstrSharedFolder.c_str();
		info.shi2_type = STYPE_DISKTREE;
		info.shi2_remark = (LPWSTR)L"";
		info.shi2_permissions = ACCESS_ALL;
		info.shi2_max_uses = -1;
		info.shi2_current_uses = 0;
		info.shi2_path = (LPWSTR)wstrSharedPath.c_str();
		info.shi2_passwd = (LPWSTR)L"";
#endif

		DWORD parm_err = 0;
		ret = NetShareAdd(NULL, 2, (LPBYTE)&info, &parm_err);
		if (ret == NERR_Success) {
			nRet = 0;	// ����I��
		} else {
			// ���L�t�H���_���쐬�ł��Ȃ������Ƃ��̏���
			kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: ���L�t�H���_���쐬�ł��܂���(NetShareAdd error %d)�B"), ret); 

			nRet = -1;	// �ُ�I��
		}
	} else {
		// ���L�t�H���_�̏�񂪎擾�ł��Ȃ����̏���
		kjm::log_info(_T("KJMLIB"), _T("kjm::util::guaranteeSharedFolder: ���L�t�H���_�̏�񂪎擾�ł��܂���(NetShareGetInfo error %d)�B"), ret); 
		
		nRet = -1;	// �ُ�I��
	}

	return nRet;
}

//---------------------------------------------------------------------
// LoadString �֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::loadString(UINT uID) {
	return kjm::util::load_string(uID);
}

//---------------------------------------------------------------------
// LoadString�֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::load_string(UINT uID, HINSTANCE hInstance) {
	std::vector<TCHAR> buffer(512);
	while (1) {
		int nRet = LoadString(hInstance, uID, &buffer[0], buffer.size());
		if (nRet == 0) {
			return _tstring();	// �����񃊃\�[�X���������́A�󕶎����Ԃ�
		} else if (buffer.size() - 1 > nRet) {
			// �o�b�t�@�̃T�C�Y - 1���A�߂�l�����傫���Ƃ��́A
			// ���ׂĂ̕�����ǂݍ��񂾂��Ƃ��m���Ȃ̂ŁA���[�v�𔲂���B
			break;
		}
		buffer.resize(buffer.size() + 512);
	}

	return &buffer[0];
}

//---------------------------------------------------------------------
// string ��Ԃ� GetComputerName �֐�
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getComputerName() {
	std::vector<TCHAR> work(MAX_COMPUTERNAME_LENGTH + 1);
	DWORD nSize = work.size();
	GetComputerName(&work.at(0), &nSize);
	return &work.at(0);
}

//---------------------------------------------------------------------
// GetFullPathName API�̃��b�p�[
//---------------------------------------------------------------------
int kjm::util::getFullPathName(const kjm::_tstring& fileName, kjm::_tstring& fullPath, kjm::_tstring& filePart) {
	std::basic_ostringstream<TCHAR> ossfunc;
	ossfunc << _T("kjm::util::getFullPathName('") << fileName << _T("',*,*)");

	// �K�v�ȃo�b�t�@�T�C�Y���v�Z
	DWORD dw = ::GetFullPathName(fileName.c_str(), 0, NULL, NULL);
	if (dw == 0) {
		DWORD dwErr = ::GetLastError();
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}

	// �Ď��s
	LPTSTR lpFilePart;
	std::vector<TCHAR> buf(dw);
	dw = ::GetFullPathName(fileName.c_str(), buf.size(), &buf[0], &lpFilePart);
	if (dw == 0) {
		DWORD dwErr = ::GetLastError();
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}

	fullPath = &buf[0];
	filePart = lpFilePart;

	return 0;
}

#if 0	// �s�v�֐�
//---------------------------------------------------------------------
// �v���Z�X�̃��W���[�������擾(GetModuleFileName �֐��̃��b�p�[)
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getModuleFileName() {
	std::vector<TCHAR> work(MAX_PATH);
	::GetModuleFileName(NULL, &work.at(0), work.size());
	return &work.at(0);
}
#endif

//---------------------------------------------------------------------
// Win32API GetModuleFileName �����S�Ɏg���֐�
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getModuleFileName(HMODULE hModule/*=NULL*/) {
	std::vector<TCHAR> work(MAX_PATH);

	DWORD dwRet;
	while ((dwRet = ::GetModuleFileName(hModule, &work[0], work.size())) == work.size()) {
		work.resize(work.size() * 2);
	}

	return &work[0];
}

//---------------------------------------------------------------------
// ���W���[�������݂���t�H���_�����擾����
//---------------------------------------------------------------------
kjm::_tstring kjm::util::getModulePath(HMODULE hModule/*=NULL*/) {
	return kjm::path::removeFileSpec(
		kjm::util::getModuleFileName(hModule));
}

_tstring kjm::util::getMscVerText(int mscVer/* = _MSC_VER*/) {
	std::vector<TCHAR> work;
	_tstring result;

	switch (mscVer) {
	case 1200:	result = _T("Visual C++ 6.0");			break;
	case 1310:	result = _T("Visual C++ .NET 2003");	break;
	case 1400:	result = _T("Visual C++ 2005");			break;
	case 1500:	result = _T("Visual C++ 2008");			break;
	case 1600:	result = _T("Visual C++ 2010");			break;
	default:
		kjm::util::sprintf_vec(work, _T("_MSC_VER = %d"), _MSC_VER);
		result = _tstring(&work[0]);
		break;
	}

	return result;
}

// ���O�̔��Ȃ��t�H���_���쐬
kjm::_tstring kjm::util::makeTempFolder(const kjm::_tstring& basePath, const kjm::_tstring& prefix) {
	static LONG s_no = 0;
	kjm::_tstring newPath;

	while (1) {
		InterlockedIncrement(&s_no);
		newPath = kjm::path::combine(basePath, kjm::util::sprintf_str(_T("%s%08X"), prefix, s_no));
		if (kjm::util::fileExists(newPath) == false) {
			break;
		}
	}

	return newPath;
}

//---------------------------------------------------------------------
// ini �t�@�C�����當������擾����(GetPrivateProfileString �֐��̃��b�p�[)
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getPrivateProfileString(const std::basic_string<TCHAR>& section,
															const std::basic_string<TCHAR>& key,
															const std::basic_string<TCHAR>& defValue,
															const std::basic_string<TCHAR>& iniFile) {
	std::vector<TCHAR> work(MAX_PATH);
	while (::GetPrivateProfileString(section.c_str(), key.c_str(), defValue.c_str(), &work.at(0), work.size(), iniFile.c_str()) == (work.size() - 1)) {
		work.resize(work.size() + MAX_PATH);
	}
	return &work.at(0);
}

//---------------------------------------------------------------------
// �t�@�C�����������擾
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::findFileName(const std::basic_string<TCHAR>& path) {
	return ::PathFindFileName(path.c_str());
}

//---------------------------------------------------------------------
// PathRemoveExtension�̃��b�p�[
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::removeExtension(const std::basic_string<TCHAR>& strPath) {
	std::vector<TCHAR> work(strPath.begin(), strPath.end());
	work.push_back('\0');

	PathRemoveExtension(&work[0]);
	return &work[0];
}

//---------------------------------------------------------------------
// PathRenameExtension�̃��b�p�[
//---------------------------------------------------------------------
std::basic_string<TCHAR>& kjm::util::pathRenameExtension(std::basic_string<TCHAR>& strPath, LPCTSTR pszExt) {
	std::vector<TCHAR> work(strPath.begin(), strPath.end());
	work.push_back('\0');
	work.resize(work.size() + lstrlen(pszExt) + MAX_PATH );	// �\���ȃo�b�t�@���m��

	if (::PathRenameExtension(&work[0], pszExt)) {
		strPath = &work[0];
	}

	return strPath;
}

//---------------------------------------------------------------------
// �p�X������Ƀp�X��ǉ�
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathAppend(::lstrcpy(&work.at(0), path.c_str()), more.c_str());
	return &work.at(0);
}

//---------------------------------------------------------------------
// �s���̉��s����菜��
//---------------------------------------------------------------------
std::basic_string<TCHAR>& kjm::util::chomp(std::basic_string<TCHAR>& s) {

	// �s���ɉ��s����������Ƃ��́A��菜��
	while (!s.empty() && (*(s.rbegin()) == '\n' || *(s.rbegin()) == '\r')) {
		s.erase(s.end() - 1);
	}

	return s;
}

//---------------------------------------------------------------------
// �p�X������̊g���q��ύX����
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::renameExtension(const std::basic_string<TCHAR> &path, const std::basic_string<TCHAR> &ext) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRenameExtension(::lstrcpy(&work.at(0), path.c_str()), ext.c_str());
	return &work.at(0);
}

//---------------------------------------------------------------------
// ���W���[���̃p�X������̊g���q��ύX����
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::renameExtension(const std::basic_string<TCHAR> &ext, HMODULE hModule/*=NULL*/) {
	return kjm::util::renameExtension(kjm::util::getModuleFileName(hModule), ext);
}

//---------------------------------------------------------------------
// �p�X������̃t�@�C������ύX����
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::renameFileSpec(const std::basic_string<TCHAR> &path, const std::basic_string<TCHAR> &fname) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveFileSpec(::lstrcpy(&work.at(0), path.c_str()));
	::PathAppend(&work.at(0), fname.c_str());
	return &work.at(0);
}

//---------------------------------------------------------------------
// �J�����g�f�B���N�g����ύX����
//---------------------------------------------------------------------
bool kjm::util::setCurrentDirectory(const std::basic_string<TCHAR>& pathName) {
#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
#if defined(UNICODE)
		Winssa_Log("kjmlog", LOGLV_INFO, "<%s> kjm::util::setCurrentDirectory(): �J�����g�f�B���N�g���� '%s' �ɕύX���܂��B",
			kjm::util::toAnsi(kjm::util::GetModuleName()).c_str(), kjm::util::toAnsi(pathName).c_str());
#else
		Winssa_Log(_T("kjmlog"), LOGLV_INFO, _T("<%s> kjm::util::setCurrentDirectory(): �J�����g�f�B���N�g���� '%s' �ɕύX���܂��B"),
			kjm::util::GetModuleName().c_str(), pathName.c_str());
#endif
	}
#endif
	BOOL bRet = ::SetCurrentDirectory(pathName.c_str());
#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		if (bRet) {
#if defined(UNICODE)
			Winssa_Log("kjmlog", LOGLV_INFO,
				"<%s> kjm::util::setCurrentDirectory(): �ύX���܂����B", kjm::util::toAnsi(kjm::util::GetModuleName()).c_str());
#else
			Winssa_Log(_T("kjmlog"), LOGLV_INFO,
				_T("<%s> kjm::util::setCurrentDirectory(): �ύX���܂����B"), kjm::util::GetModuleName().c_str());
#endif
		} else {
			DWORD dwErr = ::GetLastError();
#if defined(UNICODE)
			Winssa_Log("kjmlog", LOGLV_INFO,
				"<%s> kjm::util::setCurrentDirectory(): error %d", kjm::util::toAnsi(kjm::util::GetModuleName()).c_str(), dwErr);
#else
			Winssa_Log(_T("kjmlog"), LOGLV_INFO,
				_T("<%s> kjm::util::setCurrentDirectory(): error %d"), kjm::util::GetModuleName().c_str(), dwErr);
#endif
		}
	}
#endif
	return (bRet) ? true : false;
}

//---------------------------------------------------------------------
// �������Ԃ����S�� sprintf �֐�
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::sprintf_str(LPCTSTR format, ...) {
	std::vector<TCHAR> buffer(256);
	va_list marker;
	va_start(marker, format);
	while (_vsntprintf(&buffer[0], buffer.size(), format, marker) == -1) {
		buffer.resize(buffer.size() * 2);
	}
	va_end(marker);
	return &buffer[0];
}

//---------------------------------------------------------------------
// �����𕶎���ɕϊ�
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::to_a(int value, int radix/* = 10*/) {
	TCHAR str[80];

#if _WIN32_IE >= 0x0700
	return (_itot_s(value, str, _countof(str), radix) == 0) ? str : _T("");
#else
	return _itot(value, str, radix);
#endif
}

//---------------------------------------------------------------------
// �����𕶎���ɕϊ�(printf�����̃t�H�[�}�b�g���w��ł���)
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::to_a(int value, const std::basic_string<TCHAR>& format) {
	return kjm::util::sprintf_str(format.c_str(), value);
}

//---------------------------------------------------------------------
// �p�X������̏I�[ \ ���폜����
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::removeBackslash(const std::basic_string<TCHAR>& path) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveBackslash(::lstrcpy(&work.at(0), path.c_str()));
	return &work.at(0);
}

//---------------------------------------------------------------------
// �p�X������̃t�@�C�������폜����
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::removeFileSpec(const std::basic_string<TCHAR> &path) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveFileSpec(::lstrcpy(&work.at(0), path.c_str()));
	return &work.at(0);
}

//---------------------------------------------------------------------
// Win32API FormatMessage�����S�Ɏg���֐�
//---------------------------------------------------------------------
DWORD kjm::util::formatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments ) {

	LPTSTR pMessage = NULL;

	DWORD dwRet = ::FormatMessage(
		dwFlags | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpSource,
		dwMessageId,
		dwLanguageId,
		(LPTSTR)&pMessage,
		0,
		Arguments);

	if ( pMessage ) {
		buffer.resize( lstrlen( pMessage ) + 1 );
		lstrcpy( &buffer[0], pMessage );
		::LocalFree( pMessage );
	}

	return dwRet;
}

//---------------------------------------------------------------------
// �V�X�e���W���̃G���[���b�Z�[�W���쐬
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::formatMessageBySystem(DWORD dwErr) {
	std::basic_string<TCHAR> ret;
	PVOID pBuffer;

	DWORD dwRet = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 0, (LPTSTR)&pBuffer, 8, NULL);
	if (dwRet == 0) {
		std::basic_stringstream<TCHAR> ss;
		ss << TEXT("FormatMessage(") << dwErr << TEXT(") failed: error ") << dwErr;
		ret = ss.str();
	} else {
		ret = (LPTSTR)pBuffer;
		LocalFree(pBuffer);
	}

	return kjm::util::chomp(ret);
}

//---------------------------------------------------------------------
// �t�@�C���̗L�����m�F����
//---------------------------------------------------------------------
bool kjm::util::fileExists(const std::basic_string<TCHAR> &path) {
	return (::PathFileExists(path.c_str()) != FALSE) ? true : false;
}

//---------------------------------------------------------------------
// ���l�� human readable format �ȕ�����ɂ���
//---------------------------------------------------------------------
kjm::_tstring kjm::util::toHumanReadable(__int64 value) {
	const int kbyte = 1024;
	const int mbyte = kbyte * 1024;
	const int gbyte = mbyte * 1024;

	_otstringstream ostr;

	if (value >= gbyte) {
		ostr << (value / gbyte) << _T("G");
	} else if (value >= mbyte) {
		ostr << (value / mbyte) << _T("M");
	} else if (value >= kbyte) {
		ostr << (value / kbyte) << _T("K");
	} else {
		ostr << value << _T("B");
	}

	return ostr.str();
}

//=============================================================================
// CSV �t�@�C���������N���X
//=============================================================================
//---------------------------------------------------------------------
// CSV �t�@�C�����J��
//---------------------------------------------------------------------
bool kjm::csvFile::open(LPCTSTR pszFileName) {
	if (!m_fr.is_open()) {
		return false;
	}

#if (_MSC_VER <= 1200) && defined(UNICODE)	// VC++6�ȉ���UNICODE�w��̂Ƃ�
	// basic_ifstream<wchar_t>�́A�y�X�g���[����wchar_t�z�ɂȂ邾����
	// �֐��Ȃǂ́A���ׂāAchar���󂯎��B�s�v�c�Ȏ����ɂȂ��Ă���B
	m_fr.open(kjm::util::toAnsi(pszFileName).c_str());
#else
	m_fr.open(pszFileName);
#endif
	if (!m_fr.is_open()) {
		return false;
	}
	return true;
}

//---------------------------------------------------------------------
// CSV �t�@�C�������
//---------------------------------------------------------------------
void kjm::csvFile::close() {
	if (!m_fr.is_open()) {
		m_fr.close();
	}
}

//---------------------------------------------------------------------
// CSV �t�@�C������ 1 �s�ǂݍ���
//
// [���l]
// "...","...","..." �̌`�������T�|�[�g���Ă��܂���!!
//---------------------------------------------------------------------
int kjm::csvFile::parseCsvCol(std::vector<std::basic_string<TCHAR> >& cols) {
	std::basic_string<TCHAR> oneLine;

	cols.clear();

	if (std::getline(m_fr, oneLine).fail()) {
		return -1;
	}

	if (oneLine.empty()) {
		return 0;
	}
	
	LPCTSTR pTop = NULL;
	LPCTSTR pEnd = NULL;
	std::basic_string<TCHAR> oneCol;

	for (LPCTSTR p = oneLine.c_str(); *p != 0; p++) {
		TCHAR c = *p;
		if (c == '\"' && pTop == NULL) {			// �_�u���R�[�e�[�V���� " ��������
			pTop = p;
		} else if (c == '\"' && pTop != NULL) {		
			if (*(p + 1) == '\"') {					// ���� " ��������A
				oneCol += _T("\"");						// �PCol�̃f�[�^��������؂蕶���łȂ��A�{���� " �Ƃ݂Ȃ� 
				++p;
			} else {
				pEnd = p;
			}
		} else if (c == ',' && pEnd != NULL) {
			cols.push_back(oneCol);
			oneCol.erase();
			pTop = pEnd = NULL;
		} else {
			oneCol += c;
		}
	}

	if (pTop != NULL && pEnd != NULL) {
		cols.push_back(oneCol);
		oneCol.erase();
		pTop = pEnd = NULL;
	}

	// NULL��������A""�ɂ���iatol�Ȃǂŗ�����̂�h�����߁j
	for (size_t l = 0; l < cols.size(); l++) {
		if (cols[l].c_str() == NULL) {
			cols[l] = _T(""); 
			assert(0);
		}
		if (cols[l].empty())	{
			cols[l] = _T("0"); 
			assert(0);
		}
	}

	return cols.size();
}

//=============================================================================
// ��{�\�P�b�g�N���X
//=============================================================================
//---------------------------------------------------------------------
// sockaddr_in �����������邽�߂֗̕��֐�
//---------------------------------------------------------------------
void kjm::baseSocket::initSockaddrIn(sockaddr_in& name, const std::basic_string<TCHAR>& host, short port) {
	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_addr.S_un.S_addr = kjm::util::get_ipaddr(host.c_str());
	name.sin_port = htons(port);
}

//---------------------------------------------------------------------
// �\�P�b�g���J��
//---------------------------------------------------------------------
bool kjm::baseSocket::openSocket(int af, int type, int protocol) {
	if (this->m_sock != INVALID_SOCKET) {
		return false;
	}

	this->m_sock = socket(af, type, protocol);

	return (this->m_sock != INVALID_SOCKET) ? true : false;
}

//---------------------------------------------------------------------
// �ڑ�����
//---------------------------------------------------------------------
bool kjm::baseSocket::connectSocket(const sockaddr_in& name) {
	bool ret = (connect(this->m_sock, (const sockaddr*)&name, sizeof(name)) == 0) ? true : false;
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//---------------------------------------------------------------------
// ��M�^�C���A�E�g�̐ݒ�
//---------------------------------------------------------------------
bool kjm::baseSocket::setRecvTimeout(int msTimeout) {
	bool ret = (setsockopt(this->m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&msTimeout, sizeof(msTimeout)) == 0) ? true : false;
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//---------------------------------------------------------------------
// �o�C���h����(�|�[�g�ԍ����w�肷��ȒP�o�[�W������������)
//---------------------------------------------------------------------
bool kjm::baseSocket::bindSocket(unsigned short port) {
	struct sockaddr_in me;
	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.S_un.S_addr = INADDR_ANY;
	me.sin_port = htons(port);
	bool ret = (bind(this->m_sock, (sockaddr*)&me, sizeof(me)) == 0) ? true : false;
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//---------------------------------------------------------------------
// ��M����
//---------------------------------------------------------------------
int kjm::baseSocket::recvfrom(std::vector<char>& buf, sockaddr_in& from) {
	int fromlen = sizeof(from);
	buf.resize(8192);
	int ret = ::recvfrom(this->m_sock, &buf[0], buf.size(), 0, (sockaddr*)&from, &fromlen);
	this->m_lastError = ::WSAGetLastError();
	if (ret > 0) {
		buf.resize(ret);
	}
	return ret;
}

//---------------------------------------------------------------------
// ��M����
//---------------------------------------------------------------------
int kjm::baseSocket::recv(std::vector<char>& buf) {
	buf.resize(8192);
	int ret = ::recv(this->m_sock, &buf[0], buf.size(), 0);
	this->m_lastError = ::WSAGetLastError();
	if (ret > 0) {
		buf.resize(ret);
	}
	return ret;
}

//---------------------------------------------------------------------
// ��M����
//---------------------------------------------------------------------
void kjm::baseSocket::recvAll(std::vector<char>& buf) {
	std::vector<char> recvBuf;
	while (this->recv(recvBuf) > 0) {
		buf.insert(buf.end(), recvBuf.begin(), recvBuf.end());
		if (this->isRecvAll(buf))
			break;
	}
}

//---------------------------------------------------------------------
// �S�Ď�M���������f����֐�
//---------------------------------------------------------------------
bool kjm::baseSocket::isRecvAll(const std::vector<char>& buf) {
	return false;
}

//---------------------------------------------------------------------
// ���M����
//---------------------------------------------------------------------
int kjm::baseSocket::sendto(const std::vector<char>& buf, const sockaddr_in& to) {
	int ret = ::sendto(this->m_sock, &buf[0], buf.size(), 0, (sockaddr*)&to, sizeof(to));
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//---------------------------------------------------------------------
// ���M����
//---------------------------------------------------------------------
int kjm::baseSocket::send(const char* pStr) {
	int ret = ::send(this->m_sock, pStr, strlen(pStr), 0);
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//---------------------------------------------------------------------
// �ؒf����
//---------------------------------------------------------------------
int kjm::baseSocket::shutdown(int how) {
	int ret = ::shutdown(this->m_sock, how);
	this->m_lastError = ::WSAGetLastError();
	return ret;
}

//---------------------------------------------------------------------
// �D��ɐؒf����
//---------------------------------------------------------------------
void kjm::baseSocket::gracefulShutdown() {
	std::vector<char> buf;
	this->shutdown(SD_SEND);
	while (this->recv(buf) > 0)
		;
	this->closeSocket();
}

//---------------------------------------------------------------------
// �\�P�b�g�����
//---------------------------------------------------------------------
bool kjm::baseSocket::closeSocket() {
	bool result = true;

	if (this->m_sock != INVALID_SOCKET) {
		result = (closesocket(this->m_sock) == 0) ? true : false;
		this->m_lastError = ::WSAGetLastError();
		this->m_sock = INVALID_SOCKET;
	}

	return result;
}

//---------------------------------------------------------------------
// Winsock������
//---------------------------------------------------------------------
int kjm::baseSocket::startup() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 0), &data);
}

//=============================================================================
// �^�X�N�g���C�A�C�R���N���X
//=============================================================================
//---------------------------------------------------------------------
// �f�t�H���g�E�R���X�g���N�^
//---------------------------------------------------------------------
kjm::BiNotifyIcon::BiNotifyIcon() {

	// Add ���\�b�h�� sizeof(NOTIFYICON)�A
	// Delete ���\�b�h�� 0 ��������B
	cbSize = 0;
}


/**
 *	@brief	�f�X�g���N�^
 *
 *	@note	�^�X�N�g���C�ɓo�^�ς݂̂Ƃ��́ADelete ����B
 */
kjm::BiNotifyIcon::~BiNotifyIcon() {

	// �^�X�N�g���C�ɓo�^���Ă���Ƃ��́A�폜����B
	// Delete ���\�b�h�����Ŏ������f�B
	Delete();
}


/**
 *	@brief	�^�X�N�g���C�ɃA�C�R����ǉ�����B
 *			�����́ANOTIFYICONDATA �̃����o�[�ɑΉ�����B
 *
 *	@param[in]	hWnd				�E�B���h�E�ւ̃n���h��
 *	@param[in]	uID					�A�v���P�[�V���������ʂ��邽�߂̃^�X�N�o�[�A�C�R����ID
 *	@param[in]	uFlags				uCallbackMessage, hIcon, pszTip �̗L��/�������w�肷��t���O
 *	@param[in]	uCallbackMessage	�ʒm���b�Z�[�W�Ɏg���郁�b�Z�[�WID
 *	@param[in]	hIcon				�o�^����A�C�R���ւ̃n���h��
 *	@param[in]	pszTip				�c�[���`�b�v�̕�����
 *
 *	@return	���������Ƃ��� TRUE ��Ԃ��B
 *			���s�����Ƃ��� FALSE ��Ԃ��B
 *
 *	@note	�A�C�R�������łɓo�^�ς݂̂Ƃ��́ADelete ���Ă���Ăяo�����B
 */
BOOL kjm::BiNotifyIcon::Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip) {

	// ���łɓo�^����Ă��邩������Ȃ��̂ŁA�폜����
	Delete();

	// NOTIFYICONDATA �̃����o�[�𖄂߂�
	cbSize = sizeof(NOTIFYICONDATA);
	hWnd = hWnd;
	uID = uID;
	uFlags = uFlags;
	uCallbackMessage = uCallbackMessage;
	hIcon = hIcon;
	if (pszTip)
		lstrcpy(this->szTip, pszTip);
	else
		this->szTip[0] = '\0';

	BOOL rc = Shell_NotifyIcon(NIM_ADD, this);

	// �o�^�Ɏ��s�����Ƃ��́A���o�^���(cbSize == 0)�ɂ���B
	if (rc == FALSE) {
		cbSize = 0;
	}

	return rc;
}


/**
 *	@brief	�^�X�N�g���C����A�C�R�����폜����
 *
 *	@note	�A�C�R�������o�^�ł��A����ɏ��������B
 */
BOOL kjm::BiNotifyIcon::Delete() {

	BOOL rc = TRUE;

	// Add ���\�b�h���Ă΂�Ă���Ƃ������Ăяo��
	if (cbSize != 0) {
		rc = Shell_NotifyIcon(NIM_DELETE, this);

		// �o�^�����ɐ��������Ƃ��A���o�^���(cbSize == 0)�ɂ���B
		if (rc != FALSE) {
			cbSize = 0;
		}
	}

	return rc;
}


/**
 *	@brief	�^�X�N�g���C�A�C�R���̏���ύX����B
 *			�����́ANOTIFYICONDATA �̃����o�[�ɑΉ�����B
 *
 *	@param[in]	uFlags				uCallbackMessage, hIcon, pszTip �̗L��/�������w�肷��t���O
 *	@param[in]	uCallbackMessage	�ʒm���b�Z�[�W�Ɏg���郁�b�Z�[�WID
 *	@param[in]	hIcon				�o�^����A�C�R���ւ̃n���h��
 *	@param[in]	pszTip				�c�[���`�b�v�̕�����
 *
 *	@return	���������Ƃ��� TRUE ��Ԃ��B
 *			���s�����Ƃ��� FALSE ��Ԃ��B
 *
 *	@note	�A�C�R�������o�^�̂Ƃ��́A���s����B
 */
BOOL kjm::BiNotifyIcon::Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, LPCTSTR pszTip) {

	// Add ���\�b�h���A�܂��Ă΂�Ă��Ȃ��Ƃ��́A�G���[�I��
	if (cbSize == 0)	return FALSE;

	uFlags = uFlags;

	// �t���O�ɉ����āANOTIFYICONDATA �̃����o�[�𖄂߂�
	if (uFlags & NIF_MESSAGE)
		uCallbackMessage = uCallbackMessage;

	if (uFlags & NIF_ICON)
		hIcon = hIcon;

	if (uFlags & NIF_TIP) {
		if (pszTip) {
			lstrcpy(szTip, pszTip);
		} else {
			szTip[0] = '\0';
		}
	}

	return Shell_NotifyIcon(NIM_MODIFY, this);
}


//---------------------------------------------------------------------
// ��񃍃O���c���֐�
//---------------------------------------------------------------------
void kjm::log_info(LPCTSTR signature, LPCTSTR format, ...) {
	va_list marker;
	va_start(marker, format);	// �ό̈����̏�����

	std::vector<TCHAR> buffer(128);
	do {
		buffer.resize(buffer.size() * 2);
	} while (_vsntprintf(&buffer[0], buffer.size(), format, marker) == -1);
	va_end(marker);
#if defined(USE_WSSLOG)
#if defined(UNICODE)
	{
		std::string str1 = kjm::util::toAnsi(signature);
		std::string str2 = kjm::util::toAnsi(&buffer[0]);

		Winssa_Log(str1.c_str(), LOGLV_INFO, "%s", str2.c_str());
	}
#else
	Winssa_Log(signature, LOGLV_INFO, "%s", &buffer[0]);
#endif
#else
	std::basic_string<TCHAR> fname = 
		kjm::util::renameExtension(kjm::util::append(kjm::util::getTempPath(), kjm::util::GetModuleName()), _T(".log"));
	FILE* fp = _tfopen(fname.c_str(), _T("a+"));
	if (fp) {
		SYSTEMTIME st;
		::GetLocalTime(&st);
		_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d %s\n"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			&buffer[0]);
		fclose(fp);
	}
#endif
}


//---------------------------------------------------------------------
// �G���[���O���c���֐�
//---------------------------------------------------------------------
void kjm::log_error(LPCTSTR signature, LPCTSTR format, ...) {
	va_list marker;
	va_start(marker, format);	// �ό̈����̏�����

	std::vector<TCHAR> buffer(128);
	do {
		buffer.resize(buffer.size() * 2);
	} while (_vsntprintf(&buffer[0], buffer.size(), format, marker) == -1);
	va_end(marker);
#if defined(USE_WSSLOG)
#if defined(UNICODE)
	{
		std::string str1 = kjm::util::toAnsi(signature);
		std::string str2 = kjm::util::toAnsi(&buffer[0]);

		Winssa_Log(str1.c_str(), LOGLV_ERR, "%s", str2.c_str());
	}
#else
	Winssa_Log(signature, LOGLV_ERR, "%s", &buffer[0]);
#endif
#else
	std::basic_string<TCHAR> fname = 
		kjm::util::renameExtension(kjm::util::append(kjm::util::getTempPath(), kjm::util::GetModuleName()), _T(".log"));
	FILE* fp = _tfopen(fname.c_str(), _T("a+"));
	if (fp) {
		SYSTEMTIME st;
		::GetLocalTime(&st);
		_ftprintf(fp, _T("%04d/%02d/%02d %02d:%02d:%02d.%03d %s\n"),
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
			&buffer[0]);
		fclose(fp);
	}
#endif
}


//---------------------------------------------------------------------
// �ċA�I�Ƀf�B���N�g�����쐬�ł��� CreateDirectory �֐�
//
// [����]
// lpPathName  �f�B���N�g��������ւ̃|�C���^
//
// [�߂�l]
// TRUE   ����I��
// FALSE  �G���[�I��
//
// [����]
// ���łɃt�H���_������Ƃ�������I���BUNC���L�p�X�݂̂��w�肵���Ƃ��́A
// �p�X���Ȃ��Ă�����I���ɂȂ�B
//---------------------------------------------------------------------
BOOL kjm::create_directory(LPCTSTR lpPathName) {
	
	// �Ō�� \ �́A�ז��Ȃ̂Ŏ�菜���Ă���A�����ɓ���B
	TCHAR path_name[MAX_PATH];
	::PathRemoveBackslash(lstrcpy(path_name, lpPathName));

	if (::PathFileExists(path_name))	return TRUE;
	if (::PathIsRoot(path_name))		return TRUE;

	TCHAR parent_path[MAX_PATH];
	::PathRemoveFileSpec(lstrcpy(parent_path, path_name));

	if (kjm::create_directory(parent_path))
		return ::CreateDirectory(path_name, NULL);

	return FALSE;
}


//---------------------------------------------------------------------
// ���[�N�O���[�v�����擾����
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::netapi32::getWorkgroupName() {
	std::basic_string<TCHAR> result;

	LPWKSTA_INFO_100 pBuf = NULL;
	NET_API_STATUS ret = ::NetWkstaGetInfo(NULL, 100, (LPBYTE*)&pBuf);
	if (ret == NERR_Success) {
#if defined(UNICODE)
		result = pBuf->wki100_langroup;
#else
		// LPWSTR �Ő錾���ĂȂ���΂����Ȃ��w�b�_���ALPTSTR �Ő錾���Ă��邽��
		// LPWSTR �ɃL���X�g���K�v�BOS �̃w�b�_�̕s���B
		result = kjm::util::toAnsi((LPWSTR)pBuf->wki100_langroup);
#endif
	}

	if (pBuf != NULL) {
		::NetApiBufferFree(pBuf);
	}

	return result;
}

//=============================================================================
// win32api�̃��b�p�[
//=============================================================================
//---------------------------------------------------------------------
// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::win32::str_replace(const std::basic_string<TCHAR>& src, LPCTSTR pFind, LPCTSTR pReplace) {
	std::basic_string<TCHAR> result = src;
	std::basic_string<TCHAR>::size_type pos = 0;
	while ((pos = result.find(pFind, pos)) != std::basic_string<TCHAR>::npos) {
		result.replace(pos, lstrlen(pFind), pReplace);
	}
	return result;
}

//---------------------------------------------------------------------
// �G���[�R�[�h����V�X�e���G���[���b�Z�[�W���擾����B
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::win32::format_message(DWORD dwErrorCode) {

	std::basic_string<TCHAR> result;

	LPTSTR pBuffer = NULL;

	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&pBuffer,
		0,
		NULL);

	if (pBuffer) {
		result = pBuffer;
		result = str_replace(std::basic_string<TCHAR>(pBuffer), _T("\r\n"), _T(""));

		::LocalFree(pBuffer);
	} else {
		TCHAR buf[1024];
		wsprintf(buf, _T("�G���[ %d (0x%08X) �̃��b�Z�[�W�́A����܂���B"), dwErrorCode, dwErrorCode);
		result = buf;
	}

	return result;
}

//---------------------------------------------------------------------
// ���[�N�G���A�̒��S�ɃE�B���h�E���ړ�������B
//---------------------------------------------------------------------
BOOL kjm::win32::MoveCenterWorkarea(HWND hwnd) {
	// ���[�N�X�G���A�̋�`���擾
	RECT rcWorkarea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkarea, 0);

	// �ړ�����E�B���h�E�̋�`���擾
	RECT rcClient;
	GetWindowRect(hwnd, &rcClient);

	int nNewLeft = ((rcWorkarea.right - rcWorkarea.left) - (rcClient.right - rcClient.left)) / 2;
	int nNewTop = ((rcWorkarea.bottom - rcWorkarea.top) - (rcClient.bottom - rcClient.top)) / 2;

	// �ړ��\��̍��オWorkarea����͂ݏo�Ă���ꍇ�́A���W��0�ɂ���
	if (nNewLeft < 0)	nNewLeft = 0;
	if (nNewTop < 0)	nNewTop = 0;

	return MoveWindow(hwnd, nNewLeft, nNewTop,
		(rcClient.right - rcClient.left), (rcClient.bottom - rcClient.left), FALSE);
}


//=============================================================================
// utility �֐��N���X
//=============================================================================
//---------------------------------------------------------------------
// �z�X�g���܂��́AIP�����񂩂�IP�A�h���X���擾
//---------------------------------------------------------------------
DWORD kjm::util::get_ipaddr(LPCTSTR lpName) {
	DWORD addr = INADDR_ANY;

	if (lpName[0] == '\0')	return INADDR_ANY;

	// �܂��AIP������ϊ����Ă݂�B
#if defined(UNICODE)
	addr = inet_addr(kjm::util::toAnsi(lpName).c_str());
#else
	addr = inet_addr(lpName);
#endif
	if (addr == INADDR_NONE) {

		// DNS������
#if defined(UNICODE)
		struct hostent* hent = gethostbyname(kjm::util::toAnsi(lpName).c_str());
#else
		struct hostent* hent = gethostbyname(lpName);
#endif
		if (hent != NULL) {
			addr = *((DWORD *)(hent->h_addr_list[0]));
		} else{
			addr = INADDR_ANY;
		}
	}

	return	addr;
}

//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����
//---------------------------------------------------------------------
bool kjm::util::remove_folder(LPCTSTR pszPath) {
	bool bRet = true;
	std::vector<TCHAR> wc(MAX_PATH);

	// �����ɕK�v�ȃ��C���h�J�[�h���쐬
	PathAppend(lstrcpyn(&wc[0], pszPath, wc.size()), _T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(&wc[0], &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((lstrcmp(data.cFileName, _T(".")) != 0) && (lstrcmp(data.cFileName, _T("..")) != 0)) {
					std::vector<TCHAR> child(MAX_PATH);

					// �q���t�H���_���ċA�I�ɍ폜����
					PathAppend(lstrcpyn(&child[0], pszPath, child.size()), data.cFileName);
					if ((bRet = kjm::util::remove_folder(&child[0])) == false) {
						break;
					}
				}
			} else {
				std::vector<TCHAR> fname(MAX_PATH);
				PathAppend(lstrcpyn(&fname[0], pszPath, fname.size()), data.cFileName);

				// �폜����t�@�C���� readonly �����Ă���Ƃ��͗��Ƃ��B
				DWORD dwAttr = GetFileAttributes(&fname[0]);
				if ((dwAttr != 0xffffffff) && ((dwAttr & FILE_ATTRIBUTE_READONLY) != 0)) {
					if (!SetFileAttributes(&fname[0], dwAttr & ~FILE_ATTRIBUTE_READONLY)) {
						bRet = false;
						break;
					}
				}

				if (!DeleteFile(&fname[0])) {
					bRet = false;
					break;
				}
			}
		} while (FindNextFile(hFind, &data));

		FindClose(hFind);
	} else {
		bRet = false;
	}

	if (bRet) {
		if (!RemoveDirectory(pszPath)) {
			bRet = false;
		}
	}

	return bRet;
}

//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����
//---------------------------------------------------------------------
void kjm::util::removeFolder(const _tstring& pszPath) {
	std::list<_tstring> targetFolder;
	targetFolder.push_front(pszPath);

	while (targetFolder.size() != 0) {
		kjm::folderInfo currentFolder(targetFolder.front());

		std::vector<kjm::fileInfo> files = currentFolder.getFiles();			// �t�@�C���ꗗ���擾
		std::vector<kjm::folderInfo> folders = currentFolder.getSubFolders();	// �t�H���_�ꗗ�擾

		// �����t�@�C����S�č폜����
		for (std::vector<kjm::fileInfo>::size_type i = 0; i < files.size(); i++) {
			kjm::file::deleteFile(files[i].fullName(), true);
		}

		// �����t�H���_���Ȃ��Ƃ��́A���݂̃f�B���N�g�����폜�B
		// ���鎞�́A���X�g�ɒǉ����āA���̃��[�v�����ɉ񂷁B
		if (folders.empty()) {
			targetFolder.pop_front();
			if (::RemoveDirectory(currentFolder.getFullPath().c_str()) == FALSE) {
				DWORD dwErr = ::GetLastError();
				_otstringstream oss;
				oss << _T("�t�H���_ '") << pszPath << _T("' ���폜�ł��܂���B�T�u�t�H���_ '") << currentFolder.getFullPath() << _T("' ���폜�ł��܂���B") << kjm::util::formatMessageBySystem(dwErr);
				throw kjm::exception::exception(dwErr, oss.str());
			}
		} else {
			for (std::vector<kjm::folderInfo>::size_type i = 0; i < folders.size(); i++) {
				targetFolder.push_front(folders[i].getFullPath());
			}
		}
	}
}

//---------------------------------------------------------------------
// Unicode ������� ANSI ������ɕϊ�
//---------------------------------------------------------------------
std::string kjm::util::toAnsi(const std::wstring& src) {
	DWORD dwSizeNeed = ::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	std::vector<char> buf(dwSizeNeed);

	::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size(), NULL, NULL);

	return &buf[0];
}

//---------------------------------------------------------------------
// ANSI ������� Unicode ������ɕϊ�
//---------------------------------------------------------------------
std::wstring kjm::util::toUnicode(const std::string& src) {
	DWORD dwSizeNeed = ::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	std::vector<wchar_t> buf(dwSizeNeed);

	::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size());

	return &buf[0];
}

//---------------------------------------------------------------------
// Unicode ������� ANSI ������ɕϊ�(cp = CP_ACP)
//---------------------------------------------------------------------
std::string kjm::util::toAnsi(const std::wstring& src, int cp) {
	DWORD dwSizeNeed = ::WideCharToMultiByte(cp, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	std::vector<char> buf(dwSizeNeed);

	::WideCharToMultiByte(cp, 0, src.c_str(), -1, &buf[0], buf.size(), NULL, NULL);

	return &buf[0];
}

//---------------------------------------------------------------------
// ANSI ������� Unicode ������ɕϊ�
//---------------------------------------------------------------------
std::wstring kjm::util::toUnicode(const std::string& src, int cp) {
	DWORD dwSizeNeed = ::MultiByteToWideChar(cp, 0, src.c_str(), -1, NULL, 0);
	std::vector<wchar_t> buf(dwSizeNeed);

	::MultiByteToWideChar(cp, 0, src.c_str(), -1, &buf[0], buf.size());

	return &buf[0];
}

//---------------------------------------------------------------------
// �w��t�H���_�ȉ��̓ǂݍ��ݐ�p�������O��
//---------------------------------------------------------------------
bool kjm::util::UnsetReadOnly(const std::basic_string<TCHAR>& path) {
	std::vector<std::basic_string<TCHAR> > targetFolder;
	targetFolder.push_back(path);

	bool result = true;

	while (!targetFolder.empty()) {
		std::basic_string<TCHAR> strTarget = targetFolder.back();
		targetFolder.pop_back();

		// �^�[�Q�b�g���̂��ׂẴt�H���_�̓ǂݍ��ݐ�p���O��
		std::vector<std::basic_string<TCHAR> > files = kjm::directory::getFiles(strTarget);
		for (size_t i = 0; i < files.size(); i++) {
			if (!::SetFileAttributes(files[i].c_str(), ::GetFileAttributes(files[i].c_str()) & ~FILE_ATTRIBUTE_READONLY)) {
				result = false;
			}
		}

		// �^�[�Q�b�g���̃T�u�t�H���_��񋓂��A���̃^�[�Q�b�g�Ƃ���B
		std::vector<std::basic_string<TCHAR> > dirs = kjm::directory::getSubFolders(strTarget);
		targetFolder.insert(targetFolder.end(), dirs.begin(), dirs.end());
	}

	return result;
}


//---------------------------------------------------------------------
// ���S�� sprintf 
//
// [����]
// ��Ԗڂ̈������Avector<TCHAR>�ւ̎Q�ƂɂȂ��Ă���ȊO�A
// sprintf�֐��Ɠ����ł��B
//---------------------------------------------------------------------
int kjm::util::sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...) {
	va_list marker;
	va_start(marker, format);
	int result = kjm::util::vsprintf_vec(buf, format, marker);
	va_end(marker);
	return result;
}

//---------------------------------------------------------------------
// ������̕���
//---------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::util::split(const std::basic_string<TCHAR>& src, TCHAR c) {
	std::basic_istringstream<TCHAR> iss(str);
	std::basic_string<TCHAR> tmp;
	std::vector<std::basic_string<TCHAR>> result;
	while (std::getline(iss, tmp, c)) result.push_back(tmp);
	return result;
}

//---------------------------------------------------------------------
// ������ɕϊ�
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::toString(DWORD value) {
	TCHAR buf[80];
	return _ultot(value, buf, 10);
}

//---------------------------------------------------------------------
// �R�s�[������ CopyFile API ���g���B
// �R�s�[��̓Ǎ������𗎂Ƃ��@�\�t���B
//
// [�߂�l]  1   ����I��(�R�s�[��̓ǂݍ��ݑ����𗎂Ƃ���)
// [�߂�l]  0   ����I��
// [�߂�l] -1   CopyFile API�̎��s
// [�߂�l] -12  �R�s�[��t�@�C���̑����擾���s
// [�߂�l] -13  �R�s�[��t�@�C���̓ǂݍ��ݑ������Z�b�g���s
//---------------------------------------------------------------------
int kjm::util::copyFile2(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly/*=TRUE*/) {
	DWORD t1 = GetTickCount();
	DWORD tr = 0, tw = 0;

	SetLastError(NOERROR);

	int result = 0;

	// �R�s�[�悪 Readonly �̎��A�G���[�ɂ��Ȃ����[�h?
	if (bFailIfReadonly == FALSE && kjm::path::fileExists(lpNewFileName) != false) {
		DWORD dwAttributes = ::GetFileAttributes(lpNewFileName);
		if (dwAttributes != 0xffffffff) {
			if ((dwAttributes & FILE_ATTRIBUTE_READONLY) != 0) {
				if (::SetFileAttributes(lpNewFileName, dwAttributes & ~FILE_ATTRIBUTE_READONLY)) {
					// �����̕ύX����
					result = 1;
				} else {
					result = -13;
				}
			}
		} else {
			result = -12;
		}
	}

///	BOOL bRet = CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
///	if (bRet == FALSE) {
///		result = -1;
///	}

	HANDLE hFileSrc = CreateFile(lpExistingFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFileSrc == INVALID_HANDLE_VALUE) {
		return -1;
	}

	HANDLE hFileDst = CreateFile(lpNewFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL);
	
	if (hFileSrc != INVALID_HANDLE_VALUE && hFileDst != INVALID_HANDLE_VALUE) {
		static char buffer[1024 * 1024];	// 1M�o�b�t�@
		DWORD readSize;
		DWORD written;

		while (1) {
			if (ReadFile(hFileSrc, buffer, sizeof(buffer), &readSize, NULL) == 0) {
				result = -1;
				break;
			}
			if (readSize == 0) break;
			if (WriteFile(hFileDst, buffer, readSize, &written, NULL) == 0) {
				CloseHandle(hFileDst);
				hFileDst = INVALID_HANDLE_VALUE;
				DeleteFile(lpNewFileName);
				result = -1;
				break;
			}
		}
	} else {
		result = -1;
	}

	if (result == 0) {
		FILETIME ft;
		if (GetFileTime(hFileSrc, NULL, NULL, &ft)) {
			SetFileTime(hFileDst, NULL, NULL, &ft);
		}
	}

	if (hFileSrc != INVALID_HANDLE_VALUE) CloseHandle(hFileSrc);
	if (hFileDst != INVALID_HANDLE_VALUE) CloseHandle(hFileDst);

	DWORD t2 = GetTickCount();
	DWORD ta = t2 - t1;
	///_tprintf(_T("\n(r[%4d] + w[%4d] + o[%4d]) = a[%4d]"), tr, tw, ta - tr - tw, ta);

	return result;
}

//---------------------------------------------------------------------
// �������������������
//---------------------------------------------------------------------
kjm::_tstring kjm::util::toLower(const kjm::_tstring& src) {
	kjm::_tstring result;

#if _MSC_VER <= 1200	// VC6 �ȉ�
	for (int i = 0; i < src.size(); i++) {
		result += _totlower(src[i]);
	}
#else
	std::transform(src.begin(), src.end(), std::back_inserter(result), ::_totlower);
#endif

	return result;
}

//---------------------------------------------------------------------
// �������啶��������
//---------------------------------------------------------------------
kjm::_tstring kjm::util::toUpper(const kjm::_tstring& src) {
	kjm::_tstring result;

#if _MSC_VER <= 1200	// VC6 �ȉ�
	for (int i = 0; i < src.size(); i++) {
		result += _totupper(src[i]);
	}
#else
	std::transform(src.begin(), src.end(), std::back_inserter(result), ::_totupper);
#endif

	return result;
}

//---------------------------------------------------------------------
// ���S�� vsprintf
//---------------------------------------------------------------------
int kjm::util::vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr) {
	// �o�b�t�@���w�肳��Ă��Ȃ��Ƃ��A256�o�C�g���蓖�ĂĂ���
	if (buf.size() == 0) {
		buf.resize(256);
	}

	while (1) {

		// vsnprintf �ŕ�������쐬�B
		// �s�\���ȃo�b�t�@��Ƀf�[�^���쐬���Ă���ԁA���[�v���J��Ԃ��B
		int n = _vsntprintf(&buf[0], buf.size(), format, argptr);
		if (n != -1 && n != buf.size()) {
			break;
		}

		buf.resize(buf.size() + 256);
	}

	return lstrlen(&buf[0]);
}

//---------------------------------------------------------------------
//---------------------------------------------------------------------
std::string kjm::util::toString(const std::vector<char>& data) {
	std::vector<char>::const_iterator epos = std::find(data.begin(), data.end(), 0);
	return std::string(data.begin(), epos);
}

std::wstring kjm::util::toString(const std::vector<wchar_t>& data) {
	if (data.empty()) return std::wstring();

	std::vector<wchar_t> work = data;
	if (work[0] == 0xfffe) {
		work.erase(work.begin());
		for (int i = 0; i < work.size(); i++) {
			work[i] = ((work[i] & 0xff00) >> 8) | ((work[i] & 0x00ff) << 8);
		}
	} else if (work[0] == 0xfeff) {
		work.erase(work.begin());
	}
	std::vector<wchar_t>::iterator epos = std::find(work.begin(), work.end(), 0);
	return std::wstring(work.begin(), epos);
}

std::string kjm::util::toAnsiString(std::vector<BYTE>& data) {
	std::vector<char> work;
	while (!data.empty()) {
		char c = (char)data[0];
		work.push_back(c);
		data.erase(data.begin());
		if (c == 0) break;
	}
	return kjm::util::toString(work);
}

std::wstring kjm::util::toUnicodeString(std::vector<BYTE>& data) {
	std::vector<wchar_t> work;
	while (!data.empty()) {
		wchar_t c =(((wchar_t)data[0]) | (wchar_t)data[0+1] << 8);
		work.push_back(c);
		data.erase(data.begin(), data.begin() + 2);
		if (c == 0) break;
	}
	return kjm::util::toString(work);
}



//=============================================================================
// �f�B���N�g����T�u�f�B���N�g����ʂ��āA�쐬�A�ړ��A����ї񋓂��邽�߂�
// �ÓI���\�b�h�����J���܂��B
//=============================================================================
//---------------------------------------------------------------------
// �t�@�C���܂��̓f�B���N�g���A����т��̓��e��V�����ꏊ�ɃR�s�[���܂��B
//---------------------------------------------------------------------
BOOL kjm::directory::copy(LPCTSTR sourceDirName, LPCTSTR destDirName, bool (*is_cancel_callback)(void*), void* callback_arg) {
	BOOL bRet = FALSE;

	// �Ƃ肠�����A�R�s�[��f�B���N�g�����쐬���Ă݂�B
	if (kjm::create_directory(destDirName) == FALSE) {

		DWORD dwError = ::GetLastError();
		kjm::log_error(_T("kjmlib"),
			_T("kjm::directory::copy(\"%s\", \"%s\"): kjm::create_directory(\"%s\") error: %d : %s"),
			sourceDirName, destDirName, destDirName, dwError, kjm::win32::format_message(dwError).c_str());

		return bRet;
	}

	// �t�@�C���ƃt�H���_��񋓂��āA�R�s�[�ƍċA�Ăяo���𑱂���B
	kjm::PathString findPath = kjm::PathString(sourceDirName).Append(_T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = ::FindFirstFile(findPath.c_str(), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		bRet = TRUE;	// ����I�������ɂ��āA�ȍ~�̏����𑱂���
		do {
			if ((*is_cancel_callback)(callback_arg)) {
				break;
			}

			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((::lstrcmp(data.cFileName, _T(".")) != 0) && (::lstrcmp(data.cFileName, _T("..")) != 0)) {
					// �t�H���_���ċA�I�ɃR�s�[����B
					if (kjm::directory::copy(
							kjm::PathString(sourceDirName).Append(data.cFileName).c_str(),
							kjm::PathString(destDirName).Append(data.cFileName).c_str(),
							is_cancel_callback, callback_arg) == FALSE) {

						bRet = FALSE;
						break;
					}
				}
			} else {
				// ���������t�@�C�����R�s�[����B
				kjm::PathString sourceFileName = kjm::PathString(sourceDirName).Append(data.cFileName);
				kjm::PathString destFileName = kjm::PathString(destDirName).Append(data.cFileName);

				if (::CopyFile(sourceFileName.c_str(), destFileName.c_str(), FALSE) == FALSE) {
					bRet = FALSE;

					DWORD dwError = ::GetLastError();
					kjm::log_error(_T("kjmlib"),
						_T("kjm::directory::copy(\"%s\", \"%s\"): ::CopyFile(\"%s\", \"%s\", FALSE) error: %d : %s"),
						sourceDirName, destDirName, sourceFileName.c_str(), destFileName.c_str(), dwError, kjm::win32::format_message(dwError).c_str());

					break;
				}
			}
		} while (::FindNextFile(hFind, &data));

		::FindClose(hFind);
	} else {
		DWORD dwError = ::GetLastError();
		kjm::log_error(_T("kjmlib"),
			_T("kjm::directory::copy(\"%s\", \"%s\"): ::FindFirstFile(\"%s\", ...) error: %d : %s"),
			sourceDirName, destDirName, findPath.c_str(), dwError, kjm::win32::format_message(dwError).c_str());
	}

	return bRet;
}

//---------------------------------------------------------------------
// �f�B���N�g�����̃t�@�C����񋓂���
//---------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::directory::getFiles(
	const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard) {

	std::vector<std::basic_string<TCHAR> > result;
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(kjm::PathString(path).Append(wildCard).c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return result;
	}

	do {
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			result.push_back(kjm::PathString(path).Append(findData.cFileName).getString());
		}
	} while (::FindNextFile(hFind, &findData) != FALSE);

	::FindClose(hFind);
	return result;
}

kjm::_tstrings kjm::directory::glob(const kjm::_tstring& pattern) {
	kjm::_tstring fname = kjm::path::getFileName(pattern);
	kjm::_tstring path = kjm::path::addBackslash(kjm::path::removeFileSpec(pattern));
	kjm::_tstrings result;
	kjm::findFile ff;
	if (ff.findFirstFile(pattern)) {
		do {
			if (!ff.isDirectory()) {
				result.push_back(path + ff.getFileName());
			}
		} while (ff.findNextFile());
		ff.findClose();
	}
	return result;
}


//---------------------------------------------------------------------
// �f�B���N�g�����̃T�u�f�B���N�g����񋓂���
//---------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::directory::getSubFolders(
	const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildCard) {

	std::vector<std::basic_string<TCHAR> > result;
	WIN32_FIND_DATA findData;
	HANDLE hFind = ::FindFirstFile(kjm::PathString(path).Append(wildCard).c_str(), &findData);
	if (hFind == INVALID_HANDLE_VALUE) {
		return result;
	}

	do {
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
			if (lstrcmp(findData.cFileName, _T(".")) != 0 && lstrcmp(findData.cFileName, _T("..")) != 0) {
				result.push_back(kjm::PathString(path).Append(findData.cFileName).getString());
			}
		}
	} while (::FindNextFile(hFind, &findData) != FALSE);

	::FindClose(hFind);
	return result;
}

//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ쐬����
//---------------------------------------------------------------------
void kjm::directory::createDirectory(const kjm::_tstring& path) {
	if (::PathIsRoot(path.c_str()))		return;
	if (::PathFileExists(path.c_str()))	return;

	// �e�f�B���N�g���̍쐬�����݂�B
	std::vector<TCHAR> parent(MAX_PATH);
	if (::PathRemoveFileSpec(lstrcpy(&parent[0], path.c_str())) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("�t�H���_ '") << path << _T("' �̐e�f�B���N�g�����쐬�ł��܂���ł����B") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
	kjm::directory::createDirectory(&parent[0]);

	// �w�肳�ꂽ�f�B���N�g���̍쐬�����݂���B
	if (::CreateDirectory(path.c_str(), NULL) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("�t�H���_ '") << path << _T("' �̍쐬�Ɏ��s���܂����B") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
}

//=============================================================================
// �f�B���N�g�� ��\������N���X
//=============================================================================
//---------------------------------------------------------------------
// �f�B���N�g�������̃t�@�C���ꗗ���擾
//---------------------------------------------------------------------
std::vector<kjm::fileInfo> kjm::folderInfo::getFiles(const kjm::_tstring& strWC/* = _T("*.*")*/) const {
	std::vector<kjm::fileInfo> result;

	findFile ff;
	if (ff.findFirstFile(kjm::path::append(m_fullPath, strWC))) {
		do {
			if (!ff.isDirectory()) {
				WIN32_FIND_DATA findData;
				ff.copyWin32FindData(findData);

				/* for DEBUG
				kjm::_tstring tc = kjm::dateTime( findData.ftCreationTime , kjm::utc ).toLocalTime().toString();
				kjm::_tstring ta = kjm::dateTime( findData.ftLastAccessTime , kjm::utc ).toLocalTime().toString();
				kjm::_tstring tw = kjm::dateTime( findData.ftLastWriteTime , kjm::utc ).toLocalTime().toString();
				*/

				result.push_back(kjm::fileInfo(m_fullPath, findData));
			}
		} while (ff.findNextFile());
		ff.findClose();
	}

	return result;
}

//---------------------------------------------------------------------
// �f�B���N�g�������̃f�B���N�g���ꗗ���擾
//---------------------------------------------------------------------
std::vector<kjm::folderInfo> kjm::folderInfo::getSubFolders() const {
	std::vector<kjm::folderInfo> result;

	findFile ff;
	if (ff.findFirstFile(path::append(m_fullPath, _T("*.*")))) {
		do {
			if (ff.isDirectory() && !ff.isDots()) {
				WIN32_FIND_DATA findData;
				ff.copyWin32FindData(findData);
				result.push_back(kjm::folderInfo(m_fullPath, findData));
			}
		} while (ff.findNextFile());
		ff.findClose();
	}

	return result;
}

//=============================================================================
// �p�X������������N���X
//=============================================================================
//---------------------------------------------------------------------
// ������Ƀp�X����������
//---------------------------------------------------------------------
kjm::PathString& kjm::PathString::Append(const kjm::PathString& more) {
	std::vector<TCHAR> buf(MAX_PATH);
	
	::lstrcpy(&buf[0], m_path.c_str());
	::PathAppend(&buf[0], more.c_str());

	m_path = &buf[0];

	return *this;
}

//---------------------------------------------------------------------
// �g���q����菜��
//---------------------------------------------------------------------
kjm::PathString& kjm::PathString::removeExtension() {
	std::vector<TCHAR> buf(m_path.begin(), m_path.end());
	buf.push_back(_T('\0'));
	::PathRemoveExtension(&buf[0]);
	m_path = &buf[0];
	return *this;
}

//---------------------------------------------------------------------
// �g���q��ύX����
//---------------------------------------------------------------------
kjm::PathString& kjm::PathString::renameExtension(const kjm::_tstring& newExt) {
	m_path = kjm::util::renameExtension(m_path, newExt);
	return *this;
}

//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::PathString& kjm::PathString::operator= (const PathString& src) {
	if (this == &src)	return *this;	// ����I�u�W�F�N�g����`�F�b�N
	m_path = src.c_str();
	return *this;
}


//=============================================================================
// ���W�X�g���������N���X
//=============================================================================
//---------------------------------------------------------------------
// �R�s�[�R���X�g���N�^
//---------------------------------------------------------------------
kjm::registry::registry(const kjm::registry& src) {
	if (src.m_hkey == NULL) {
		this->m_hkey = NULL;
	} else {
		::DuplicateHandle(::GetCurrentProcess(), src.m_hkey, ::GetCurrentProcess(), (LPHANDLE)&this->m_hkey, 0, TRUE, DUPLICATE_SAME_ACCESS);
	}
}

//---------------------------------------------------------------------
// RegOpenKeyEx �̃��b�p�[
//---------------------------------------------------------------------
LONG kjm::registry::reg_open_key_ex(HKEY hkey, LPCTSTR lpSubKey, REGSAM samDesired) {
	assert(m_hkey == NULL);

	return ::RegOpenKeyEx(hkey, lpSubKey, 0, samDesired, &m_hkey);
}

//---------------------------------------------------------------------
// RegQueryValueEx �̃��b�p�[
//---------------------------------------------------------------------
LONG kjm::registry::reg_query_value_ex(LPTSTR lpValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) const {
	assert(m_hkey != NULL);

	return ::RegQueryValueEx(m_hkey, lpValueName, NULL, lpType, lpData, lpcbData);
}

//---------------------------------------------------------------------
// RegCloseKey �̃��b�p�[
//---------------------------------------------------------------------
LONG kjm::registry::reg_close_key() {
	LONG result = ERROR_SUCCESS;
	if (m_hkey != NULL) {
		result = ::RegCloseKey(m_hkey);
		m_hkey = NULL;
	}
	return result;
}

//---------------------------------------------------------------------
// ��������擾����֐�
//
// �l�̃^�C�v��
//   REG_SZ �̂Ƃ��́A���̂܂ܕԂ��B
//   REG_EXPAND_SZ �̂Ƃ��́AExpandEnvironmentStrings ��ʂ��Ă���Ԃ��B
//   REG_DWORD �̂Ƃ��́A_ultoa ��ʂ��Ă���Ԃ��B
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::registry::get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const {
	assert(lpDefault != NULL);

	std::basic_string<TCHAR> result(lpDefault);	// �f�t�H���g�l�Ŗ߂�l���쐬���Ă���

	// �l�ɕK�v�ȏ����擾
	DWORD dwType;
	DWORD cbData = 0;
	if (reg_query_value_ex(lpValueName, &dwType, NULL, &cbData) != ERROR_SUCCESS) {
		return result;	// �֐��Ɏ��s�����̂Ńf�t�H���g�l��߂��ďI��
	}

	// �f�[�^�̎�ނ����Ή��̂Ƃ��́A�f�t�H���g�l��߂��ďI��
	if (dwType != REG_SZ && dwType != REG_EXPAND_SZ && dwType != REG_DWORD) {
		return result;
	}

	// �o�b�t�@���m�ۂ��āA�f�[�^���擾
	std::vector<TCHAR> buf(cbData);
	if (reg_query_value_ex(lpValueName, &dwType, (LPBYTE)&buf[0], &cbData) != ERROR_SUCCESS) {
		return result;	// �֐��Ɏ��s�����̂Ńf�t�H���g�l��߂��ďI��
	}

	// �f�[�^�̎�ނɉ����āA�߂�l���쐬
	switch (dwType) {
	case REG_SZ:
		result = &buf[0];
		break;
		
	case REG_EXPAND_SZ:
		result = win32::expand_environment_strings(&buf[0]);
		break;

	case REG_DWORD:
		{
			TCHAR temp[32];	// u_long �̍ő�l���\���ɕ\���ł���T�C�Y
			result = _ultot(*(LPDWORD)&buf[0], temp, 10);
		}
		break;
	}

	return result;	// �擾�����������߂�
}

//---------------------------------------------------------------------
// RegEnumValue �̃��b�p�[
//---------------------------------------------------------------------
LONG kjm::registry::reg_enum_value(DWORD dwIndex, kjm::_tstring& valueName, DWORD& type, std::vector<BYTE>& data) {
	LONG ret;

	std::vector<TCHAR> buf1(64);
	std::vector<BYTE> buf2(64);
	do {
		DWORD cbBuf1 = buf1.size();
		DWORD cbBuf2 = buf2.size();
		ret = ::RegEnumValue(m_hkey, dwIndex, &buf1[0], &cbBuf1, NULL, &type, &buf2[0], &cbBuf2);
		if (ret == ERROR_SUCCESS) {
			break;
		} else if (ret == ERROR_NO_MORE_ITEMS) {
			break;
		} else if (ret == ERROR_MORE_DATA) {
			if (cbBuf1 == buf1.size()) buf1.resize(buf1.size() * 2);
			if (cbBuf2 == buf2.size()) buf2.resize(buf2.size() * 2);
		}
	} while (true);

	valueName = &buf1[0];
	data = buf2;

	return ret;
}

//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::registry& kjm::registry::operator =(const kjm::registry& rhs) {
	if (this == &rhs)
		return *this;

	if (rhs.m_hkey == NULL) {
		this->m_hkey = NULL;
	} else {
		::DuplicateHandle(::GetCurrentProcess(), rhs.m_hkey, ::GetCurrentProcess(), (LPHANDLE)&this->m_hkey, 0, TRUE, DUPLICATE_SAME_ACCESS);
	}

	return *this;
}

//---------------------------------------------------------------------
// ���W�X�g�����J��
//---------------------------------------------------------------------
LONG kjm::registry::RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired ) {

	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	RegCloseKey();

	LONG ret = ::RegOpenKeyEx( hKey, lpSubKey, ulOptions, samDesired, &m_hkey );

	return ret;
}


//---------------------------------------------------------------------
// ���W�X�g�������
//---------------------------------------------------------------------
LONG kjm::registry::RegCloseKey() {
	LONG ret = ERROR_SUCCESS;

	if ( m_hkey ) {
		ret = ::RegCloseKey( m_hkey );
		m_hkey = NULL;
	}

	return ret;
}


//---------------------------------------------------------------------
// ���W�X�g������l���擾
//---------------------------------------------------------------------
LONG kjm::registry::RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data ) {

	LONG ret;
	DWORD cbData = 0;

	// �K�v�ȃo�b�t�@�T�C�Y���擾
	ret = ::RegQueryValueEx( m_hkey, lpValueName, lpReserved, lpType, NULL, &cbData );
	if ( ret == ERROR_SUCCESS ) {

		// �o�b�t�@���m�ۂ��Đ����ɌĂяo��
		data.resize( cbData );
		ret = ::RegQueryValueEx( m_hkey, lpValueName, lpReserved, lpType, &data[0], &cbData );
	}

	return ret;
}

//=============================================================================
// �C�x���g���O�ɃA�N�Z�X���邽�߂̃N���X
//=============================================================================
//---------------------------------------------------------------------
// �C�x���g���O���o�b�N�A�b�v����
//---------------------------------------------------------------------
void kjm::event_log::backup(const kjm::_tstring& sourceName, const kjm::_tstring& backupFileName) {
	// �֐����쐬
	std::basic_ostringstream<TCHAR> ossfunc;
	ossfunc << _T("kjm::event_log::backup(\"") << sourceName << _T("\",\"") << backupFileName << _T("\")");

	// �C�x���g���O���J��
	HANDLE hEventLog = ::OpenEventLog(NULL, sourceName.c_str());
	if (hEventLog == NULL) {
		DWORD dwErr = ::GetLastError();
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}

	// �C�x���g���O���o�b�N�A�b�v
	DWORD dwErr = NOERROR;
	if (::BackupEventLog(hEventLog, backupFileName.c_str()) == FALSE) {
		dwErr = ::GetLastError();
	}
	::CloseEventLog(hEventLog);

	// �o�b�N�A�b�v���ɃG���[���������Ă����Ƃ��́A��O��Ԃ�
	if (dwErr != NOERROR) {
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}
}

//=============================================================================
// �v���Z�X�̋N���Ɋւ���N���X
//=============================================================================
//---------------------------------------------------------------------
// GetProcessWindow�֐��Ŏg�p����R�[���o�b�N
//
// [����]
// HWND hwnd		�񋓂��ꂽ�E�B���h�E�ւ̃n���h��
// LPARAM lParam	rkProcess�ւ̃|�C���^�B�L���X�g���Ďg��
//
// [���l]
// �񋓂��ꂽ�E�B���h�E�Ɠ����v���Z�XID�������̂��������ꍇ
// ���̃n���h���������o�[�ϐ��ɕۑ����A�񋓂��I������B
//---------------------------------------------------------------------
BOOL CALLBACK kjm::process::process_EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	kjm::process* p = reinterpret_cast<kjm::process*>(lParam);

	DWORD dwProcessId;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == p->m_pi.dwProcessId) {
		p->m_hwnd = hwnd;
		return FALSE;
	}

	return TRUE;
}

//---------------------------------------------------------------------
// �f�t�H���g�E�R���X�g���N�^
//---------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));
	memset(&m_szCommandLine, 0, sizeof(m_szCommandLine));
}

//---------------------------------------------------------------------
// �X�^�[�g�A�b�v���(wShowWindow)��ݒ肷��
//---------------------------------------------------------------------
void kjm::process::SetSIShowWindow(WORD newShowWindow) {
	m_si.wShowWindow = newShowWindow;
	m_si.dwFlags |= STARTF_USESHOWWINDOW;
}

//---------------------------------------------------------------------
// �v���Z�XID����v���Z�X���I�������郆�[�e�B���e�B�֐�
//---------------------------------------------------------------------
BOOL kjm::process::terminateProcess(DWORD dwProcessId, UINT uExitCode) {
	BOOL bRet = FALSE;
	kjm::process proc;
	if (proc.openProcess(PROCESS_TERMINATE, FALSE, dwProcessId)) {
		bRet = proc.kill(uExitCode);
	}
	return bRet;
}

//---------------------------------------------------------------------
// �X�^�[�g�A�b�v���(wShowWindow)���N���A(���g�p��)����
//---------------------------------------------------------------------
void kjm::process::ClearSIShowWindow() {
	m_si.wShowWindow = 0;
	m_si.dwFlags &= ~STARTF_USESHOWWINDOW;
}

//---------------------------------------------------------------------
// �v���Z�X�̎��s
//---------------------------------------------------------------------
BOOL kjm::process::createProcess(const std::basic_string<TCHAR>& commandLine) {
	assert(m_pi.hProcess == NULL);

	if (m_pi.hProcess)	return FALSE;	// ���łɉ������s���Ă���

	lstrcpy(m_szCommandLine, commandLine.c_str());
	m_si.cb = sizeof(m_si);

#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
#if defined(UNICODE)
		Winssa_Log("kjmlib", LOGLV_INFO, "<%s> kjm::process::createProcess: '%s' �����s���܂��B", kjm::util::toAnsi(kjm::util::GetModuleName()).c_str(), kjm::util::toAnsi(m_szCommandLine).c_str());
#else
		Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: '%s' �����s���܂��B"), kjm::util::GetModuleName().c_str(), m_szCommandLine);
#endif
	}
#endif

	BOOL bRet = CreateProcess(NULL, m_szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);

#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		if (bRet) {
#if defined(UNICODE)
			Winssa_Log("kjmlib", LOGLV_INFO, "<%s> kjm::process::createProcess: ���s���܂����B", kjm::util::toAnsi(kjm::util::GetModuleName()).c_str());
#else
			Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: ���s���܂����B"), kjm::util::GetModuleName().c_str());
#endif
		} else {
			DWORD dwErr = ::GetLastError();
#if defined(UNICODE)
			Winssa_Log("kjmlib", LOGLV_INFO, "<%s> kjm::process::createProcess: error %d", kjm::util::toAnsi(kjm::util::GetModuleName()).c_str(), dwErr);
#else
			Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: error %d"), kjm::util::GetModuleName().c_str(), dwErr);
#endif
		}
	}
#endif
	return bRet;
}

//---------------------------------------------------------------------
// �v���Z�X�n���h�������
//
// [����]
// �v���Z�X�n���h�������B�v���Z�X�̏I���ł͂Ȃ��̂Œ��ӁB
// �v���Z�X�̏I���́A�v���Z�X�Ԃ̎�茈�߂ɂ��B
//---------------------------------------------------------------------
void kjm::process::Close() {
	if (m_pi.hThread)	CloseHandle(m_pi.hThread);
	if (m_pi.hProcess)	CloseHandle(m_pi.hProcess);
	memset(&m_pi, 0, sizeof(m_pi));
}

//---------------------------------------------------------------------
// �v���Z�X�̏I���R�[�h���擾����
//---------------------------------------------------------------------
DWORD kjm::process::getExitCode() {
	DWORD exitCode = 0;
	::GetExitCodeProcess(this->m_pi.hProcess, &exitCode);
	return exitCode;
}

//---------------------------------------------------------------------
// �v���Z�X�Ɋ֘A�t����ꂽ�E�B���h�E��Ԃ�
//---------------------------------------------------------------------
HWND kjm::process::GetProcessWindow() {
	assert(m_pi.hProcess);

	if (!m_pi.hProcess)	return NULL;

	m_hwnd = NULL;
	EnumWindows(kjm::process::process_EnumWindowsProc, reinterpret_cast<LPARAM>(this));
	return m_hwnd;
}

//=============================================================================
// �R�}���h���C����͂ɓn���I�v�V�������\����
//=============================================================================
//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::optionInfo& kjm::optionInfo::operator=(const kjm::optionInfo& rhs) {
	if (this == &rhs) return *this;
	m_longOption = rhs.m_longOption;
	m_shortOption = rhs.m_shortOption;
	m_optionArgInfo = rhs.m_optionArgInfo;
	m_optDescription = rhs.m_optDescription;
	m_argDescription = rhs.m_argDescription;
	return *this;
}

//=============================================================================
// �R�}���h���C����̓N���X
//=============================================================================
//---------------------------------------------------------------------
// �R�}���h���C�����
//---------------------------------------------------------------------
int kjm::cmdLine::parse(int argc, TCHAR** argv, const optionInfoList& opts) {

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];	// �I�v�V��������

			if (c == '-') {	// �����O�I�v�V����
				// ��v���郍���O�I�v�V����������
				size_t j;
				for (j = 0; j < opts.size(); j++) {
					if (_tcsncicmp(opts[j].m_longOption.c_str(), &argv[i][2], opts[j].m_longOption.length()) == 0) {
						break;
					}
				}
				if (j < opts.size()) {
					// ��v����
					const kjm::optionInfo& oi = opts[j];

					if (oi.m_optionArgInfo == kjm::no_argument) {
						// �I�v�V�����ɂ͈����������ꍇ
						this->m_options[oi.m_longOption] = _T("");
					} else if (oi.m_optionArgInfo == kjm::required_argument) {
						if (argv[i][2 + oi.m_longOption.length()] == '=') {
							// �I�v�V�����̈����������Ă���ꍇ
							this->m_options[oi.m_longOption] = &argv[i][2 + oi.m_longOption.length() + 1];
						} else {
							// �ǂ����悤�E�E�E
							assert(0);
						}
					} else {
						if (argv[i][2 + oi.m_longOption.length()] == '=') {
							// �I�v�V�����̈����������Ă���ꍇ
							this->m_options[oi.m_longOption] = &argv[i][2 + oi.m_longOption.length() + 1];
						} else {
							// �I�v�V�����ɂ͈����������ꍇ
							this->m_options[oi.m_longOption] = _T("");
						}
					}
				}
			} else {
				kjm::optionInfo oi(std::basic_string<TCHAR>(1, c), std::basic_string<TCHAR>(1, c), kjm::optional_argument);
	#if _MSC_VER <= 1200	// VC++6�ȉ�
				// �����̂��߂����̊֐��I�u�W�F�N�g���K�v�ɂȂ��Ă��܂��B
				// �{���̌���d�l�ł́A���̂悤�ȃN���X�͕s�v�B
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(), optionInfoShortNameFinder(std::basic_string<TCHAR>(1, c)));
	#elif _MSC_VER <= 1500	// VC++2008
				// VC++2008�ł́Abind2nd�Ɏw�肷��֐��́A�������Q�ƂɂȂ��Ă��Ă͂����Ȃ����߁A
				// �|�C���^��n���֐�����ʂɍ쐬���Ďg�p���Ă���B
				// �{���̌���d�l�ł́A���̂悤�ȃN���X�͕s�v�B
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(),
					bind2nd(mem_fun_ref(&kjm::optionInfo::isSameShortOption_P), &std::basic_string<TCHAR>(1, c)));
	#else	// VC++2010�ȍ~
				optionInfoList::const_iterator ci = 
					std::find_if(opts.begin(), opts.end(),
					bind2nd(mem_fun_ref(&kjm::optionInfo::isSameShortOption), std::basic_string<TCHAR>(1, c)));
	#endif
				if (ci != opts.end()) {
					oi = *ci;
				}

				if (oi.m_optionArgInfo == kjm::no_argument) {
					// �I�v�V�����ɂ͈����������ꍇ
					this->m_options[oi.m_longOption] = _T("");
				} else if (oi.m_optionArgInfo == kjm::required_argument) {
					if (argv[i][2] != 0) {
						// �I�v�V�����̈����������Ă���ꍇ
						this->m_options[oi.m_longOption] = &argv[i][2];
					} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
						// �I�v�V�����̈��������� argv �ɑ����Ă���ꍇ
						this->m_options[oi.m_longOption] = argv[++i];
					} else {
						// �ǂ����悤�E�E�E
						assert(0);
					}
				} else {
					if (argv[i][2] != 0) {
						// �I�v�V�����̈����������Ă���ꍇ
						this->m_options[oi.m_longOption] = &argv[i][2];
					} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
						// �I�v�V�����̈��������� argv �ɑ����Ă���ꍇ
						this->m_options[oi.m_longOption] = argv[++i];
					} else {
						// �I�v�V�����ɂ͈����������ꍇ
						this->m_options[oi.m_longOption] = _T("");
					}
				}
			}
		} else {
			this->m_arguments.push_back(argv[i]);
		}
	}
	return 0;
}

//---------------------------------------------------------------------
// �R�}���h���C�����
//---------------------------------------------------------------------
int kjm::cmdLine::chkarg(int argc, TCHAR **argv, const kjm::_tstring &shortOpts) {
	kjm::optionInfoList opts;

	// �V���[�g�I�v�V������`�����񂩂� optionInfoList �𐶐�����
	for (kjm::_tstring::size_type i = 0; i < shortOpts.length(); i++) {
		int optArg = no_argument;
		TCHAR c = shortOpts[i];
		if (((i + 1) < shortOpts.length()) && (shortOpts[i + 1] == _T(':'))) {
			optArg = required_argument;
		}

		opts.push_back(kjm::optionInfo(_T(""), kjm::_tstring(1, c), optArg));
	}

	return this->parse(argc, argv, opts);
}

//=============================================================================
// �R�}���h���C����̓N���X
//=============================================================================
//---------------------------------------------------------------------
// �R�}���h���C���������
//
// @arg  hasArgShortOpts  �����������Ƃ�����V���[�g�I�v�V������񋓂���������ւ̃|�C���^
//---------------------------------------------------------------------
int kjm::commandLine::parse(int argc, TCHAR** argv, TCHAR* hasArgShortOpts) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '/' || argv[i][0] == '-') {
			// �I�v�V�����Ƃ��ď�������
			if (argv[i][1] == '/' || argv[i][1] == '-') {
				// �����O�I�v�V�����Ƃ��ď�������

				// = ��T���B����΁A���������I�v�V����
				TCHAR* p = std::find(&argv[i][2], &argv[i][lstrlen(argv[i])], '=');
				if (p != &argv[i][lstrlen(argv[i])]) {
					m_optList[_tstring(&argv[i][2], p)] = (p + 1);
				} else {
					m_optList[&argv[i][2]] = _T("");
				}
			} else {
				// �V���[�g�I�v�V�����Ƃ��ď�������

				// ���������I�v�V�������ǂ������ׂ�
				if (std::find(&hasArgShortOpts[0], &hasArgShortOpts[lstrlen(hasArgShortOpts)], argv[i][1]) != &hasArgShortOpts[lstrlen(hasArgShortOpts)]) {
					// (�����)���������o��
					if (argv[i][2] != '\0') {
						// �I�v�V�����ɑ��������񂪂���̂ŁA������A�I�v�V�����̈����Ƃ��č̗p����
						m_optList[_tstring(1, argv[i][1])] = &argv[i][2];
					} else if (((i + 1) < argc) && (argv[i + 1][0] != '-' && argv[i + 1][0] != '/')) {
						// �I�v�V�����̎��̃I�v�V�������I�v�V����������Ŏn�܂��Ă��Ȃ��Ƃ��A
						// ������A�I�v�V�����̈����Ƃ��č̗p����
						m_optList[_tstring(1, argv[i][1])] = argv[i + 1];
						++i;
					} else {
						// �I�v�V�����̈����͖�������
						m_optList[_tstring(1, argv[i][1])] = _T("");
					}
				} else {
					m_optList[_tstring(1, argv[i][1])] = _T("");
				}
			}
		} else {
			// �I�v�V�����ȊO�̈����Ƃ��ď�������
			m_argList.push_back(argv[i]);
		}
	}

	return 0;
}

//=============================================================================
// OpenEventLog/CloseEventLog��΂ɂ���N���X
//=============================================================================
//---------------------------------------------------------------------
// �C�x���g���O���J��
//---------------------------------------------------------------------
BOOL kjm::eventLog::openEventLog(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName) {
	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	closeEventLog();

	m_uncServerName = (lpUNCServerName) ? lpUNCServerName : _T("");
	m_sourceName = (lpSourceName) ? lpSourceName : _T("");

	m_hEventLog = ::OpenEventLog(lpUNCServerName, lpSourceName);
	
	return (m_hEventLog) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// �ۑ������C�x���g���O���J��
//---------------------------------------------------------------------
BOOL kjm::eventLog::OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName ) {

	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	closeEventLog();

	m_uncServerName = ( lpUNCServerName ) ? lpUNCServerName : _T("");
	m_sourceName = ( lpSourceName ) ? lpSourceName : _T("");
	m_fileName = ( lpFileName ) ? lpFileName : _T("");

	m_hEventLog = ::OpenBackupEventLog( lpUNCServerName, lpFileName );
	return ( m_hEventLog ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// �C�x���g���O�����
//---------------------------------------------------------------------
BOOL kjm::eventLog::closeEventLog() {
	BOOL ret = TRUE;
	if (m_hEventLog) {
		ret = ::CloseEventLog(m_hEventLog);
		m_hEventLog = NULL;
	}
	return ret;
}


//---------------------------------------------------------------------
// �C�x���g���O��ǂ�
//---------------------------------------------------------------------
BOOL kjm::eventLog::readEventLog(DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventLogRecord& record) {

	DWORD dwReadBytes = 0;
	DWORD dwNextSize = 0;
	std::vector<BYTE> buffer;

	// �ɏ��̃o�b�t�@���m�ۂ��ăT�C�Y�����炤
	buffer.resize( 1 );

	BOOL bResult = ::ReadEventLog(
		m_hEventLog,
		dwReadFlags,
		dwRecordOffset,
		&buffer[0],
		buffer.size(),
		&dwReadBytes,
		&dwNextSize);

	// �o�b�t�@�s���G���[�ȊO�́A�G���[�I��
	if(!bResult && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		return bResult;
	}

	// �o�b�t�@�̍Ċ��蓖��
	buffer.resize( dwNextSize );

	/* �C�x���g���O�̓ǂݎ�� */
	bResult = ::ReadEventLog(
		m_hEventLog,
		dwReadFlags,
		dwRecordOffset,
		&buffer[0],
		buffer.size(),
		&dwReadBytes,
		&dwNextSize);

	record = kjm::eventLogRecord(m_sourceName.c_str(), (EVENTLOGRECORD*)&buffer[0]);

	return bResult;
}

//---------------------------------------------------------------------
// �C�x���g���O���o�b�N�A�b�v����
//---------------------------------------------------------------------
void kjm::eventLog::backup(LPCTSTR sourceName, LPCTSTR backupFileName) {
	// �֐����쐬
	std::basic_ostringstream<TCHAR> ossfunc;
	ossfunc << _T("kjm::event_log::backup(\"") << sourceName << _T("\",\"") << backupFileName << _T("\")");

	// �C�x���g���O���J��
	HANDLE hEventLog = ::OpenEventLog(NULL, sourceName);
	if (hEventLog == NULL) {
		DWORD dwErr = ::GetLastError();
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}

	// �C�x���g���O���o�b�N�A�b�v
	DWORD dwErr = NOERROR;
	if (::BackupEventLog(hEventLog, backupFileName) == FALSE) {
		dwErr = ::GetLastError();
	}
	::CloseEventLog(hEventLog);

	// �o�b�N�A�b�v���ɃG���[���������Ă����Ƃ��́A��O��Ԃ�
	if (dwErr != NOERROR) {
		ossfunc << _T(" at ") << __FILE__ << _T("(") << __LINE__ << _T("): ") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::kjmException(dwErr, ossfunc.str());
	}
}

//---------------------------------------------------------------------
// 1���R�[�h����eventlog������킷�N���X
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------
kjm::eventLogRecord& kjm::eventLogRecord::operator= ( const kjm::eventLogRecord& src ) {
	m_source_name = src.m_source_name;
	m_raw_record = src.m_raw_record;

	return *this;
}


//---------------------------------------------------------------------
// �C�x���g���O�̐��������ɂ���R���X�g���N�^
//---------------------------------------------------------------------
kjm::eventLogRecord::eventLogRecord(LPCTSTR lpSourceName, const EVENTLOGRECORD* src) : m_source_name(lpSourceName) {
	// �����̃R�s�[
	m_raw_record.resize(src->Length);
	memmove(&m_raw_record[0], src, src->Length);
}

//---------------------------------------------------------------------
// �����ݕ�����̎擾
//---------------------------------------------------------------------
int kjm::eventLogRecord::GetArgs(const EVENTLOGRECORD *pBuf, std::vector<std::basic_string<TCHAR> >& strings) {
	if(pBuf->NumStrings == 0) {
		return strings.size();
	}

	// �������X�g���擾
	LPTSTR cp = (LPTSTR)((LPBYTE)pBuf + pBuf->StringOffset);

	for (int i = 0; i < pBuf->NumStrings; i++) {
		strings.push_back(cp);
		cp += lstrlen(cp) + 1;
	}

	return strings.size();
}

//---------------------------------------------------------------------
// �\�[�X�����烂�W���[�������擾
//---------------------------------------------------------------------
BOOL kjm::eventLogRecord::GetModuleNameFromSourceName(LPCTSTR SourceName, LPCTSTR EntryName, std::vector<TCHAR>& ExpandedName) {
	DWORD lResult;
	std::vector<BYTE> ModuleName;
	BOOL bReturn = FALSE;

	// Application���O�p�̃��W�X�g���L�[���I�[�v��
	kjm::registry appKey;
	
	std::basic_string<TCHAR> key = std::basic_string<TCHAR>(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\"));
	key += m_source_name;

	lResult = appKey.RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		key.c_str(),
		0,
		KEY_READ);

	if ( lResult == ERROR_SUCCESS ) {

		// �\�[�X�̏�񂪊i�[����Ă��郌�W�X�g�����I�[�v��

		kjm::registry sourceKey;

		lResult = sourceKey.RegOpenKeyEx(
			appKey.getKey(),
			SourceName,
			0,
			KEY_READ);

		if ( lResult == ERROR_SUCCESS ) {

			// �\�[�X���W���[�������擾
			lResult = sourceKey.RegQueryValueEx(
				_T("EventMessageFile"),
				NULL,
				NULL,
				ModuleName);

			if ( lResult == ERROR_SUCCESS ) {

				// ���ϐ���W�J
				kjm::util::expandEnvironmentStrings( (LPCTSTR)&ModuleName[0], ExpandedName );

				/* ����I�� */
				bReturn = TRUE;
			}
		}
	}

	return bReturn;
}

//---------------------------------------------------------------------
// �\�����b�Z�[�W�̍쐬
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::eventLogRecord::DispMessage(
	const std::basic_string<TCHAR>& SourceName, LPCTSTR EntryName, const std::vector<std::basic_string<TCHAR> >& strings, DWORD MessageId) {

	BOOL bResult;
	BOOL bReturn = FALSE;
	std::vector<TCHAR> SourceModuleName;
	std::vector<TCHAR> message;
	TCHAR* oneModuleName;
	DWORD dwRet;

	/* �\�[�X���W���[�������擾 */	
	bResult = GetModuleNameFromSourceName(SourceName.c_str(), EntryName, SourceModuleName);
	if(!bResult) return std::basic_string<TCHAR>(_T(""));

	oneModuleName = _tcstok( &SourceModuleName[0], _T(";") );
	while ( oneModuleName ) {

		kjm::library sourceModule;

		// �\�[�X���W���[�������[�h
		sourceModule.LoadLibraryEx(
			oneModuleName,
			NULL,
			DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

		if ( sourceModule.getInstance() ) {

			std::vector<LPCTSTR> Args;
			for (size_t i = 0; i < strings.size(); i++) {
				Args.push_back(strings[i].c_str());
			}

			/* ���b�Z�[�W���쐬 */
			dwRet = kjm::util::formatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
				sourceModule.getInstance() ,
				MessageId,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				message,
				(va_list *)(Args.empty() ? NULL : &Args[0]));


			/* ����I�� */
			if ( dwRet != 0 ) {
				bReturn = TRUE;
				break;
			}
		}

		oneModuleName = _tcstok( NULL, _T(";") );
	}

	if(message.size() > 0) {
		return std::basic_string<TCHAR>(&message[0]);
	}

	return _T("");
}

//---------------------------------------------------------------------
// �C�x���g���O�ɋL�^���ꂽ��������Ƃ肾��
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::eventLogRecord::EventIDText() {
	std::vector<std::basic_string<TCHAR> > strings;
	GetArgs(getRaw(), strings);

	return DispMessage(get_SourceName(), _T("EventMessageFile"), strings, get_EventID());
}

//---------------------------------------------------------------------
// �C�x���g�ɋL�^���ꂽ�R���s���[�^�����擾
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::eventLogRecord::ComputerName() {
	LPCTSTR cp = (LPCTSTR)(&m_raw_record[0] + sizeof(EVENTLOGRECORD));	// SourceName �̐擪�Ɉړ�
	return cp + (lstrlen(cp) + 1);	// SourceName �̎��̕����񂪃R���s���[�^��
}

std::vector<BYTE> kjm::eventLogRecord::Data() {

	std::vector<BYTE> data( DataLength() );

	for ( size_t i = 0; i < data.size(); i++) {
		data.at( i ) = *(((unsigned char *)getRaw()) + DataOffset() + i);
	}

	return data;
}


//---------------------------------------------------------------------
// LoadLibraryEx/FreeLibrary��΂ɂ���N���X
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ���C�u�����̃��[�h
//---------------------------------------------------------------------
BOOL kjm::library::LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags ) {
	
	// ���łɃ��[�h���Ă��邩������Ȃ��̂ŁA�J��
	FreeLibrary();

	m_hLibModule = ::LoadLibraryEx( lpLibFileName, hFile, dwFlags );
	
	return ( m_hLibModule ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// ���C�u�����̊J��
//---------------------------------------------------------------------
BOOL kjm::library::FreeLibrary() {
	BOOL ret = TRUE;

	if ( m_hLibModule ) {
		ret = ::FreeLibrary( m_hLibModule );
		m_hLibModule = NULL;
	}

	return ret;
}


//=============================================================================
// ���Ԃ��ȒP�Ɉ����N���X
//=============================================================================
//---------------------------------------------------------------------
//
//---------------------------------------------------------------------
std::basic_string<TCHAR> kjm::time::strftime(LPCTSTR format) {
	TCHAR buf[ 512 ];
	::_tcsftime( buf, sizeof( buf ), format, localtime( &m_time ) );
	return std::basic_string<TCHAR>( buf );
}

//---------------------------------------------------------------------
// ������Ŏ��Ԃ��w�肷��R���X�g���N�^
//---------------------------------------------------------------------
void kjm::time::parse(LPCTSTR time_text) {

	int y, m, d, h, n, s;
	int ret = _stscanf(time_text, _T("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &n, &s );

	tm t;
	memset( &t, 0, sizeof( t ) );

	t.tm_year = y - 1900;
	t.tm_mon = m - 1;
	t.tm_mday = d;
	t.tm_hour = h;
	t.tm_min = n;
	t.tm_sec = s;

	m_time = mktime( &t );
}

//---------------------------------------------------------------------
// �b�̉����Z
//---------------------------------------------------------------------
kjm::time& kjm::time::add_second( int sec ) {

	tm tm_work = *localtime( &m_time );

	tm_work.tm_sec += sec;

	m_time = mktime( &tm_work );

	return *this;
}

//---------------------------------------------------------------------
// kjm::time�N���X���t�@�C���ɕۑ�
//---------------------------------------------------------------------
bool kjm::time::save(FILE* fp) {
	return (fwrite(&m_time, sizeof(m_time), 1, fp) == 1);
}

//---------------------------------------------------------------------
// kjm::time�N���X���t�@�C������Ǎ�
//---------------------------------------------------------------------
bool kjm::time::load(FILE* fp) {
	return (fread(&m_time, sizeof(m_time), 1, fp) == 1);
}


//=============================================================================
// �����������N���X
//=============================================================================
//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::timeSpan& kjm::timeSpan::operator =(const kjm::timeSpan &rhs) {
	if (this == &rhs) return *this;
	m_span = rhs.m_span;
	return *this;
}


//=============================================================================
// class dateTime ���Ԃ��ȒP�Ɉ����N���X
//=============================================================================
//---------------------------------------------------------------------
// ���t�𑫂��� dateTime ��Ԃ�
//---------------------------------------------------------------------
kjm::dateTime kjm::dateTime::addDays(int nDays) const {
	LONGLONG ll = get_Value() + (kjm::timeSpan::TicksPerDay * nDays);
	return dateTime(*(FILETIME*)&ll);
}

//---------------------------------------------------------------------
// ���Ԃ𕶎���(YYYYMMDD)�ɕϊ�
//---------------------------------------------------------------------
kjm::_tstring kjm::dateTime::toString_yyyymmdd() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&m_ft, &st);

	_otstringstream oss;
	oss.imbue(std::locale::classic());
	oss << std::setfill(_T('0'))
		<< st.wYear
		<< std::setw(2) << st.wMonth
		<< std::setw(2) << st.wDay;
	return oss.str();
}

//---------------------------------------------------------------------
// ���t���t�H�[�}�b�g���ďo��
//---------------------------------------------------------------------
kjm::_tstring kjm::dateTime::toLongDateString() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&this->m_ft, &st);
	return kjm::util::sprintf_str(_T("%d�N%d��%d��"), st.wYear, st.wMonth, st.wDay);
}

//---------------------------------------------------------------------
// ���Ԃ��t�H�[�}�b�g���ďo��
//---------------------------------------------------------------------
kjm::_tstring kjm::dateTime::toLongTimeString() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&this->m_ft, &st);
	return kjm::util::sprintf_str(_T("%02d:%02d:%02d"), st.wHour, st.wMinute, st.wSecond);
}

//---------------------------------------------------------------------
// �������t�H�[�}�b�g���ďo��
//---------------------------------------------------------------------
kjm::_tstring kjm::dateTime::toString(const _tstring& format) const {
	time_t t = (time_t)*this;
	tm tmBuf = *localtime(&t);
	TCHAR buf[96];
	_tcsftime(buf, 96, format.c_str(), &tmBuf);
	return buf;
}

//---------------------------------------------------------------------
// ������ɕϊ�
//---------------------------------------------------------------------
kjm::_tstring kjm::dateTime::toString() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&m_ft, &st);

	_otstringstream oss;

	// std::locale::global(std::locale("japanese")) �őS�̂���{��ɂ��Ă���ƁA
	// 2,010/10/15 �݂����ȕ����񂪏o���オ���Ă��܂��B
	// ������m���ɉ�����邽�߂ɁA***.imbue(std::locale("C")) ���Ă���B

	oss.imbue(std::locale("C"));
	oss << std::setfill(_T('0'))
		<< st.wYear << _T("/")
		<< std::setw(2) << st.wMonth << _T("/") 
		<< std::setw(2) << st.wDay << _T(" ") 
		<< std::setw(2) << st.wHour << _T(":") 
		<< std::setw(2) << st.wMinute << _T(":") 
		<< std::setw(2) << st.wSecond;
	STL_SETLOCALE_JAPAN;

	return oss.str();
}

//---------------------------------------------------------------------
// ������̓����Ɠ����� dateTime �I�u�W�F�N�g�𐶐�
//---------------------------------------------------------------------
kjm::dateTime kjm::dateTime::parse(const kjm::_tstring& text) {
	std::vector<TCHAR> work(text.begin(), text.end());
	work.push_back('\0');

	LPTSTR py = _tcstok(&work[0], _T("/-"));
	LPTSTR pm = _tcstok(NULL, _T("/-"));
	LPTSTR pd = _tcstok(NULL, _T(" "));
	LPTSTR ph = _tcstok(NULL, _T(":"));
	LPTSTR pn = _tcstok(NULL, _T(":"));
	LPTSTR ps = _tcstok(NULL, _T(""));

	if (!py || !pm || !pd || !ph || !pn || !ps) {
		throw kjm::formatException();
	}

	int y = _tcstol(py, NULL, 10);
	int m = _tcstol(pm, NULL, 10);
	int d = _tcstol(pd, NULL, 10);
	int h = _tcstol(ph, NULL, 10);
	int n = _tcstol(pn, NULL, 10);
	int s = _tcstol(ps, NULL, 10);

	SYSTEMTIME st = {0};
	st.wYear = y;
	st.wMonth = m;
	st.wDay = d;
	st.wHour = h;
	st.wMinute = n;
	st.wSecond = s;

	FILETIME ft;
	if (SystemTimeToFileTime(&st, &ft) == FALSE) {
		DWORD dwErr = ::GetLastError();
		throw kjm::formatException();
	}


	return kjm::dateTime(ft);
}

//---------------------------------------------------------------------
// �����������ɕϊ����Ă݂�
//---------------------------------------------------------------------
bool kjm::dateTime::tryParse(const kjm::_tstring& text, kjm::dateTime& result) {
	bool ret = false;
	try {
		result = kjm::dateTime::parse(text);
		ret = true;
	} catch (kjm::exception ex) {
		ret = false;
	}
	return ret;
}

//=============================================================================
// ���ϐ��������N���X
//=============================================================================
//---------------------------------------------------------------------
// �w�肵��������ɖ��ߍ��܂�Ă���e���ϐ��̖��O���A���̕ϐ��̒l��\��������
// �Œu�����A�u����̕�����S�̂�Ԃ��܂��B
//---------------------------------------------------------------------
_tstring kjm::environment::expandEnvironmentVariables(const _tstring& name) {
	std::vector<TCHAR> work(MAX_PATH);
	kjm::util::expandEnvironmentStrings(name.c_str(), work);
	return &work[0];
}

//=============================================================================
// C����� fopen �����S�Ɏg���N���X
//=============================================================================
//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kjm::cfile::~cfile() {
	this->close();
}

//---------------------------------------------------------------------
// fopen�̑���
//---------------------------------------------------------------------
bool kjm::cfile::open(const kjm::_tstring& fname, const kjm::_tstring& mode) {
	if (m_fp != NULL) {
		this->close();
	}

	m_fp = _tfopen(fname.c_str(), mode.c_str());
	
	return (m_fp != NULL);
}

//---------------------------------------------------------------------
// fclose�̑���
//---------------------------------------------------------------------
bool kjm::cfile::close() {
	bool result = true;

	if (m_fp) {
		result = (fclose(m_fp) == 0);
		m_fp = NULL;
	}

	return result;
}

//=============================================================================
// �t�@�C���������N���X
//=============================================================================
//---------------------------------------------------------------------
// �t�@�C�����폜����
//---------------------------------------------------------------------
void kjm::file::deleteFile(const _tstring& path, bool force) {
	// �u�����폜�v���w�肳��Ă���Ƃ��́A�Ƃ肠�����A�ǂݍ��ݐ�p�����𗎂Ƃ�
	if (force) {
		kjm::file::setAttributes(path, (kjm::file::getAttributes(path) & ~FILE_ATTRIBUTE_READONLY));
	}

	// ���ۂɃt�@�C�����폜����
	if (::DeleteFile(path.c_str()) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("�t�@�C�� '") << path << _T("' ���폜�ł��܂���B") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
}

//---------------------------------------------------------------------
// �ŏI�X�V�������擾(UTC)
//---------------------------------------------------------------------
kjm::dateTime kjm::file::getLastWriteTimeUtc(const kjm::_tstring &path) {
	HANDLE hFile = ::CreateFile(path.c_str(), 0, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return kjm::dateTime();
	}

	FILETIME ft;
	::GetFileTime(hFile, NULL, NULL, &ft);
	::CloseHandle(hFile);
	return kjm::dateTime(ft, kjm::utc);
}

//---------------------------------------------------------------------
// �t�@�C���̑������擾����B
//---------------------------------------------------------------------
DWORD kjm::file::getAttributes(const kjm::_tstring& path) {
	DWORD dwAttr = ::GetFileAttributes(path.c_str());
	if (dwAttr == INVALID_FILE_ATTRIBUTES) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("�t�@�C�� '") << path << _T("' �̑������擾�ł��܂���B") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}

	return dwAttr;
}

//---------------------------------------------------------------------
// �t�@�C���̃T�C�Y���擾����
//---------------------------------------------------------------------
__int64 kjm::file::getFileSize(HANDLE hFile) {
	LARGE_INTEGER fileSize;
	if (GetFileSizeEx(hFile, &fileSize) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("�t�@�C���T�C�Y�̎擾�Ɏ��s���܂����B") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
	return fileSize.QuadPart;
}

//---------------------------------------------------------------------
// �t�@�C���̑�����ݒ肷��B
//---------------------------------------------------------------------
void kjm::file::setAttributes(const kjm::_tstring& path, DWORD fa) {
	if (::SetFileAttributes(path.c_str(), fa) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("�t�@�C�� '") << path << _T("' �̑�����ݒ�ł��܂���B") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
}

//=============================================================================
// �t�@�C���������N���X
//=============================================================================
//---------------------------------------------------------------------
// �R�s�[�R���X�g���N�^�Ƒ�����Z�q�Ŏg�������o�[�R�s�[�֐�
//---------------------------------------------------------------------
void kjm::fileInfo::copy_from_fileInfo(const kjm::fileInfo& src) {
	m_fullPath = src.m_fullPath;
	m_creationTime = src.m_creationTime;
	m_lastAccessTime = src.m_lastAccessTime;
	m_lastWriteTime = src.m_lastWriteTime;
	m_fileSize = src.m_fileSize;
	m_dwFileAttributes = src.m_dwFileAttributes;
	m_hasInfo = src.m_hasInfo;
}

//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------
kjm::fileInfo::fileInfo() {
	m_fileSize = 0;
	m_dwFileAttributes = 0;
	m_hasInfo = false;
}

//---------------------------------------------------------------------
// �R�s�[�R���X�g���N�^
//---------------------------------------------------------------------
kjm::fileInfo::fileInfo(const kjm::fileInfo& src) {
	this->copy_from_fileInfo(src);
}

//---------------------------------------------------------------------
// WIN32_FIND_DATA �ŏ���������R���X�g���N�^
//---------------------------------------------------------------------
kjm::fileInfo::fileInfo(const _tstring& strParentPath, const WIN32_FIND_DATA& rFindData) {
	m_fullPath = path::append(strParentPath, rFindData.cFileName);

	m_creationTime = dateTime(rFindData.ftCreationTime, kjm::utc);
	m_lastAccessTime = dateTime(rFindData.ftLastAccessTime, kjm::utc);
	m_lastWriteTime = dateTime(rFindData.ftLastWriteTime, kjm::utc);

	LARGE_INTEGER li = {rFindData.nFileSizeLow, rFindData.nFileSizeHigh};
	m_fileSize = li.QuadPart;

	m_dwFileAttributes = rFindData.dwFileAttributes;

	m_hasInfo = true;	// [���������Ă���]�ŁA������
}

//---------------------------------------------------------------------
// �t�@�C�����ŏ���������R���X�g���N�^
//---------------------------------------------------------------------
kjm::fileInfo::fileInfo(const _tstring& strName) {
	m_fullPath = strName;

	findFile ff;
	if (ff.findFirstFile(strName)) {
		m_creationTime = ff.getCreationTime();
		m_lastAccessTime = ff.getLastAccessTime();
		m_lastWriteTime = ff.getLastWriteTime();

		m_fileSize = ff.getFileSize();
		m_dwFileAttributes = ff.getFileAttributes();

		m_hasInfo = true;	// [���������Ă���]�ŁA������
	} else {
		m_fileSize = 0;
		m_dwFileAttributes = 0;
		m_hasInfo = false;	// [���������Ă��Ȃ�]�ŁA������
	}
}

//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::fileInfo& kjm::fileInfo::operator =(const kjm::fileInfo& rhs) {
	if (this != &rhs) {
		copy_from_fileInfo(rhs);
	}
	return *this;
}

//---------------------------------------------------------------------
// �t�@�C���̑����������Ԃ�
//---------------------------------------------------------------------
kjm::_tstring kjm::fileInfo::get_AttribText(const kjm::_tstring& format/*=_T("ASHR")*/) const {
	_otstringstream result;
	for (kjm::_tstring::const_iterator it = format.begin(); it != format.end(); ++it) {
		if ((*it == 'A') || (*it == 'a')) {
			result << (isArchive() ? 'A' : ' ');
		} else if ((*it == 'R') || (*it == 'r')) {
			result << (isReadonly() ? 'R' : ' ');
		} else if ((*it == 'S') || (*it == 's')) {
			result << (isSystem() ? 'S' : ' ');
		} else if ((*it == 'H') || (*it == 'h')) {
			result << (isHidden() ? 'H' : ' ');
		} else {
			result << '?';
		}
	}
	return result.str();
}

//=============================================================================
// ini�t�@�C������������N���X
//=============================================================================
//---------------------------------------------------------------------
// �Z�N�V�����f�[�^���擾
//---------------------------------------------------------------------
std::vector<TCHAR> kjm::iniFile::getSectionData(const kjm::_tstring& section) const {
	std::vector<TCHAR> result(512);

	while (::GetPrivateProfileSection(section.c_str(), &result[0], result.size(), m_fileName.c_str()) == (result.size() - 2)) {
		result.resize(result.size() * 2);
	}

	return result;
}

//---------------------------------------------------------------------
// �Z�N�V�����f�[�^����������
//---------------------------------------------------------------------
BOOL kjm::iniFile::writeSectionData(const kjm::_tstring& section, const std::vector<TCHAR>& sectionData) {
	return ::WritePrivateProfileSection(section.c_str(), &sectionData[0], m_fileName.c_str());
}

//---------------------------------------------------------------------
// �Z�N�V�����ꗗ���擾
//---------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::iniFile::getSectionNames() const {
	assert(!m_fileName.empty());

	std::vector<std::basic_string<TCHAR> > result;
	std::vector<TCHAR> work;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileSectionNames(&work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 2);

	std::vector<TCHAR>::iterator sp = work.begin();
	std::vector<TCHAR>::iterator ep;
	while ((ep = std::find(sp, work.end(), '\0')) != sp) {
		result.push_back(std::basic_string<TCHAR>(sp, ep));
		sp = ep + 1;
	}

	return result;
}


//-------------------------------------------------------------------------------
// �Z�N�V�������̃L�[�ꗗ���擾
//-------------------------------------------------------------------------------
std::vector<std::basic_string<TCHAR> > kjm::iniFile::getKeyNames(const std::basic_string<TCHAR>& strSectionName) const {
	assert(!m_fileName.empty());

	std::vector<std::basic_string<TCHAR> > result;
	std::vector<TCHAR> work;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileString(strSectionName.c_str(), NULL, _T(""), &work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 2);

	std::vector<TCHAR>::iterator sp = work.begin();
	std::vector<TCHAR>::iterator ep;
	while ((ep = std::find(sp, work.end(), '\0')) != sp) {
		result.push_back(std::basic_string<TCHAR>(sp, ep));
		sp = ep + 1;
	}

	return result;
}


//-------------------------------------------------------------------------------
// ini�t�@�C������l���擾
//-------------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::iniFile::getString(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strDefault, BOOL *pbUseDefault/*=NULL*/) const {
	assert(!m_fileName.empty());

	std::vector<TCHAR> work;
	std::basic_string<TCHAR> value1;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(), &work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 1);

	value1 = &work[0];

	// �f�t�H���g���g�������ǂ����𔻒肷��
	if (pbUseDefault) {
		std::basic_string<TCHAR> strDefault2 = _T("x") + strDefault;
		std::basic_string<TCHAR> value2 = getString(strSection, strKey, strDefault2);

		*pbUseDefault = (value1 != value2) ? TRUE : FALSE;
	}

	return &work[0];
}

//-------------------------------------------------------------------------------
// ini�t�@�C�����琔�l���擾
//-------------------------------------------------------------------------------
UINT kjm::iniFile::getInt(const kjm::_tstring& strSection, const kjm::_tstring& strKey, UINT defValue, BOOL* pfUseDefault) const {
	assert(!m_fileName.empty());

	UINT value1 = ::GetPrivateProfileInt(strSection.c_str(), strKey.c_str(), defValue, m_fileName.c_str());

	// �f�t�H���g���g�������ǂ����𔻒肷��
	if (pfUseDefault) {
		UINT value2 = ::GetPrivateProfileInt(strSection.c_str(), strKey.c_str(), ~defValue, m_fileName.c_str());

		*pfUseDefault = (value1 != value2) ? TRUE : FALSE;
	}

	return value1;
}

//-------------------------------------------------------------------------------
// ini�t�@�C���ɒl����������
//-------------------------------------------------------------------------------
BOOL kjm::iniFile::writeString(const std::basic_string<TCHAR>& strSection, const std::basic_string<TCHAR>& strKey, const std::basic_string<TCHAR>& strValue) {
	assert(!m_fileName.empty());

	return ::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), this->m_fileName.c_str());
}


//-------------------------------------------------------------------------------
// �ʂ�ini�t�@�C���̓��e���}�[�W����
//-------------------------------------------------------------------------------
void kjm::iniFile::mergeIniFile(const iniFile& newIniFile) {
	assert(!m_fileName.empty());

	// �}�[�W����INI�t�@�C������A�S�Z�N�V������ǂݍ���
	std::vector<std::basic_string<TCHAR> > sections = newIniFile.getSectionNames();
	for (size_t i = 0; i < sections.size(); i++) {

		// �}�[�W����INI�t�@�C���̃Z�N�V��������A�S�L�[��ǂݍ���
		std::vector<std::basic_string<TCHAR> > newKeys = newIniFile.getKeyNames(sections[i]);

		// �}�[�W�����INI(����)�t�@�C���̃Z�N�V����������S�L�[��ǂݍ���
		std::vector<std::basic_string<TCHAR> > orgKeys = this->getKeyNames(sections[i]);

		for (size_t j = 0; j < newKeys.size(); j++) {

			// �}�[�W�����INI(����)�t�@�C���ɓ����L�[�����݂��邩�ǂ������ׂ�
			// �}�[�W�����INI(����)�t�@�C���ɃL�[�����݂��Ȃ��ꍇ�A�V����INI�t�@�C������L�[�̒l����������
			// (�}�[�W�����INI(����)�t�@�C���ɓ����L�[�����݂���Ƃ��͉������Ȃ��̂ŁA���e���ۏႳ��銴��)

			std::vector<std::basic_string<TCHAR> >::iterator p = std::find(orgKeys.begin(), orgKeys.end(), newKeys[j]);
			if (p == orgKeys.end()) {
				std::basic_string<TCHAR> value = newIniFile.getString(sections[i], newKeys[j], _T(""));
				this->writeString(sections[i], newKeys[j], value);

				///if (g_kjmlog) g_kjmlog->writeInfo(_T("[%s] %s=%s ���}�[�W�B"), sections[i].c_str(), newKeys[j].c_str(), value.c_str());
			}
		}
	}
}

//=============================================================================
// Mutex����������N���X
//=============================================================================
//---------------------------------------------------------------------
// �~���[�e�b�N�I�u�W�F�N�g�쐬�t���R���X�g���N�^
//---------------------------------------------------------------------
kjm::mutex::mutex(const kjm::_tstring &name) : m_hMutex(NULL), m_create_status(0), m_lock_count(0) {
	create(name);	// ��O�͋N���肦�Ȃ��B�`�F�b�N�́Ais_open�Ń`�F�b�N
}

//---------------------------------------------------------------------
// �R�s�[�R���X�g���N�^
//---------------------------------------------------------------------
kjm::mutex::mutex(const kjm::mutex& src) {
	// �R�s�[�����I�[�v���O�Ȃ�A��������K�v���Ȃ��̂ŏI���B
	if (!src.is_open())	{
		m_hMutex = NULL;
		m_create_status = 0;
		m_lock_count = 0;
		return;
	}

	__duplicate(src);			// �n���h���𕡐�
}

//---------------------------------------------------------------------
// Mutex�̍쐬
//
// arg : name = mutex �̖��O
// ret : true = ����I���Afalse = �ȏ�I��
// exp : mutex_error
//---------------------------------------------------------------------
bool kjm::mutex::create(const kjm::_tstring &name) {
	// �������ς� instance �̎��́A��O�� throw
	if (m_hMutex)	throw mutex_error(mutex_error::already_initialized);

	// mutex �̍쐬
	m_hMutex = ::CreateMutex(NULL, FALSE, name.c_str());
	m_create_status = ::GetLastError();
	return (m_hMutex != 0);
}

//---------------------------------------------------------------------
// Mutex�����
//
// note: ���������̃C���X�^���X�Ɏg�p���Ă�ok
//       lock���̃I�u�W�F�N�g�ɑ΂��ẮAunlock���Ă���N���[�Y����B
//---------------------------------------------------------------------
void kjm::mutex::close() {
	if (m_hMutex) {
		while (m_lock_count)
			if (!unlock())
				break;

		::CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}
}

//---------------------------------------------------------------------
// �w�莞�ԃV�O�i����ԂɂȂ�̂�҂�
//
// arg : dwMillisec = �^�C���A�E�g����(ms)
// ret : WaitForSingleObject API �Q��
// exp : mutex_error
// note: mutex���������ς݂̏ꍇ�́AWaitForSingleObject���ĂԂ����B
//---------------------------------------------------------------------
DWORD kjm::mutex::lock(DWORD dwMillisec) {
	if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
	
	DWORD dw = ::WaitForSingleObject(m_hMutex, dwMillisec);
	if (dw == WAIT_OBJECT_0 || dw == WAIT_ABANDONED) {
		++m_lock_count;
	}

	return dw;
}

//---------------------------------------------------------------------
// mutex���J������
//
// arg : �Ȃ�
// ret : ReleaseMutex API �Q��
// exp : mutex_error
// note: mutex���������ς݂̏ꍇ�́AReleaseMutex ���ĂԂ����B
//---------------------------------------------------------------------
BOOL kjm::mutex::unlock() {
	if (!m_hMutex)	throw mutex_error(mutex_error::not_initialized);
	
	BOOL ret = ::ReleaseMutex(m_hMutex);
	if (ret) {
		--m_lock_count;
	}

	return ret;
}

//---------------------------------------------------------------------
// ������Z�q
//
// @param	src	�R�s�[���C���X�^���X
// @note
// ���g�̃~���[�e�b�N�X���A�쐬�ς݂̏ꍇ�A������B
//---------------------------------------------------------------------
kjm::mutex& kjm::mutex::operator= (const kjm::mutex& src) {

	// �R�s�[���ƃR�s�[�悪�A�����I�u�W�F�N�g�̏ꍇ�́A���������^�[��
	if (&src == this)	return *this;

	// �����̃~���[�e�b�N�X����Ă���
	close();

	// �R�s�[�����I�[�v���O�Ȃ�A���̂܂܏I���B
	if (!src.is_open())	return *this;

	// �n���h���𕡐�
	__duplicate(src);

	return *this;
}

//---------------------------------------------------------------------
// �n���h���������ʊ֐�
//---------------------------------------------------------------------
void kjm::mutex::__duplicate(const kjm::mutex& src) {

	if (::DuplicateHandle(
		GetCurrentProcess(),
		src.m_hMutex,
		GetCurrentProcess(),
		&m_hMutex,
		0,
		TRUE,
		DUPLICATE_SAME_ACCESS)) {

		// ���������B�ق��̃����o����������B
		m_create_status = src.m_create_status;
		m_lock_count = src.m_lock_count;
	} else {
		m_hMutex = NULL;	// ���s�����Ƃ��́A�O�̂��߃n���h�����N���A
	}

	m_last_error = ::GetLastError();	// DuplicateHandle�̌��ʃR�[�h��ۑ�
}


//=============================================================================
// class log ���ʂ̃��O����
//=============================================================================
//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------
kjm::log::log() {
	m_rotateCount = 9;
	m_rotateSize = 1 * 1024 * 1024;
	m_stop = true;

	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �e���|�����p�X���擾
	m_strLogPath = kjm::util::getTempPath();

	// ���O�t�@�C���̊�{�ɂȂ�t�@�C�������������s�t�@�C��������擾
	_tstring strFileSpec = kjm::path::findFileName(kjm::util::getModuleFileName());
	m_strPrefix = kjm::path::removeExtension(strFileSpec);

	// �t�@�C�����𐶐�����
	//m_strBaseFileName = kjm::path::append(m_strLogPath, m_strPrefix + _T("{YMD}.log"));
	m_strBaseFileName = kjm::path::append(m_strLogPath, m_strPrefix + _T(".log"));
}

//---------------------------------------------------------------------
// �Â����O�̍폜
//---------------------------------------------------------------------
void kjm::log::deleteOldLog() {
#if defined(USE_WSSLOG)
	WLDeleteOldLog();
#endif
}

//---------------------------------------------------------------------
// ���O���V�t�g����
//---------------------------------------------------------------------
void kjm::log::shift(const _tstring& fname) {
	kjm::_tstring base = kjm::path::removeExtension(fname);
	kjm::_tstring ext = kjm::path::getExtension(fname);
	
	kjm::_tstring bakFile = kjm::util::sprintf_str(_T("%s_%d%s"), base.c_str(), this->m_rotateCount, ext.c_str());

	if (kjm::path::fileExists(bakFile)) {
		DeleteFile(bakFile.c_str());
	}

	for (int i = this->m_rotateCount - 1; i >= 1; i--) {
		MoveFile(
			kjm::util::sprintf_str(_T("%s_%d%s"), base.c_str(), i, ext.c_str()).c_str(),
			kjm::util::sprintf_str(_T("%s_%d%s"), base.c_str(), i + 1, ext.c_str()).c_str());
	}

	MoveFile(fname.c_str(), kjm::util::sprintf_str(_T("%s_1%s"), base.c_str(), ext.c_str()).c_str());
}

//---------------------------------------------------------------------
// ���O�V�X�e���̖��O��Ԃ�
//---------------------------------------------------------------------
kjm::_tstring kjm::log::get_MyName() {
	kjm::_tstring result;
#if defined(USE_WSSLOG)
	char buf[MAX_PATH * 2];
	WLGetMyName(buf, MAX_PATH * 2);

#if defined(UNICODE)
	result = kjm::util::toUnicode(buf);
#else
	result = buf;
#endif

#else
	result = _T("(���[�J��)");
#endif
	return result;
}

//---------------------------------------------------------------------
// ���O�V�X�e���̏������O�Ɏc��
//---------------------------------------------------------------------
void kjm::log::writeMyName() {
#if defined(USE_WSSLOG)
	char buf[MAX_PATH * 2];
	WLGetMyName(buf, MAX_PATH * 2);
	write(
		_T("���p���O�V�X�e��: WssLog [%s]"),
		kjm::util::toUnicode(buf).c_str()
	);
#else
	writeInfo(_T("���p���O�V�X�e��: Local  [%s]"), _T("local"));
#endif
}

//---------------------------------------------------------------------
// ���O�̃t�@�C�������[����ݒ肷��
//---------------------------------------------------------------------
void kjm::log::set_LogNameType(int nNameType) {
#if defined(USE_WSSLOG)
	WLSetLogNameType(nNameType);
#else
#endif
}

//---------------------------------------------------------------------
// ���O�̃t�@�C�������[�����擾����
//---------------------------------------------------------------------
long kjm::log::get_LogNameType() {
#if defined(USE_WSSLOG)
	return WLGetLogNameType();
#else
	return 0;
#endif
}

//---------------------------------------------------------------------
// ���O�̕ۑ���� %TEMP%\<subdir> �ɂ���
//---------------------------------------------------------------------
void kjm::log::setLogPath_Temp(const kjm::_tstring &subdir/* = kjm::_tstring()*/) {
	set_log_path(kjm::path::combine(kjm::util::getTempPath(), subdir));
}

//---------------------------------------------------------------------
// ���O�̕ۑ���� �X�N���v�g�Ɠ����ꏊ\<subdir> �ɂ���
//---------------------------------------------------------------------
void kjm::log::setLogPath_App(const kjm::_tstring &subdir/* = kjm::_tstring()*/) {
	set_log_path(kjm::path::combine(kjm::util::getModulePath(), subdir));
}

//---------------------------------------------------------------------
// ���O�t�@�C���̏o�͐��ݒ肷��
//---------------------------------------------------------------------
void kjm::log::set_log_path(const _tstring& strPath) {
	// �o�͐�t�H���_���쐬���Ă���
	kjm::directory::createDirectory(strPath);

#if defined(USE_WSSLOG)
#if defined(UNICODE)
	WLSetLogPath(kjm::util::toAnsi(strPath).c_str());
#else
	WLSetLogPath(strPath.c_str());
#endif
#else
	m_strLogPath = strPath;

	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �t�@�C�����𐶐�����
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T(".log"));
#endif
}

//---------------------------------------------------------------------
// ���O�t�@�C���̏o�͐�𐔒l�Őݒ肷��
//---------------------------------------------------------------------
void kjm::log::set_log_path(int nMode, const kjm::_tstring &more/* = kjm::_tstring()*/) {
	kjm::_tstring logPathBase;

	switch (nMode) {
		case LOGPATH_EXEPATH:
			logPathBase = kjm::util::getModulePath();
			break;

		default:
			logPathBase = kjm::util::getTempPath();
			break;
	}

	if (!more.empty()) {
		logPathBase = kjm::path::append(logPathBase, more);
	}

	this->set_log_path(logPathBase);
}

//---------------------------------------------------------------------
// ���O�t�@�C���̏o�͐���擾����
//---------------------------------------------------------------------
kjm::_tstring kjm::log::get_log_path() {
#if defined(USE_WSSLOG)
	char strPath[MAX_PATH];
	WLGetLogPath(strPath, MAX_PATH);
#if defined(UNICODE)
	return kjm::util::toUnicode(strPath);
#else
	return strPath;
#endif
#else
	return m_strLogPath;
#endif
}

//---------------------------------------------------------------------
// ���O�t�@�C���̃v���t�B�N�X��ݒ肷��
//---------------------------------------------------------------------
void kjm::log::set_prefix(const kjm::_tstring& strPrefix) {
	m_strPrefix = strPrefix;

	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �t�@�C�����𐶐�����
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T(".log"));
}

//---------------------------------------------------------------------
// �S���O
//---------------------------------------------------------------------
void kjm::log::writeAll(LPCTSTR pszFormat, ...) {
	if (this->m_stop) return;

	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
#if defined(UNICODE)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_ALWAYS, "%s", kjm::util::toAnsi(&buf[0]).c_str());
#else
	Winssa_Log(get_Ident().c_str(), LOGLV_ALWAYS, "%s", &buf[0]);
#endif
#else
	this->write(_T("%s"), &buf[0]);
#endif
}

//---------------------------------------------------------------------
// �G���[���O
//---------------------------------------------------------------------
void kjm::log::writeError(LPCTSTR pszFormat, ...) {
	if (this->m_stop) return;

	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
#if defined(UNICODE)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_ERR, "%s", kjm::util::toAnsi(&buf[0]).c_str());
#else
	Winssa_Log(get_Ident().c_str(), LOGLV_ERR, "%s", &buf[0]);
#endif
#else
	this->write(_T("%s"), &buf[0]);
#endif
}

//---------------------------------------------------------------------
// ��񃍃O
//---------------------------------------------------------------------
void kjm::log::writeInfo(LPCTSTR pszFormat, ...) {
	if (this->m_stop) return;

	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
#if defined(UNICODE)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_INFO, "%s", kjm::util::toAnsi(&buf[0]));
#else
	Winssa_Log(get_Ident().c_str(), LOGLV_INFO, "%s", &buf[0]);
#endif
#else
	this->write(_T("%s"), &buf[0]);
#endif
}

//---------------------------------------------------------------------
// ���O�̏o�͊֐�
//---------------------------------------------------------------------
void kjm::log::write(LPCTSTR format, ...) {
	if (this->m_stop) return;

#ifdef USE_WSSLOG
	va_list marker;
	std::vector<TCHAR> work;
	int n;

	va_start(marker, format);

	do {
		work.resize(work.size() + MAX_PATH);
		n = _vsntprintf(&work[0], work.size(), format, marker);
	} while (n == -1 || n == work.size());

	va_end(marker);

#if defined(UNICODE)
	Winssa_Log(kjm::util::toAnsi(m_strIdent).c_str(), LOGLV_INFO, "%s", kjm::util::toAnsi(&work[0]).c_str());
#else
	Winssa_Log(m_strIdent.c_str(), LOGLV_INFO, "%s", &work[0]);
#endif
#else
	va_list marker;
	va_start(marker, format);

	// ��{�t�@�C��������A���ۂ̃��O�t�@�C�����𐶐�����
	kjm::_tstring fname = kjm::util::strReplace(m_strBaseFileName, _T("{YMD}"), kjm::dateTime::getLocalTime().toString_yyyymmdd() );

	{	// ���O�V�t�g�̂��߂̏���
		if (this->m_rotateSize > 0) {
			struct _stat buffer;
			if (_tstat(fname.c_str(), &buffer) == 0) {
				if (buffer.st_size > this->m_rotateSize) {
					this->shift(fname);
				}
			}
		}
	}

	FILE* fp = NULL;
	for (int i = 0; i < 100; i++) {
		fp = _tfsopen(fname.c_str(), _T("a+"), _SH_DENYWR);
		if (fp) break;
		Sleep(0);
	}

	if (fp) {
		_ftprintf(fp, _T("%s [%04X]"), kjm::dateTime::getLocalTime().toString().c_str(), GetCurrentThreadId());
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
void kjm::log::writee(LPCTSTR format, ...) {

	va_list marker;
	std::vector<TCHAR> work;
	int n;

	va_start(marker, format);

	do {
		work.resize(work.size() + MAX_PATH);
#if _MSC_VER <= 1200	// VC6�ȉ�
		n = _vsntprintf(&work[0], work.size(), format, marker);
#else
		n = _vsntprintf_s(&work[0], work.size(), _TRUNCATE, format, marker);
#endif
	} while (n == -1 || n == work.size());

	va_end(marker);

	write(_T("%s"), &work[0]);

	// �W���o�͂ɂ��o��
	_tprintf(_T("%s\n"), &work[0]);
}

//=====================================================================
// class optinfo
//=====================================================================

// �f�t�H���g�R���X�g���N�^
kjm::optinfo::optinfo() {
	this->ShortOption = '\0';
	this->LongOption = _T("");
	this->OptArgRequirement = kjm::no_argument;

	this->HasOption = false;
	this->OptArg = _T("");
}

// �I�v�V�������w��t���R���X�g���N�^
kjm::optinfo::optinfo(TCHAR shortOption, const _tstring& longOption, kjm::optarg_requirement flag) {
	this->ShortOption = shortOption;
	this->LongOption = longOption;
	this->OptArgRequirement = flag;

	this->HasOption = false;
	this->OptArg = _T("");
}

// �I�v�V�������w��t���R���X�g���N�^(�V���[�g�v�V��������)
kjm::optinfo::optinfo(const _tstring& longOption, kjm::optarg_requirement flag) {
	this->ShortOption = '\0';
	this->LongOption = longOption;
	this->OptArgRequirement = flag;

	this->HasOption = false;
	this->OptArg = _T("");
}

// �R�s�[�R���X�g���N�^
kjm::optinfo::optinfo(const kjm::optinfo& src) {
	this->ShortOption = src.ShortOption;
	this->LongOption = src.LongOption;
	this->OptArgRequirement = src.OptArgRequirement;

	this->HasOption = src.HasOption;
	this->OptArg = src.OptArg;
}

// ������Z�q
kjm::optinfo& kjm::optinfo::operator=(const kjm::optinfo& rhs) {
	if (this != &rhs) {
		this->ShortOption = rhs.ShortOption;
		this->LongOption = rhs.LongOption;
		this->OptArgRequirement = rhs.OptArgRequirement;

		this->HasOption = rhs.HasOption;
		this->OptArg = rhs.OptArg;
	}
	return *this;
}

//=====================================================================
// class optparse
//=====================================================================

// �R�s�[�R���X�g���N�^
kjm::optparse::optparse(const kjm::optparse& src) {
	this->m_opts = src.m_opts;
}

// ������Z�q
kjm::optparse& kjm::optparse::operator=(const kjm::optparse& rhs) {
	if (this != &rhs) {
		this->m_opts = rhs.m_opts;
	}
	return *this;
}

// ���p�\�ȃI�v�V������ǉ�����
void kjm::optparse::add(const kjm::optinfo& opt) {
	// �����O�I�v�V�������L�[�ɂ��āAmap�ɒǉ�����B
	this->m_opts[opt.LongOption] = opt;
}

// �V���[�g�I�v�V�������w�肵�ăI�v�V��������T��
kjm::optinfo& kjm::optparse::findOption(TCHAR shortOpt) {
	for (OPTION_MAP_Iter i = m_opts.begin(); i != m_opts.end(); i++) {
		if (i->second.ShortOption == shortOpt) {
			return i->second;
		}
	}
	throw kjm::invalidOption(kjm::encoding::to_mbs(_T("-") + _tstring(1, shortOpt)));
}

// �����O�I�v�V�������w�肵���I�v�V��������T��
kjm::optinfo& kjm::optparse::findOption(const _tstring& longOpt) {
	if (m_opts.find(longOpt) != m_opts.end()) {
		return m_opts[longOpt];
	}
	throw kjm::invalidOption(kjm::encoding::to_mbs(_T("--") + longOpt));
}

// �����O�I�v�V��������A�z�z�񕗂ɃI�v�V���������擾
kjm::optinfo& kjm::optparse::operator[](const _tstring& longOpt) {
	return findOption(longOpt);
}

// �I�v�V�����̉��
kjm::optparse::ARGS kjm::optparse::parse(int argc, TCHAR** argv) {
	std::list<_tstring> args;
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];
			if (c == '-') {
				// �����O�I�v�V�����̏���
				_tstring work(&argv[i][2]);
				_tstring::size_type pos = work.find_first_of('=');
				_tstring option_name = _tstring(work, 0, pos);

				optinfo& opt = findOption(option_name);
				opt.HasOption = true;
				if (opt.OptArgRequirement == required_argument) {
					if (pos != _tstring::npos) {
						opt.OptArg = _tstring(work, pos + 1);
					} else {
						// �����̃I�v�V�������w�肳��Ă��Ȃ��I
						throw kjm::missingArgument(kjm::encoding::to_mbs(_T("--") + option_name));
					}
				} else if (opt.OptArgRequirement == optional_argument) {
					if (pos != _tstring::npos) {
						opt.OptArg = _tstring(work, pos + 1);
					} else {
						// �����̃I�v�V�������w�肳��Ă��Ȃ�(optional�Ȃ̂Ő���)
					}
				}
			} else {
				// �V���[�g�v�V�����̏���
				optinfo& opt = findOption(c);
				opt.HasOption = true;
				if (opt.OptArgRequirement == required_argument) {
					if (argv[i][2] != '\0') {
						// �V���[�g�I�v�V�����ɑ����ăI�v�V�����̈����������Ă��鎞
						opt.OptArg = &argv[i][1];
					} else if (argv[i][2] == '\0' && (i + 1) < argc) {
						// �V���[�g�I�v�V�����̎��̈���������ꍇ
						// optional_argument�ƈႢ�A�K�������Ƃ��Ĉ���
						opt.OptArg = argv[i + 1];
						++i;
					} else {
						// �����̃I�v�V�������w�肳��Ă��Ȃ��I
						throw kjm::missingArgument(kjm::encoding::to_mbs(_T("-") + _tstring(1, c)));
					}
				} else if (opt.OptArgRequirement == optional_argument) {
					if (argv[i][2] != '\0') {
						// �V���[�g�I�v�V�����ɑ����ăI�v�V�����̈����������Ă��鎞
						opt.OptArg = &argv[i][1];
					} else if (argv[i][2] == '\0' && (i + 1) < argc) {
						// �V���[�g�I�v�V�����̎��̈���������ꍇ
						if (argv[i + 1][0] != '-') {	// �I�v�V�����łȂ���΁A�����Ƃ��Ĉ���
							opt.OptArg = argv[i + 1];
							++i;
						}
					}
				}
			}
		} else {
			// �I�v�V�����ȊO�̈���
			args.push_back(argv[i]);
		}
	}
	return args;
}

//=============================================================================
// ODBC���������[�e�B���e�B�N���X
// ���p����Ƃ��́AUSE_ODBC32���`����
//=============================================================================
#pragma region USE_ODBC32
#if defined(USE_ODBC32)
//---------------------------------------------------------------------
// �C���X�g�[���ς݂�ODBC�h���C�o�ꗗ���擾
//---------------------------------------------------------------------
void kjm::odbcUtil::getDriverList(std::vector<kjm::_tstring>& drivers) {
	drivers.clear();

	SQLHANDLE henv = NULL;
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);

	if (ret == SQL_SUCCESS) {
		SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0);

		std::vector<SQLTCHAR> desc(1024);
		std::vector<SQLTCHAR> attr(1024);
		SQLSMALLINT descSizeNeed, attrSizeNeed;

		ret = SQLDrivers(henv, SQL_FETCH_FIRST, &desc[0], desc.size(), &descSizeNeed, &attr[0], attr.size(), &attrSizeNeed);
		while (ret == SQL_SUCCESS) {
#if defined(UNICODE)
			drivers.push_back(&desc[0]);
#else
			drivers.push_back((char*)&desc[0]);
#endif
			ret = SQLDrivers(henv, SQL_FETCH_NEXT, &desc[0], desc.size(), &descSizeNeed, &attr[0], attr.size(), &attrSizeNeed);
		}
		ret = SQLFreeHandle(SQL_HANDLE_ENV, &henv);
	}
}

//---------------------------------------------------------------------
// �C���X�g�[���ς݂�ODBC�h���C�o�ꗗ���擾(ODBC2.0��)
//---------------------------------------------------------------------
void kjm::odbcUtil::getDriverList_ODBCv2(std::vector<kjm::_tstring>& drivers) {
	drivers.clear();

	// ODBC2.0�܂ł́A������
	SQLHENV henv = NULL;
	SQLRETURN ret = SQLAllocEnv(&henv);
	if (ret == SQL_SUCCESS) {
		std::vector<SQLTCHAR> desc(1024);
		std::vector<SQLTCHAR> attr(1024);
		SQLSMALLINT descSizeNeed, attrSizeNeed;

		ret = SQLDrivers(henv, SQL_FETCH_FIRST, &desc[0], desc.size(), &descSizeNeed, &attr[0], attr.size(), &attrSizeNeed);
		while (ret == SQL_SUCCESS) {
#if defined(UNICODE)
			drivers.push_back(&desc[0]);
#else
			drivers.push_back((char*)&desc[0]);
#endif
			ret = SQLDrivers(henv, SQL_FETCH_NEXT, &desc[0], desc.size(), &descSizeNeed, &attr[0], attr.size(), &attrSizeNeed);
		}
		ret = SQLFreeEnv(henv);
	}
}

//---------------------------------------------------------------------
// �C���X�g�[���ς݂̃h���C�o�𖼑O�Ō���
//---------------------------------------------------------------------
kjm::_tstring kjm::odbcUtil::findDriver(const kjm::_tstring& pattern) {
	kjm::_tstring pat = kjm::util::toUpper(pattern);

	std::vector<kjm::_tstring> drivers;
	kjm::odbcUtil::getDriverList(drivers);

	for (std::vector<kjm::_tstring>::size_type i = 0; i < drivers.size(); i++) {
		kjm::_tstring s1 = kjm::util::toUpper(drivers[i]);
		if (s1.find(pat) != s1.npos) {
			return drivers[i];
		}
	}

	return kjm::_tstring();
}

//---------------------------------------------------------------------
// �h���C�o���C���X�g�[���ς݂��ǂ������ׂ�
//---------------------------------------------------------------------
bool kjm::odbcUtil::isInstalled(const kjm::_tstring& driverName) {
	kjm::_tstring driverNameUC = kjm::util::toUpper(driverName);

	std::vector<kjm::_tstring> drivers;
	kjm::odbcUtil::getDriverList(drivers);

	for (std::vector<kjm::_tstring>::size_type i = 0; i < drivers.size(); i++) {
		if (driverNameUC == kjm::util::toUpper(drivers[i])) {
			return true;
		}
	}

	return false;
}
#endif
#pragma endregion USE_ODBC32

//=============================================================================
// XML ����A���ȒP�Ƀf�[�^��ǂݏo���Ă��܂��N���X
//=============================================================================
#pragma region USE_MSXML6
#if defined(USE_MSXML6)
//---------------------------------------------------------------------
// XML ����A���ȒP�Ƀf�[�^��ǂݏo���Ă��܂��֐�
//---------------------------------------------------------------------
kjm::_tstring kjm::xmlez::getText(const kjm::_tstring& xmlFile, const kjm::_tstring &tagName) {
	_bstr_t result;

	MSXML2::IXMLDOMDocumentPtr doc;
	HRESULT hr = doc.CreateInstance(MSXML2::CLSID_DOMDocument);
	try {
		doc->async = VARIANT_FALSE;
		if (doc->load(xmlFile.c_str()) != VARIANT_FALSE) {
			MSXML2::IXMLDOMNodeListPtr nl = doc->getElementsByTagName(tagName.c_str());
			if (nl->length > 0) {
				result = nl->item[0]->text;
			}
		}
	} catch (_com_error &e) {
	}

	LPCTSTR pres = (LPCTSTR)result;
	return (pres) ? pres : _T("");
}
#endif
#pragma endregion USE_MSXML6

//---------------------------------------------------------------------------------------
// CopyFile API�Ɠ��l�̓���������֐�(�G���[�ӏ����ڍׂɓ��肵�����Ƃ��Ɏg�p)
// [�߂�l]  0   ����I��
// [�߂�l] -1   �R�s�[���t�@�C���̃I�[�v���Ɏ��s
// [�߂�l] -2   �R�s�[��t�@�C���̃I�[�v���Ɏ��s
// [�߂�l] -3   �R�s�[���t�@�C���̓ǂݍ��݂Ɏ��s
// [�߂�l] -4   �R�s�[��t�@�C���̏������݂Ɏ��s
// [�߂�l] -5   �R�s�[��t�@�C���Ɏw��T�C�Y�������݂ł��Ȃ�
// [�߂�l] -6   �R�s�[���t�@�C���̃^�C���X�^���v�擾���s
// [�߂�l] -7   �R�s�[��t�@�C���̃^�C���X�^���v�X�V���s
// [�߂�l] -8   �R�s�[���t�@�C�������̂Ɏ��s
// [�߂�l] -9   �R�s�[��t�@�C�������̂Ɏ��s
// [�߂�l] -10  �R�s�[���t�@�C���̑����擾���s
// [�߂�l] -11  �R�s�[��t�@�C���̑����X�V���s
//---------------------------------------------------------------------------------------
static int copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, LPDWORD pdwErr = NULL) {

#if !defined(_countof)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

	SetLastError(NOERROR);
	if (pdwErr) *pdwErr = NOERROR;

	int result = 0;

	// �R�s�[���t�@�C�����J��
	HANDLE hFile1 = ::CreateFile(lpExistingFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile1 == INVALID_HANDLE_VALUE) {
		if (pdwErr) *pdwErr = ::GetLastError();
		return -1;
	}

	// �R�s�[��t�@�C�����J��
	HANDLE hFile2 = ::CreateFile(lpNewFileName, GENERIC_WRITE, 0, NULL, (bFailIfExists) ? CREATE_NEW : CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile2 == INVALID_HANDLE_VALUE) {
		if (pdwErr) *pdwErr = ::GetLastError();
		::CloseHandle(hFile1);
		return -2;
	}

	SetLastError(NOERROR);
	if (pdwErr) *pdwErr = NOERROR;

	char buffer[8192];
	DWORD dwNumberOfBytesRead;
	DWORD dwNumberOfBytesWritten;

	do {
		if (::ReadFile(hFile1, buffer, _countof(buffer), &dwNumberOfBytesRead, NULL) == FALSE) {
			if (pdwErr) *pdwErr = ::GetLastError();
			result = -3;
			break;
		}

		if (dwNumberOfBytesRead > 0) {
			if (::WriteFile(hFile2, buffer, dwNumberOfBytesRead, &dwNumberOfBytesWritten, NULL)) {
				if (dwNumberOfBytesRead != dwNumberOfBytesWritten) {
					if (pdwErr) *pdwErr = ::GetLastError();
					result = -5;
					break;
				}
			} else {
				if (pdwErr) *pdwErr = ::GetLastError();
				result = -4;
				break;
			}
		}
	} while (dwNumberOfBytesRead == _countof(buffer));

	// �R�s�[������ɏI�������Ƃ��́A�^�C���X�^���v���R�s�[���ɍ��킹��
	if (result == 0) {
		FILETIME lastWriteTime;

		if (::GetFileTime(hFile1, NULL, NULL, &lastWriteTime)) {
			if (::SetFileTime(hFile2, NULL, NULL, &lastWriteTime)) {
				// �^�C���X�^���v�̕�������
			} else {
				if (pdwErr) *pdwErr = ::GetLastError();
				result = -7;
			}
		} else {
			if (pdwErr) *pdwErr = ::GetLastError();
			result = -6;
		}
	}

	if (::CloseHandle(hFile1) == FALSE) {
		if (pdwErr) *pdwErr = ::GetLastError();
		result = -8;
	}
	if (::CloseHandle(hFile2) == FALSE) {
		if (pdwErr) *pdwErr = ::GetLastError();
		result = -9;
	}

	// �R�s�[������ɐi��ł���Ƃ��́A�������R�s�[����
	if (result == 0) {
		DWORD dwAttributes = ::GetFileAttributes(lpExistingFileName);
		if (dwAttributes != 0xffffffff) {
			if (::SetFileAttributes(lpNewFileName, dwAttributes | FILE_ATTRIBUTE_ARCHIVE)) {
				// �����̕�������
			} else {
				if (pdwErr) *pdwErr = ::GetLastError();
				result = -11;
			}
		} else {
			if (pdwErr) *pdwErr = ::GetLastError();
			result = -10;
		}
	}

	return result;
}

/****************************************************************************
  lpExistingFileName�̃t�@�C����lpNewFileName�ɃR�s�[���܂��B�R�s�[���s���ɂ�
  �w��񐔕����g���C���s���܂��B

  <�p�����[�^>
  lpExistingFileName  �R�s�[��
  lpNewFileName       �R�s�[��
  bFailIfExists       �㏑�����s����(FALSE�ŏ㏑�����s��)
  retry               �Ď��s������s����
  wait                �Ď��s���s���܂ŉ��b�҂̂�

  <�߂�l>
  �R�s�[�ɐ��������Ƃ�TRUE�A�R�s�[�Ɏ��s�����Ƃ�FALSE��Ԃ��B
****************************************************************************/
BOOL IOUtil__CopyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, int retry, int wait, LPDWORD pdwErr/* = NULL*/) {
	BOOL bRet = FALSE;
	DWORD dwErr;
	int ret;

	for (int i = 0; i <= retry; i++) {
		dwErr = NOERROR;
		ret = copyFile(lpExistingFileName, lpNewFileName, bFailIfExists, pdwErr);
		if (ret == 0) {
			bRet = TRUE;
			break;
		}

		dwErr = (pdwErr) ? *pdwErr : GetLastError();
#if defined(USE_WSSLOG)
        Winssa_Log(_T("KJMLIB"), LOGLV_ERR, _T("�R�s�[���s(%d): %s -> %s"), i + 1, lpExistingFileName, lpNewFileName);
        Winssa_Log(_T("KJMLIB"), LOGLV_ERR, _T("�G���[�R�[�h: %d : %d"), ret, dwErr);
#endif

		Sleep(wait * 1000);
	}

	// T2X005744 ����������G���[�ɂ��Ȃ����߂̑΍��������ł���Ă��܂��B
	if ((bRet == FALSE) && (dwErr == 183)) {
		bRet = TRUE;
#if defined(USE_WSSLOG)
        Winssa_Log(_T("KJMLIB"), LOGLV_ERR, _T("<T2X005744>�΍�����"));
#endif
	}

	return bRet;
}

//---------------------------------------------------------------------
// ���b�N�t�@�C�������S�Ɉ������߂̃N���X
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ���b�N����
//---------------------------------------------------------------------
BOOL kjm::Lockfile::Lock(const std::basic_string<TCHAR>& strLockfile) {
	// ���b�N�t�@�C���̍폜�����݂�(���s���Ă��C�ɂ��Ȃ�)
	// �V�X�e���̋����I���ȂǂŁA���b�N�t�@�C�����c�邱�Ƃ����邽�߁B
	// ���̂悤�ɂ��āA�c�������b�N�t�@�C���́A�폜���邱�Ƃ��ł���B
	::DeleteFile(strLockfile.c_str());

	this->m_hFile = ::CreateFile(strLockfile.c_str(), CREATE_NEW, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
	return (this->m_hFile != INVALID_HANDLE_VALUE) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// �A�����b�N����
//---------------------------------------------------------------------
BOOL kjm::Lockfile::Unlock() {
	BOOL bRet = TRUE;

	if (this->m_hFile != INVALID_HANDLE_VALUE) {
		bRet = ::CloseHandle(this->m_hFile);
		this->m_hFile = INVALID_HANDLE_VALUE;
	}

	return bRet;
}
