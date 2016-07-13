
#include "kjm.h"


//---------------------------------------------------------------------
// LoadLibraryEx/FreeLibraryを対にするクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------
kjm::library::library() : m_hLibModule( NULL ) {
}


//---------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------
kjm::library::~library() {
	FreeLibrary();
}


//---------------------------------------------------------------------
// ライブラリのロード
//---------------------------------------------------------------------
BOOL kjm::library::LoadLibraryEx( LPCTSTR lpLibFileName, HANDLE hFile, DWORD dwFlags ) {
	
	// すでにロードしているかもしれないので、開放
	FreeLibrary();

	m_hLibModule = ::LoadLibraryEx( lpLibFileName, hFile, dwFlags );
	
	return ( m_hLibModule ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// ライブラリの開放
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
// 1レコード分のeventlogをあらわすクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// コンストラクタ
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

	// イベントソースの保存
	m_source_name = lpSourceName;

	// 生情報のコピー
	m_raw_record.resize( src->Length );
	memmove( m_raw_record.begin(), src, src->Length );
}


//---------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------
kjm::eventlog_record::~eventlog_record() {
}


/* 埋込み文字列の取得 */
int kjm::eventlog_record::GetArgs(const EVENTLOGRECORD *pBuf, std::vector<char*>& args )
{
	args.clear();

	char *cp;
	WORD ArgCount;

	if(pBuf->NumStrings == 0) return args.size();

	/* 引数リストを取得 */
	args.resize( pBuf->NumStrings );
	cp = (char *)pBuf + (pBuf->StringOffset);

	for(ArgCount=0; ArgCount<pBuf->NumStrings; ArgCount++) {
		args[ArgCount] = cp;
		cp += strlen(cp) + 1;
	}
	return args.size();
}

/* ソース名からモジュール名を取得 */
BOOL kjm::eventlog_record::GetModuleNameFromSourceName( const char *SourceName, const char *EntryName, std::vector<TCHAR>& ExpandedName) {
	DWORD lResult;
	std::vector<BYTE> ModuleName;
	BOOL bReturn = FALSE;

	// Applicationログ用のレジストリキーをオープン
	kjm::registry appKey;
	
	std::string key = std::string( "SYSTEM\\CurrentControlSet\\Services\\EventLog\\" );
	key += m_source_name;

	lResult = appKey.RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		key.c_str(),
		0,
		KEY_READ);

	if ( lResult == ERROR_SUCCESS ) {

		// ソースの情報が格納されているレジストリをオープン

		kjm::registry sourceKey;

		lResult = sourceKey.RegOpenKeyEx(
			appKey.getKey(),
			SourceName,
			0,
			KEY_READ);

		if ( lResult == ERROR_SUCCESS ) {

			// ソースモジュール名を取得
			lResult = sourceKey.RegQueryValueEx(
				"EventMessageFile",
				NULL,
				NULL,
				ModuleName);

			if ( lResult == ERROR_SUCCESS ) {

				// 環境変数を展開
				kjm::ExpandEnvironmentStrings( (LPCTSTR)ModuleName.begin(), ExpandedName );

				/* 正常終了 */
				bReturn = TRUE;
			}
		}
	}

	return bReturn;
}

/* メッセージの表示 */
std::string kjm::eventlog_record::DispMessage( const char *SourceName, const char *EntryName, const char **Args, DWORD MessageId) {
	BOOL bResult;
	BOOL bReturn = FALSE;
	std::vector<TCHAR> SourceModuleName;
	std::vector<TCHAR> message;
	char* oneModuleName;
	DWORD dwRet;

	/* ソースモジュール名を取得 */	
	bResult = GetModuleNameFromSourceName(SourceName, EntryName, SourceModuleName);
	if(!bResult) return std::string("");

	oneModuleName = strtok( SourceModuleName.begin(), ";" );
	while ( oneModuleName ) {

		kjm::library sourceModule;

		// ソースモジュールをロード
		sourceModule.LoadLibraryEx(
			oneModuleName,
			NULL,
			DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);

		if ( sourceModule.getInstance() ) {

			/* メッセージを作成 */
			dwRet = kjm::FormatMessage(
				FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_ARGUMENT_ARRAY,
				sourceModule.getInstance() ,
				MessageId,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				message,
				(va_list *)Args);


			/* 正常終了 */
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
// 各種アクセサ(生情報からデータを切り出す)
//---------------------------------------------------------------------
std::string kjm::eventlog_record::SourceName() {
	char* cp = (char *)getRaw();
	cp += sizeof(EVENTLOGRECORD);

	return std::string( cp );
}

std::string kjm::eventlog_record::ComputerName() {
	char* cp = (char *)getRaw();
	cp += sizeof(EVENTLOGRECORD);

	cp += strlen( cp ) + 1;		// SourceName分ポインタを進める

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
// OpenEventLog/CloseEventLogを対にするクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------
kjm::eventlog::eventlog() : m_hEventLog( NULL ) {
}


//---------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------
kjm::eventlog::~eventlog() {
	CloseEventLog();
}


//---------------------------------------------------------------------
// イベントログを開く
//---------------------------------------------------------------------
BOOL kjm::eventlog::OpenEventLog( LPCTSTR lpUNCServerName, LPCTSTR lpSourceName ) {

	// すでに開いているかもしれないので、閉じる
	CloseEventLog();

	m_unc_server_name = ( lpUNCServerName ) ? lpUNCServerName : "";
	m_source_name = ( lpSourceName ) ? lpSourceName : "";

	m_hEventLog = ::OpenEventLog( lpUNCServerName, lpSourceName );
	return ( m_hEventLog ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// イベントログを閉じる
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
// イベントログを読む
//---------------------------------------------------------------------
BOOL kjm::eventlog::ReadEventLog( DWORD dwReadFlags, DWORD dwRecordOffset, kjm::eventlog_record& record ) {

	DWORD dwReadBytes = 0;
	DWORD dwNextSize = 0;
	std::vector<BYTE> buffer;

	// 極小のバッファを確保してサイズをもらう
	buffer.resize( 1 );

	BOOL bResult = ::ReadEventLog(
		m_hEventLog,
		dwReadFlags,
		dwRecordOffset,
		buffer.begin(),
		buffer.size(),
		&dwReadBytes,
		&dwNextSize);

	// バッファ不足エラー以外は、エラー終了
	if(!bResult && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
		return bResult;
	}

	// バッファの再割り当て
	buffer.resize( dwNextSize );

	/* イベントログの読み取り */
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
// RegOpenKeyEx/RegCloseKeyを対にするクラス
//---------------------------------------------------------------------

//---------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------
kjm::registry::registry() : m_hKey( NULL ) {
}


//---------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------
kjm::registry::~registry() {
	RegCloseKey();
}


//---------------------------------------------------------------------
// レジストリを開く
//---------------------------------------------------------------------
LONG kjm::registry::RegOpenKeyEx( HKEY hKey, LPCTSTR lpSubKey, DWORD ulOptions, REGSAM samDesired ) {

	// すでに開いているかもしれないので、閉じる
	RegCloseKey();

	LONG ret = ::RegOpenKeyEx( hKey, lpSubKey, ulOptions, samDesired, &m_hKey );

	return ret;
}


//---------------------------------------------------------------------
// レジストリを閉じる
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
// レジストリから値を取得
//---------------------------------------------------------------------
LONG kjm::registry::RegQueryValueEx( LPCTSTR lpValueName, LPDWORD lpReserved, LPDWORD lpType, std::vector<BYTE>& data ) {

	LONG ret;
	DWORD cbData = 0;

	// 必要なバッファサイズを取得
	ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, NULL, &cbData );
	if ( ret == ERROR_SUCCESS ) {

		// バッファを確保して正式に呼び出す
		data.resize( cbData );
		ret = ::RegQueryValueEx( m_hKey, lpValueName, lpReserved, lpType, data.begin(), &cbData );
	}

	return ret;
}


//---------------------------------------------------------------------
// 物理ドライブ(SMART)を処理するクラス
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
	{196,	"Reallocation Event Count"},
	{197,	"Current Pending Sector Count"},
	{198,	"Off-line Scan Uncorrectable Sector Count"},
	{199,	"Ultra DMA CRC Error Rate"},
	{200,	"Write Error Rate"},
	{201,	"Soft Read Error Rate"},
	{223,	"Load/Unload Retry Count"},
	{0,		"Not used"}
};

//---------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------
kjm::physical_drive::physical_drive() : m_disk_handle( INVALID_HANDLE_VALUE ) {
}


//---------------------------------------------------------------------
// デストラクタ
//---------------------------------------------------------------------
kjm::physical_drive::~physical_drive() {
	CloseDrive();
}


//---------------------------------------------------------------------
// 物理ドライブを開く
//---------------------------------------------------------------------
BOOL kjm::physical_drive::OpenDrive( LPCTSTR driveName ) {
	m_disk_handle = ::CreateFile(driveName,
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL, OPEN_EXISTING, 0, NULL);

	return ( m_disk_handle != INVALID_HANDLE_VALUE ) ? TRUE : FALSE;
}


//---------------------------------------------------------------------
// 物理ドライブを閉じる
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
//	S.M.A.R.T.をサポートしているか調査する
//		返り値:0以外：サポートしている
//			   0    ：サポートしていない
//-------------------------------------------------------------------------*/
bool kjm::physical_drive::isSupportedSMART()
{
	IDENTIFYDATA identifyData;
	if (0 != identifyDevice(&identifyData))
		return 0;
	/*------SMARTの実装されているか82Word目の0bitを調べる------------------*/
	return (identifyData.commandSet1 & 1) ? true : false;
}


/*---------------------------------------------------------------------------
//	IdentifyDeviceコマンドを実行しデバイス情報を取得する
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int kjm::physical_drive::identifyDevice(IDENTIFYDATA* identifyData)
{
	SENDCMDINPARAMS 		scip;
	IDENTIFYCMDOUTPARAMS	idCmdOut;
	DWORD       			bytesReturned;
    BOOL					status;
	int						ret;

	/*------コマンド設定を行う---------------------------------------------*/
	memset(&idCmdOut, 0, sizeof(idCmdOut));
	memset(&scip, 0, sizeof(scip));
	scip.irDriveRegs.bCommandReg		= ID_CMD;
	
	/*------コマンド実行---------------------------------------------------*/
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
//	S.M.A.R.T.機能を停止する
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartDisable()
{
	SENDCMDINPARAMS 	scip;
	SENDCMDOUTPARAMS	scop;
	DWORD       		bytesReturned;
    BOOL				status;

	/*------コマンド設定を行う---------------------------------------------*/
	memset(&scip, 0, sizeof(scip));
	memset(&scop, 0, sizeof(scop));
	scip.irDriveRegs.bFeaturesReg	= DISABLE_SMART;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;

	/*------コマンド実行---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_SEND_DRIVE_COMMAND,
							(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)&scop,	sizeof(SENDCMDOUTPARAMS) - 1,
							&bytesReturned,	NULL);

	return FALSE != status? 0 : -1;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.機能を有効にする
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartEnable()
{
	SENDCMDINPARAMS 	scip;
	SENDCMDOUTPARAMS	scop;
	DWORD       		bytesReturned;
    BOOL				status;

	/*------コマンド設定を行う---------------------------------------------*/
	memset(&scip, 0, sizeof(scip));
	memset(&scop, 0, sizeof(scop));
	scip.irDriveRegs.bFeaturesReg	= ENABLE_SMART;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;

	/*------コマンド実行---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_SEND_DRIVE_COMMAND,
							(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)&scop,	sizeof(SENDCMDOUTPARAMS) - 1,
							&bytesReturned,	NULL);

	return FALSE != status? 0 : -1;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.の自動セーブ機能を有効or無効にする
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartEnableDisableAutoSave(char enable)
{
	SENDCMDINPARAMS 	scip;
	SENDCMDOUTPARAMS	scop;
	DWORD       		bytesReturned;
    BOOL				status;

	/*------コマンド設定を行う---------------------------------------------*/
	memset(&scip, 0, sizeof(scip));
	memset(&scop, 0, sizeof(scop));
	scip.irDriveRegs.bFeaturesReg		= ENABLE_DISABLE_AUTOSAVE;
	scip.irDriveRegs.bSectorCountReg	= enable;
	scip.irDriveRegs.bCylLowReg			= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg		= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg		= SMART_CMD;

	/*------コマンド実行---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_SEND_DRIVE_COMMAND,
							(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)&scop,	sizeof(SENDCMDOUTPARAMS) - 1,
							&bytesReturned,	NULL);

	return FALSE != status? 0 : -1;
}


/*---------------------------------------------------------------------------
//	デバイスの信頼度を取得する
//		返り値:0      ：成功（問題なし）
//		       1      ：成功（問題あり）
//			   0,1以外：失敗
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartReturnStatus()
{
	SENDCMDINPARAMS 		scip;
	READSTATUSCMDOUTPARAMS	ideRegOutCmd;
	DWORD       			bytesReturned;
    BOOL					status;

	/*------コマンド設定を行う---------------------------------------------*/
	memset(&scip, 0, sizeof(scip));
	memset(&ideRegOutCmd, 0, sizeof(ideRegOutCmd));
	scip.irDriveRegs.bFeaturesReg	= RETURN_SMART_STATUS;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;

	/*------コマンド実行---------------------------------------------------*/
	status = ::DeviceIoControl(m_disk_handle,	SMART_SEND_DRIVE_COMMAND,
							(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
							(LPVOID)&ideRegOutCmd,	sizeof(ideRegOutCmd),
							&bytesReturned,	NULL);
	if (FALSE == status)
		return -1;

	if (0x4f == ideRegOutCmd.ideRegData.bCylLowReg &&
		0xc2 == ideRegOutCmd.ideRegData.bCylHighReg )
		return 0;/*問題なし*/
	if (0xf4 == ideRegOutCmd.ideRegData.bCylLowReg &&
		0x2c == ideRegOutCmd.ideRegData.bCylHighReg )
		return 1;/*問題あり*/
	return -1;
}


/*---------------------------------------------------------------------------
//	S.M.A.R.T.の属性値を取得する
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartReadAttributeValues(SMARTATTRIBUTESDATA* attrData)
{
	SENDCMDINPARAMS 		scip;
	ATTRIBUTECMDOUTPARAMS	attrCmdOut;
	DWORD       			bytesReturned;
    BOOL					status;
	int						ret;

	/*------コマンド設定を行う---------------------------------------------*/
	memset(&attrCmdOut, 0, sizeof(attrCmdOut));
	memset(&scip, 0, sizeof(scip));
	scip.irDriveRegs.bFeaturesReg	= READ_ATTRIBUTES;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;
	
	/*------コマンド実行---------------------------------------------------*/
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
//	S.M.A.R.T.のしきい値を取得する
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int kjm::physical_drive::smartReadAttributeThresholds(SMARTHRESHOLDSDATA* thData)
{
	SENDCMDINPARAMS 		scip;
	THRESHOLDCMDOUTPARAMS	thCmdOut;
	DWORD       			bytesReturned;
    BOOL					status;
	int						ret;

	/*------コマンド設定を行う---------------------------------------------*/
	memset(&thCmdOut, 0, sizeof(thCmdOut));
	memset(&scip, 0, sizeof(scip));
	scip.irDriveRegs.bFeaturesReg	= READ_THRESHOLDS;
	scip.irDriveRegs.bCylLowReg		= SMART_CYL_LOW;//0x4f
	scip.irDriveRegs.bCylHighReg	= SMART_CYL_HI;//0xc2
	scip.irDriveRegs.bCommandReg	= SMART_CMD;
	
	/*------コマンド実行---------------------------------------------------*/
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
// SMARTを使って、現在のHDD温度を取得する
//---------------------------------------------------------------------
int kjm::physical_drive::getTemperature() {
	
	int temperature = -1;

	SMARTATTRIBUTESDATA	attrData;
	memset( &attrData, 0, sizeof( attrData ) );

	if ( smartReadAttributeValues( &attrData ) == 0 ) {

		for ( int i = 0; i < countof( attrData.AttributeData ); i++ ) {

			// 温度情報を探す
			if ( attrData.AttributeData[ i ].attributeID == 194 ) {
				temperature = attrData.AttributeData[ i ].vendorSpecific[ 0 ];
				break;
			}
		}
	}

	return temperature;
}


//---------------------------------------------------------------------
// SMARTを使って、現在のDrive Power Cycle Countを取得する
//---------------------------------------------------------------------
int kjm::physical_drive::getDrivePowerCycleCount() {
	
	int value = -1;

	SMARTATTRIBUTESDATA	attrData;
	memset( &attrData, 0, sizeof( attrData ) );

	if ( smartReadAttributeValues( &attrData ) == 0 ) {

		for ( int i = 0; i < countof( attrData.AttributeData ); i++ ) {

			// Drive Power Cycle Countを探す
			if ( attrData.AttributeData[ i ].attributeID == 12 ) {

				// たぶん DWORD で、単位は「times (回数)」
				value = *(DWORD*)attrData.AttributeData[ i ].vendorSpecific;
				break;
			}
		}
	}

	return value;
}


//---------------------------------------------------------------------
// SMARTを使って、現在のPower-On Hoursを取得する
//---------------------------------------------------------------------
int kjm::physical_drive::getPowerOnHours() {
	
	int value = -1;

	SMARTATTRIBUTESDATA	attrData;
	memset( &attrData, 0, sizeof( attrData ) );

	if ( smartReadAttributeValues( &attrData ) == 0 ) {

		for ( int i = 0; i < countof( attrData.AttributeData ); i++ ) {

			// Power-On Hoursを探す
			if ( attrData.AttributeData[ i ].attributeID == 9 ) {

				// たぶん DWORD で、単位は「分」
				value = *(DWORD*)attrData.AttributeData[ i ].vendorSpecific;
				break;
			}
		}
	}

	return value;
}


//---------------------------------------------------------------------
// SMARTを使って、現在のStart/Stop Countを取得する
//---------------------------------------------------------------------
int kjm::physical_drive::getStartStopCount() {
	
	int value = -1;

	SMARTATTRIBUTESDATA	attrData;
	memset( &attrData, 0, sizeof( attrData ) );

	if ( smartReadAttributeValues( &attrData ) == 0 ) {

		for ( int i = 0; i < countof( attrData.AttributeData ); i++ ) {

			// Power-On Hoursを探す
			if ( attrData.AttributeData[ i ].attributeID == 4 ) {

				// たぶん DWORD で、単位は「回数」
				value = *(DWORD*)attrData.AttributeData[ i ].vendorSpecific;
				break;
			}
		}
	}

	return value;
}


//---------------------------------------------------------------------
// Win32API ExpandEnvironmentStringsを安全に使う関数
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
// Win32API FormatMessageを安全に使う関数
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

