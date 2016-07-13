//---------------------------------------------------------------------
// kjm::eventlog class
//---------------------------------------------------------------------


#include "kjm_library.h"
#include "kjm_registry.h"
#include "kjm_util.h"

#include "kjm_eventlog.h"


//---------------------------------------------------------------------
// 1���R�[�h����eventlog������킷�N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------
kjm::eventlog_record& kjm::eventlog_record::operator= ( const kjm::eventlog_record& src ) {
	m_source_name = src.m_source_name;
	m_raw_record = src.m_raw_record;

	return *this;
}

kjm::eventlog_record::eventlog_record( LPCTSTR lpSourceName, const EVENTLOGRECORD* src ) {

	// �C�x���g�\�[�X�̕ۑ�
	m_source_name = lpSourceName;

	// �����̃R�s�[
	m_raw_record.resize( src->Length );
	memmove( m_raw_record.begin(), src, src->Length );
}


/* �����ݕ�����̎擾 */
int kjm::eventlog_record::GetArgs(const EVENTLOGRECORD *pBuf, std::vector<char*>& args )
{
	args.clear();

	char *cp;
	WORD ArgCount;

	if(pBuf->NumStrings == 0) return args.size();

	/* �������X�g���擾 */
	args.resize( pBuf->NumStrings );
	cp = (char *)pBuf + (pBuf->StringOffset);

	for(ArgCount=0; ArgCount<pBuf->NumStrings; ArgCount++) {
		args[ArgCount] = cp;
		cp += strlen(cp) + 1;
	}
	return args.size();
}

/* �\�[�X�����烂�W���[�������擾 */
BOOL kjm::eventlog_record::GetModuleNameFromSourceName( const char *SourceName, const char *EntryName, std::vector<TCHAR>& ExpandedName) {
	DWORD lResult;
	std::vector<BYTE> ModuleName;
	BOOL bReturn = FALSE;

	// Application���O�p�̃��W�X�g���L�[���I�[�v��
	kjm::registry appKey;
	
	std::string key = std::string( "SYSTEM\\CurrentControlSet\\Services\\EventLog\\" );
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
				"EventMessageFile",
				NULL,
				NULL,
				ModuleName);

			if ( lResult == ERROR_SUCCESS ) {

				// ���ϐ���W�J
				kjm::util::ExpandEnvironmentStrings( (LPCTSTR)ModuleName.begin(), ExpandedName );

				/* ����I�� */
				bReturn = TRUE;
			}
		}
	}

	return bReturn;
}

/* ���b�Z�[�W�̕\�� */
std::string kjm::eventlog_record::DispMessage( const char *SourceName, const char *EntryName, const char **Args, DWORD MessageId) {
	BOOL bResult;
	BOOL bReturn = FALSE;
	std::vector<TCHAR> SourceModuleName;
	std::vector<TCHAR> message;
	char* oneModuleName;
	DWORD dwRet;

	/* �\�[�X���W���[�������擾 */	
	bResult = GetModuleNameFromSourceName(SourceName, EntryName, SourceModuleName);
	if(!bResult) return std::string("");

	oneModuleName = strtok( SourceModuleName.begin(), ";" );
	while ( oneModuleName ) {

		kjm::library sourceModule;

		// �\�[�X���W���[�������[�h
		sourceModule.LoadLibraryEx(
			oneModuleName,
			NULL,
			DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

		if ( sourceModule.getInstance() ) {

			/* ���b�Z�[�W���쐬 */
			dwRet = kjm::util::FormatMessage(
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

		oneModuleName = strtok( NULL, ";" );
	}

	if(message.size() > 0) return std::string(message.begin());
	else {
		char buf[32];
		wsprintf(buf, "(%d)\n", MessageId);
		return std::string( buf );
	}
}


//---------------------------------------------------------------------
// �e��A�N�Z�T(����񂩂�f�[�^��؂�o��)
//---------------------------------------------------------------------
std::string kjm::eventlog_record::SourceName() {
	char* cp = (char *)getRaw();
	cp += sizeof(EVENTLOGRECORD);

	return std::string( cp );
}

std::string kjm::eventlog_record::ComputerName() {
	char* cp = (char *)getRaw();
	cp += sizeof(EVENTLOGRECORD);

	cp += strlen( cp ) + 1;		// SourceName���|�C���^��i�߂�

	return std::string( cp );
}

std::vector<BYTE> kjm::eventlog_record::Data() {

	std::vector<BYTE> data( DataLength() );

	for ( int i = 0; i < data.size(); i++) {
		data.at( i ) = *(((unsigned char *)getRaw()) + DataOffset() + i);
	}

	return data;
}


//---------------------------------------------------------------------
// OpenEventLog/CloseEventLog��΂ɂ���N���X
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// �C�x���g���O���J��
//---------------------------------------------------------------------
BOOL kjm::eventlog::OpenEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName ) {

	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	CloseEventLog();

	m_unc_server_name = ( lpUNCServerName ) ? lpUNCServerName : "";
	m_source_name = ( lpSourceName ) ? lpSourceName : "";

	m_hEventLog = ::OpenEventLog( lpUNCServerName, lpSourceName );
	return ( m_hEventLog ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// �ۑ������C�x���g���O���J��
//---------------------------------------------------------------------
BOOL kjm::eventlog::OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName ) {

	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	CloseEventLog();

	m_unc_server_name = ( lpUNCServerName ) ? lpUNCServerName : "";
	m_source_name = ( lpSourceName ) ? lpSourceName : "";
	m_file_name = ( lpFileName ) ? lpFileName : "";

	m_hEventLog = ::OpenBackupEventLog( lpUNCServerName, lpFileName );
	return ( m_hEventLog ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// �C�x���g���O�����
//---------------------------------------------------------------------
BOOL kjm::eventlog::CloseEventLog() {
	BOOL ret = TRUE;

	if ( m_hEventLog ) {
		ret = ::CloseEventLog( m_hEventLog );
		m_hEventLog = NULL;
	}

	return ret;
}


//---------------------------------------------------------------------
// �C�x���g���O��ǂ�
//---------------------------------------------------------------------
BOOL kjm::eventlog::ReadEventLog( DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventlog_record& record ) {

	DWORD dwReadBytes = 0;
	DWORD dwNextSize = 0;
	std::vector<BYTE> buffer;

	// �ɏ��̃o�b�t�@���m�ۂ��ăT�C�Y�����炤
	buffer.resize( 1 );

	BOOL bResult = ::ReadEventLog(
		m_hEventLog,
		dwReadFlags,
		dwRecordOffset,
		buffer.begin(),
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
		buffer.begin(),
		buffer.size(),
		&dwReadBytes,
		&dwNextSize);

	record = kjm::eventlog_record( m_source_name.c_str(), (EVENTLOGRECORD*)buffer.begin() );

	return bResult;
}
