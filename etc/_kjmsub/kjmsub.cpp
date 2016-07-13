//-----------------------------------------------------------------------------
// kjmsub ver.0.2010.7.30
//-----------------------------------------------------------------------------
#include "kjmsub.h"

// kjmsub ���̓��샍�O���c�����߂̃��O�I�u�W�F�N�g���w�肷��B
// kjm::util::set_logObj ���ĂԂ��ƂŁA�����g���[�X�����邱�Ƃ��ł���悤�ɂȂ�B

static kjm::log* g_kjmlog = NULL;

//-----------------------------------------------------------------------------
// kjmsub ��������L�^���邽�߂̃��O�I�u�W�F�N�g��ݒ�
//-----------------------------------------------------------------------------
void kjm::util::set_logObj(kjm::log* pLogObj) {
	g_kjmlog = pLogObj;
}

//-----------------------------------------------------------------------------
// __int64 �� iostream �ɏo�͂��邽�߂̊֐�
//-----------------------------------------------------------------------------
#if _MSC_VER < 1500		// ���Ȃ��Ƃ��Avs2008����́A__int64���󂯎��ostream�����݂���̂ŁA�����ɂ���B
_otstream& operator<<(_otstream& os, __int64 i) {
	TCHAR buf[20];
	_stprintf(buf, _T("%I64d"), i);
	os << buf;
	return os;
}
#endif


#if _MSC_VER < 1500
errno_t ctime_s(char* buffer, size_t numberOfElements, const time_t *time) {
	strcpy(buffer, ::ctime(time));
	return 0;
}

errno_t _wctime_s(wchar_t* buffer, size_t numberOfElements, const time_t *time) {
	wcscpy(buffer, ::_wctime(time));
	return 0;
}

char* strtok_s(char* strToken, const char* strDelimit, char** context) {
	return strtok(strToken, strDelimit);
}

wchar_t* wcstok_s(wchar_t* strToken, const wchar_t* strDelimit, wchar_t** context) {
	return wcstok(strToken, strDelimit);
}

int _vsnprintf_s(char *buffer, size_t sizeOfBuffer, size_t count, const char *format, va_list argptr) {
	return _vsnprintf(buffer, sizeOfBuffer, format, argptr);
}

int _vsnwprintf_s(wchar_t *buffer, size_t sizeOfBuffer, size_t count, const wchar_t *format, va_list argptr) {
	return _vsnwprintf(buffer, sizeOfBuffer, format, argptr);
}

errno_t localtime_s(struct tm* _tm, const time_t *time) {
	*_tm = *localtime(time);
	return 0;
}

errno_t fopen_s(FILE** pFile, const char *filename, const char *mode) {
//	*pFile = fopen(filename, mode);
	return (*pFile != NULL) ? 0 : errno;
}

errno_t _wfopen_s(FILE** pFile, const wchar_t *filename, const wchar_t *mode) {
//	*pFile = _wfopen(filename, mode);
	return (*pFile != NULL) ? 0 : errno;
}
#endif


//-----------------------------------------------------------------------------
//	�f�B���N�g�����̃t�@�C���ꗗ���擾
//-----------------------------------------------------------------------------
bool kjm::dir(LPCTSTR pszPath, LPCTSTR pszWildcard, std::vector<_tstring>& result) {
	std::vector<_tstring> tempResult;
	TCHAR wc[MAX_PATH];

	PathAppend(lstrcpy(wc, pszPath), pszWildcard);

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(wc, &data);
	if (hFind == INVALID_HANDLE_VALUE) {
		return false;
	}

	do {
		tempResult.push_back(data.cFileName);
	} while (FindNextFile(hFind, &data));

	FindClose(hFind);

	result.insert(result.end(), tempResult.begin(), tempResult.end());

	return true;
}

//=============================================================================
// �R�}���h���C���������
//
// @arg  hasArgShortOpts  �����������Ƃ�����V���[�g�I�v�V������񋓂���������ւ̃|�C���^
//=============================================================================
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
// �C�x���g���O���Ǘ�����N���X(class eventLog)
//=============================================================================
//-----------------------------------------------------------------------------
// �C�x���g���O���J��
//-----------------------------------------------------------------------------
BOOL kjm::eventLog::openEventLog(LPCTSTR lpUNCServerName, LPCTSTR lpSourceName) {
	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	closeEventLog();

	m_uncServerName = (lpUNCServerName) ? lpUNCServerName : _T("");
	m_sourceName = (lpSourceName) ? lpSourceName : _T("");

	m_hEventLog = ::OpenEventLog(lpUNCServerName, lpSourceName);
	
	return (m_hEventLog) ? TRUE : FALSE;
}


//-----------------------------------------------------------------------------
// �ۑ������C�x���g���O���J��
//-----------------------------------------------------------------------------
BOOL kjm::eventLog::OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName ) {

	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	closeEventLog();

	m_uncServerName = ( lpUNCServerName ) ? lpUNCServerName : _T("");
	m_sourceName = ( lpSourceName ) ? lpSourceName : _T("");
	m_fileName = ( lpFileName ) ? lpFileName : _T("");

	m_hEventLog = ::OpenBackupEventLog( lpUNCServerName, lpFileName );
	return ( m_hEventLog ) ? TRUE : FALSE;
}


//-----------------------------------------------------------------------------
// �C�x���g���O�����
//-----------------------------------------------------------------------------
BOOL kjm::eventLog::closeEventLog() {
	BOOL ret = TRUE;
	if (m_hEventLog) {
		ret = ::CloseEventLog(m_hEventLog);
		m_hEventLog = NULL;
	}
	return ret;
}


//-----------------------------------------------------------------------------
// �C�x���g���O��ǂ�
//-----------------------------------------------------------------------------
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


//=============================================================================
// 1���R�[�h����eventlog������킷�N���X(class eventLogRecord)
//=============================================================================
//-----------------------------------------------------------------------------
// �R���X�g���N�^
//-----------------------------------------------------------------------------
kjm::eventLogRecord& kjm::eventLogRecord::operator= ( const kjm::eventLogRecord& src ) {
	m_source_name = src.m_source_name;
	m_raw_record = src.m_raw_record;

	return *this;
}


//-----------------------------------------------------------------------------
// �C�x���g���O�̐��������ɂ���R���X�g���N�^
//-----------------------------------------------------------------------------
kjm::eventLogRecord::eventLogRecord(LPCTSTR lpSourceName, const EVENTLOGRECORD* src) : m_source_name(lpSourceName) {
	// �����̃R�s�[
	m_raw_record.resize(src->Length);
	memmove(&m_raw_record[0], src, src->Length);
}


//-----------------------------------------------------------------------------
// �����ݕ�����̎擾
//-----------------------------------------------------------------------------
int kjm::eventLogRecord::GetArgs(const EVENTLOGRECORD *pBuf, std::vector<LPTSTR>& args) {
	args.clear();

	LPTSTR cp;
	WORD ArgCount;

	if(pBuf->NumStrings == 0) return args.size();

	/* �������X�g���擾 */
	args.resize( pBuf->NumStrings );
	cp = (LPTSTR)pBuf + (pBuf->StringOffset);

	for(ArgCount=0; ArgCount<pBuf->NumStrings; ArgCount++) {
		args[ArgCount] = cp;
		cp += lstrlen(cp) + 1;
	}
	return args.size();
}

//-----------------------------------------------------------------------------
// �\�[�X�����烂�W���[�������擾
//-----------------------------------------------------------------------------
BOOL kjm::eventLogRecord::GetModuleNameFromSourceName(LPCTSTR SourceName, LPCTSTR EntryName, std::vector<TCHAR>& ExpandedName) {
	DWORD lResult;
	std::vector<BYTE> ModuleName;
	BOOL bReturn = FALSE;

	// Application���O�p�̃��W�X�g���L�[���I�[�v��
	kjm::registry appKey;
	
	_tstring key = _tstring(_T("SYSTEM\\CurrentControlSet\\Services\\EventLog\\"));
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
				///////kjm::util::expandEnvironmentStrings((LPCTSTR)&ModuleName[0], ExpandedName );

				/* ����I�� */
				bReturn = TRUE;
			}
		}
	}

	return bReturn;
}

//-----------------------------------------------------------------------------
// ���b�Z�[�W�̕\��
//-----------------------------------------------------------------------------
_tstring kjm::eventLogRecord::DispMessage(LPCTSTR SourceName, LPCTSTR EntryName, TCHAR **Args, DWORD MessageId) {
	BOOL bResult;
	BOOL bReturn = FALSE;
	std::vector<TCHAR> SourceModuleName;
	std::vector<TCHAR> message;
	LPTSTR oneModuleName;
	DWORD dwRet;

	/* �\�[�X���W���[�������擾 */	
	bResult = GetModuleNameFromSourceName(SourceName, EntryName, SourceModuleName);
	if(!bResult) return _tstring(_T(""));

	TCHAR* context = NULL;
	oneModuleName = _tcstok_s(&SourceModuleName[0], _T(";"), &context);
	while ( oneModuleName ) {

		kjm::library sourceModule;

		// �\�[�X���W���[�������[�h
		sourceModule.LoadLibraryEx(
			oneModuleName,
			NULL,
			DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

		if ( sourceModule.getInstance() ) {

			/* ���b�Z�[�W���쐬 */
			dwRet = kjm::util::formatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
				sourceModule.getInstance() ,
				MessageId,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				message,
				(va_list *)Args);


			/* ����I�� */
			if ( dwRet != 0 ) {
				bReturn = TRUE;
				break;
			}
		}

		oneModuleName = _tcstok_s(NULL, _T(";"), &context);
	}

	if(message.size() > 0) return _tstring(&message[0]);
	else {
		TCHAR buf[32];
		wsprintf(buf, _T("(%d)\n"), MessageId);
		return _tstring( buf );
	}
}

//-----------------------------------------------------------------------------
// �C�x���g���O�̃R���s���[�^�����擾
//-----------------------------------------------------------------------------
_tstring kjm::eventLogRecord::ComputerName() {
	LPTSTR cp = (LPTSTR)getRaw();
	cp += sizeof(EVENTLOGRECORD);

	cp += lstrlen( cp ) + 1;		// SourceName���|�C���^��i�߂�

	return _tstring( cp );
}

//-----------------------------------------------------------------------------
// �C�x���g���O�̃f�[�^���擾
//-----------------------------------------------------------------------------
std::vector<BYTE> kjm::eventLogRecord::Data() {

	std::vector<BYTE> data( DataLength() );

	for (std::vector<BYTE>::size_type i = 0; i < data.size(); i++) {
		data.at( i ) = *(((unsigned char *)getRaw()) + DataOffset() + i);
	}

	return data;
}


//=============================================================================
// �t�@�C���̈�ʓI�ȑ����񋟂���N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �t�@�C�����폜����
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �t�@�C���̑������擾����B
//-----------------------------------------------------------------------------
DWORD kjm::file::getAttributes(const _tstring& path) {
	DWORD dwAttr = ::GetFileAttributes(path.c_str());
	if (dwAttr == INVALID_FILE_ATTRIBUTES) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("�t�@�C�� '") << path << _T("' �̑������擾�ł��܂���B") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}

	return dwAttr;
}

//-----------------------------------------------------------------------------
// �t�@�C���̑�����ݒ肷��B
//-----------------------------------------------------------------------------
void kjm::file::setAttributes(const _tstring& path, DWORD fa) {
	if (::SetFileAttributes(path.c_str(), fa) == FALSE) {
		DWORD dwErr = ::GetLastError();
		_otstringstream oss;
		oss << _T("�t�@�C�� '") << path << _T("' �̑�����ݒ�ł��܂���B") << kjm::util::formatMessageBySystem(dwErr);
		throw kjm::exception::exception(dwErr, oss.str());
	}
}


//=============================================================================
// �t�@�C���̈�ʓI�ȑ����񋟂���N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ쐬����
//-----------------------------------------------------------------------------
void kjm::directory::createDirectory(const _tstring& path) {
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
//-----------------------------------------------------------------------------
// �f�B���N�g�������̃t�@�C���ꗗ���擾
//-----------------------------------------------------------------------------
std::vector<kjm::fileInfo> kjm::folderInfo::getFiles(const _tstring& strWC/* = _T("*.*")*/) {
	std::vector<kjm::fileInfo> result;

	findFile ff;
	if (ff.findFirstFile(kjm::path::append(m_fullPath, strWC))) {
		do {
			if (!ff.isDirectory()) {
				WIN32_FIND_DATA findData;
				ff.copyWin32FindData(findData);
				result.push_back(kjm::fileInfo(m_fullPath, findData));
			}
		} while (ff.findNextFile());
		ff.findClose();
	}

	return result;
}

//-----------------------------------------------------------------------------
// �f�B���N�g�������̃f�B���N�g���ꗗ���擾
//-----------------------------------------------------------------------------
std::vector<kjm::folderInfo> kjm::folderInfo::getSubFolders() {
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
};


//=============================================================================
// ini�t�@�C������������N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �Z�N�V�����ꗗ���擾
//-----------------------------------------------------------------------------
std::vector<_tstring> kjm::iniFile::getSectionNames() const {
	assert(!m_fileName.empty());

	std::vector<_tstring> result;
	std::vector<TCHAR> work;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileSectionNames(&work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 2);

	std::vector<TCHAR>::iterator sp = work.begin();
	std::vector<TCHAR>::iterator ep;
	while ((ep = std::find(sp, work.end(), '\0')) != sp) {
		result.push_back(_tstring(sp, ep));
		sp = ep + 1;
	}

	return result;
}

//-----------------------------------------------------------------------------
// �Z�N�V�������̃L�[�ꗗ���擾
//-----------------------------------------------------------------------------
std::vector<_tstring> kjm::iniFile::getKeyNames(const _tstring& strSectionName) const {
	assert(!m_fileName.empty());

	std::vector<_tstring> result;
	std::vector<TCHAR> work;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileString(strSectionName.c_str(), NULL, _T(""), &work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 2);

	std::vector<TCHAR>::iterator sp = work.begin();
	std::vector<TCHAR>::iterator ep;
	while ((ep = std::find(sp, work.end(), '\0')) != sp) {
		result.push_back(_tstring(sp, ep));
		sp = ep + 1;
	}

	return result;
}

//-----------------------------------------------------------------------------
// ini�t�@�C������l���擾
//-----------------------------------------------------------------------------
_tstring kjm::iniFile::getString(const _tstring& strSection, const _tstring& strKey, const _tstring& strDefault, BOOL *pbUseDefault/*=NULL*/) const {
	assert(!m_fileName.empty());

	std::vector<TCHAR> work;
	_tstring value1;

	DWORD dwRet;
	do {
		work.resize(work.size() + 128);
		dwRet = ::GetPrivateProfileString(strSection.c_str(), strKey.c_str(), strDefault.c_str(), &work[0], work.size(), m_fileName.c_str());
	} while (dwRet == work.size() - 1);

	value1 = &work[0];

	// �f�t�H���g���g�������ǂ����𔻒肷��
	if (pbUseDefault) {
		_tstring strDefault2 = _T("x") + strDefault;
		_tstring value2 = getString(strSection, strKey, strDefault2);

		*pbUseDefault = (value1 != value2) ? TRUE : FALSE;
	}

	return &work[0];
}

//-----------------------------------------------------------------------------
// ini�t�@�C���ɒl����������
//-----------------------------------------------------------------------------
BOOL kjm::iniFile::writeString(const _tstring& strSection, const _tstring& strKey, const _tstring& strValue) {
	assert(!m_fileName.empty());

	return ::WritePrivateProfileString(strSection.c_str(), strKey.c_str(), strValue.c_str(), this->m_fileName.c_str());
}

//-----------------------------------------------------------------------------
// �ʂ�ini�t�@�C���̓��e���}�[�W����
//-----------------------------------------------------------------------------
void kjm::iniFile::mergeIniFile(const iniFile& newIniFile) {
	assert(!m_fileName.empty());

	// �}�[�W����INI�t�@�C������A�S�Z�N�V������ǂݍ���
	std::vector<_tstring> sections = newIniFile.getSectionNames();
	for (std::vector<_tstring>::size_type i = 0; i < sections.size(); i++) {

		// �}�[�W����INI�t�@�C���̃Z�N�V��������A�S�L�[��ǂݍ���
		std::vector<_tstring> newKeys = newIniFile.getKeyNames(sections[i]);

		// �}�[�W�����INI(����)�t�@�C���̃Z�N�V����������S�L�[��ǂݍ���
		std::vector<_tstring> orgKeys = this->getKeyNames(sections[i]);

		for (std::vector<_tstring>::size_type j = 0; j < newKeys.size(); j++) {

			// �}�[�W�����INI(����)�t�@�C���ɓ����L�[�����݂��邩�ǂ������ׂ�
			// �}�[�W�����INI(����)�t�@�C���ɃL�[�����݂��Ȃ��ꍇ�A�V����INI�t�@�C������L�[�̒l����������
			// (�}�[�W�����INI(����)�t�@�C���ɓ����L�[�����݂���Ƃ��͉������Ȃ��̂ŁA���e���ۏႳ��銴��)

			std::vector<_tstring>::iterator p = std::find(orgKeys.begin(), orgKeys.end(), newKeys[j]);
			if (p == orgKeys.end()) {
				_tstring value = newIniFile.getString(sections[i], newKeys[j], _T(""));
				this->writeString(sections[i], newKeys[j], value);

				if (g_kjmlog) g_kjmlog->writeInfo(_T("[%s] %s=%s ���}�[�W�B"), sections[i].c_str(), newKeys[j].c_str(), value.c_str());
			}
		}
	}
}


//=============================================================================
// LoadLibraryEx/FreeLibrary��΂ɂ���N���X
//=============================================================================
//-----------------------------------------------------------------------------
// ���C�u�����̃��[�h
//-----------------------------------------------------------------------------
BOOL kjm::library::LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags ) {
	
	// ���łɃ��[�h���Ă��邩������Ȃ��̂ŁA�J��
	FreeLibrary();

	m_hLibModule = ::LoadLibraryEx( lpLibFileName, hFile, dwFlags );
	
	return ( m_hLibModule ) ? TRUE : FALSE;
}

//-----------------------------------------------------------------------------
// ���C�u�����̊J��
//-----------------------------------------------------------------------------
BOOL kjm::library::FreeLibrary() {
	BOOL ret = TRUE;

	if ( m_hLibModule ) {
		ret = ::FreeLibrary( m_hLibModule );
		m_hLibModule = NULL;
	}

	return ret;
}


//=============================================================================
// ���ʂ̃��O����
//=============================================================================
//---------------------------------------------------------------------
// �f�t�H���g�R���X�g���N�^
//---------------------------------------------------------------------
kjm::log::log() {

	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �e���|�����p�X���擾
	m_strLogPath = kjm::util::getTempPath();

	// ���O�t�@�C���̊�{�ɂȂ�t�@�C�������������s�t�@�C��������擾
	_tstring strFileSpec = kjm::path::findFileName(kjm::util::getModuleFileName());
	m_strPrefix = kjm::path::removeExtension(strFileSpec);

	// �t�@�C�����𐶐�����
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T("{YMD}.log"));
}

//-----------------------------------------------------------------------------
// �Â����O�̍폜
//-----------------------------------------------------------------------------
void kjm::log::deleteOldLog() {
#if defined(USE_WSSLOG)
	WLDeleteOldLog();
#endif
}

//-----------------------------------------------------------------------------
// ���O�V�X�e���̖��O��Ԃ�
//-----------------------------------------------------------------------------
_tstring kjm::log::get_MyName() {
	_tstring result;
#if defined(USE_WSSLOG)
	char buf[MAX_PATH * 2];
	WLGetMyName(buf, MAX_PATH * 2);
	result = kjm::util::toUnicode(buf);
#else
	result = _T("(���[�J��)");
#endif
	return result;
}

//-----------------------------------------------------------------------------
// ���O�V�X�e���̏������O�Ɏc��
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// ���O�t�@�C���̏o�͐��ݒ肷��
//-----------------------------------------------------------------------------
void kjm::log::set_log_path(const _tstring& strPath) {
#if defined(USE_WSSLOG)
	WLSetLogPath(kjm::util::toAnsi(strPath).c_str());
#else
	m_strLogPath = strPath;

	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �t�@�C�����𐶐�����
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T("{YMD}.log"));
#endif
}

//-----------------------------------------------------------------------------
// ���O�t�@�C���̏o�͐���擾����
//-----------------------------------------------------------------------------
_tstring kjm::log::get_log_path() {
#if defined(USE_WSSLOG)
	char strPath[MAX_PATH];
	WLGetLogPath(strPath, MAX_PATH);
	return kjm::util::toUnicode(strPath);
#else
	return m_strLogPath;
#endif
}

//-----------------------------------------------------------------------------
// ���O�t�@�C���̃v���t�B�N�X��ݒ肷��
//-----------------------------------------------------------------------------
void kjm::log::set_prefix(const _tstring& strPrefix) {
	m_strPrefix = strPrefix;

	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �t�@�C�����𐶐�����
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T("{YMD}.log"));
}

//-----------------------------------------------------------------------------
// �S���O
//-----------------------------------------------------------------------------
void kjm::log::writeAll(LPCTSTR pszFormat, ...) {
	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_ALWAYS, "%s", kjm::util::toAnsi(&buf[0]).c_str());
#endif
}

//-----------------------------------------------------------------------------
// �G���[���O
//-----------------------------------------------------------------------------
void kjm::log::writeError(LPCTSTR pszFormat, ...) {
	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_ERR, "%s", kjm::util::toAnsi(&buf[0]).c_str());
#endif
}

//-----------------------------------------------------------------------------
// ��񃍃O
//-----------------------------------------------------------------------------
void kjm::log::writeInfo(LPCTSTR pszFormat, ...) {
	std::vector<TCHAR> buf(256);
	va_list marker;
	va_start(marker, pszFormat);
	int result = kjm::util::vsprintf_vec(buf, pszFormat, marker);
	va_end(marker);

#if defined(USE_WSSLOG)
	Winssa_Log(kjm::util::toAnsi(get_Ident()).c_str(), LOGLV_INFO, "%s", kjm::util::toAnsi(&buf[0]));
#endif
}

//-----------------------------------------------------------------------------
// ���O�̏o�͊֐�
//-----------------------------------------------------------------------------
void kjm::log::write(LPCTSTR format, ...) {
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

	Winssa_Log(kjm::util::toAnsi(m_strIdent).c_str(), LOGLV_INFO, "%s", kjm::util::toAnsi(&work[0]).c_str());
#else
	va_list marker;
	va_start(marker, format);

	// ��{�t�@�C��������A���ۂ̃��O�t�@�C�����𐶐�����
	_tstring fname = kjm::util::strReplace(m_strBaseFileName, _T("{YMD}"), kjm::dateTime::getLocalTime().toString_yyyymmdd() );

	FILE* fp = NULL;
	for (int i = 0; i < 100; i++) {
		fp = _tfsopen(fname.c_str(), _T("a+"), _SH_DENYWR);
		if (fp) break;
		Sleep(0);
	}

	if (fp) {
		_ftprintf(fp, _T("%s "), kjm::dateTime::getLocalTime().toString().c_str());
		_vftprintf(fp, format, marker);
		_ftprintf(fp, _T("\n"));

		fclose(fp);
	}

	va_end(marker);
#endif
}

//-----------------------------------------------------------------------------
// ���O�̏o�͊֐�(��ʏo�͂���)
//-----------------------------------------------------------------------------
void kjm::log::writee(LPCTSTR format, ...) {

	va_list marker;
	std::vector<TCHAR> work;
	int n;

	va_start(marker, format);

	do {
		work.resize(work.size() + MAX_PATH);
		n = _vsntprintf_s(&work[0], work.size(), _TRUNCATE, format, marker);
	} while (n == -1 || n == work.size());

	va_end(marker);

	write(_T("%s"), &work[0]);

	// �W���o�͂ɂ��o��
	_tprintf(_T("%s\n"), &work[0]);
}

//=============================================================================
// ���W�X�g�����Ǘ�����N���X(class registry)
//=============================================================================
//-----------------------------------------------------------------------------
// ���W�X�g�����J��
//-----------------------------------------------------------------------------
LONG kjm::registry::RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired ) {

	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	RegCloseKey();

	LONG ret = ::RegOpenKeyEx( hKey, lpSubKey, ulOptions, samDesired, &m_hKey );

	return ret;
}

//-----------------------------------------------------------------------------
// ���W�X�g�������
//-----------------------------------------------------------------------------
LONG kjm::registry::RegCloseKey() {
	LONG ret = ERROR_SUCCESS;

	if ( m_hKey ) {
		ret = ::RegCloseKey( m_hKey );
		m_hKey = NULL;
	}

	return ret;
}

//-----------------------------------------------------------------------------
// ���W�X�g������l���擾
//-----------------------------------------------------------------------------
LONG kjm::registry::RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data ) {

	LONG ret;
	DWORD cbData = 0;

	// �K�v�ȃo�b�t�@�T�C�Y���擾
	ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, NULL, &cbData );
	if ( ret == ERROR_SUCCESS ) {

		// �o�b�t�@���m�ۂ��Đ����ɌĂяo��
		data.resize( cbData );
		ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, &data[0], &cbData );
	}

	return ret;
}



//=============================================================================
// ���Ԃ��ȒP�Ɉ����N���X
//=============================================================================
//-----------------------------------------------------------------------------
// ������Ŏ��Ԃ��w�肷��R���X�g���N�^
//-----------------------------------------------------------------------------
void kjm::dateTime::parse(LPCTSTR time_text) {
	int y, m, d, h, n, s;
#if _MSC_VER < 1500
	int ret = _stscanf(time_text, _T("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &n, &s);
#else
	int ret = _stscanf_s(time_text, _T("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &n, &s);
#endif

	tm t;
	memset(&t, 0, sizeof(t));

	t.tm_year = y - 1900;
	t.tm_mon = m - 1;
	t.tm_mday = d;
	t.tm_hour = h;
	t.tm_min = n;
	t.tm_sec = s;

	setTimeT(mktime(&t));
}

//-----------------------------------------------------------------------------
// kjm::dateTime�N���X���t�@�C���ɕۑ�
//-----------------------------------------------------------------------------
bool kjm::dateTime::save(FILE* fp) {
	time_t t = getTimeT();
	return (fwrite(&t, sizeof(t), 1, fp) == 1);
}

//-----------------------------------------------------------------------------
// kjm::dateTime�N���X���t�@�C������Ǎ�
//-----------------------------------------------------------------------------
bool kjm::dateTime::load(FILE* fp) {
	time_t t = getTimeT();
	return (fread(&t, sizeof(t), 1, fp) == 1);
}

//-----------------------------------------------------------------------------
// ������ɕϊ�
//-----------------------------------------------------------------------------
_tstring kjm::dateTime::toString() const {
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
	return oss.str();
}

//---------------------------------------------------------------------
// ���Ԃ𕶎���(YYYYMMDD)�ɕϊ�
//---------------------------------------------------------------------
_tstring kjm::dateTime::toString_yyyymmdd() const {
	SYSTEMTIME st;
	FileTimeToSystemTime(&m_ft, &st);

	_otstringstream oss;
	oss << std::setfill(_T('0'))
		<< st.wYear
		<< std::setw(2) << st.wMonth
		<< std::setw(2) << st.wDay;
	return oss.str();
}

//-----------------------------------------------------------------------------
// ���Ԃ��t�H�[�}�b�g���ĕ�����ɂ���
//-----------------------------------------------------------------------------
_tstring kjm::dateTime::strftime(LPCTSTR format) {
	TCHAR buf[512];
	time_t t = getTimeT();

	struct tm tmBuf;
	if (localtime_s(&tmBuf, &t) != 0)
		return _tstring();

	::_tcsftime(buf, sizeof(buf), format, &tmBuf);
	return _tstring(buf);
}

//=============================================================================
// �X���b�h�N���X�̒�`
//=============================================================================
// �܂��A����܂���B


//=============================================================================
// environment �N���X�̒�`
//=============================================================================
//-----------------------------------------------------------------------------
// �w�肵��������ɖ��ߍ��܂�Ă���e���ϐ��̖��O���A���̕ϐ��̒l��\��������
// �Œu�����A�u����̕�����S�̂�Ԃ��܂��B
//-----------------------------------------------------------------------------
_tstring kjm::environment::expandEnvironmentVariables(const _tstring& name) {
	std::vector<TCHAR> work(MAX_PATH);
	kjm::util::expandEnvironmentStrings(name.c_str(), work);
	return &work[0];
}

//=============================================================================
// ���[�e�B���e�B�֐��p��static�N���X
//=============================================================================
//-----------------------------------------------------------------------------
// Unicode ������� Ansi ������ɕϊ�
//-----------------------------------------------------------------------------
std::string kjm::util::toAnsi(const std::wstring& src) {
	std::vector<char> work;

	DWORD dwSizeNeeded = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);

	work.resize(dwSizeNeeded);

	WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &work[0], work.size(), NULL, NULL);

	return &work[0];
}

//-----------------------------------------------------------------------------
// Ansi ������� Unicode ������ɕϊ�
//-----------------------------------------------------------------------------
std::wstring kjm::util::toUnicode(const std::string& src) {
	std::vector<wchar_t> work;

	DWORD dwSizeNeeded = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);

	work.resize(dwSizeNeeded);

	MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &work[0], work.size());

	return &work[0];
}

//-----------------------------------------------------------------------------
// �o�b�t�@����CRC16���v�Z����
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �t�@�C������CRC16���v�Z����
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
// �t�@�C������CRC16���v�Z����
//-----------------------------------------------------------------------------
unsigned short kjm::util::calcCRC16(const _tstring& pfname) {
	unsigned short crc = 0xffff;
	FILE* fp = NULL;

	if (_tfopen_s(&fp, pfname.c_str(), _T("rb")) == 0) {
		crc = kjm::util::calcCRC16(fp);
		fclose(fp);
	}

	return crc;
}

//-----------------------------------------------------------------------------
// �s���̉��s�������폜����B
//-----------------------------------------------------------------------------
_tstring& kjm::util::chomp(_tstring& s) {

	// �s���ɉ��s����('\x0d' or '\x0a')������Ƃ��́A��菜��
	while (!s.empty() && (*(s.rbegin()) == '\x0d' || *(s.rbegin()) == '\x0a')) {
		s.erase(s.end() - 1);
	}

	return s;
}

//-----------------------------------------------------------------------------
// CopyFile API�Ɠ��l�̓���������֐�(�G���[�ӏ����ڍׂɓ��肵�����Ƃ��Ɏg�p)
// [�߂�l]  1   ����I��(�R�s�[��̓ǂݍ��ݑ����𗎂Ƃ���)
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
// [�߂�l] -12  �R�s�[��t�@�C���̑����擾���s
// [�߂�l] -13  �R�s�[��t�@�C���̓ǂݍ��ݑ������Z�b�g���s
//-----------------------------------------------------------------------------
int kjm::util::copyFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName, BOOL bFailIfExists, BOOL bFailIfReadonly/*=TRUE*/) {
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

	// �R�s�[���t�@�C�����J��
	HANDLE hFile1 = ::CreateFile(lpExistingFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile1 == INVALID_HANDLE_VALUE) {
		return -1;
	}

	// �R�s�[��t�@�C�����J��
	HANDLE hFile2 = ::CreateFile(lpNewFileName, GENERIC_WRITE, 0, NULL, (bFailIfExists) ? CREATE_NEW : CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile2 == INVALID_HANDLE_VALUE) {
		::CloseHandle(hFile1);
		return -2;
	}

	SetLastError(NOERROR);

	char buffer[8192];
	DWORD dwNumberOfBytesRead;
	DWORD dwNumberOfBytesWritten;

	do {
		DWORD tr1 = GetTickCount();
		if (::ReadFile(hFile1, buffer, _countof(buffer), &dwNumberOfBytesRead, NULL) == FALSE) {
			result = -3;
			break;
		}
		DWORD tr2 = GetTickCount();
		tr += (tr2 - tr1);

		if (dwNumberOfBytesRead > 0) {
			DWORD tw1 = GetTickCount();
			if (::WriteFile(hFile2, buffer, dwNumberOfBytesRead, &dwNumberOfBytesWritten, NULL)) {
				if (dwNumberOfBytesRead != dwNumberOfBytesWritten) {
					result = -5;
					break;
				}
			} else {
				result = -4;
				break;
			}
			DWORD tw2 = GetTickCount();
			tw += (tw2 - tw1);
		}
	} while (dwNumberOfBytesRead == _countof(buffer));

	// �R�s�[������ɏI�������Ƃ��́A�^�C���X�^���v���R�s�[���ɍ��킹��
	if (result == 0) {
		FILETIME lastWriteTime;

		if (::GetFileTime(hFile1, NULL, NULL, &lastWriteTime)) {
			if (::SetFileTime(hFile2, NULL, NULL, &lastWriteTime)) {
				// �^�C���X�^���v�̕�������
			} else {
				result = -7;
			}
		} else {
			result = -6;
		}
	}

	if (::CloseHandle(hFile1) == FALSE) {
		result = -8;
	}
	if (::CloseHandle(hFile2) == FALSE) {
		result = -9;
	}

	// �R�s�[������ɐi��ł���Ƃ��́A�������R�s�[����
	if (result == 0) {
		DWORD dwAttributes = ::GetFileAttributes(lpExistingFileName);
		if (dwAttributes != 0xffffffff) {
			if (::SetFileAttributes(lpNewFileName, dwAttributes | FILE_ATTRIBUTE_ARCHIVE)) {
				// �����̕�������
			} else {
				result = -11;
			}
		} else {
			result = -10;
		}
	}

	DWORD t2 = GetTickCount();
	DWORD ta = t2 - t1;
	///_tprintf(_T("\n(r[%4d] + w[%4d] + o[%4d]) = a[%4d]"), tr, tw, ta - tr - tw, ta);

	return result;
}

//-----------------------------------------------------------------------------
// �R�s�[������ CopyFile API ���g���B
// �R�s�[��̓Ǎ������𗎂Ƃ��@�\�t���B
//
// [�߂�l]  1   ����I��(�R�s�[��̓ǂݍ��ݑ����𗎂Ƃ���)
// [�߂�l]  0   ����I��
// [�߂�l] -1   CopyFile API�̎��s
// [�߂�l] -12  �R�s�[��t�@�C���̑����擾���s
// [�߂�l] -13  �R�s�[��t�@�C���̓ǂݍ��ݑ������Z�b�g���s
//-----------------------------------------------------------------------------
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

	BOOL bRet = CopyFile(lpExistingFileName, lpNewFileName, bFailIfExists);
	if (bRet == FALSE) {
		result = -1;
	}

	DWORD t2 = GetTickCount();
	DWORD ta = t2 - t1;
	///_tprintf(_T("\n(r[%4d] + w[%4d] + o[%4d]) = a[%4d]"), tr, tw, ta - tr - tw, ta);

	return result;
}

//-----------------------------------------------------------------------------
// �t�H���_���R�s�[����B
//-----------------------------------------------------------------------------
int kjm::util::copyFolder(const _tstring& srcFolder, const _tstring& dstFolder, bool (*funcIsExclude)(const _tstring& strFname), kjm::copyFolderInfo* pCopyFolderInfo, bool ignoreCopyError) {
	/***
	SHFILEOPSTRUCT fileOp;

	_tstring strFrom = srcFolder + _T('\0');
	_tstring strTo = dstFolder + _T('\0');

	memset(&fileOp, 0, sizeof(fileOp));
	fileOp.hwnd = NULL;
	fileOp.wFunc = FO_COPY;
	fileOp.pFrom = strFrom.c_str();
	fileOp.pTo = strTo.c_str();
	fileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;

	int ret = SHFileOperation(&fileOp);
	return ret;
	***/

	if (!kjm::util::createDirectory(dstFolder.c_str())) {
		if (pCopyFolderInfo) {
			pCopyFolderInfo->srcName = srcFolder;
			pCopyFolderInfo->dstName = dstFolder;
			pCopyFolderInfo->copyFileError = 0;
			pCopyFolderInfo->dwError = GetLastError();
		}

		return -1;
	}

	kjm::folderInfo src(srcFolder);
	std::vector<kjm::fileInfo> files = src.getFiles();
	std::vector<kjm::folderInfo> subFolders = src.getSubFolders();

	int ret;

	for (std::vector<kjm::fileInfo>::size_type i = 0; i < files.size(); i++) {
		if (funcIsExclude && funcIsExclude(kjm::path::append(srcFolder, files[i].name()))) {
			continue;
		}

		ret = kjm::util::copyFile(
			kjm::path::append(srcFolder, files[i].name()).c_str(),
			kjm::path::append(dstFolder, files[i].name()).c_str(),
			FALSE, FALSE);

		/*
		ret = kjm::util::copyFile2(
			kjm::path::append(srcFolder, files[i].name()).c_str(),
			kjm::path::append(dstFolder, files[i].name()).c_str(),
			FALSE, FALSE);
		*/

		if (ignoreCopyError) {
			if (ret != 0 && ret != 1) {
				if (pCopyFolderInfo) {
					pCopyFolderInfo->srcName = kjm::path::append(srcFolder, files[i].name());
					pCopyFolderInfo->dstName = kjm::path::append(dstFolder, files[i].name());
					pCopyFolderInfo->copyFileError = ret;
					pCopyFolderInfo->dwError = GetLastError();
				}
			}
			ret = 0;
		} else {
			if (ret != 0 && ret != 1) {
				if (pCopyFolderInfo) {
					pCopyFolderInfo->srcName = kjm::path::append(srcFolder, files[i].name());
					pCopyFolderInfo->dstName = kjm::path::append(dstFolder, files[i].name());
					pCopyFolderInfo->copyFileError = ret;
					pCopyFolderInfo->dwError = GetLastError();
				}

				return ret;
			}
		}
	}

	{
		for (std::vector<kjm::folderInfo>::size_type i = 0; i < subFolders.size(); i++) {
			if (funcIsExclude && funcIsExclude(kjm::path::append(srcFolder, subFolders[i].getFileName()))) {
				continue;
			}

			ret = kjm::util::copyFolder(
				kjm::path::append(srcFolder, subFolders[i].getFileName()),
				kjm::path::append(dstFolder, subFolders[i].getFileName()),
				funcIsExclude,
				pCopyFolderInfo);

			if (ret != 0) {
				return ret;
			}
		}
	}

	return 0;
}

//-----------------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ쐬����
//-----------------------------------------------------------------------------
bool kjm::util::createDirectory(const _tstring& strPathName) {
	if (PathIsRoot(strPathName.c_str()))		return true;
	if (PathFileExists(strPathName.c_str()))	return true;

	std::vector<TCHAR> parent(MAX_PATH);
	PathRemoveFileSpec(lstrcpy(&parent[0], strPathName.c_str()));

	if (kjm::util::createDirectory(&parent[0])) {
		if (CreateDirectory(strPathName.c_str(), NULL)) {
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Windows���V���b�g�_�E��������
// [�߂�l]
// 0  ����I��
// -1 AdjustTokenPrivileges API�ŃG���[
// -2 ExitWindowsEx API�ŃG���[
//-----------------------------------------------------------------------------
int kjm::util::exitWindows() {
	int result = 0;

	// Windows ���V���b�g�_�E��
	OSVERSIONINFO ovi;
	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&ovi);
	if (ovi.dwPlatformId == VER_PLATFORM_WIN32_NT){
		// NT�Ή�
		HANDLE hToken;
		TOKEN_PRIVILEGES tp;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)){
			//SE_SHUTDOWN_NAME��������������
			LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			if (GetLastError() != ERROR_SUCCESS) {
				return -1;
			}
			CloseHandle(hToken);
		}

		///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT�n] �����s���܂��B\n"));
		if (ExitWindowsEx(EWX_REBOOT, 0) == FALSE) {
			///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT�n] �Ɏ��s���܂���(error %u)�B\n"), GetLastError());
			result = -2;
		}
		
	} else {
		// Windows95/98/Me �p
		///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95�n] �����s���܂��B\n"));
		if (ExitWindowsEx(EWX_REBOOT, 0) == FALSE) {
			///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95�n] �Ɏ��s���܂���(error %u)�B\n"), GetLastError());
			result = -2;
		}
	}

	return result;
}

//-----------------------------------------------------------------------------
// Win32API ExpandEnvironmentStrings�����S�Ɏg���֐�
//-----------------------------------------------------------------------------
DWORD kjm::util::expandEnvironmentStrings(LPCTSTR lpSrc, std::vector<TCHAR>& rDst) {
	DWORD dwRet;
	
	if (rDst.empty()) {
		rDst.resize(MAX_PATH);
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

//-----------------------------------------------------------------------------
// Win32API FormatMessage�����S�Ɏg���֐�
//-----------------------------------------------------------------------------
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
		lstrcpy(&buffer[0], pMessage);
		::LocalFree( pMessage );
	}

	return dwRet;
}

//-----------------------------------------------------------------------------
// �V�X�e���W���̃G���[���b�Z�[�W���쐬
//-----------------------------------------------------------------------------
_tstring kjm::util::formatMessageBySystem(DWORD dwErr) {
	_tstring ret;
	PVOID pBuffer;

	DWORD dwRet = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 0, (LPTSTR)&pBuffer, 8, NULL);
	if (dwRet == 0) {
		std::basic_stringstream<TCHAR> ss;
		ss << TEXT("FormatMessage(") << dwErr << TEXT(") failed: error ") << dwErr;
		ret = ss.str();
	} else {
		ret = (LPTSTR)pBuffer;
		kjm::util::chomp(ret);
		LocalFree(pBuffer);
	}

	return ret;
}

//-----------------------------------------------------------------------------
// GetComputerNameEx�֐�(vc6�p)
//-----------------------------------------------------------------------------
BOOL kjm::util::__GetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPTSTR lpBuffer, LPDWORD nSize) {
	typedef BOOL (WINAPI *GCNEX)(COMPUTER_NAME_FORMAT, LPTSTR, LPDWORD);

	BOOL bRet = FALSE;
	HMODULE hModule = LoadLibrary(TEXT("kernel32.dll"));
	
	if (hModule != NULL) {
#if defined(_MBCS)
		GCNEX pFunc = (GCNEX)GetProcAddress(hModule, "GetComputerNameExA");
#else
		GCNEX pFunc = (GCNEX)GetProcAddress(hModule, "GetComputerNameExW");
#endif
		if (pFunc != NULL) {
			bRet = pFunc(NameType, lpBuffer, nSize);
		}
		FreeLibrary(hModule);
	}

	return bRet;
}

//-----------------------------------------------------------------------------
// string��Ԃ�GetComputerNameEx�֐�
//-----------------------------------------------------------------------------
_tstring kjm::util::getComputerNameEx(COMPUTER_NAME_FORMAT NameType) {
	DWORD dwSize = 0;

	// �K�v�ȃo�b�t�@�T�C�Y���擾(�G���[�`�F�b�N�Ȃ�)
	kjm::util::__GetComputerNameEx(NameType, NULL, &dwSize);

	// �o�b�t�@���m��
	std::vector<TCHAR> buf(dwSize);
	if (!kjm::util::__GetComputerNameEx(NameType, &buf[0], &dwSize))
		throw ::GetLastError();

	return &buf[0];
}

//-----------------------------------------------------------------------------
// string��Ԃ�GetDlgItemText�֐�
// [����] hDlg        �_�C�A���O�{�b�N�X�̃n���h��
// [����] nIDDlgItem  �R���g���[���̎��ʎq
// [��O] throw ::GetLastError();
//-----------------------------------------------------------------------------
_tstring kjm::util::getDlgItemText(HWND hDlg, int nIDDlgItem) {
	std::vector<TCHAR> work(32);

	do {
		UINT ret = ::GetDlgItemText(hDlg, nIDDlgItem, &work[0], work.size());
		DWORD dwErr = ::GetLastError();

		if ((ret == 0) && (dwErr != 0))
			throw dwErr;

		if ((ret == 0) || (ret + 1 < work.size()))
			break;

		work.resize(work.size() * 2);
	} while (1);

	return &work[0];
}

//---------------------------------------------------------------------
// GetEnvironmentVariable�֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::getEnvironmentVariable(LPCTSTR lpName) {
	// �K�v�ȕ����������߂�
	DWORD dwRet = GetEnvironmentVariable(lpName, NULL, 0);
	if (dwRet == 0) {
		return _tstring();	// ���ϐ��Ȃ��̎��́A�󕶎����Ԃ�
	}

	std::vector<TCHAR> buffer(dwRet);	// �K�v�ȃo�b�t�@���m��

	GetEnvironmentVariable(lpName, &buffer[0], buffer.size());
	return &buffer[0];
}

//-----------------------------------------------------------------------------
// �w��t�@�C���̃T�C�Y��Ԃ�(GetFileSize API�̃��b�p�[)
//-----------------------------------------------------------------------------
__int64 kjm::util::getFileSize(LPCTSTR lpszFileName) {
	// �T�C�Y���擾����t�@�C���̃n���h�����쐬
	HANDLE hFile = ::CreateFile(lpszFileName, 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return -1;
	}

	// �T�C�Y���擾����
	DWORD dwHigh = 0;
	DWORD dwLow = ::GetFileSize(hFile, &dwHigh);
	if (dwLow == 0xffffffff && ::GetLastError() != NO_ERROR) {
		::CloseHandle(hFile);
		return -2;
	}

	::CloseHandle(hFile);

	LARGE_INTEGER li = {dwLow, dwHigh};
	return li.QuadPart;
}

//-----------------------------------------------------------------------------
// �t�@�C���̃o�[�W�������\�[�X���擾
//-----------------------------------------------------------------------------
_tstring kjm::util::getFileVersion(const _tstring &strFileName) {
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

//-----------------------------------------------------------------------------
// Win32API GetModuleFileName �����S�Ɏg���֐�
//-----------------------------------------------------------------------------
_tstring kjm::util::getModuleFileName(HMODULE hModule) {
	std::vector<TCHAR> filename(MAX_PATH);

	DWORD dwRet;
	while ((dwRet = ::GetModuleFileName(hModule, &filename[0], filename.size())) == filename.size()) {
		filename.resize(filename.size() * 2);
	}

	return &filename[0];
}

//---------------------------------------------------------------------
// GetPrivateProfileString�֐��̃��b�p�[
//---------------------------------------------------------------------
_tstring kjm::util::getPrivateProfileString(LPCTSTR lpSectionName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPCTSTR lpFileName) {
	std::vector<TCHAR> buffer(512);
	while (1) {
		int nRet = GetPrivateProfileString(lpSectionName, lpKeyName, lpDefault, &buffer[0], buffer.size(), lpFileName);
		if ((int)buffer.size() - 1 > nRet) {
			// �o�b�t�@�̃T�C�Y - 1���A�߂�l�����傫���Ƃ��́A
			// ���ׂĂ̕�����ǂݍ��񂾂��Ƃ��m���Ȃ̂ŁA���[�v�𔲂���B
			break;
		}
		buffer.resize(buffer.size() * 2);
	}

	return &buffer[0];
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
		} else if ((int)buffer.size() - 1 > nRet) {
			// �o�b�t�@�̃T�C�Y - 1���A�߂�l�����傫���Ƃ��́A
			// ���ׂĂ̕�����ǂݍ��񂾂��Ƃ��m���Ȃ̂ŁA���[�v�𔲂���B
			break;
		}
		buffer.resize(buffer.size() * 2);
	}

	return &buffer[0];
}

//-----------------------------------------------------------------------------
// ���[�N�G���A�̒��S�ɃE�B���h�E���ړ�����
//-----------------------------------------------------------------------------
BOOL kjm::util::moveCenterWorkarea(HWND hwnd) {
	// ���[�N�X�G���A�̋�`���擾
	RECT rcWorkarea;
	::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWorkarea, 0);

	// �ړ�����E�B���h�E�̋�`���擾
	RECT rcClient;
	::GetWindowRect(hwnd, &rcClient);

	int nNewLeft = ((rcWorkarea.right - rcWorkarea.left) - (rcClient.right - rcClient.left)) / 2;
	int nNewTop = ((rcWorkarea.bottom - rcWorkarea.top) - (rcClient.bottom - rcClient.top)) / 2;

	// �ړ��\��̍��オWorkarea����͂ݏo�Ă���ꍇ�́A���W��0�ɂ���
	if (nNewLeft < 0)	nNewLeft = 0;
	if (nNewTop < 0)	nNewTop = 0;

	return ::MoveWindow(hwnd, nNewLeft, nNewTop, 
		(rcClient.right - rcClient.left), (rcClient.bottom - rcClient.left), FALSE);
}

//-----------------------------------------------------------------------------
// SetComputerNameEx�֐�(vc6�p)
//-----------------------------------------------------------------------------
BOOL kjm::util::__SetComputerNameEx(COMPUTER_NAME_FORMAT NameType, LPCTSTR lpBuffer) {
	typedef BOOL (WINAPI *SCNEx)(COMPUTER_NAME_FORMAT, LPCTSTR);

	BOOL bRet = FALSE;

	HMODULE hModule = LoadLibrary(TEXT("kernel32.dll"));
	if (hModule) {
#if defined(_MBCS)
		SCNEx pFunc = (SCNEx)GetProcAddress(hModule, "SetComputerNameExA");
#else
		SCNEx pFunc = (SCNEx)GetProcAddress(hModule, "SetComputerNameExW");
#endif
		if (pFunc) {
			bRet = pFunc(NameType, lpBuffer);
		}

		FreeLibrary(hModule);
	}

	return bRet;
}

//-----------------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����
//-----------------------------------------------------------------------------
void kjm::util::removeFolder(const _tstring& pszPath) {
	int i;
	std::list<_tstring> targetFolder;
	targetFolder.push_front(pszPath);

	while (targetFolder.size() != 0) {
		kjm::folderInfo currentFolder(targetFolder.front());

		std::vector<kjm::fileInfo> files = currentFolder.getFiles();			// �t�@�C���ꗗ���擾
		std::vector<kjm::folderInfo> folders = currentFolder.getSubFolders();	// �t�H���_�ꗗ�擾

		// �����t�@�C����S�č폜����
		for (i = 0; i < files.size(); i++) {
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
			for (i = 0; i < folders.size(); i++) {
				targetFolder.push_front(folders[i].getFullPath());
			}
		}
	}
}

//-----------------------------------------------------------------------------
// �������u������BVB��StrReplace�̂悤�Ȋ֐��B
//-----------------------------------------------------------------------------
_tstring kjm::util::strReplace(const _tstring& src, const _tstring& strFind, const _tstring& strReplace) {
	_tstring result = src;
	_tstring::size_type pos = 0;
	while ((pos = result.find(strFind, pos)) != _tstring::npos) {
		result.replace(pos, lstrlen(strFind.c_str()), strReplace);
	}
	return result;
}

//-----------------------------------------------------------------------------
// ���l�� human readable format �ȕ�����ɂ���
//-----------------------------------------------------------------------------
_tstring kjm::util::toHumanReadable(__int64 value) {
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

//-----------------------------------------------------------------------------
// �������������������
//-----------------------------------------------------------------------------
_tstring kjm::util::toLower(const _tstring& src) {
	_tstring result;
	std::transform(src.begin(), src.end(), std::back_inserter(result), ::_totlower);
	return result;
}

//-----------------------------------------------------------------------------
// �������啶��������
//-----------------------------------------------------------------------------
_tstring kjm::util::toUpper(const _tstring& src) {
	_tstring result;
	std::transform(src.begin(), src.end(), std::back_inserter(result), ::_totupper);
	return result;
}

//-----------------------------------------------------------------------------
// ���S�� vsprintf
//-----------------------------------------------------------------------------
int kjm::util::vsprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, va_list argptr) {
	// �o�b�t�@���w�肳��Ă��Ȃ��Ƃ��A256�o�C�g���蓖�ĂĂ���
	if (buf.size() == 0) {
		buf.resize(256);
	}

	while (1) {

		// vsnprintf �ŕ�������쐬�B
		// �s�\���ȃo�b�t�@��Ƀf�[�^���쐬���Ă���ԁA���[�v���J��Ԃ��B
		int n = _vsntprintf_s(&buf[0], buf.size(), _TRUNCATE, format, argptr);
		if (n != -1 && n != buf.size()) {
			break;
		}

		buf.resize(buf.size() + 256);
	}

	return lstrlen(&buf[0]);
}

//-----------------------------------------------------------------------------
// ���S�� sprintf 
//
// [����]
// ��Ԗڂ̈������Avector<TCHAR>�ւ̎Q�ƂɂȂ��Ă���ȊO�A
// sprintf�֐��Ɠ����ł��B
//-----------------------------------------------------------------------------
int kjm::util::sprintf_vec(std::vector<TCHAR>& buf, LPCTSTR format, ...) {
	va_list marker;
	va_start(marker, format);
	int result = kjm::util::vsprintf_vec(buf, format, marker);
	va_end(marker);
	return result;
}


//---------------------------------------------------------------------
// pszPath�Ŏw�肵���f�B���N�g�����́A���ݓ�������nDay�ȏ�Â��t�@�C��
// ���폜���܂��B
//
// �폜�Ώۂ́ApszPath�����̃t�@�C�������ł��B
// �폜�̊�́A(���ݓ��� - �ŏI�X�V����)�̒l�Ȃ̂ŁA���Ԃ܂ōl�����āA
// nDay�Ɣ�r���܂��B
//---------------------------------------------------------------------
void kjm::util::cleanupDir(LPCTSTR pszPath, int nDay) {
	_tstring findPath = kjm::path::append(pszPath, _T("*.*"));

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(findPath.c_str(), &data);
	if (hFind == INVALID_HANDLE_VALUE) {
		return;
	}

	do {
		if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
			__int64 diff = (kjm::dateTime::getSystemTime() - kjm::dateTime(data.ftLastWriteTime)) / FT_1DAY;
			if (diff >= nDay) {
				_tstring delFile = kjm::path::append(pszPath, data.cFileName);
				DeleteFile(delFile.c_str());
			}
		}
	} while (FindNextFile(hFind, &data));

	FindClose(hFind);
}

// UNICODE�������Shift JIS������ɕϊ�
std::string kjm::util::convertToAscii(const std::wstring& src) {
	std::vector<char> buf;

	DWORD dwSize = WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	buf.resize(dwSize);

	WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size(), NULL, NULL);

	return &buf[0];
}

// Shift JIS�������UNICODE������ɕϊ�
std::wstring kjm::util::convertToUnicode(const std::string& src) {
	std::vector<wchar_t> buf;

	DWORD dwSize = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	buf.resize(dwSize);

	MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], buf.size());

	return &buf[0];
}

//=============================================================================
// class path �̎���
//=============================================================================
//-----------------------------------------------------------------------------
// �p�X����������
//-----------------------------------------------------------------------------
_tstring kjm::path::append(const _tstring& strPath, const _tstring& strMore) {
	TCHAR szTemp[MAX_PATH];
	PathAppend(lstrcpy(szTemp, strPath.c_str()), strMore.c_str());
	return szTemp;
}

//-----------------------------------------------------------------------------
// ���΃p�X���쐬����
//-----------------------------------------------------------------------------
_tstring kjm::path::relativePathTo(const _tstring& from, DWORD attrFrom, const _tstring& to, DWORD attrTo) {
	std::vector<TCHAR> path(MAX_PATH);
	if (::PathRelativePathTo(&path[0], from.c_str(), attrFrom, to.c_str(), attrTo) == FALSE) {
		_otstringstream oss;
		oss << _T("kjm::path::relativePathTo error: PathRelativePathTo(\"") << from << _T("\", ") 
			<< attrFrom << _T(", \"") << to << _T("\", ") << attrTo << _T(") �Ɏ��s���܂����B");
		throw kjm::exception(-1, oss.str());
	}
	return &path[0];
}

//-----------------------------------------------------------------------------
// PathRemoveExtension �֐��̃��b�p�[
//-----------------------------------------------------------------------------
_tstring kjm::path::removeExtension(const _tstring& strPath) {
	TCHAR szTemp[MAX_PATH];
	PathRemoveExtension(lstrcpy(szTemp, strPath.c_str()));
	return szTemp;
}

//-----------------------------------------------------------------------------
// �t���p�X����t�@�C��������菜��
//-----------------------------------------------------------------------------
_tstring kjm::path::removeFileSpec(const _tstring& strPath) {
	TCHAR szTemp[MAX_PATH];
	PathRemoveFileSpec(lstrcpy(szTemp, strPath.c_str()));
	return szTemp;
}

//-----------------------------------------------------------------------------
// �g���q��ύX����
//-----------------------------------------------------------------------------
_tstring kjm::path::renameExtension(const _tstring& pszPath, const _tstring& pszExt) {
	TCHAR szTemp[MAX_PATH];
	if (::PathRenameExtension(::lstrcpy(szTemp, pszPath.c_str()), pszExt.c_str())) {
		_otstringstream oss;
		oss << _T("kjm::path::renameExtension error: PathRenameExtension(\"")
			<< pszPath << _T("\", \"") << pszExt << _T("\") �Ɏ��s���܂����B");
		throw kjm::exception(-1, oss.str());
	}
	return szTemp;
}

//=============================================================================
// class process �̎���
//=============================================================================
//---------------------------------------------------------------------
// �f�t�H���g �R���X�g���N�^
//---------------------------------------------------------------------
kjm::process::process() {
	memset(&m_si, 0, sizeof(m_si));
	memset(&m_pi, 0, sizeof(m_pi));

	m_si.cb = sizeof(m_si);
}

//---------------------------------------------------------------------
// �w�肵���v���Z�X�����s����
//---------------------------------------------------------------------
BOOL kjm::process::run(LPCTSTR pCommandLine) {
	TCHAR temp[MAX_PATH];

	BOOL bRet = CreateProcess(
		NULL,
		lstrcpy(temp, pCommandLine),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&m_si,
		&m_pi
	);

	return bRet;
}

//---------------------------------------------------------------------
// �v���Z�X�n���h�������B
// �v���Z�X�̏I����҂̂́Await�֐��Ȃ̂Œ��ӁB
//---------------------------------------------------------------------
void kjm::process::close() {
	if (m_pi.hThread != NULL) {
		CloseHandle(m_pi.hThread);
	}

	if (m_pi.hProcess != NULL) {
		CloseHandle(m_pi.hProcess);
	}

	memset(&m_pi, 0, sizeof(m_pi));
}


int kjm::encoding::GetTextEncoding(const std::vector<BYTE>& bs) {
	if (bs.size() >= 4 && (bs[0] == 0xFF && bs[1] == 0xFE && bs[2] == 0x00 && bs[3] == 0x00)) {
		return ENCODE_UTF_32; // UTF-32
	}

	if (bs.size() >= 4 && (bs[0] == 0x00 && bs[1] == 0x00 && bs[2] == 0xFE && bs[3] == 0xFF)) {
		return ENCODE_UTF_32_BIG_ENDIAN; // UTF-32 Big Endian
	}

	if (bs.size() >= 2 && (bs[0] == 0xFF && bs[1] == 0xFE)) {
		return ENCODE_UTF_16;  // UTF-16
	}

	if (bs.size() >= 2 && (bs[0] == 0xFE && bs[1] == 0xFF)) {
		return ENCODE_UTF_16_BIG_ENDIAN;  // UTF-16 Big Endian
	}

	if (kjm::encoding::IsJis(bs) == true) {
		return ENCODE_JIS; // ���{�� (JIS)
	}

	if (kjm::encoding::IsAscii(bs) == true) {
		return ENCODE_US_ASCII; // US-ASCII
	}

	int utf8 = 0, sjis = 0, euc = 0;  // �����R�[�h����p.
	bool bomFrag = false;             // UTF-8 BOM �̔���p.

	bool Utf8Flag = kjm::encoding::IsUTF8(bs, utf8, bomFrag);
	bool SJisFlag = kjm::encoding::IsSJIS(bs, sjis);
	bool EucFlag  = kjm::encoding::IsEUC(bs, euc);

	if (Utf8Flag == true || SJisFlag == true || EucFlag == true) {
		if (euc > sjis && euc > utf8) {
			return ENCODE_EUC_JP; // ���{�� (EUC)
		} else if (sjis > euc && sjis > utf8) {
			return ENCODE_SHIFT_JIS;   // ���{�� (�V�t�g JIS)
		} else if (utf8 > euc && utf8 > sjis) {
			if (bomFrag == true) {
				return ENCODE_UTF_8;    // UTF-8 (BOM����)
			} else {
				return ENCODE_UTF_8N;   // UTF-8N (BOM�Ȃ�)
			}
		}
	}
	return 0;
}

bool kjm::encoding::IsJis(const std::vector<BYTE>& bs) {
	int len = bs.size();
	BYTE b1, b2, b3, b4, b5, b6;

	for (int i = 0; i < len; i++) {
		b1 = bs[i];

		if (b1 > 0x7F) {
			return false;   // Not ISO-2022-JP (0x00�`0x7F)
		} else if (i < len - 2) {
			b2 = bs[i + 1]; b3 = bs[i + 2];
			if (b1 == 0x1B && b2 == 0x28 && b3 == 0x42) {
				return true;    // ESC ( B  : JIS ASCII
			} else if (b1 == 0x1B && b2 == 0x28 && b3 == 0x4A) {
				return true;    // ESC ( J  : JIS X 0201-1976 Roman Set
			} else if (b1 == 0x1B && b2 == 0x28 && b3 == 0x49) {
				return true;    // ESC ( I  : JIS X 0201-1976 kana
			} else if (b1 == 0x1B && b2 == 0x24 && b3 == 0x40) { 
				return true;    // ESC $ @  : JIS X 0208-1978(old_JIS)
			} else if (b1 == 0x1B && b2 == 0x24 && b3 == 0x42) {
				return true;    // ESC $ B  : JIS X 0208-1983(new_JIS)
			} 
		} else if (i < len - 3) { 
			b2 = bs[i + 1]; b3 = bs[i + 2]; b4 = bs[i + 3];
			if (b1 == 0x1B && b2 == 0x24 && b3 == 0x28 && b4 == 0x44) {
				return true;    // ESC $ ( D  : JIS X 0212-1990�iJIS_hojo_kanji�j
			}
		} else if (i < len - 5) { 
			b2 = bs[i + 1]; b3 = bs[i + 2]; b4 = bs[i + 3]; b5 = bs[i + 4]; b6 = bs[i + 5];
			if (b1 == 0x1B && b2 == 0x26 && b3 == 0x40 && b4 == 0x1B && b5 == 0x24 && b6 == 0x42) { 
				return true;    // ESC & @ ESC $ B  : JIS X 0208-1990, JIS X 0208:1997
			}
		} else {
			continue;
		}
	}

	return false;
}

bool kjm::encoding::IsAscii(const std::vector<BYTE>& bs) { 
	for (std::vector<BYTE>::size_type i = 0; i < bs.size(); i++) {
		if (bs[i] >= 0x00 && bs[i] <= 0x7F) {
			continue; // ASCII : 0x00�`0x7F
		} else { 
			return false;
		}
	}
	return true;
}

bool kjm::encoding::IsSJIS(const std::vector<BYTE>& bs, int& sjis) {
	int len = bs.size();
	BYTE b1, b2;

	for (int i = 0; i < len; i++) {
		b1 = bs[i];

		if (b1 >= 0x00 && b1 <= 0x7F) {
			continue;  // ASCII : 0x00�`0x7F
		}

		if (b1 >= 0xA1 && b1 <= 0xDF) {
			continue;  // kana  : 0xA1�`0xDF
		}

		if (i < len - 1) {
			b2 = bs[i + 1];

			if (((b1 >= 0x81 && b1 <= 0x9F) || (b1 >= 0xE0 && b1 <= 0xFC)) && ((b2 >= 0x40 && b2 <= 0x7E) || (b2 >= 0x80 && b2 <= 0xFC))) {
				// kanji first byte  : 0x81�`0x9F or 0xE0�`0xFC
				//       second byte : 0x40�`0x7E or 0x80�`0xFC
				i += 1; sjis += 2;
				continue;
			}
		}

		return false;
	}

	return true;
}

bool kjm::encoding::IsEUC(const std::vector<BYTE>& bs, int& euc) {
	int len = bs.size();
	BYTE b1, b2, b3;

	for (int i = 0; i < len; i++) { 
		b1 = bs[i];

		if (b1 >= 0x00 && b1 <= 0x7F) {
			continue;  // ASCII : 0x00�`0x7F
		}

		if (i < len - 1) {
			b2 = bs[i + 1];

			if ((b1 >= 0xA1 && b1 <= 0xFE) && (b2 >= 0xA1 && b2 <= 0xFE)) {
				i += 1; euc += 2;
				continue;     // kanji - 0xA1�`0xFE, 0xA1�`0xFE
			}

			if ((b1 == 0x8E) && (b2 >= 0xA1 && b2 <= 0xDF)) {
				i += 1; euc += 2;
				continue;     // kana - 0x8E, 0xA1�`0xDF
			}
		}

		if (i < len - 2) {
			b2 = bs[i + 1]; b3 = bs[i + 2];

			if ((b1 == 0x8F) && (b2 >= 0xA1 && b2 <= 0xFE) && (b3 >= 0xA1 && b3 <= 0xFE)) {
				i += 2; euc += 3;
				continue;       // hojo kanji - 0x8F, 0xA1�`0xFE, 0xA1�`0xFE
			}
		}

		return false;
	}

	return true;
}

bool kjm::encoding::IsUTF8(const std::vector<BYTE>& bs, int& utf8, bool& bomFlag) {
	int len = bs.size();
	BYTE b1, b2, b3, b4;

	for (int i = 0; i < len; i++) {
		b1 = bs[i];

		if (b1 >= 0x00 && b1 <= 0x7F) {
			continue;  // ASCII : 0x00�`0x7F
		}

		if (i < len - 1) {
			b2 = bs[i + 1];

			if ((b1 >= 0xC0 && b1 <= 0xDF) && (b2 >= 0x80 && b2 <= 0xBF)) {
				i += 1; utf8 += 2;  // 2 byte char
				continue;
			}
		}

		if (i < len - 2) {
			b2 = bs[i + 1]; b3 = bs[i + 2];

			if (b1 == 0xEF && b2 == 0xBB && b3 == 0xBF) {
				bomFlag = true;     // BOM : 0xEF 0xBB 0xBF
				i += 2; utf8 += 3;

				continue;
			}

			if ((b1 >= 0xE0 && b1 <= 0xEF) && (b2 >= 0x80 && b2 <= 0xBF) && (b3 >= 0x80 && b3 <= 0xBF)) {
				i += 2; utf8 += 3;  // 3 byte char
				continue;
			}
		}

		if (i < len - 3) {
			b2 = bs[i + 1]; b3 = bs[i + 2]; b4 = bs[i + 3];

			if ((b1 >= 0xF0 && b1 <= 0xF7) && (b2 >= 0x80 && b2 <= 0xBF) && (b3 >= 0x80 && b3 <= 0xBF) && (b4 >= 0x80 && b4 <= 0xBF)) {
				i += 3; utf8 += 4;  // 4 byte char
				continue;
			}
		}

		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
HRESULT kjm::encoding::convertINetString(
	LPDWORD lpdwMode,
    DWORD dwSrcEncoding,
    DWORD dwDstEncoding,
    LPCSTR lpSrcStr,
    LPINT lpnSrcSize,
    LPBYTE lpDstStr,
    LPINT lpnDstSize
) {
	typedef HRESULT (WINAPI *CINS)(LPDWORD, DWORD, DWORD, LPCSTR, LPINT, LPBYTE, LPINT);

	HRESULT bRet = E_FAIL;
	HMODULE hModule = LoadLibrary(TEXT("mlang.dll"));
	
	if (hModule != NULL) {
		CINS pFunc = (CINS)GetProcAddress(hModule, "ConvertINetString");
		if (pFunc != NULL) {
			bRet = pFunc(lpdwMode, dwSrcEncoding, dwDstEncoding, lpSrcStr, lpnSrcSize, lpDstStr, lpnDstSize);
		}
		FreeLibrary(hModule);
	}

	return bRet;
}

//---------------------------------------------------------------------
// �h���C�u�̏���ێ�����N���X
//---------------------------------------------------------------------
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
_tstring kjm::drive::get_DriveTypeString() {
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
kjm::driveList::driveList() {
	DWORD dwDrives = GetLogicalDrives();

	for (int i = 'A'; i <= 'Z'; i++) {
		if (dwDrives & (1 << (i - 'A'))) {
			m_driveList.push_back(kjm::drive(i - 'A'));
		}
	}
}

//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
kjm::driveList& kjm::driveList::operator =(const kjm::driveList& rhs) {
	if (this == &rhs) return *this;

	m_driveList = rhs.m_driveList;

	return *this;
}
void kjm::csvFile::open(LPCTSTR fname) {
#if defined(UNICODE)
	m_ifs.open(kjm::util::convertToAscii(fname).c_str());
#else
	m_ifs.open(fname);
#endif
	m_ifs.imbue(std::locale("japanese"));
}

bool kjm::csvFile::getline() {
	_tstring line;

	if (std::getline(m_ifs, line).fail())
		return false;

	parse_csv(line);

	return true;
}


void kjm::csvFile::parse_csv(const _tstring& line) {
	_tstring::size_type p = 0;

	m_cols.clear();

	bool col_top = true;
	bool esc_mode = false;
	_tstring col_data;

	for (_tstring::const_iterator i = line.begin(); i != line.end(); i++) {
		if (col_top && *i == '\"') {
			col_top = false;
			esc_mode = true;
			continue;
		}

		col_top = false;

		if (esc_mode) {
			if (*i != '\"') {
				col_data += *i;
			} else if ((i + 1) == line.end() || *(i + 1) != '\"') {
				esc_mode = false;
			} else if (*(i + 1) == '\"') {
				col_data += *i;
				i++;
			}
		} else {
			if (*i != ',') {
				col_data += *i;
			} else {
				m_cols.push_back(col_data);
				col_data.erase();
				col_top = true;
			}
		}
	}

	if (!col_top) {
		m_cols.push_back(col_data);
	}
}
