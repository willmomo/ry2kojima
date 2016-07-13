#include <windows.h>
#include <stdio.h>
#include <lm.h>

#define FMTSTR	"%14s: %s\n"
#define FMTNUM	"%14s: %d\n"

//---------------------------------------------------------------------
// [�T�v]
// ASCII�������Unicode������ɕϊ�
//
// [����]
// pszSrc	�ϊ���ASCII������ւ̃|�C���^
//
// [�߂�l]
// �ϊ����Unicode������ւ̃|�C���^�B�ϊ��ł��Ȃ��ꍇ�́ANULL��Ԃ��B
//
// [���l]
// �g�p��́Afree(�߂�l)����K�v����B
//---------------------------------------------------------------------

LPWSTR StrToWStr(LPCSTR pszSrc)
{
	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(\0�܂�))���擾
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nSizeNeeded == 0) {
		return NULL;
	}

	// �o�b�t�@���m��
	LPWSTR pwsz = (LPWSTR)malloc(sizeof(WCHAR) * nSizeNeeded);
	if (pwsz == NULL) {
		return NULL;
	}

	// �ϊ�
	int ret = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return NULL;
	}

	return pwsz;
}

//---------------------------------------------------------------------
// [�T�v]
// Unicode�������ASCII������ɕϊ�
//
// [����]
// pwszSrc	�ϊ���Unicode������ւ̃|�C���^
//
// [�߂�l]
// �ϊ����ASCII������ւ̃|�C���^�B�ϊ��ł��Ȃ��ꍇ�́ANULL��Ԃ��B
//
// [���l]
// �g�p��́Afree(�߂�l)����K�v����B
//---------------------------------------------------------------------

LPSTR WStrToStr(LPCWSTR pwszSrc)
{
	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(\0�܂�))���擾
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0) {
		return NULL;
	}

	// �o�b�t�@���m��
	LPSTR psz = (LPSTR)malloc(sizeof(CHAR) * nSizeNeeded);
	if (psz == NULL) {
		return NULL;
	}

	// �ϊ�
	int ret = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return NULL;
	}

	return psz;
}

//---------------------------------------------------------------------
// [�T�v]
// shi502_type�ɑΉ������������Ԃ�
//---------------------------------------------------------------------

static const char* shi502_type_to_string(DWORD shi502_type)
{
	static char share_work[512];

	switch (shi502_type & ~STYPE_SPECIAL) {
	case STYPE_DISKTREE:
		if (shi502_type & STYPE_SPECIAL) {
			strcpy(share_work, "�f�B�X�N �h���C�u (�Ǘ����L)");
		} else {
			strcpy(share_work, "�f�B�X�N �h���C�u");
		}
		break;
	case STYPE_PRINTQ:
		if (shi502_type & STYPE_SPECIAL) {
			strcpy(share_work, "�v�����g�L���[ (�Ǘ����L)");
		} else {
			strcpy(share_work, "�v�����g�L���[");
		}
		break;
	case STYPE_DEVICE:
		if (shi502_type & STYPE_SPECIAL) {
			strcpy(share_work, "�ʐM�f�o�C�X (�Ǘ����L)");
		} else {
			strcpy(share_work, "�ʐM�f�o�C�X");
		}
		break;
	case STYPE_IPC:
		if (shi502_type & STYPE_SPECIAL) {
			strcpy(share_work, "IPC (�Ǘ����L)");
		} else {
			strcpy(share_work, "IPC");
		}
		break;
	default:
		sprintf(share_work, "(�s��: 0x%08X)", shi502_type);
		break;
	}

	return share_work;
}

//---------------------------------------------------------------------
// [�T�v]
// �v���O�����E�X�^�[�g
//---------------------------------------------------------------------

int main(int argc, char** argv)
{
	// �����`�F�b�N
	if (argc != 3) {
		printf("error: NetShareGetInfo <�T�[�o�[��> <���L��>\n");
		return -1;
	}

	LPWSTR pwszServer = StrToWStr(argv[1]);	// �T�[�o�[����Unicode�ɕϊ�
	if (!pwszServer) {
		printf("error: �T�[�o�[����Unicode�ɕϊ��ł��܂���\n");
		return -1;
	}

	LPWSTR pwszShared = StrToWStr(argv[2]);	// ���L����Unicode�ɕϊ�
	if (!pwszShared) {
		printf("error: ���L����Unicode�ɕϊ��ł��܂���B\n");
		free(pwszServer);
		return -1;
	}

	SHARE_INFO_502* pInfo;
	NET_API_STATUS ret;

	ret = NetShareGetInfo((LPTSTR)pwszServer, (LPTSTR)pwszShared, 502, (LPBYTE*)&pInfo);
	if (ret == NERR_Success) {
		LPSTR p;
		
		p = WStrToStr((LPCWSTR)pInfo->shi502_netname);
		printf(FMTSTR, "���L�t�H���_", p);
		free(p);

		printf(FMTSTR, "�^�C�v", shi502_type_to_string(pInfo->shi502_type));

		p = WStrToStr((LPCWSTR)pInfo->shi502_remark);
		printf(FMTSTR, "�R�����g", p);
		free(p);

		if (pInfo->shi502_max_uses == -1) {
			printf(FMTSTR, "���[�U�[������", "������");
		} else {
			printf(FMTNUM, "���[�U�[������", pInfo->shi502_max_uses);
		}

		printf(FMTNUM, "�ڑ���", pInfo->shi502_current_uses);

		p = WStrToStr((LPCWSTR)pInfo->shi502_path);
		printf(FMTSTR, "���L�p�X", p);
		free(p);

		NetApiBufferFree(pInfo);
	} else {
		printf("error %d: NetShareGetInfo�ŃG���[�ł��B\n", ret);
	}

	// ��n��
	free(pwszServer);
	free(pwszShared);

	return 0;
}
