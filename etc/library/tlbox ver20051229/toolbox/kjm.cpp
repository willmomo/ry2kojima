
#include <sstream>
#include <tchar.h>
#include "kjm.h"
#include <shlwapi.h>


//---------------------------------------------------------------------
// ���Ԃ��ȒP�Ɉ����N���X
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ���ݓ�����ێ�����C���X�^���X��Ԃ��B
//---------------------------------------------------------------------
kjm::time kjm::time::GetCurrentTime() {
	return kjm::time( ::time( NULL ) );
}


//---------------------------------------------------------------------
// �ێ�������t���Actime �֐��ŕ�����ɂ��ĕԂ��B
//---------------------------------------------------------------------
std::string kjm::time::ctime() {
	return kjm::chomp( std::string( ::ctime( &m_time ) ) );
}


//---------------------------------------------------------------------
// LoadLibraryEx/FreeLibrary��΂ɂ���N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------
kjm::library::library() : m_hLibModule( NULL ) {
}


//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kjm::library::~library() {
	FreeLibrary();
}


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


//---------------------------------------------------------------------
// 1���R�[�h����eventlog������킷�N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------
kjm::eventlog_record::eventlog_record() {
}

kjm::eventlog_record::eventlog_record( const kjm::eventlog_record& src ) :
	m_source_name( src.m_source_name ), m_raw_record( src.m_raw_record )
{
}

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


//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kjm::eventlog_record::~eventlog_record() {
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
				kjm::ExpandEnvironmentStrings( (LPCTSTR)ModuleName.begin(), ExpandedName );

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
			dwRet = kjm::FormatMessage(
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
// �R���X�g���N�^
//---------------------------------------------------------------------
kjm::eventlog::eventlog() : m_hEventLog( NULL ) {
}


//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kjm::eventlog::~eventlog() {
	CloseEventLog();
}


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
// �ۑ����ꂽ�C�x���g���O���J��
//---------------------------------------------------------------------
BOOL kjm::eventlog::OpenBackupEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName, LPCTSTR lpFileName ) {

	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	CloseEventLog();

	m_unc_server_name = ( lpUNCServerName ) ? lpUNCServerName : "";
	m_source_name = ( lpSourceName ) ? lpSourceName : "";

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


//---------------------------------------------------------------------
// �C�x���g���O���o�b�N�A�b�v����
//---------------------------------------------------------------------
BOOL kjm::eventlog::BackupEventLog( LPCTSTR lpBackupFileName ) {
	return ::BackupEventLog( m_hEventLog, lpBackupFileName );
}


//---------------------------------------------------------------------
// RegOpenKeyEx/RegCloseKey��΂ɂ���N���X
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------
kjm::registry::registry() : m_hKey( NULL ) {
}


//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kjm::registry::~registry() {
	RegCloseKey();
}


//---------------------------------------------------------------------
// ���W�X�g�����J��
//---------------------------------------------------------------------
LONG kjm::registry::RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired ) {

	// ���łɊJ���Ă��邩������Ȃ��̂ŁA����
	RegCloseKey();

	LONG ret = ::RegOpenKeyEx( hKey, lpSubKey, ulOptions, samDesired, &m_hKey );

	return ret;
}


//---------------------------------------------------------------------
// ���W�X�g�������
//---------------------------------------------------------------------
LONG kjm::registry::RegCloseKey() {
	LONG ret = ERROR_SUCCESS;

	if ( m_hKey ) {
		ret = ::RegCloseKey( m_hKey );
		m_hKey = NULL;
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
	ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, NULL, &cbData );
	if ( ret == ERROR_SUCCESS ) {

		// �o�b�t�@���m�ۂ��Đ����ɌĂяo��
		data.resize( cbData );
		ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, data.begin(), &cbData );
	}

	return ret;
}


//---------------------------------------------------------------------
// �����h���C�u(SMART)����������N���X
//---------------------------------------------------------------------

ATTRVALUEANDNAME kjm::attrValueAndNames[] = {
	{1,		"Raw Read Error Rate"},
	{2,		"Througput Performance"},
	{3,		"Spin Up Time"},
	{4,		"Start/Stop Count"},
	{5,		"Reallocated Sector Count"},
	{6,		"Read Channel Margin"},
	{7,		"Seek Error Rate"},
	{8,		"Seek Time Performance"},
	{9,		"Power On Hours Count"},
	{10,	"Spin Retry Count"},
	{11,	"Recalibration Retries"},
	{12,	"Device Power Cycle Count"},
	{191,	"G Sense Error Rate"},
	{192,	"Power-off Retract Count"},
	{193,	"Load/Unload Cycle Count"},
	{194,	"Temperature"},
	{195,	"Hardware ECC Recovered"},
	{196,	"Reallocation Event Count"},
	{197,	"Current Pending Sector Count"},
	{198,	"Off-line Scan Uncorrectable Sector Count"},
	{199,	"Ultra DMA CRC Error Rate"},
	{200,	"Write Error Rate"},
	{201,	"Soft Read Error Rate"},
	{202,	"Data Address Mark Error"},
	{203,	"Run Out Cancel"},
	{204,	"Soft ECC Correction"},
	{205,	"Thermal Asperity Rate"},
	{207,	"Spin High Current"},
	{208,	"Spin Buzz"},
	{209,	"Offline Seek Performance"},
	{223,	"Load/Unload Retry Count"},
	{0,		"Not used"}
};

//---------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------
kjm::physical_drive::physical_drive() : m_disk_handle( INVALID_HANDLE_VALUE ) {
}


//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kjm::physical_drive::~physical_drive() {
	CloseDrive();
}


//---------------------------------------------------------------------
// �����h���C�u���J��
//---------------------------------------------------------------------
BOOL kjm::physical_drive::OpenDrive( LPCTSTR driveName ) {
	m_disk_handle = ::CreateFile(driveName,
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL, OPEN_EXISTING, 0, NULL);

	return ( m_disk_handle != INVALID_HANDLE_VALUE ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// �����h���C�u�����
//---------------------------------------------------------------------
BOOL kjm::physical_drive::CloseDrive() {
	BOOL ret = TRUE;

	if ( m_disk_handle != INVALID_HANDLE_VALUE ) {
		ret = ::CloseHandle( m_disk_handle );
		m_disk_handle = INVALID_HANDLE_VALUE;
	}

	return ret;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.���T�|�[�g���Ă��邩��������
//		�Ԃ�l:0�ȊO�F�T�|�[�g���Ă���
//			   0    �F�T�|�[�g���Ă��Ȃ�
//-------------------------------------------------------------------------*/
bool kjm::physical_drive::isSupportedSMART()
{
	IDENTIFYDATA identifyData;
	if (0 != identifyDevice(&identifyData))
		return 0;
	/*------SMART�̎�������Ă��邩82Word�ڂ�0bit�𒲂ׂ�------------------*/
	return (identifyData.commandSet1 & 1) ? true : false;
}


/*---------------------------------------------------------------------------
//	IdentifyDevice�R�}���h�����s���f�o�C�X�����擾����
//		�Ԃ�l:0    �F����
//			   0�ȊO�F���s
//-------------------------------------------------------------------------*/
int kjm::physical_drive::identifyDevice(IDENTIFYDATA* identifyData)
{
	SENDCMDINPARAMS 		scip;
	IDENTIFYCMDOUTPARAMS	idCmdOut;
	DWORD       			bytesReturned;
    BOOL					status;
	int						ret;

	/*------�R�}���h�ݒ���s��---------------------------------------------*/
	memset(&idCmdOut, 0, sizeof(idCmdOut));
	memset(&scip, 0, sizeof(scip));
	scip.irDriveRegs.bCommandReg		= ID_CMD;
	
	/*------�R�}���h���s---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_RCV_DRIVE_DATA,
             				(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
               				(LPVOID)&idCmdOut,	sizeof(idCmdOut),
               				&bytesReturned, NULL);
	if(FALSE != status) {
		memcpy(identifyData, &idCmdOut.identifyData, sizeof(IDENTIFYDATA));
		ret = 0;
	} else {
		memset(identifyData, 0, sizeof(IDENTIFYDATA));
		ret = -1;
	}

	return ret;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.�@�\���~����
//		�Ԃ�l:0    �F����
//			   0�ȊO�F���s
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartDisable()
{
	SENDCMDINPARAMS 	scip;
	SENDCMDOUTPARAMS	scop;
	DWORD       		bytesReturned;
    BOOL				status;

	/*------�R�}���h�ݒ���s��---------------------------------------------*/
	memset(&scip, 0, sizeof(scip));
	memset(&scop, 0, sizeof(scop));
	scip.irDriveRegs.bFeaturesReg	= DISABLE_SMART;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;

	/*------�R�}���h���s---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_SEND_DRIVE_COMMAND,
							(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)&scop,	sizeof(SENDCMDOUTPARAMS) - 1,
							&bytesReturned,	NULL);

	return FALSE != status? 0 : -1;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.�@�\��L���ɂ���
//		�Ԃ�l:0    �F����
//			   0�ȊO�F���s
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartEnable()
{
	SENDCMDINPARAMS 	scip;
	SENDCMDOUTPARAMS	scop;
	DWORD       		bytesReturned;
    BOOL				status;

	/*------�R�}���h�ݒ���s��---------------------------------------------*/
	memset(&scip, 0, sizeof(scip));
	memset(&scop, 0, sizeof(scop));
	scip.irDriveRegs.bFeaturesReg	= ENABLE_SMART;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;

	/*------�R�}���h���s---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_SEND_DRIVE_COMMAND,
							(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)&scop,	sizeof(SENDCMDOUTPARAMS) - 1,
							&bytesReturned,	NULL);

	return FALSE != status? 0 : -1;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.�̎����Z�[�u�@�\��L��or�����ɂ���
//		�Ԃ�l:0    �F����
//			   0�ȊO�F���s
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartEnableDisableAutoSave(char enable)
{
	SENDCMDINPARAMS 	scip;
	SENDCMDOUTPARAMS	scop;
	DWORD       		bytesReturned;
    BOOL				status;

	/*------�R�}���h�ݒ���s��---------------------------------------------*/
	memset(&scip, 0, sizeof(scip));
	memset(&scop, 0, sizeof(scop));
	scip.irDriveRegs.bFeaturesReg		= ENABLE_DISABLE_AUTOSAVE;
	scip.irDriveRegs.bSectorCountReg	= enable;
	scip.irDriveRegs.bCylLowReg			= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg		= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg		= SMART_CMD;

	/*------�R�}���h���s---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_SEND_DRIVE_COMMAND,
							(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)&scop,	sizeof(SENDCMDOUTPARAMS) - 1,
							&bytesReturned,	NULL);

	return FALSE != status? 0 : -1;
}


/*---------------------------------------------------------------------------
//	�f�o�C�X�̐M���x���擾����
//		�Ԃ�l:0      �F�����i���Ȃ��j
//		       1      �F�����i��肠��j
//			   0,1�ȊO�F���s
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartReturnStatus()
{
	SENDCMDINPARAMS 		scip;
	READSTATUSCMDOUTPARAMS	ideRegOutCmd;
	DWORD       			bytesReturned;
    BOOL					status;

	/*------�R�}���h�ݒ���s��---------------------------------------------*/
	memset(&scip, 0, sizeof(scip));
	memset(&ideRegOutCmd, 0, sizeof(ideRegOutCmd));
	scip.irDriveRegs.bFeaturesReg	= RETURN_SMART_STATUS;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;

	/*------�R�}���h���s---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_SEND_DRIVE_COMMAND,
							(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)&ideRegOutCmd,	sizeof(ideRegOutCmd),
							&bytesReturned,	NULL);
	if (FALSE == status)
		return -1;

	if (0x4f == ideRegOutCmd.ideRegData.bCylLowReg &&
		0xc2 == ideRegOutCmd.ideRegData.bCylHighReg )
		return 0;/*���Ȃ�*/
	if (0xf4 == ideRegOutCmd.ideRegData.bCylLowReg &&
		0x2c == ideRegOutCmd.ideRegData.bCylHighReg )
		return 1;/*��肠��*/
	return -1;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.�̑����l���擾����
//		�Ԃ�l:0    �F����
//			   0�ȊO�F���s
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartReadAttributeValues(SMARTATTRIBUTESDATA* attrData)
{
	SENDCMDINPARAMS 		scip;
	ATTRIBUTECMDOUTPARAMS	attrCmdOut;
	DWORD       			bytesReturned;
    BOOL					status;
	int						ret;

	/*------�R�}���h�ݒ���s��---------------------------------------------*/
	memset(&attrCmdOut, 0, sizeof(attrCmdOut));
	memset(&scip, 0, sizeof(scip));
	scip.irDriveRegs.bFeaturesReg	= READ_ATTRIBUTES;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;
	
	/*------�R�}���h���s---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_RCV_DRIVE_DATA,
             				(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
               				(LPVOID)&attrCmdOut,	sizeof(attrCmdOut),
               				&bytesReturned, NULL);
	if(FALSE != status) {
		memcpy(attrData, &attrCmdOut.attributeData, sizeof(SMARTATTRIBUTESDATA));
		ret = 0;
	} else {
		memset(attrData, 0, sizeof(SMARTATTRIBUTESDATA));
		ret = -1;
	}

	return ret;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.�̂������l���擾����
//		�Ԃ�l:0    �F����
//			   0�ȊO�F���s
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartReadAttributeThresholds(SMARTHRESHOLDSDATA* thData)
{
	SENDCMDINPARAMS 		scip;
	THRESHOLDCMDOUTPARAMS	thCmdOut;
	DWORD       			bytesReturned;
    BOOL					status;
	int						ret;

	/*------�R�}���h�ݒ���s��---------------------------------------------*/
	memset(&thCmdOut, 0, sizeof(thCmdOut));
	memset(&scip, 0, sizeof(scip));
	scip.irDriveRegs.bFeaturesReg	= READ_THRESHOLDS;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;
	
	/*------�R�}���h���s---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_RCV_DRIVE_DATA,
             				(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
               				(LPVOID)&thCmdOut,	sizeof(thCmdOut),
               				&bytesReturned, NULL);
	if(FALSE != status) {
		memcpy(thData, &thCmdOut.thresholdData, sizeof(SMARTHRESHOLDSDATA));
		ret = 0;
	} else {
		memset(thData, 0, sizeof(SMARTHRESHOLDSDATA));
		ret = -1;
	}

	return ret;
}


//---------------------------------------------------------------------
// SMART���g���āA���݂�HDD���x���擾����
//---------------------------------------------------------------------
int kjm::physical_drive::getTemperature() {
	
	int temperature = -1;

	SMARTATTRIBUTESDATA	attrData;
	memset( &attrData, 0, sizeof( attrData ) );

	if ( smartReadAttributeValues( &attrData ) == 0 ) {

		for ( int i = 0; i < countof( attrData.AttributeData ); i++ ) {

			// ���x����T��
			if ( attrData.AttributeData[ i ].attributeID == 194 ) {
				temperature = attrData.AttributeData[ i ].vendorSpecific[ 0 ];
				break;
			}
		}
	}

	return temperature;
}


//---------------------------------------------------------------------
// SMART���g���āA���݂�Drive Power Cycle Count���擾����
//---------------------------------------------------------------------
int kjm::physical_drive::getDrivePowerCycleCount() {
	
	int value = -1;

	SMARTATTRIBUTESDATA	attrData;
	memset( &attrData, 0, sizeof( attrData ) );

	if ( smartReadAttributeValues( &attrData ) == 0 ) {

		for ( int i = 0; i < countof( attrData.AttributeData ); i++ ) {

			// Drive Power Cycle Count��T��
			if ( attrData.AttributeData[ i ].attributeID == 12 ) {

				// ���Ԃ� DWORD �ŁA�P�ʂ́utimes (��)�v
				value = *(DWORD*)attrData.AttributeData[ i ].vendorSpecific;
				break;
			}
		}
	}

	return value;
}


//---------------------------------------------------------------------
// SMART���g���āA���݂�Power-On Hours���擾����
//---------------------------------------------------------------------
int kjm::physical_drive::getPowerOnHours() {
	
	int value = -1;

	SMARTATTRIBUTESDATA	attrData;
	memset( &attrData, 0, sizeof( attrData ) );

	if ( smartReadAttributeValues( &attrData ) == 0 ) {

		for ( int i = 0; i < countof( attrData.AttributeData ); i++ ) {

			// Power-On Hours��T��
			if ( attrData.AttributeData[ i ].attributeID == 9 ) {

				// ���Ԃ� DWORD �ŁA�P�ʂ́u���v
				value = *(DWORD*)attrData.AttributeData[ i ].vendorSpecific;
				break;
			}
		}
	}

	return value;
}


//---------------------------------------------------------------------
// SMART���g���āA���݂�Start/Stop Count���擾����
//---------------------------------------------------------------------
int kjm::physical_drive::getStartStopCount() {
	
	int value = -1;

	SMARTATTRIBUTESDATA	attrData;
	memset( &attrData, 0, sizeof( attrData ) );

	if ( smartReadAttributeValues( &attrData ) == 0 ) {

		for ( int i = 0; i < countof( attrData.AttributeData ); i++ ) {

			// Power-On Hours��T��
			if ( attrData.AttributeData[ i ].attributeID == 4 ) {

				// ���Ԃ� DWORD �ŁA�P�ʂ́u�񐔁v
				value = *(DWORD*)attrData.AttributeData[ i ].vendorSpecific;
				break;
			}
		}
	}

	return value;
}


//---------------------------------------------------------------------
// SMART����CSV�`���̕�����ɂ���
//---------------------------------------------------------------------
std::string kjm::physical_drive::toCSVString() {

	std::stringstream s;

	SMARTATTRIBUTESDATA	attrData;
	SMARTHRESHOLDSDATA thData;

	memset( &attrData, 0, sizeof( attrData ) );
	memset( &thData, 0, sizeof( thData ) );

	smartReadAttributeValues( &attrData );
	smartReadAttributeThresholds( &thData );

	for ( int index = 0; index < NUM_ATTRIBUTE_STRUCTS; ++index ) {
		int attrID = attrData.AttributeData[ index ].attributeID;
		if ( 0 != attrID ) {
			char*	attrName;
			int		arrayIndex = 0;
			while ( 0 != kjm::attrValueAndNames[arrayIndex].value ) {
				if ( attrID == kjm::attrValueAndNames[arrayIndex].value ) {
					break;
				}
				++arrayIndex;
			}
			
			if ( 0 != kjm::attrValueAndNames[arrayIndex].value ) {
				attrName = kjm::attrValueAndNames[arrayIndex].name;
			} else {
				attrName = "Unknown Attribute";
			}


			s << attrID << _T( "," );
			s << _T( "\"" ) << attrName << _T( "\"," );
			s << attrData.AttributeData[ index ].statusFlag << _T( "," );
			s << ( WORD )attrData.AttributeData[ index ].attributeValue << _T( "," );
			s << ( WORD )attrData.AttributeData[ index ].worstValue << _T( "," );
			
			s << ( WORD )thData.ThresholdData[index].attributeThreshold << _T( "," );
			
			for ( int i = 0; i < countof( attrData.AttributeData[ index ].vendorSpecific ); i++ ) {
				s << ( WORD )attrData.AttributeData[ index ].vendorSpecific[ i ] << _T( "," );
			}
			s << std::endl;
		}
	}

	return s.str();
}


//---------------------------------------------------------------------
// �p�X�p�̏�����ǉ����� string �N���X
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// �R���X�g���N�^
//---------------------------------------------------------------------
kjm::path_string::path_string() : std::basic_string<char>() {
}
kjm::path_string::path_string( const kjm::path_string& rhs ) : std::basic_string<char>( rhs ) {
}
kjm::path_string::path_string( std::string::const_pointer s ) : std::basic_string<char>( s ) {
}


//---------------------------------------------------------------------
// �f�X�g���N�^
//---------------------------------------------------------------------
kjm::path_string::~path_string() {
}


//---------------------------------------------------------------------
// PathFileExists�̃��b�p�[
//---------------------------------------------------------------------
BOOL kjm::path_string::FileExists() {
	return ::PathFileExists( this->c_str() );
}


//---------------------------------------------------------------------
// PathRemoveFileSpec�̃��b�p�[
//---------------------------------------------------------------------
kjm::path_string& kjm::path_string::RemoveFileSpec() {

	std::vector<char> work( begin(), end() );
	work.push_back( '\0' );

	if ( ::PathRemoveFileSpec( work.begin() ) ) {
		*this = work.begin();
	}

	return *this;
}


//---------------------------------------------------------------------
// PathAppend�̃��b�p�[
//---------------------------------------------------------------------
kjm::path_string& kjm::path_string::Append( LPCTSTR pMore) {

	std::vector<char> work( begin(), end() );
	work.push_back( '\0' );
	work.resize( work.size() + strlen( pMore ) + MAX_PATH );	// �\���ȃo�b�t�@���m��

	if ( ::PathAppend( work.begin(), pMore ) ) {
		*this = work.begin();
	}

	return *this;
}


//---------------------------------------------------------------------
// PathRenameExtension�̃��b�p�[
//---------------------------------------------------------------------
kjm::path_string& kjm::path_string::RenameExtension( LPCTSTR pszExt ) {

	std::vector<char> work( begin(), end() );
	work.push_back( '\0' );
	work.resize( work.size() + strlen( pszExt ) + MAX_PATH );	// �\���ȃo�b�t�@���m��

	if ( ::PathRenameExtension( work.begin(), pszExt ) ) {
		*this = work.begin();
	}

	return *this;
}


//---------------------------------------------------------------------
// path_string���쐬����t�@�N�g�����\�b�h
//---------------------------------------------------------------------
kjm::path_string kjm::path_string::GetModuleFileName( HMODULE hModule /*=NULL*/ ) {

	std::vector<char> filename;
	kjm::GetModuleFileName( NULL, filename );

	return kjm::path_string( filename.begin() );
}


//---------------------------------------------------------------------
// ������ like �ȕ֗��֐�
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// �s���̉��s����菜��
//---------------------------------------------------------------------
std::string& kjm::chomp( std::string& s ) {

	// �s���ɉ��s����������Ƃ��́A��菜��
	while ( !s.empty() && ( *( s.rbegin() ) == '\n' ) ) {
		s.erase( s.end() - 1 );
	}

	return s;
}


//---------------------------------------------------------------------
// Win32API ExpandEnvironmentStrings�����S�Ɏg���֐�
//---------------------------------------------------------------------
DWORD kjm::ExpandEnvironmentStrings( LPCTSTR lpSrc, std::vector<TCHAR>& rDst ) {

	DWORD dwRet;
	
	while ( ( dwRet = ::ExpandEnvironmentStrings( lpSrc, rDst.begin(), rDst.size() ) ) > rDst.size() ) {
		if ( dwRet == 0 ) {
			break;
		} else {
			rDst.resize( dwRet );
		}
	}

	return dwRet;
}

//---------------------------------------------------------------------
// Win32API FormatMessage�����S�Ɏg���֐�
//---------------------------------------------------------------------
DWORD kjm::FormatMessage( DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, std::vector<TCHAR>& buffer, va_list* Arguments ) {

	LPTSTR pMessage = NULL;

	DWORD dwRet = ::FormatMessage(
		dwFlags | FORMAT_MESSAGE_ALLOCATE_BUFFER,
		lpSource,
		dwMessageId,
		dwLanguageId,
		(LPSTR)&pMessage,
		0,
		Arguments);

	if ( pMessage ) {
		buffer.resize( lstrlen( pMessage ) + 1 );
		lstrcpy( buffer.begin(), pMessage );
		::LocalFree( pMessage );
	}

	return dwRet;
}


//---------------------------------------------------------------------
// Win32API GetModuleFileName �����S�Ɏg���֐�
//---------------------------------------------------------------------
DWORD kjm::GetModuleFileName( HMODULE hModule, std::vector<TCHAR>& rFilename ) {

	rFilename.resize( MAX_PATH );

	DWORD dwRet;
	while ( ( dwRet = ::GetModuleFileName( hModule, rFilename.begin(), rFilename.size() ) ) == rFilename.size() ) {
		rFilename.resize( rFilename.size() * 2 );
	}

	return dwRet;
}


//---------------------------------------------------------------------
// Win32API GetPrivateProfileString �����S�Ɏg���֐�
//---------------------------------------------------------------------
DWORD kjm::GetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, std::vector<TCHAR>& returnedString, LPCTSTR lpFileName ) {

	DWORD dwRet = 512;

	do {
		returnedString.resize( dwRet * 2 );
		dwRet = ::GetPrivateProfileString( lpAppName, lpKeyName, lpDefault, returnedString.begin(), returnedString.size(), lpFileName );
	} while ( dwRet == returnedString.size() - 1 || dwRet == returnedString.size() - 2 );

	return dwRet;
}

//---------------------------------------------------------------------
// Win32API LoadString �����S�Ɏg���֐�
//---------------------------------------------------------------------
std::string kjm::kjm_LoadString( HINSTANCE hInstance, UINT uID ) {

	int ret = 0;
	std::vector<char> work( 512 );

	while ( ( ret = ::LoadString( hInstance, uID, work.begin(), work.size() ) ) == work.size() ) {

		if ( ret == 0 ) {
			// �G���[�Ȃ̂ŁA�󕶎����Ԃ��ďI���B
			return std::string();
		}

		work.resize( work.size() * 2 );
	}

	return std::string( work.begin() );
}


//---------------------------------------------------------------------
// �����P�ɕ֗��Ȋ֐�
//---------------------------------------------------------------------
//---------------------------------------------------------------------
// ���K�w�f�B���N�g���̍쐬�B
//
// @param lpPathName �쐬����t�H���_�̃t���p�X�B
// @return ����I���̂Ƃ�TRUE�A�t�H���_���쐬�ł��Ȃ������Ƃ�FALSE��Ԃ��B
//---------------------------------------------------------------------
BOOL kjm::MakeDirectory( LPCTSTR lpDir )
{
	// �t�H���_�����݂���΁A����I��
	if ( ::PathFileExists( lpDir ) )	return TRUE;

	// ���[�g�t�H���_���w���Ă���ɂ�������炸�A�t�H���_�������Ƃ��́A�ُ�I��
	if ( ::PathIsRoot( lpDir ) )		return FALSE;

	// ���̃t�H���_���쐬����
	if ( MakeDirectory( kjm::path_string( lpDir ).Append( _T( ".." ) ).c_str() ) ) {
		return ::CreateDirectory( lpDir, NULL );
	} else {
		return FALSE;
	}
}
