//---------------------------------------------------------------------
// kjm::physical_drive class
//---------------------------------------------------------------------


#include <sstream>
#include "kjm_util.h"
#include "kjm_physical_drive.h"


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


