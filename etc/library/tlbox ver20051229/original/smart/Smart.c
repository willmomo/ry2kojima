/*---------------------------------------------------------------------------
//	S.M.A.R.T.�e�X�g�v���O����
//	Copyright(C) by Y.Furukawa All rights reserved
//-------------------------------------------------------------------------*/
#define _WIN32_WINNT 0x400

#include <stdio.h>

#include "Smart.h"

/*----------��`-----------------------------------------------------------*/
ATTRVALUEANDNAME attrValueAndNames[] = {
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
#define	NUM_ATTRIBUTE_STRUCTS	30

/*----------�֐��錾-------------------------------------------------------*/
int isSupportedSMART(HANDLE diskHandle);
int identifyDevice(HANDLE diskHandle, IDENTIFYDATA* identifyData);
int smartDisable(HANDLE diskHandle);
int smartEnable(HANDLE diskHandle);
int smartEnableDisableAutoSave(HANDLE diskHandle, char enable);
int smartReturnStatus(HANDLE diskHandle);
int smartReadAttributeValues(HANDLE				  diskHandle,
							 SMARTATTRIBUTESDATA* attrData);
int smartReadAttributeThresholds(HANDLE				 diskHandle,
								 SMARTHRESHOLDSDATA* thData);
void DisplayAttribute(SMARTATTRIBUTESDATA*	attrData,
					  SMARTHRESHOLDSDATA*	thData);

/*---------------------------------------------------------------------------
//		Main�֐�
//-------------------------------------------------------------------------*/
int main(void)
{
	int					retCode;
	HANDLE				diskHandle = NULL;
	SMARTATTRIBUTESDATA	attrData;
	SMARTHRESHOLDSDATA	thData;

	memset(&attrData, 0, sizeof(SMARTATTRIBUTESDATA));
	memset(&thData, 0, sizeof(SMARTHRESHOLDSDATA));
	/*------�f�o�C�X�N���X�̃C���^�[�t�F�[�X�n���h���̎擾-----------------*/
	/*------�f�B�X�N0�̃n���h���̍쐬--------------------------------------*/
    diskHandle = CreateFile("\\\\.\\PhysicalDrive0",
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE == diskHandle) {
		printf("�����h���C�u�̃n���h���쐬�Ɏ��s���܂���\n");
		return -1;
    }

	/*------SMART�R�}���h���T�|�[�g����Ă��邩�m�F����--------------------*/
	if (0 == isSupportedSMART(diskHandle)) {
		printf("SMART���T�|�[�g����Ă��܂���\n");
		CloseHandle(diskHandle);
		return -1;
	}

	/*------SMART�@�\�𖳌��ɂ���------------------------------------------*/
	printf("SMART�@�\�̖��������s���܂�\n");
	if (0 != smartDisable(diskHandle)) {
		printf("SMART�@�\�̖������Ɏ��s���܂���\n");
	}

	/*------SMART�@�\��L���ɂ���------------------------------------------*/
	printf("SMART�@�\�̗L�������s���܂�\n");
	if (0 != smartEnable(diskHandle)) {
		printf("SMART�@�\�̗L�����Ɏ��s���܂���\n");
		CloseHandle(diskHandle);
		return -1;
	}

	/*------SMART�����Z�[�u�@�\�𖳌��ɂ���--------------------------------*/
	printf("SMART�����Z�[�u�@�\�̖��������s���܂�\n");
	if (0 != smartEnableDisableAutoSave(diskHandle, 0)) {
		printf("SMART�����Z�[�u�@�\�̖������Ɏ��s���܂���\n");
	}

	/*------SMART�����Z�[�u�@�\��L���ɂ���--------------------------------*/
	printf("SMART�����Z�[�u�@�\�̗L�������s���܂�\n");
	if (0 != smartEnableDisableAutoSave(diskHandle, 0xf1)) {
		printf("SMART�����Z�[�u�@�\�̗L�����Ɏ��s���܂���\n");
	}

	/*------SMART�̏�Ԏ擾���s��------------------------------------------*/
	printf("SMART�̏�Ԏ擾���s���܂�\n");
	retCode = smartReturnStatus(diskHandle);
	switch(retCode)
	{
		case 0:
			printf("�����Ԃł�\n");
			break;
		case 1:
			printf("��肪����܂�\n");
			break;
		default:
			printf("SMART�̏�Ԏ擾�Ɏ��s���܂���\n");
	}

	/*------SMART�̑����l���擾����----------------------------------------*/
	printf("SMART�̑����l�̎擾���s���܂�\n");
	if (0 != smartReadAttributeValues(diskHandle, &attrData)) {
		printf("SMART�̑����l�̎擾�Ɏ��s���܂���\n");
		CloseHandle(diskHandle);
		return -1;
	}

	/*------SMART��臒l���擾����------------------------------------------*/
	printf("SMART��臒l�̎擾���s���܂�\n");
	if (0 != smartReadAttributeThresholds(diskHandle, &thData)) {
		printf("SMART��臒l�̎擾�Ɏ��s���܂���\n");
	}

	/*------SMART�̑����l��臒l��\������----------------------------------*/
	DisplayAttribute(&attrData, &thData);

	/*------�I������-------------------------------------------------------*/
	CloseHandle(diskHandle);

	return 0;
}
/*---------------------------------------------------------------------------
//	S.M.A.R.T.���T�|�[�g���Ă��邩��������
//		�Ԃ�l:0�ȊO�F�T�|�[�g���Ă���
//			   0    �F�T�|�[�g���Ă��Ȃ�
//-------------------------------------------------------------------------*/
int isSupportedSMART(HANDLE diskHandle)
{
	IDENTIFYDATA identifyData;
	if (0 != identifyDevice(diskHandle, &identifyData))
		return 0;
	/*------SMART�̎�������Ă��邩82Word�ڂ�0bit�𒲂ׂ�------------------*/
	return identifyData.commandSet1 & 1;
}
/*---------------------------------------------------------------------------
//	IdentifyDevice�R�}���h�����s���f�o�C�X�����擾����
//		�Ԃ�l:0    �F����
//			   0�ȊO�F���s
//-------------------------------------------------------------------------*/
int identifyDevice(HANDLE diskHandle, IDENTIFYDATA* identifyData)
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
	status = DeviceIoControl(diskHandle,	SMART_RCV_DRIVE_DATA,
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
int smartDisable(HANDLE diskHandle)
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
	status = DeviceIoControl(diskHandle,	SMART_SEND_DRIVE_COMMAND,
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
int smartEnable(HANDLE diskHandle)
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
	status = DeviceIoControl(diskHandle,	SMART_SEND_DRIVE_COMMAND,
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
int smartEnableDisableAutoSave(HANDLE diskHandle, char enable)
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
	status = DeviceIoControl(diskHandle,	SMART_SEND_DRIVE_COMMAND,
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
int smartReturnStatus(HANDLE diskHandle)
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
	status = DeviceIoControl(diskHandle,	SMART_SEND_DRIVE_COMMAND,
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
int smartReadAttributeValues(HANDLE				  diskHandle,
							 SMARTATTRIBUTESDATA* attrData)
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
	status = DeviceIoControl(diskHandle,	SMART_RCV_DRIVE_DATA,
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
int smartReadAttributeThresholds(HANDLE				 diskHandle,
								 SMARTHRESHOLDSDATA* thData)
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
	status = DeviceIoControl(diskHandle,	SMART_RCV_DRIVE_DATA,
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
/*---------------------------------------------------------------------------
//	S.M.A.R.T.�̑����l�Ƃ������l��\������
//-------------------------------------------------------------------------*/
void DisplayAttribute(SMARTATTRIBUTESDATA*	attrData,
					  SMARTHRESHOLDSDATA*	thData)
{
	int		index;
	BYTE	attrID;

	/*------�e�f�[�^�\���̂̃��r�W������\������---------------------------*/
	printf("\n�����f�[�^�̃��r�W����          臒l�f�[�^�̃��r�W����\n" \
		   "         %04Xh                           %04Xh\n\n", 
				attrData->revision, 				thData->revision);

	/*------�������Ƒ����l��臒l��\������---------------------------------*/
	printf("-ID- -           ��     ��     ��           - -���ݒl- -�Œ�l- -臒l-\n");

	for (index = 0; index < NUM_ATTRIBUTE_STRUCTS; ++index) {
		attrID = attrData->AttributeData[index].attributeID;
		if (0 != attrID) {
			char*	attrName;
			int		arrayIndex = 0;
			while (0 != attrValueAndNames[arrayIndex].value) {
				if (attrID == attrValueAndNames[arrayIndex].value)
					break;
				++arrayIndex;
			}
			if (0 != attrValueAndNames[arrayIndex].value)
				attrName = attrValueAndNames[arrayIndex].name;
			else
				attrName = "Unknown Attribute";

			printf("%3d %-40s    %3d      %3d     %3d\n", 
					attrData->AttributeData[index].attributeID,
					attrName, 
					attrData->AttributeData[index].attributeValue, 
					attrData->AttributeData[index].worstValue, 
					thData->ThresholdData[index].attributeThreshold);
		}
	}
}
/*-------------------------------------------------------------------------*/
