//---------------------------------------------------------------------
// kjmlib : Version 0.2011.6.7
//
// �v���R���p�C���ς݃w�b�_���g�p���Ȃ��悤�ɐݒ肵�Ă��������B
//
// r.kojima
//---------------------------------------------------------------------
#include "kjmlib.h"

static bool s_verboseMode = false;

void kjm::setVerboseMode(bool mode) { s_verboseMode = mode; }
bool kjm::getVerboseMode() { return s_verboseMode; }

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

//-----------------------------------------------------------------------------
// �V���[�g�J�b�g�̍쐬
//
// CoInitialize(NULL)/CoUninitialize()�́A�A�v���P�[�V�������s���Ă��������B
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �����t�@�C�����폜�o�� DeleteFile 
//-----------------------------------------------------------------------------
bool kjm::util::deleteFiles(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& wildcard) {
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
	} while (::FindNextFile(hFind, &findData));

	::FindClose(hFind);

	return result;
}

//-----------------------------------------------------------------------------
// ����t�H���_�ւ̃p�X���擾����
//
// nFolder �̒l�́Ashlobj.h �ɒ�`���Ă���ACSIDL_�Ŏn�܂�萔���w��B
// ��\�I�Ȃ��̂Ƃ��āA
// CSIDL_STARTUP ... ���[�U�[���Ƃ̃X�^�[�g�A�b�v
// CSIDL_COMMON_STARTUP ... All Users�̃X�^�[�g�A�b�v
//
// ���Ȃ��Ƃ��Avs2008����L���Ȋ֐�
//-----------------------------------------------------------------------------
#if _WIN32_IE >= 0x0700
std::basic_string<TCHAR> kjm::util::getSpecialFolderLocation(int nFolder) {
	std::vector<TCHAR> path(MAX_PATH);
	SHGetFolderPath(NULL, nFolder, NULL, SHGFP_TYPE_CURRENT, &path[0]);
	return &path[0];
}
#endif

//-----------------------------------------------------------------------------
// %TEMP%�t�H���_���擾����
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::getTempPath() {
	std::vector<TCHAR> path(MAX_PATH);
	::GetTempPath(path.size(), &path[0]);
	return &path[0];
}

//-----------------------------------------------------------------------------
// �w��̋��L�t�H���_�Ƌ��L�p�X�̑��݂�ۏ؂���
//-----------------------------------------------------------------------------
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
	ret = NetShareGetInfo(NULL, (LPTSTR)wstrSharedFolder.c_str(), 2, (LPBYTE*)&pInfo);
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
		info.shi2_netname = (LPTSTR)wstrSharedFolder.c_str();
		info.shi2_type = STYPE_DISKTREE;
		info.shi2_remark = (LPTSTR)L"";
		info.shi2_permissions = ACCESS_ALL;
		info.shi2_max_uses = -1;
		info.shi2_current_uses = 0;
		info.shi2_path = (LPTSTR)wstrSharedPath.c_str();
		info.shi2_passwd = (LPTSTR)L"";
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

//-----------------------------------------------------------------------------
// �t�@�C�����������擾
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::findFileName(const std::basic_string<TCHAR>& path) {
	return ::PathFindFileName(path.c_str());
}

//-----------------------------------------------------------------------------
// �p�X������Ƀp�X��ǉ�
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::append(const std::basic_string<TCHAR>& path, const std::basic_string<TCHAR>& more) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathAppend(::lstrcpy(&work.at(0), path.c_str()), more.c_str());
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

//-----------------------------------------------------------------------------
// �J�����g�f�B���N�g����ύX����
//-----------------------------------------------------------------------------
bool kjm::util::setCurrentDirectory(const std::basic_string<TCHAR>& pathName) {
#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		Winssa_Log(_T("kjmlog"), LOGLV_INFO, _T("<%s> kjm::util::setCurrentDirectory(): �J�����g�f�B���N�g���� '%s' �ɕύX���܂��B"),
			kjm::util::GetModuleName().c_str(), pathName.c_str());
	}
#endif
	BOOL bRet = ::SetCurrentDirectory(pathName.c_str());
#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		if (bRet) {
			Winssa_Log(_T("kjmlog"), LOGLV_INFO,
				_T("<%s> kjm::util::setCurrentDirectory(): �ύX���܂����B"), kjm::util::GetModuleName().c_str());
		} else {
			DWORD dwErr = ::GetLastError();
			Winssa_Log(_T("kjmlog"), LOGLV_INFO,
				_T("<%s> kjm::util::setCurrentDirectory(): error %d"), kjm::util::GetModuleName().c_str(), dwErr);
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
	while (_vsntprintf(&buffer.at(0), buffer.size(), format, marker) == -1) {
		buffer.resize(buffer.size() * 2);
	}
	va_end(marker);
	return &buffer.at(0);
}

//-----------------------------------------------------------------------------
// �����𕶎���ɕϊ�
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::to_a(int value, int radix/* = 10*/) {
	TCHAR str[80];

#if _WIN32_IE >= 0x0700
	return (_itot_s(value, str, _countof(str), radix) == 0) ? str : _T("");
#else
	return _itot(value, str, radix);
#endif
}

//-----------------------------------------------------------------------------
// �����𕶎���ɕϊ�(printf�����̃t�H�[�}�b�g���w��ł���)
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::to_a(int value, const std::basic_string<TCHAR>& format) {
	return kjm::util::sprintf_str(format.c_str(), value);
}

//-----------------------------------------------------------------------------
// �p�X������̏I�[ \ ���폜����
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::removeBackslash(const std::basic_string<TCHAR>& path) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveBackslash(::lstrcpy(&work.at(0), path.c_str()));
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// �p�X������̃t�@�C�������폜����
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::util::removeFileSpec(const std::basic_string<TCHAR> &path) {
	std::vector<TCHAR> work(MAX_PATH);
	::PathRemoveFileSpec(::lstrcpy(&work.at(0), path.c_str()));
	return &work.at(0);
}

//-----------------------------------------------------------------------------
// �t�@�C���̗L�����m�F����
//-----------------------------------------------------------------------------
bool kjm::util::fileExists(const std::basic_string<TCHAR> &path) {
	return (::PathFileExists(path.c_str()) != FALSE) ? true : false;
}

//=============================================================================
// CSV �t�@�C���������N���X
//=============================================================================
//-----------------------------------------------------------------------------
// CSV �t�@�C�����J��
//-----------------------------------------------------------------------------
bool kjm::csvFile::open(LPCTSTR pszFileName) {
	if (!m_fr.is_open()) {
		return false;
	}

	m_fr.open(pszFileName);
	if (!m_fr.is_open()) {
		return false;
	}
	return true;
}

//-----------------------------------------------------------------------------
// CSV �t�@�C�������
//-----------------------------------------------------------------------------
void kjm::csvFile::close() {
	if (!m_fr.is_open()) {
		m_fr.close();
	}
}

//-----------------------------------------------------------------------------
// CSV �t�@�C������ 1 �s�ǂݍ���
//
// [���l]
// "...","...","..." �̌`�������T�|�[�g���Ă��܂���!!
//-----------------------------------------------------------------------------
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
	for (int l = 0; l < cols.size(); l++) {
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
//-----------------------------------------------------------------------------
// �\�P�b�g���J��
//-----------------------------------------------------------------------------
bool kjm::baseSocket::openSocket(int af, int type, int protocol) {
	if (this->m_sock != INVALID_SOCKET) {
		return false;
	}

	this->m_sock = socket(af, type, protocol);

	return (this->m_sock != INVALID_SOCKET) ? true : false;
}

//-----------------------------------------------------------------------------
// ��M�^�C���A�E�g�̐ݒ�
//-----------------------------------------------------------------------------
bool kjm::baseSocket::setRecvTimeout(int msTimeout) {
	return (setsockopt(this->m_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&msTimeout, sizeof(msTimeout)) == 0) ? true : false;
}

//-----------------------------------------------------------------------------
// �o�C���h����(�|�[�g�ԍ����w�肷��ȒP�o�[�W������������)
//-----------------------------------------------------------------------------
bool kjm::baseSocket::bindSocket(unsigned short port) {
	struct sockaddr_in me;
	memset(&me, 0, sizeof(me));
	me.sin_family = AF_INET;
	me.sin_addr.S_un.S_addr = INADDR_ANY;
	me.sin_port = htons(port);
	return (bind(this->m_sock, (sockaddr*)&me, sizeof(me)) == 0) ? true : false;
}

//-----------------------------------------------------------------------------
// ��M����
//-----------------------------------------------------------------------------
int kjm::baseSocket::recvfrom(std::vector<char>& buf, sockaddr_in& from) {
	int fromlen = sizeof(from);
	buf.resize(8192);
	int ret = ::recvfrom(this->m_sock, &buf[0], buf.size(), 0, (sockaddr*)&from, &fromlen);
	if (ret > 0) {
		buf.resize(ret);
	}
	return ret;
}

//-----------------------------------------------------------------------------
// ���M����
//-----------------------------------------------------------------------------
int kjm::baseSocket::sendto(const std::vector<char>& buf, const sockaddr_in& to) {
	return ::sendto(this->m_sock, &buf[0], buf.size(), 0, (sockaddr*)&to, sizeof(to));
}

//-----------------------------------------------------------------------------
// �\�P�b�g�����
//-----------------------------------------------------------------------------
bool kjm::baseSocket::closeSocket() {
	bool result = true;

	if (this->m_sock != INVALID_SOCKET) {
		result = (closesocket(this->m_sock) == 0) ? true : false;
		this->m_sock = INVALID_SOCKET;
	}

	return result;
}

//-----------------------------------------------------------------------------
// Winsock������
//-----------------------------------------------------------------------------
int kjm::baseSocket::startup() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 0), &data);
}

//=============================================================================
// �^�X�N�g���C�A�C�R���N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �f�t�H���g�E�R���X�g���N�^
//-----------------------------------------------------------------------------
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
	Winssa_Log(signature, LOGLV_INFO, "%s", &buffer[0]);
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
	Winssa_Log(signature, LOGLV_ERR, "%s", &buffer[0]);
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


//-----------------------------------------------------------------------------
// ���[�N�O���[�v�����擾����
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// Unicode ������� ANSI ������ɕϊ�
//-----------------------------------------------------------------------------
std::string kjm::util::toAnsi(const std::wstring& src) {
	DWORD dwSizeNeed = ::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	std::vector<char> buf(dwSizeNeed);

	::WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size(), NULL, NULL);

	return &buf[0];
}

//-----------------------------------------------------------------------------
// ANSI ������� Unicode ������ɕϊ�
//-----------------------------------------------------------------------------
std::wstring kjm::util::toUnicode(const std::string& src) {
	DWORD dwSizeNeed = ::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	std::vector<wchar_t> buf(dwSizeNeed);

	::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size());

	return &buf[0];
}

//-----------------------------------------------------------------------------
// �w��t�H���_�ȉ��̓ǂݍ��ݐ�p�������O��
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �f�B���N�g�����̃t�@�C����񋓂���
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �f�B���N�g�����̃T�u�f�B���N�g����񋓂���
//-----------------------------------------------------------------------------
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

#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: '%s' �����s���܂��B"),
			kjm::util::GetModuleName().c_str(), m_szCommandLine);
	}
#endif

	BOOL bRet = CreateProcess(NULL, m_szCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &m_si, &m_pi);

#if defined(USE_WSSLOG)
	if (kjm::getVerboseMode()) {
		if (bRet) {
			Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: ���s���܂����B"),
				kjm::util::GetModuleName().c_str(), m_szCommandLine);
		} else {
			DWORD dwErr = ::GetLastError();
			Winssa_Log(_T("kjmlib"), LOGLV_INFO, _T("<%s> kjm::process::createProcess: error %d"),
				kjm::util::GetModuleName().c_str(), dwErr);
		}
	}
#endif
	return bRet;
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
// �v���Z�X�̏I���R�[�h���擾����
//-----------------------------------------------------------------------------
DWORD kjm::process::getExitCode() {
	DWORD exitCode = 0;
	::GetExitCodeProcess(this->m_pi.hProcess, &exitCode);
	return exitCode;
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

//=============================================================================
// �R�}���h���C����̓N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �R�}���h���C�����
//-----------------------------------------------------------------------------
int kjm::cmdLine::parse(int argc, TCHAR** argv) {
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			TCHAR c = argv[i][1];	// �I�v�V��������
			if (argv[i][2] != 0) {
				// �I�v�V�����̈����������Ă���ꍇ
				this->m_options[std::basic_string<TCHAR>(1, c)] = &argv[i][2];
			} else if ((argv[i][2] == 0) && (i+1 < argc) && (argv[i+1][0] != '-')) {
				// �I�v�V�����̈��������� argv �ɑ����Ă���ꍇ
				this->m_options[std::basic_string<TCHAR>(1, c)] = argv[++i];
			} else {
				// �I�v�V�����ɂ͈����������ꍇ
				this->m_options[std::basic_string<TCHAR>(1, c)] = _T("");
			}
		} else {
			this->m_arguments.push_back(argv[i]);
		}
	}
	return 0;
}

//-----------------------------------------------------------------------------
// �I�v�V�����̈������擾����
//-----------------------------------------------------------------------------
std::basic_string<TCHAR> kjm::cmdLine::getOptArgument(const std::basic_string<TCHAR>& key) {
	return (this->hasOption(key) ? this->m_options[key] : _T(""));
}

