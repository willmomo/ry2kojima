//-----------------------------------------------------------------------------
// kjmsub ver.0.2010.8.27
//-----------------------------------------------------------------------------
#include "kjmsub.h"
#include <share.h>

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
// �p�t�H�[�}���X�f�[�^����������N���X(class cpuUsage)
//=============================================================================
//-----------------------------------------------------------------------------
// �p�t�H�[�}���X�f�[�^�擾�J�n(95��)
//-----------------------------------------------------------------------------
BOOL kjm::cpuUsage::StartPerf95() {
	LONG rc;
	HKEY hkey;

	// �p�t�H�[�}���X�E�f�[�^�̎��W��L���ɂ���
	rc = RegOpenKeyEx(HKEY_DYN_DATA, _T("PerfStats\\StartStat"), 0, KEY_READ, &hkey);
	if (rc == ERROR_SUCCESS) {
		DWORD dwType, cbData;
		LPBYTE pByte;
		rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, NULL, &cbData);
		if (rc == ERROR_SUCCESS) {
			pByte = (LPBYTE)LocalAlloc(LPTR, cbData);
			rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, pByte, &cbData);
			if (rc != ERROR_SUCCESS) {
				TCHAR szBuffer[128];
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					rc,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
			}
			LocalFree(pByte);
		} else {
			TCHAR szBuffer[128];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				rc,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
		}
		RegCloseKey(hkey);
	} else {
		TCHAR szBuffer[128];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			rc,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// �p�t�H�[�}���X�f�[�^�擾�I��(95��)
//-----------------------------------------------------------------------------
BOOL kjm::cpuUsage::StopPerf95() {
	LONG rc;
	HKEY hkey;

	// �p�t�H�[�}���X�E�f�[�^�̎��W�𖳌��ɂ���
	rc = RegOpenKeyEx(HKEY_DYN_DATA, _T("PerfStats\\StopStat"), 0, KEY_READ, &hkey);
	if (rc == ERROR_SUCCESS) {
		DWORD dwType, cbData;
		LPBYTE pByte;
		rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, NULL, &cbData);
		if (rc == ERROR_SUCCESS) {
			pByte = (LPBYTE)LocalAlloc(LPTR, cbData);
			rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, pByte, &cbData);
			if (rc != ERROR_SUCCESS) {
				TCHAR szBuffer[128];
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					rc,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
			}
			LocalFree(pByte);
		} else {
			TCHAR szBuffer[128];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				rc,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
		}
		RegCloseKey(hkey);
	} else {
		TCHAR szBuffer[128];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			rc,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// �p�t�H�[�}���X�f�[�^�̎擾(95��)
//-----------------------------------------------------------------------------
INT kjm::cpuUsage::GetCPUUsage95() {
	LONG rc;
	HKEY hkey;
	INT inData = 0;

	// object\counter�y�A�̃p�t�H�[�}���X�E�f�[�^����������
	rc = RegOpenKeyEx(HKEY_DYN_DATA, _T("PerfStats\\StatData"), 0, KEY_READ, &hkey);
	if (rc == ERROR_SUCCESS) {
		DWORD dwType, dwData, cbData = sizeof(DWORD);
		rc = RegQueryValueEx(hkey, _T("KERNEL\\CPUUsage"), NULL, &dwType, (LPBYTE)&dwData, &cbData);
		if (rc == ERROR_SUCCESS) {
			inData = dwData;
		} else {
			TCHAR szBuffer[128];
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				rc,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
		}
		RegCloseKey(hkey);
	} else {
		TCHAR szBuffer[128];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			rc,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szBuffer, sizeof(szBuffer)/sizeof(TCHAR), NULL);
	}

	return inData;
}

//-----------------------------------------------------------------------------
// �p�t�H�[�}���X�f�[�^�̎擾�p�T�|�[�g�֐�(NT��)
//-----------------------------------------------------------------------------
PVOID kjm::cpuUsage::CounterDataPtr(PPERF_DATA_BLOCK pDataBlock, DWORD dwObjectNameTitleIndex, WCHAR* pszInstanceName, DWORD dwCounterNameTitleIndex) {
	PPERF_OBJECT_TYPE pObjectType = (PPERF_OBJECT_TYPE)((PBYTE)pDataBlock + pDataBlock->HeaderLength);
	for (DWORD i = 0; i < pDataBlock->NumObjectTypes; i++) {
		if (pObjectType->ObjectNameTitleIndex == dwObjectNameTitleIndex) {
			if (pObjectType->NumInstances < 0) {
				PPERF_COUNTER_BLOCK pCounterBlock = (PPERF_COUNTER_BLOCK)((PBYTE)pObjectType + pObjectType->DefinitionLength);
				PPERF_COUNTER_DEFINITION pCounterDefinition = (PPERF_COUNTER_DEFINITION)((PBYTE)pObjectType + pObjectType->HeaderLength);
				for (DWORD j = 0; j < pObjectType->NumCounters; j++) {
					if (pCounterDefinition->CounterNameTitleIndex == dwCounterNameTitleIndex) {
						return (PVOID)((PBYTE)pCounterBlock + pCounterDefinition->CounterOffset);
					}

					pCounterDefinition = (PPERF_COUNTER_DEFINITION)((PBYTE)pCounterDefinition + pCounterDefinition->ByteLength);
				}

				return NULL;
			} else {
				WCHAR* pszDebug = NULL;
				PPERF_INSTANCE_DEFINITION pInstanceDefinition = (PPERF_INSTANCE_DEFINITION)((PBYTE)pObjectType + pObjectType->DefinitionLength);
				for (int k = 0; k < pObjectType->NumInstances; k++) {
					PPERF_COUNTER_BLOCK pCounterBlock = (PPERF_COUNTER_BLOCK)((PBYTE)pInstanceDefinition + pInstanceDefinition->ByteLength);

					pszDebug = (WCHAR*)((PBYTE)pInstanceDefinition + pInstanceDefinition->NameOffset);
					if (wcscmp((WCHAR*)((PBYTE)pInstanceDefinition + pInstanceDefinition->NameOffset), pszInstanceName) == 0) {
						PPERF_COUNTER_DEFINITION pCounterDefinition = (PPERF_COUNTER_DEFINITION)((PBYTE)pObjectType + pObjectType->HeaderLength);
						for (DWORD j = 0; j < pObjectType->NumCounters; j++) {
							if (pCounterDefinition->CounterNameTitleIndex == dwCounterNameTitleIndex) {
								return (PVOID)((PBYTE)pCounterBlock + pCounterDefinition->CounterOffset);
							}

							pCounterDefinition = (PPERF_COUNTER_DEFINITION)((PBYTE)pCounterDefinition + pCounterDefinition->ByteLength);
						}

						return NULL;
					}

					pInstanceDefinition = (PPERF_INSTANCE_DEFINITION)((PBYTE)pCounterBlock + pCounterBlock->ByteLength);
				}

				return NULL;
			}
		}

		pObjectType = (PPERF_OBJECT_TYPE)((PBYTE)pObjectType + pObjectType->TotalByteLength);
	}

	return NULL;
}

	LARGE_INTEGER liOld;
	LARGE_INTEGER PerfTime100nSecOld;
//-----------------------------------------------------------------------------
// �p�t�H�[�}���X�f�[�^�̎擾(NT��)
//-----------------------------------------------------------------------------
INT kjm::cpuUsage::GetCPUUsageNT() {

#define TOTALBYTES		43008
#define BYTEINCREMENT	1024

	PPERF_DATA_BLOCK PerfData = NULL;
	DWORD BufferSize = TOTALBYTES;
	//static LARGE_INTEGER liOld;
	//static LARGE_INTEGER PerfTime100nSecOld;

//	PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);	// �o�b�t�@���m��
	PerfData = (PPERF_DATA_BLOCK)LocalAlloc(LPTR, BufferSize);	// �o�b�t�@���m��
	if (PerfData == NULL)
		return -1;

//	while (RegQueryValueEx(HKEY_PERFORMANCE_DATA, "Global", NULL, NULL, (LPBYTE)PerfData, &BufferSize) == ERROR_MORE_DATA) {
	while (RegQueryValueEx(HKEY_PERFORMANCE_DATA, _T("238"), NULL, NULL, (LPBYTE)PerfData, &BufferSize) == ERROR_MORE_DATA) {
		BufferSize += BYTEINCREMENT;	// �\���ɑ傫�ȃo�b�t�@���m��
//		PerfData = (PPERF_DATA_BLOCK)realloc(PerfData, BufferSize);
		PerfData = (PPERF_DATA_BLOCK)LocalReAlloc((HGLOBAL)PerfData, BufferSize, 0);
		if (PerfData == NULL)
			return -1;
	}

	LARGE_INTEGER* pli = (LARGE_INTEGER*)CounterDataPtr(PerfData, 238, L"0", 6);

	if (liOld.QuadPart == 0 || PerfTime100nSecOld.QuadPart == 0) {
		PerfTime100nSecOld.QuadPart = PerfData->PerfTime100nSec.QuadPart;
		liOld.QuadPart = pli->QuadPart;
		return 0;
	}

	INT C = (INT)(pli->QuadPart - liOld.QuadPart);
	INT T = (INT)(PerfData->PerfTime100nSec.QuadPart - PerfTime100nSecOld.QuadPart);

	PerfTime100nSecOld.QuadPart = PerfData->PerfTime100nSec.QuadPart;
	liOld.QuadPart = pli->QuadPart;

//	free(PerfData);
	if (LocalFree((HGLOBAL)PerfData) != NULL)
		return -1;

	INT ret = (INT)((1.0 - ((double)C / (double)T)) * 100.0);
	return (ret < 0) ? 0 : ret;
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
				kjm::util::expandEnvironmentStrings((LPCTSTR)&ModuleName[0], ExpandedName );

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

	oneModuleName = _tcstok(&SourceModuleName[0], _T(";") );
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

		oneModuleName = _tcstok(NULL, _T(";"));
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

	for ( int i = 0; i < data.size(); i++) {
		data.at( i ) = *(((unsigned char *)getRaw()) + DataOffset() + i);
	}

	return data;
}


//=============================================================================
// �f�B���N�g�� ��\������N���X
//=============================================================================
//-----------------------------------------------------------------------------
// �f�B���N�g�������̃t�@�C���ꗗ���擾
//-----------------------------------------------------------------------------
std::vector<kjm::file> kjm::folder::getFiles(const _tstring& strWC/* = _T("*.*")*/) {
	std::vector<kjm::file> result;

	findFile ff;
	if (ff.findFirstFile(kjm::path::append(m_fullPath, strWC))) {
		do {
			if (!ff.isDirectory()) {
				WIN32_FIND_DATA findData;
				ff.copyWin32FindData(findData);
				result.push_back(kjm::file(m_fullPath, findData));
			}
		} while (ff.findNextFile());
		ff.findClose();
	}

	return result;
}

//-----------------------------------------------------------------------------
// �f�B���N�g�������̃f�B���N�g���ꗗ���擾
//-----------------------------------------------------------------------------
std::vector<kjm::folder> kjm::folder::getSubFolders() {
	std::vector<kjm::folder> result;

	findFile ff;
	if (ff.findFirstFile(path::append(m_fullPath, _T("*.*")))) {
		do {
			if (ff.isDirectory() && !ff.isDots()) {
				WIN32_FIND_DATA findData;
				ff.copyWin32FindData(findData);
				result.push_back(kjm::folder(m_fullPath, findData));
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
	for (int i = 0; i < sections.size(); i++) {

		// �}�[�W����INI�t�@�C���̃Z�N�V��������A�S�L�[��ǂݍ���
		std::vector<_tstring> newKeys = newIniFile.getKeyNames(sections[i]);

		// �}�[�W�����INI(����)�t�@�C���̃Z�N�V����������S�L�[��ǂݍ���
		std::vector<_tstring> orgKeys = this->getKeyNames(sections[i]);

		for (int j = 0; j < newKeys.size(); j++) {

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
// ���O�t�@�C���̏o�͐��ݒ肷��
//-----------------------------------------------------------------------------
void kjm::log::set_log_path(const _tstring& strPath) {
	m_strLogPath = strPath;

	// ���ʎq�ɋ󕶎����ݒ�
	m_strIdent = _T("");

	// �t�@�C�����𐶐�����
	m_strBaseFileName = kjm::path::append(
		m_strLogPath,
		m_strPrefix + _T("{YMD}.log"));
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
	Winssa_Log(get_Ident().c_str(), LOGLV_ALWAYS, _T("%s"), buf.begin());
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
	Winssa_Log(get_Ident().c_str(), LOGLV_ERR, _T("%s"), buf.begin());
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
	Winssa_Log(get_Ident().c_str(), LOGLV_INFO, _T("%s"), buf.begin());
#endif
}

//-----------------------------------------------------------------------------
// ���O�̏o�͊֐�
//-----------------------------------------------------------------------------
void kjm::log::write(LPCTSTR format, ...) {
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
		n = _vsntprintf(work.begin(), work.size(), format, marker);
	} while (n == -1 || n == work.size());

	va_end(marker);

	write(_T("%s"), work.begin());

	// �W���o�͂ɂ��o��
	_tprintf(_T("%s\n"), work.begin());
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
	int ret = _stscanf(time_text, _T("%d/%d/%d %d:%d:%d"), &y, &m, &d, &h, &n, &s);

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


//=============================================================================
// ���[�e�B���e�B�֐��p��static�N���X
//=============================================================================
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

	FILE* fp = _tfopen(pfname.c_str(), _T("rb"));
	if (fp) {
		crc = kjm::util::calcCRC16(fp);
		fclose(fp);
	}

	return crc;
}

//-----------------------------------------------------------------------------
// �s���̉��s����菜��
//-----------------------------------------------------------------------------
_tstring& kjm::util::chomp(_tstring& s) {

	// �s���ɉ��s����������Ƃ��́A��菜��
	while (!s.empty() && 
		  (   (*(s.rbegin()) == '\n') || (*(s.rbegin()) == '\r')   )
		) {
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

	kjm::folder src(srcFolder);
	std::vector<kjm::file> files = src.getFiles();
	std::vector<kjm::folder> subFolders = src.getSubFolders();

	int ret;
	int i;

	for (i = 0; i < files.size(); i++) {
		if (funcIsExclude && funcIsExclude(kjm::path::append(srcFolder, files[i].getFileName()))) {
			continue;
		}

		ret = kjm::util::copyFile(
			kjm::path::append(srcFolder, files[i].getFileName()).c_str(),
			kjm::path::append(dstFolder, files[i].getFileName()).c_str(),
			FALSE, FALSE);

		/*
		ret = kjm::util::copyFile2(
			kjm::path::append(srcFolder, files[i].getFileName()).c_str(),
			kjm::path::append(dstFolder, files[i].getFileName()).c_str(),
			FALSE, FALSE);
		*/

		if (ignoreCopyError) {
			if (ret != 0 && ret != 1) {
				if (pCopyFolderInfo) {
					pCopyFolderInfo->srcName = kjm::path::append(srcFolder, files[i].getFileName());
					pCopyFolderInfo->dstName = kjm::path::append(dstFolder, files[i].getFileName());
					pCopyFolderInfo->copyFileError = ret;
					pCopyFolderInfo->dwError = GetLastError();
				}
			}
			ret = 0;
		} else {
			if (ret != 0 && ret != 1) {
				if (pCopyFolderInfo) {
					pCopyFolderInfo->srcName = kjm::path::append(srcFolder, files[i].getFileName());
					pCopyFolderInfo->dstName = kjm::path::append(dstFolder, files[i].getFileName());
					pCopyFolderInfo->copyFileError = ret;
					pCopyFolderInfo->dwError = GetLastError();
				}

				return ret;
			}
		}
	}

	for (i = 0; i < subFolders.size(); i++) {
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

	return 0;
}

//-----------------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ쐬����
//-----------------------------------------------------------------------------
bool kjm::util::createDirectory(LPCTSTR strPathName) {
	if (PathIsRoot(strPathName))		return true;
	if (PathFileExists(strPathName))	return true;

	std::vector<TCHAR> parent(MAX_PATH);
	PathRemoveFileSpec(lstrcpy(&parent[0], strPathName));

	if (kjm::util::createDirectory(&parent[0])) {
		if (CreateDirectory(strPathName, NULL)) {
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------------
// Windows���V���b�g�_�E��������
// [����]
// uFlags  ExitWindowsEx API ���Q��
// [�߂�l]
// 0  ����I��
// -1 AdjustTokenPrivileges API�ŃG���[
// -2 ExitWindowsEx API�ŃG���[
//-----------------------------------------------------------------------------
int kjm::util::exitWindows(UINT uFlags) {
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
		if (ExitWindowsEx(uFlags, 0) == FALSE) {
			///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[NT�n] �Ɏ��s���܂���(error %u)�B\n"), GetLastError());
			result = -2;
		}
		
	} else {
		// Windows95/98/Me �p
		///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95�n] �����s���܂��B\n"));
		if (ExitWindowsEx(uFlags, 0) == FALSE) {
			///LogWrite(_T("ExitWindowsEx(EWX_REBOOT)[95�n] �Ɏ��s���܂���(error %u)�B\n"), GetLastError());
			result = -2;
		}
	}

	return result;
}

//-----------------------------------------------------------------------------
// Win32API ExpandEnvironmentStrings�����S�Ɏg���֐�
//-----------------------------------------------------------------------------
DWORD kjm::util::expandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst ) {

	DWORD dwRet;
	
	while ( ( dwRet = ::ExpandEnvironmentStrings( lpSrc, &rDst[0], rDst.size() ) ) > rDst.size() ) {
		if ( dwRet == 0 ) {
			break;
		} else {
			rDst.resize( dwRet );
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
		LocalFree(pBuffer);
	}

	return ret;
}

//---------------------------------------------------------------------
// �z�X�g���܂��́AIP�����񂩂�IP�A�h���X���擾
//---------------------------------------------------------------------
DWORD kjm::util::getAddr(LPCSTR lpName) {
	DWORD addr = INADDR_ANY;

	if (lpName[0] == '\0')	return INADDR_ANY;

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

	return	addr;
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

//---------------------------------------------------------------------
// ���C���[�h�E�B���h�E�̕s�����x�Ɠ����̃J���[�L�[��ݒ�
//
// hwnd     ���C���[�h�E�B���h�E�̃n���h�����w�肵�܂��B���C���[�h�E�B
//          ���h�E�́ACreateWindowEx �֐��� WS_EX_LAYERED ���w�肵�Ē�
//          �ڍ쐬���邩�ASetWindowLong �֐��� WS_EX_LAYERED ���w�肵��
//          �����̃E�B���h�E����쐬���܂��B
// crKey    ���C���[�h�E�B���h�E�̍쐬���Ɏg�������̃J���[�L�[���w�肷
//          ��\���̂ւ̃|�C���^���w�肵�܂��B�E�B���h�E�ɂ���ĕ`�悳
//          ��邱�̐F�̃s�N�Z���͂��ׂē����ɂȂ�܂��B
// bAlpha   ���C���[�h�E�B���h�E�̕s�����x�������A���t�@�l���w�肵�܂��B
//          �\���̂� SourceConstantAlpha �����o�Ɏ��Ă��܂��B0 ���w�肷
//          ��ƁA�E�B���h�E�͊��S�ɓ����ɂȂ�܂��B255 ���w�肷��ƁA
//          �E�B���h�E�͕s�����ɂȂ�܂��B
// dwFlags  �A�N�V�����t���O���w�肵�܂��B���̒l��g�ݍ��킹�Ďg���܂��B
//          �l            �Ӗ� 
//          LWA_COLORKEY  �����F�Ƃ��� crKey ���g���܂��B 
//          LWA_ALPHA     bAlpha ���g���āA���C���[�h�E�B���h�E�̕s����
//                        �x�����肵�܂��B
//---------------------------------------------------------------------
BOOL kjm::util::setLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags) {
	typedef BOOL (WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

	BOOL ret = FALSE;

	HMODULE hlibUser32 = LoadLibrary(_T("user32.dll"));
	if (hlibUser32) {
		SLWA pfunc = (SLWA)GetProcAddress(hlibUser32, "SetLayeredWindowAttributes");
		if (pfunc) {
			ret = pfunc(hwnd, crKey, bAlpha, dwFlags);
		}

		FreeLibrary(hlibUser32);
	}

	return ret;
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
// PathRenameExtension�̃��b�p�[
//-----------------------------------------------------------------------------
_tstring& kjm::util::pathRenameExtension(_tstring& strPath, LPCTSTR pszExt) {
	std::vector<TCHAR> work(strPath.begin(), strPath.end());
	work.push_back('\0');
	work.resize(work.size() + lstrlen(pszExt) + MAX_PATH );	// �\���ȃo�b�t�@���m��

	if (::PathRenameExtension(&work[0], pszExt)) {
		strPath = &work[0];
	}

	return strPath;
}

//-----------------------------------------------------------------------------
// �f�B���N�g�����ċA�I�ɍ폜����
//-----------------------------------------------------------------------------
bool kjm::util::removeFolder(LPCTSTR pszPath) {
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
					if ((bRet = kjm::util::removeFolder(&child[0])) == false) {
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
	std::vector<TCHAR> buf(src.begin(), src.end());
	buf.push_back('\0');

	return _tcslwr(&buf[0]);
}

//-----------------------------------------------------------------------------
// �������啶��������
//-----------------------------------------------------------------------------
_tstring kjm::util::toUpper(const _tstring& src) {
	std::vector<TCHAR> buf(src.begin(), src.end());
	buf.push_back('\0');

	return _tcsupr(&buf[0]);
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
		int n = _vsntprintf(&buf[0], buf.size(), format, argptr);
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

