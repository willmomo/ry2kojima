/*---------------------------------------------------------------------------
//	S.M.A.R.T.テストプログラム
//	Copyright(C) by Y.Furukawa All rights reserved
//-------------------------------------------------------------------------*/
#define _WIN32_WINNT 0x400

#include <stdio.h>

#include "Smart.h"

/*----------定義-----------------------------------------------------------*/
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

/*----------関数宣言-------------------------------------------------------*/
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
//		Main関数
//-------------------------------------------------------------------------*/
int main(void)
{
	int					retCode;
	HANDLE				diskHandle = NULL;
	SMARTATTRIBUTESDATA	attrData;
	SMARTHRESHOLDSDATA	thData;

	memset(&attrData, 0, sizeof(SMARTATTRIBUTESDATA));
	memset(&thData, 0, sizeof(SMARTHRESHOLDSDATA));
	/*------デバイスクラスのインターフェースハンドルの取得-----------------*/
	/*------ディスク0のハンドルの作成--------------------------------------*/
    diskHandle = CreateFile("\\\\.\\PhysicalDrive0",
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL, OPEN_EXISTING, 0, NULL);
	if(INVALID_HANDLE_VALUE == diskHandle) {
		printf("物理ドライブのハンドル作成に失敗しました\n");
		return -1;
    }

	/*------SMARTコマンドがサポートされているか確認する--------------------*/
	if (0 == isSupportedSMART(diskHandle)) {
		printf("SMARTがサポートされていません\n");
		CloseHandle(diskHandle);
		return -1;
	}

	/*------SMART機能を無効にする------------------------------------------*/
	printf("SMART機能の無効化を行います\n");
	if (0 != smartDisable(diskHandle)) {
		printf("SMART機能の無効化に失敗しました\n");
	}

	/*------SMART機能を有効にする------------------------------------------*/
	printf("SMART機能の有効化を行います\n");
	if (0 != smartEnable(diskHandle)) {
		printf("SMART機能の有効化に失敗しました\n");
		CloseHandle(diskHandle);
		return -1;
	}

	/*------SMART自動セーブ機能を無効にする--------------------------------*/
	printf("SMART自動セーブ機能の無効化を行います\n");
	if (0 != smartEnableDisableAutoSave(diskHandle, 0)) {
		printf("SMART自動セーブ機能の無効化に失敗しました\n");
	}

	/*------SMART自動セーブ機能を有効にする--------------------------------*/
	printf("SMART自動セーブ機能の有効化を行います\n");
	if (0 != smartEnableDisableAutoSave(diskHandle, 0xf1)) {
		printf("SMART自動セーブ機能の有効化に失敗しました\n");
	}

	/*------SMARTの状態取得を行う------------------------------------------*/
	printf("SMARTの状態取得を行います\n");
	retCode = smartReturnStatus(diskHandle);
	switch(retCode)
	{
		case 0:
			printf("正常状態です\n");
			break;
		case 1:
			printf("問題があります\n");
			break;
		default:
			printf("SMARTの状態取得に失敗しました\n");
	}

	/*------SMARTの属性値を取得する----------------------------------------*/
	printf("SMARTの属性値の取得を行います\n");
	if (0 != smartReadAttributeValues(diskHandle, &attrData)) {
		printf("SMARTの属性値の取得に失敗しました\n");
		CloseHandle(diskHandle);
		return -1;
	}

	/*------SMARTの閾値を取得する------------------------------------------*/
	printf("SMARTの閾値の取得を行います\n");
	if (0 != smartReadAttributeThresholds(diskHandle, &thData)) {
		printf("SMARTの閾値の取得に失敗しました\n");
	}

	/*------SMARTの属性値と閾値を表示する----------------------------------*/
	DisplayAttribute(&attrData, &thData);

	/*------終了処理-------------------------------------------------------*/
	CloseHandle(diskHandle);

	return 0;
}
/*---------------------------------------------------------------------------
//	S.M.A.R.T.をサポートしているか調査する
//		返り値:0以外：サポートしている
//			   0    ：サポートしていない
//-------------------------------------------------------------------------*/
int isSupportedSMART(HANDLE diskHandle)
{
	IDENTIFYDATA identifyData;
	if (0 != identifyDevice(diskHandle, &identifyData))
		return 0;
	/*------SMARTの実装されているか82Word目の0bitを調べる------------------*/
	return identifyData.commandSet1 & 1;
}
/*---------------------------------------------------------------------------
//	IdentifyDeviceコマンドを実行しデバイス情報を取得する
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int identifyDevice(HANDLE diskHandle, IDENTIFYDATA* identifyData)
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
//	S.M.A.R.T.機能を停止する
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int smartDisable(HANDLE diskHandle)
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
	status = DeviceIoControl(diskHandle,	SMART_SEND_DRIVE_COMMAND,
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
int smartEnable(HANDLE diskHandle)
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
	status = DeviceIoControl(diskHandle,	SMART_SEND_DRIVE_COMMAND,
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
int smartEnableDisableAutoSave(HANDLE diskHandle, char enable)
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
	status = DeviceIoControl(diskHandle,	SMART_SEND_DRIVE_COMMAND,
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
int smartReturnStatus(HANDLE diskHandle)
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
	status = DeviceIoControl(diskHandle,	SMART_SEND_DRIVE_COMMAND,
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
int smartReadAttributeValues(HANDLE				  diskHandle,
							 SMARTATTRIBUTESDATA* attrData)
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
//	S.M.A.R.T.のしきい値を取得する
//		返り値:0    ：成功
//			   0以外：失敗
//-------------------------------------------------------------------------*/
int smartReadAttributeThresholds(HANDLE				 diskHandle,
								 SMARTHRESHOLDSDATA* thData)
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
//	S.M.A.R.T.の属性値としきい値を表示する
//-------------------------------------------------------------------------*/
void DisplayAttribute(SMARTATTRIBUTESDATA*	attrData,
					  SMARTHRESHOLDSDATA*	thData)
{
	int		index;
	BYTE	attrID;

	/*------各データ構造体のリビジョンを表示する---------------------------*/
	printf("\n属性データのリビジョン          閾値データのリビジョン\n" \
		   "         %04Xh                           %04Xh\n\n", 
				attrData->revision, 				thData->revision);

	/*------属性名と属性値と閾値を表示する---------------------------------*/
	printf("-ID- -           属     性     名           - -現在値- -最低値- -閾値-\n");

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
