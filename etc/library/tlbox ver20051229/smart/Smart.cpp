/*---------------------------------------------------------------------------
//	S.M.A.R.T.�e�X�g�v���O����
//	Copyright(C) by Y.Furukawa All rights reserved
//-------------------------------------------------------------------------*/
//#define _WIN32_WINNT 0x400

#include <stdio.h>

#include "../ReadEventLog/kjm.h"


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

	kjm::physical_drive disk;
	if ((disk.OpenDrive( "\\\\.\\PhysicalDrive0" )) == FALSE) {
		printf("�����h���C�u�̃n���h���쐬�Ɏ��s���܂���\n");
		return -1;
    }

	/*------SMART�R�}���h���T�|�[�g����Ă��邩�m�F����--------------------*/
	if (0 == disk.isSupportedSMART()) {
		printf("SMART���T�|�[�g����Ă��܂���\n");
		return -1;
	}

	/*------SMART�@�\�𖳌��ɂ���------------------------------------------*/
	printf("SMART�@�\�̖��������s���܂�\n");
	if (0 != disk.smartDisable()) {
		printf("SMART�@�\�̖������Ɏ��s���܂���\n");
	}

	/*------SMART�@�\��L���ɂ���------------------------------------------*/
	printf("SMART�@�\�̗L�������s���܂�\n");
	if (0 != disk.smartEnable()) {
		printf("SMART�@�\�̗L�����Ɏ��s���܂���\n");
		return -1;
	}

	/*------SMART�����Z�[�u�@�\�𖳌��ɂ���--------------------------------*/
	printf("SMART�����Z�[�u�@�\�̖��������s���܂�\n");
	if (0 != disk.smartEnableDisableAutoSave(0)) {
		printf("SMART�����Z�[�u�@�\�̖������Ɏ��s���܂���\n");
	}

	/*------SMART�����Z�[�u�@�\��L���ɂ���--------------------------------*/
	printf("SMART�����Z�[�u�@�\�̗L�������s���܂�\n");
	if (0 != disk.smartEnableDisableAutoSave(0xf1)) {
		printf("SMART�����Z�[�u�@�\�̗L�����Ɏ��s���܂���\n");
	}

	/*------SMART�̏�Ԏ擾���s��------------------------------------------*/
	printf("SMART�̏�Ԏ擾���s���܂�\n");
	retCode = disk.smartReturnStatus();
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
	if (0 != disk.smartReadAttributeValues(&attrData)) {
		printf("SMART�̑����l�̎擾�Ɏ��s���܂���\n");
		return -1;
	}

	/*------SMART��臒l���擾����------------------------------------------*/
	printf("SMART��臒l�̎擾���s���܂�\n");
	if (0 != disk.smartReadAttributeThresholds(&thData)) {
		printf("SMART��臒l�̎擾�Ɏ��s���܂���\n");
	}

	/*------SMART�̑����l��臒l��\������----------------------------------*/
	DisplayAttribute(&attrData, &thData);

	// ���݂̉��x��\��
	printf( "���݉��x: %d\n", disk.getTemperature() );
	printf( "Drive Power Cycle Count: %d\n", disk.getDrivePowerCycleCount() );
	printf( "Power-On Hours: %d\n", disk.getPowerOnHours());
	printf( "Start/Stop Count: %d\n", disk.getStartStopCount());

	return 0;
}
