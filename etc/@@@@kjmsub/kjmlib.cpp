//-----------------------------------------------------------------------------
// �v���R���p�C���ς݃w�b�_���g�p���Ȃ��悤�ɐݒ肵�Ă��������B
//
// r.kojima
//-----------------------------------------------------------------------------

#include "kjmlib.h"

//-----------------------------------------------------------------------------
// OutputDebugString ���ȒP�Ɏg�����߂̊֐�
//-----------------------------------------------------------------------------
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

//=============================================================================
// kjm::util�̎���
//=============================================================================
//---------------------------------------------------------------------
// �R�s�[���̒����ɂ��邷�ׂẴt�@�C�����R�s�[��ɃR�s�[����
//---------------------------------------------------------------------
DWORD kjm::util::CopyFiles(LPCTSTR pszSrcPath, LPCTSTR pszDstPath) {

	assert(pszSrcPath != NULL);
	assert(pszSrcPath != NULL);

	std::string module_name = kjm::util::GetModuleName();
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

//-----------------------------------------------------------------------------
// �v���Z�X�̃��W���[�������擾(GetModuleFileName �֐��̃��b�p�[)
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getModuleFileName() {
	std::vector<TCHAR> work(MAX_PATH);
	::GetModuleFileName(NULL, &work.at(0), work.size());
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// ini �t�@�C�����當������擾����(GetPrivateProfileString �֐��̃��b�p�[)
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getPrivateProfileString(LPCTSTR section, LPCTSTR key, LPCTSTR defValue, LPCTSTR iniFile) {
	std::vector<TCHAR> work(MAX_PATH);
	while (::GetPrivateProfileString(section, key, defValue, &work.at(0), work.size(), iniFile) == (work.size() - 1)) {
		work.resize(work.size() + MAX_PATH);
	}
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// �p�X������̊g���q��ύX����
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::renameExtension(const std::basic_string<TCHAR> &path, const std::basic_string<TCHAR> &ext) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRenameExtension(::lstrcpy(&work.at(0), path.c_str()), ext.c_str());
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// �p�X������̃t�@�C������ύX����
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::renameFileSpec(const std::basic_string<TCHAR> &path, const std::basic_string<TCHAR> &fname) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveFileSpec(::lstrcpy(&work.at(0), path.c_str()));
	::PathAppend(&work.at(0), fname.c_str());
	return &work.at(0);
}

/// �f�t�H���g�E�R���X�g���N�^
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
BOOL kjm::BiNotifyIcon::Add(HWND hWnd, UINT uID, UINT uFlags, UINT uCallbackMessage, HICON hIcon, const char* pszTip) {

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
BOOL kjm::BiNotifyIcon::Modify(UINT uFlags, UINT uCallbackMessage, HICON hIcon, char* pszTip) {

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
void kjm::log_info(const char* signature, const char* format, ...) {
#if defined(USE_WSSLOG)
	va_list marker;
	va_start(marker, format);	// �ό̈����̏�����

	std::vector<char> buffer(128);
	do {
		buffer.resize(buffer.size() * 2);
	} while (_vsnprintf(buffer.begin(), buffer.size(), format, marker) == -1);
	va_end(marker);

	Winssa_Log(signature, LOGLV_INFO, buffer.begin());
#endif
}


//---------------------------------------------------------------------
// �G���[���O���c���֐�
//---------------------------------------------------------------------
void kjm::log_error(const char* signature, const char* format, ...) {
#if defined(USE_WSSLOG)
	va_list marker;
	va_start(marker, format);	// �ό̈����̏�����

	std::vector<char> buffer(128);
	do {
		buffer.resize(buffer.size() * 2);
	} while (_vsnprintf(buffer.begin(), buffer.size(), format, marker) == -1);
	va_end(marker);

	Winssa_Log(signature, LOGLV_ERR, buffer.begin());
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
// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
//---------------------------------------------------------------------
std::string kjm::win32::str_replace(const std::string& src, LPCTSTR pFind, LPCTSTR pReplace) {
	std::string result = src;
	std::string::size_type pos = 0;
	while ((pos = result.find(pFind, pos)) != std::string::npos) {
		result.replace(pos, lstrlen(pFind), pReplace);
	}
	return result;
}


//---------------------------------------------------------------------
// �G���[�R�[�h����V�X�e���G���[���b�Z�[�W���擾����B
//---------------------------------------------------------------------
std::string kjm::win32::format_message(DWORD dwErrorCode) {

	std::string result;

	char* pBuffer = NULL;

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
		result = str_replace(std::string(pBuffer), "\r\n", "");

		::LocalFree(pBuffer);
	} else {
		char buf[1024];
		wsprintf(buf, "�G���[ %d (0x%08X) �̃��b�Z�[�W�́A����܂���B", dwErrorCode, dwErrorCode);
		result = buf;
	}

	return result;
}


//---------------------------------------------------------------------
// �z�X�g���܂��́AIP�����񂩂�IP�A�h���X���擾
//---------------------------------------------------------------------
DWORD kjm::util::get_ipaddr(LPCSTR lpName) {
	DWORD addr = INADDR_ANY;
#if 0
	if (lpName[0] == '\0')	return INADDR_ANY;

	DWORD addr = INADDR_ANY;

	// �܂��AIP������ϊ����Ă݂�B
	addr = inet_addr(lpName);
	if (addr == INADDR_NONE) {

		// DNS������
		struct hostent* hent = gethostbyname(lpName);
		if (hent != NULL) {
			addr = *((DWORD *)(hent->h_addr_list[0]));
		} else{
			addr = INADDR_ANY;
		}
	}
#endif
	return	addr;
}

//---------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����
//---------------------------------------------------------------------
bool kjm::util::remove_folder(LPCTSTR pszPath) {
	bool bRet = true;
	std::vector<TCHAR> wc(MAX_PATH);

	// �����ɕK�v�ȃ��C���h�J�[�h���쐬
	///PathAppend(lstrcpyn(wc.begin(), pszPath, wc.size()), _T("*.*"));
	PathAppend(lstrcpyn(&wc.at(0), pszPath, wc.size()), _T("*.*"));

	WIN32_FIND_DATA data;
	///HANDLE hFind = FindFirstFile(wc.begin(), &data);
	HANDLE hFind = FindFirstFile(&wc.at(0), &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((lstrcmp(data.cFileName, _T(".")) != 0) && (lstrcmp(data.cFileName, _T("..")) != 0)) {
					std::vector<TCHAR> child(MAX_PATH);

					// �q���t�H���_���ċA�I�ɍ폜����
					///PathAppend(lstrcpyn(child.begin(), pszPath, child.size()), data.cFileName);
					PathAppend(lstrcpyn(&child.at(0), pszPath, child.size()), data.cFileName);
					///if ((bRet = kjm::util::remove_folder(child.begin())) == false) {
					if ((bRet = kjm::util::remove_folder(&child.at(0))) == false) {
						break;
					}
				}
			} else {
				std::vector<TCHAR> fname(MAX_PATH);
				///PathAppend(lstrcpyn(fname.begin(), pszPath, fname.size()), data.cFileName);
				PathAppend(lstrcpyn(&fname.at(0), pszPath, fname.size()), data.cFileName);

				// �폜����t�@�C���� readonly �����Ă���Ƃ��͗��Ƃ��B
				///DWORD dwAttr = GetFileAttributes(fname.begin());
				DWORD dwAttr = GetFileAttributes(&fname.at(0));
				if ((dwAttr != 0xffffffff) && ((dwAttr & FILE_ATTRIBUTE_READONLY) != 0)) {
					///if (!SetFileAttributes(fname.begin(), dwAttr & ~FILE_ATTRIBUTE_READONLY)) {
					if (!SetFileAttributes(&fname.at(0), dwAttr & ~FILE_ATTRIBUTE_READONLY)) {
						bRet = false;
						break;
					}
				}

				///if (!DeleteFile(fname.begin())) {
				if (!DeleteFile(&fname.at(0))) {
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


//=============================================================================
// �f�B���N�g����T�u�f�B���N�g����ʂ��āA�쐬�A�ړ��A����ї񋓂��邽�߂�
// �ÓI���\�b�h�����J���܂��B
//=============================================================================
//-----------------------------------------------------------------------------
// �t�@�C���܂��̓f�B���N�g���A����т��̓��e��V�����ꏊ�ɃR�s�[���܂��B
//-----------------------------------------------------------------------------
BOOL kjm::directory::copy(LPCTSTR sourceDirName, LPCTSTR destDirName) {
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
			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
				if ((::lstrcmp(data.cFileName, _T(".")) != 0) && (::lstrcmp(data.cFileName, _T("..")) != 0)) {
					// �t�H���_���ċA�I�ɃR�s�[����B
					if (kjm::directory::copy(
							kjm::PathString(sourceDirName).Append(data.cFileName).c_str(),
							kjm::PathString(destDirName).Append(data.cFileName).c_str()) == FALSE) {

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


//=============================================================================
// ���W�X�g���������N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �R�s�[�R���X�g���N�^
//-----------------------------------------------------------------------------
kjm::registry::registry(const kjm::registry& src) {
	if (src.m_hkey == NULL) {
		this->m_hkey = NULL;
	} else {
		::DuplicateHandle(::GetCurrentProcess(), src.m_hkey, ::GetCurrentProcess(), (LPHANDLE)&this->m_hkey, 0, TRUE, DUPLICATE_SAME_ACCESS);
	}
}

//-----------------------------------------------------------------------------
// RegOpenKeyEx �̃��b�p�[
//-----------------------------------------------------------------------------
LONG kjm::registry::reg_open_key_ex(HKEY hkey, LPCTSTR lpSubKey, REGSAM samDesired) {
	assert(m_hkey == NULL);

	return ::RegOpenKeyEx(hkey, lpSubKey, 0, samDesired, &m_hkey);
}

//-----------------------------------------------------------------------------
// RegQueryValueEx �̃��b�p�[
//-----------------------------------------------------------------------------
LONG kjm::registry::reg_query_value_ex(LPTSTR lpValueName, LPDWORD lpType, LPBYTE lpData, LPDWORD lpcbData) const {
	assert(m_hkey != NULL);

	return ::RegQueryValueEx(m_hkey, lpValueName, NULL, lpType, lpData, lpcbData);
}

//-----------------------------------------------------------------------------
// RegCloseKey �̃��b�p�[
//-----------------------------------------------------------------------------
LONG kjm::registry::reg_close_key() {
	LONG result = ERROR_SUCCESS;
	if (m_hkey != NULL) {
		result = ::RegCloseKey(m_hkey);
		m_hkey = NULL;
	}
	return result;
}

//-----------------------------------------------------------------------------
// ��������擾����֐�
//
// �l�̃^�C�v��
//   REG_SZ �̂Ƃ��́A���̂܂ܕԂ��B
//   REG_EXPAND_SZ �̂Ƃ��́AExpandEnvironmentStrings ��ʂ��Ă���Ԃ��B
//   REG_DWORD �̂Ƃ��́A_ultoa ��ʂ��Ă���Ԃ��B
//-----------------------------------------------------------------------------
std::string kjm::registry::get_string(LPTSTR lpValueName, LPCTSTR lpDefault) const {
	assert(lpDefault != NULL);

	std::string result(lpDefault);	// �f�t�H���g�l�Ŗ߂�l���쐬���Ă���

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
	std::vector<char> buf(cbData);
	///if (reg_query_value_ex(lpValueName, &dwType, (LPBYTE)buf.begin(), &cbData) != ERROR_SUCCESS) {
	if (reg_query_value_ex(lpValueName, &dwType, (LPBYTE)&buf.at(0), &cbData) != ERROR_SUCCESS) {
		return result;	// �֐��Ɏ��s�����̂Ńf�t�H���g�l��߂��ďI��
	}

	// �f�[�^�̎�ނɉ����āA�߂�l���쐬
	switch (dwType) {
	case REG_SZ:
		///result = buf.begin();
		result = &buf.at(0);
		break;
		
	case REG_EXPAND_SZ:
		///result = win32::expand_environment_strings(buf.begin());
		result = win32::expand_environment_strings(&buf.at(0));
		break;

	case REG_DWORD:
		{
			char temp[32];	// u_long �̍ő�l���\���ɕ\���ł���T�C�Y
			///result = _ultoa(*(LPDWORD)buf.begin(), temp, 10);
			result = _ultoa(*(LPDWORD)&buf.at(0), temp, 10);
		}
		break;
	}

	return result;	// �擾�����������߂�
}

//-----------------------------------------------------------------------------
// ������Z�q
//-----------------------------------------------------------------------------
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


//=============================================================================
// �C�x���g���O�ɃA�N�Z�X���邽�߂̃N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �C�x���g���O���o�b�N�A�b�v����
//-----------------------------------------------------------------------------
BOOL kjm::event_log::backup(LPCTSTR sourceName, LPCTSTR backupFileName) {
	HANDLE hEventLog = ::OpenEventLog(NULL, sourceName);
	if (hEventLog == NULL)
		return FALSE;

	BOOL ret = ::BackupEventLog(hEventLog, backupFileName);

	::CloseEventLog(hEventLog);

	return ret;
}


//=============================================================================
// �v���Z�X�̋N���Ɋւ���N���X
//=============================================================================
//-----------------------------------------------------------------------------
// GetProcessWindow�֐��Ŏg�p����R�[���o�b�N
//
// [����]
// HWND hwnd		�񋓂��ꂽ�E�B���h�E�ւ̃n���h��
// LPARAM lParam	rkProcess�ւ̃|�C���^�B�L���X�g���Ďg��
//
// [���l]
// �񋓂��ꂽ�E�B���h�E�Ɠ����v���Z�XID�������̂��������ꍇ
// ���̃n���h���������o�[�ϐ��ɕۑ����A�񋓂��I������B
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �f�t�H���g�E�R���X�g���N�^
//-----------------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));
	memset(&m_szCommandLine, 0, sizeof(m_szCommandLine));
}

//-----------------------------------------------------------------------------
// �X�^�[�g�A�b�v���(wShowWindow)��ݒ肷��
//-----------------------------------------------------------------------------
void kjm::process::SetSIShowWindow(WORD newShowWindow) {
	m_si.wShowWindow = newShowWindow;
	m_si.dwFlags |= STARTF_USESHOWWINDOW;
}

//-----------------------------------------------------------------------------
// �X�^�[�g�A�b�v���(wShowWindow)���N���A(���g�p��)����
//-----------------------------------------------------------------------------
void kjm::process::ClearSIShowWindow() {
	m_si.wShowWindow = 0;
	m_si.dwFlags &= ~STARTF_USESHOWWINDOW;
}

//-----------------------------------------------------------------------------
// �v���Z�X�̎��s
//-----------------------------------------------------------------------------
BOOL kjm::process::createProcess(const std::basic_string<TCHAR>& commandLine) {
	assert(m_pi.hProcess == NULL);

	if (m_pi.hProcess)	return FALSE;	// ���łɉ������s���Ă���

	lstrcpy(m_szCommandLine, commandLine.c_str());
	m_si.cb = sizeof(m_si);

	return CreateProcess(NULL, m_szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);
}

//-----------------------------------------------------------------------------
// �v���Z�X�n���h�������
//
// [����]
// �v���Z�X�n���h�������B�v���Z�X�̏I���ł͂Ȃ��̂Œ��ӁB
// �v���Z�X�̏I���́A�v���Z�X�Ԃ̎�茈�߂ɂ��B
//-----------------------------------------------------------------------------
void kjm::process::Close() {
	if (m_pi.hThread)	CloseHandle(m_pi.hThread);
	if (m_pi.hProcess)	CloseHandle(m_pi.hProcess);
	memset(&m_pi, 0, sizeof(m_pi));
}

//-----------------------------------------------------------------------------
// �v���Z�X�Ɋ֘A�t����ꂽ�E�B���h�E��Ԃ�
//-----------------------------------------------------------------------------
HWND kjm::process::GetProcessWindow() {
	assert(m_pi.hProcess);

	if (!m_pi.hProcess)	return NULL;

	m_hwnd = NULL;
	EnumWindows(kjm::process::process_EnumWindowsProc, reinterpret_cast<LPARAM>(this));
	return m_hwnd;
}
