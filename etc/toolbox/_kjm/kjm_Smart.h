/*---------------------------------------------------------------------------
//	S.M.A.R.T.テストプログラム(ヘッダー)
//	Copyright(C) by Y.Furukawa All rights reserved
//-------------------------------------------------------------------------*/

#pragma once


#include <windows.h>
#include <WinIoCtl.h>
/*------アライメントの設定を1バイト単位にする------------------------------*/
#ifdef _MSC_VER
	#pragma pack(push, before_include1, 1)
#else
	/*各コンパイラでアライメントの設定を1バイト単位にしてください*/
#endif
/*----------IdentifyDeviceで取得できるデータ内容を表した構造体-------------*/
typedef struct IdentifyData
{
	USHORT	genConfig;
	USHORT	numCyls;
	USHORT	specificConfig;
	USHORT	numHeads;
	USHORT	bytesPerTrack;
	USHORT	bytesPerSector;
	USHORT	sectorsPerTrack;
	USHORT	reservedCompactFlash0[2];
	USHORT	retired;
	CHAR	serialNumber[20];
	USHORT	bufferType;
	USHORT	bufferSize;
	USHORT	ECCSize;
	CHAR	firmwareRev[8];
	CHAR	modelNumber[40];
	USHORT	moreVendorUnique;
	USHORT	doubleWordIO;
	USHORT	capabilities[2];
	USHORT	PIOTiming;
	USHORT	DMATiming;
	USHORT	BS;
	USHORT	numCurrentCyls;
	USHORT	numCurrentHeads;
	USHORT	numCurrentSectorsPerTrack;
	ULONG	currentSectorCapacity;
	USHORT	multiSectorStuff;
	ULONG	totalAddressableSectors;
	USHORT	singleWordDMA;
	USHORT	multiWordDMA;
	USHORT	advancedPIO;
	USHORT	minMultiDMACycleTime;
	USHORT	manufacturerMultiDMACycleTime;
	USHORT	minPIOCycleTimeWithoutFlowCtrl;
	USHORT	minPIOCycleTimeWithIORDYFlowCtrl;
	USHORT	reserved0[2];
	USHORT	reservedIdentifyPacketDevice[4];
	USHORT	queueDepth;
	USHORT	reservedSerialATA[4];
	USHORT	majorVersion;
	USHORT	minorVersion;
	USHORT	commandSet1;
	USHORT	commandSet2;
	USHORT	commandSetExtension;
	USHORT	commandSetEnabled1;
	USHORT	commandSetEnabled2;
	USHORT	commandSetDefault;
	USHORT	ultraDMA;
	USHORT	timeRequiredSecurityErase;
	USHORT	timeRequiredEnhancedSecurityErase;
	USHORT	currentAdvancedPower;
	USHORT	masterPasswordRevision;
	USHORT	hardwareReset;
	USHORT	acousticManagement;
	USHORT	streamMinRequestSize;
	USHORT	streamTransferTimeDMA;
	USHORT	streamAccessLatency;
	USHORT	streamingPerformanceGranularity[2];
	ULONG64	maximum48LBAAddress;
	USHORT	streamTransferTimePIO;
	USHORT	reserved1;
	USHORT	sectorSize;
	USHORT	interSeekDelay;
	USHORT	worldWideName[4];
	USHORT	reserved128bitWorldWideName[4];
	USHORT	reservedTechnicalReport;
	USHORT	logicalSectorSize[2];
	USHORT	reserved2[8];
	USHORT	removableMediaStatusNotify;//127
	USHORT	securityStatus;
	USHORT	vendorSpecific[31];
	USHORT	CFAPowerMode1;
	USHORT	reservedCompactFlash1[15];		
	BYTE	currentMediaSerialNumber[60];
	USHORT	SCTCommandSet;
	USHORT  reserved3[48];
	USHORT	integrity;
} IDENTIFYDATA;
/*----------S.M.A.R.T.属性値の基本構造体-----------------------------------*/
typedef struct SMARTAttributeData
{
	BYTE	attributeID;
	USHORT	statusFlag;
	BYTE	attributeValue;
	BYTE	worstValue;
	BYTE	vendorSpecific[7];
} SMARTATTRIBUTEDATA;
/*----------S.M.A.R.T.属性値の集合構造体-----------------------------------*/
typedef struct SMARTAttributesData
{
	USHORT	revision;
	struct	SMARTAttributeData AttributeData[30];
	BYTE	offLineStatus;
	BYTE	executionStatus;
	USHORT	completeTotalTime;
	BYTE	vendorSpecific;
	BYTE	offLineCapability;
	USHORT	SMARTCapability;
	BYTE	SMARTLoggingCapability;
	BYTE	failureCheckPoint;
	BYTE	shortSelfTestCompletionTime;
	BYTE	extendedSelfTestCompletionTime;
	BYTE	reserved[12];
	BYTE	vendorUnique[125];
	BYTE	checksum;
} SMARTATTRIBUTESDATA;
/*----------S.M.A.R.T.しきい値の基本構造体---------------------------------*/
typedef struct SMARTThresholdData
{
	BYTE	attributeID;
	BYTE	attributeThreshold;
	BYTE	Reserved[10];
} SMARTTHRESHOLDDATA;
/*----------S.M.A.R.T.しきい値の集合構造体---------------------------------*/
typedef struct SMARThresholdsData
{
	USHORT	revision;
	struct	SMARTThresholdData ThresholdData[30];
	BYTE	offLineStatus;
	BYTE	executionStatus;
	USHORT	completeTotalTime;
	BYTE	vendorSpecific;
	BYTE	offLineCapability;
	USHORT	SMARTCapability;
	BYTE	SMARTLoggingCapability;
	BYTE	failureCheckPoint;
	BYTE	shortSelfTestCompletionTime;
	BYTE	extendedSelfTestCompletionTime;
	BYTE	reserved[12];
	BYTE	vendorUnique[125];
	BYTE	checksum;
} SMARTHRESHOLDSDATA;
/*----------IdentifyDeviceコマンド出力の構造体-----------------------------*/
typedef struct IdentifyCmdOutParams
{
	DWORD				bufferSize;
	DRIVERSTATUS		driverStatus;
	IDENTIFYDATA		identifyData;
} IDENTIFYCMDOUTPARAMS;
/*----------S.M.A.R.T.ReadStatusコマンド出力の構造体-----------------------*/
typedef struct ReadStatusCmdOutParams
{
	DWORD			bufferSize;
	DRIVERSTATUS	driverStatus;
	IDEREGS			ideRegData;
} READSTATUSCMDOUTPARAMS;
/*----------S.M.A.R.T.ReadDataコマンド出力の構造体-------------------------*/
typedef struct AttributeCmdOutParams
{
	DWORD				bufferSize;
	DRIVERSTATUS		driverStatus;
	SMARTATTRIBUTESDATA	attributeData;
} ATTRIBUTECMDOUTPARAMS;
/*----------S.M.A.R.T.ReadAttributeThresholdコマンド出力の構造体-----------*/
typedef struct ThresholdCmdOutParams
{
	DWORD				bufferSize;
	DRIVERSTATUS		driverStatus;
	SMARTHRESHOLDSDATA	thresholdData;
} THRESHOLDCMDOUTPARAMS;
/*------アライメントの設定を元に戻す---------------------------------------*/
#ifdef _MSC_VER
	#pragma pack(pop, before_include1)
#else
	/*各コンパイラでアライメントの設定を元の単位に戻してください*/
#endif
typedef struct attrValueAndName
{
	BYTE	value;
	char*	name;
} ATTRVALUEANDNAME;
/*-------------------------------------------------------------------------*/

