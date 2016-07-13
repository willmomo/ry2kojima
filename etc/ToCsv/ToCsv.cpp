#include "../kjmlib/kjmlib.h"
#if defined(_DEBUG)
#pragma comment(lib, "../kjmlib/library/kjmlibd.lib")
#else
#pragma comment(lib, "../kjmlib/library/kjmlib.lib")
#endif

//---------------------------------------------------------------------
// �c�Ɨ���p�̍\����
//---------------------------------------------------------------------
typedef struct _EIGYO_RIREKI_A{
	unsigned char	code1;
	unsigned char	code2;
	unsigned short	hhmms0;
	unsigned short	data1;
	unsigned short	data2;
	unsigned short	data3;
	unsigned short	data4;
	long			ldata5;
} eigyou_rireki_A;

//---------------------------------------------------------------------
// ��F�؂ŕ\���ΏۂɂȂ锭�����ǂ������ׂ�
//
// [�߂�l]
//  = 1 �\���Ώۂ̔����B
//  = 0 �\�����Ȃ������B
//---------------------------------------------------------------------
int IsFxSupport(int code1, int code2) {
	int c1 = code1 & 0x7f;	// �����r�b�g�𗎂Ƃ�
	int c1c2 = ((c1 > 0x07 ? c1 : 0) << 8) | code2;

	int ret = 0;	// [�\�����Ȃ�]�ŏ�����

	switch (c1c2) {
	case 0x003b:	// �s��
	case 0x003c:	// �s��2
	case 0x0040:	// �s��1�f��
	case 0x0041:	// �s��2�f��
	case 0x0042:	// �h�A�f��
	case 0x0043:	// �h�A2�f��
	case 0x0044:	// �Z�[�t�ُ�
	case 0x0045:	// ���܉񐔈ُ�
	case 0x0046:	// ���܏o�ʈُ�
	case 0x0047:	// �h�A����
	case 0x0048:	// �h�A�ُ�
	case 0x0049:	// �X�^�[�g�ُ�
	case 0x004a:	// �h�A�I�[�v��
	case 0x004b:	// �h�A2�I�[�v��
	case 0x004c:	// �A�E�g�_��
	case 0x004d:	// �Z�[�t�_��
	case 0x004e:	// ���܏o�ʓ_��
	case 0x004f:	// �⋋�_��
	case 0x0050:	// �X�^�[�g�_��
	case 0x0051:	// �h�A�_��
	case 0x0052:	// �����ғ��_��
	case 0x0053:	// ����1�f��
	case 0x0054:	// ����2�f��
	case 0x0055:	// �J�[�h�f��
	case 0x0056:	// ������ُ�
	case 0x0057:	// ����ُ�
	case 0x0058:	// ���ܔ���ُ�
	case 0x007b:	// �ăv���C�f��
	case 0x007c:	// �����@�f��
	case 0x007d:	// �T���h�g���u��
	case 0x007e:	// �h�A3�I�[�v��
	case 0x007f:	// �s��3
	case 0x0bf1:	// �@��_��	
	case 0x0bf6:	// �䃉���v�ʐM�ُ�
	case 0x0bf7:	// �T���h���o�G���[
	case 0x0cf2:	// TN�ʐM�ُ�
	case 0x0cf3:	// TNIO�ʐM�ُ�
	case 0x0cf4:	// ���v���C�ʐM�ُ�
	case 0x0cf5:	// �R�C���Z���N�^�_��
	case 0x1101:	// �⋋�ُ�(�T���h)
	case 0x1201:	// �⋋�@�_��
		ret = 1;	// [�\������]�ɕύX
		break;
	}

	return ret;
}

//---------------------------------------------------------------------
// �c�Ɨ�����CSV������
//---------------------------------------------------------------------
int ToCsv_Eireki(LPCTSTR pszFname) {
	FILE* fp = _tfopen(pszFname, _T("rb"));
	if (fp == NULL) {
		_ftprintf(stderr, _T("�t�@�C�� '%s' ���A�I�[�v���ł��܂���Berror %d\n"), pszFname, errno);
		return -1;
	}

	eigyou_rireki_A buf;

	// 1���R�[�h�ڂ́A�����Ȃ̂ŃX�L�b�v
	if (fread(&buf, sizeof(buf), 1, fp) != 1) {
		return -1;
	}
	// �w�b�_����ʂɏo��
	_tprintf(_T("rno,code1,code2,hhmms0,data1,data2,data3,data4,ldata5,��F��\n"));

	int rno = 1;
	while (1) {
		if (fread(&buf, sizeof(buf), 1, fp) != 1) {
			break;
		}

		// CSV�ϊ����ĉ�ʂɏo��
		_tprintf(_T("%d,%u,%u,%u,%u,%u,%u,%u,%d,%d\n"), 
			rno++, buf.code1, buf.code2, buf.hhmms0, buf.data1, buf.data2, buf.data3, buf.data4, buf.ldata5, IsFxSupport(buf.code1, buf.code2));
	}

	fclose(fp);

	return 0;
}

//---------------------------------------------------------------------
// �v���O�����X�^�[�g
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	for (int i = 1; i < argc; i++) {
		ToCsv_Eireki(argv[i]);
	}

	return 0;
}
