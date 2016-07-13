/*---------------------------------------------------------------------------
//	S.M.A.R.T.�e�X�g�v���O����(�w�b�_�[)
//	Copyright(C) by Y.Furukawa All rights reserved
//-------------------------------------------------------------------------*/

#pragma once


#include <windows.h>
#include <WinIoCtl.h>
/*------�A���C�����g�̐ݒ��1�o�C�g�P�ʂɂ���------------------------------*/
#ifdef _MSC_VER
	#pragma pack(push, before_include1, 1)
#else
	/*�e�R���p�C���ŃA���C�����g�̐ݒ��1�o�C�g�P�ʂɂ��Ă�������*/
#endif
/*----------IdentifyDevice�Ŏ擾�ł���f�[�^���e��\�����\����-------------*/
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
/*----------S.M.A.R.T.�����l�̊�{�\����-----------------------------------*/
typedef struct SMARTAttributeData
{
	BYTE	attributeID;
	USHORT	statusFlag;
	BYTE	attributeValue;
	BYTE	worstValue;
	BYTE	vendorSpecific[7];
} SMARTATTRIBUTEDATA;
/*----------S.M.A.R.T.�����l�̏W���\����-----------------------------------*/
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
/*----------S.M.A.R.T.�������l�̊�{�\����---------------------------------*/
typedef struct SMARTThresholdData
{
	BYTE	attributeID;
	BYTE	attributeThreshold;
	BYTE	Reserved[10];
} SMARTTHRESHOLDDATA;
/*----------S.M.A.R.T.�������l�̏W���\����---------------------------------*/
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
/*----------IdentifyDevice�R�}���h�o�͂̍\����-----------------------------*/
typedef struct IdentifyCmdOutParams
{
	DWORD				bufferSize;
	DRIVERSTATUS		driverStatus;
	IDENTIFYDATA		identifyData;
} IDENTIFYCMDOUTPARAMS;
/*----------S.M.A.R.T.ReadStatus�R�}���h�o�͂̍\����-----------------------*/
typedef struct ReadStatusCmdOutParams
{
	DWORD			bufferSize;
	DRIVERSTATUS	driverStatus;
	IDEREGS			ideRegData;
} READSTATUSCMDOUTPARAMS;
/*----------S.M.A.R.T.ReadData�R�}���h�o�͂̍\����-------------------------*/
typedef struct AttributeCmdOutParams
{
	DWORD				bufferSize;
	DRIVERSTATUS		driverStatus;
	SMARTATTRIBUTESDATA	attributeData;
} ATTRIBUTECMDOUTPARAMS;
/*----------S.M.A.R.T.ReadAttributeThreshold�R�}���h�o�͂̍\����-----------*/
typedef struct ThresholdCmdOutParams
{
	DWORD				bufferSize;
	DRIVERSTATUS		driverStatus;
	SMARTHRESHOLDSDATA	thresholdData;
} THRESHOLDCMDOUTPARAMS;
/*------�A���C�����g�̐ݒ�����ɖ߂�---------------------------------------*/
#ifdef _MSC_VER
	#pragma pack(pop, before_include1)
#else
	/*�e�R���p�C���ŃA���C�����g�̐ݒ�����̒P�ʂɖ߂��Ă�������*/
#endif
typedef struct attrValueAndName
{
	BYTE	value;
	char*	name;
} ATTRVALUEANDNAME;
/*-------------------------------------------------------------------------*/

