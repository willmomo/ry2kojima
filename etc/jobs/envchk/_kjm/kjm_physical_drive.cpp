//---------------------------------------------------------------------
// kjm::physical_drive class
//---------------------------------------------------------------------


#include <sstream>
#include "kjm_util.h"
#include "kjm_physical_drive.h"


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
	{220,	"Disk Shift"},
	{222,	"Loaded Hours"},
	{223,	"Load/Unload Retry Count"},
	{224,	"Load Friction"},
	{226,	"Load-in Time"},
	{228,	"Power-Off Retract Count"},
	{240,	"Head Flying Hours"},
	{0,		"Not used"}
};


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
// SMART情報をCSV形式の文字列にする
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


