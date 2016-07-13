/*---------------------------------------------------------------------------
//	S.M.A.R.T.テストプログラム
//	Copyright(C) by Y.Furukawa All rights reserved
//-------------------------------------------------------------------------*/
//#define _WIN32_WINNT 0x400

#include <stdio.h>

#include "../ReadEventLog/kjm.h"


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
			while (0 != kjm::attrValueAndNames[arrayIndex].value) {
				if (attrID == kjm::attrValueAndNames[arrayIndex].value)
					break;
				++arrayIndex;
			}
			if (0 != kjm::attrValueAndNames[arrayIndex].value)
				attrName = kjm::attrValueAndNames[arrayIndex].name;
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

	kjm::physical_drive disk;
	if ((disk.OpenDrive( "\\\\.\\PhysicalDrive0" )) == FALSE) {
		printf("物理ドライブのハンドル作成に失敗しました\n");
		return -1;
    }

	/*------SMARTコマンドがサポートされているか確認する--------------------*/
	if (0 == disk.isSupportedSMART()) {
		printf("SMARTがサポートされていません\n");
		return -1;
	}

	/*------SMART機能を無効にする------------------------------------------*/
	printf("SMART機能の無効化を行います\n");
	if (0 != disk.smartDisable()) {
		printf("SMART機能の無効化に失敗しました\n");
	}

	/*------SMART機能を有効にする------------------------------------------*/
	printf("SMART機能の有効化を行います\n");
	if (0 != disk.smartEnable()) {
		printf("SMART機能の有効化に失敗しました\n");
		return -1;
	}

	/*------SMART自動セーブ機能を無効にする--------------------------------*/
	printf("SMART自動セーブ機能の無効化を行います\n");
	if (0 != disk.smartEnableDisableAutoSave(0)) {
		printf("SMART自動セーブ機能の無効化に失敗しました\n");
	}

	/*------SMART自動セーブ機能を有効にする--------------------------------*/
	printf("SMART自動セーブ機能の有効化を行います\n");
	if (0 != disk.smartEnableDisableAutoSave(0xf1)) {
		printf("SMART自動セーブ機能の有効化に失敗しました\n");
	}

	/*------SMARTの状態取得を行う------------------------------------------*/
	printf("SMARTの状態取得を行います\n");
	retCode = disk.smartReturnStatus();
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
	if (0 != disk.smartReadAttributeValues(&attrData)) {
		printf("SMARTの属性値の取得に失敗しました\n");
		return -1;
	}

	/*------SMARTの閾値を取得する------------------------------------------*/
	printf("SMARTの閾値の取得を行います\n");
	if (0 != disk.smartReadAttributeThresholds(&thData)) {
		printf("SMARTの閾値の取得に失敗しました\n");
	}

	/*------SMARTの属性値と閾値を表示する----------------------------------*/
	DisplayAttribute(&attrData, &thData);

	// 現在の温度を表示
	printf( "現在温度: %d\n", disk.getTemperature() );
	printf( "Drive Power Cycle Count: %d\n", disk.getDrivePowerCycleCount() );
	printf( "Power-On Hours: %d\n", disk.getPowerOnHours());
	printf( "Start/Stop Count: %d\n", disk.getStartStopCount());

	return 0;
}
