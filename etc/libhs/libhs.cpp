//---------------------------------------------------------------------
// �����`�F�b�N�A���O���֐��Q

#define HS_IMPREMENT

#include "libhs.h"

//---------------------------------------------------------------------
// [�T�v]
// ���s�t�@�C������߂��֐�(�G���[�͖������̂ƐM����)
//---------------------------------------------------------------------

static const char *hs_GetModuleFileName()
{
	static char szFilename[MAX_PATH];

	if (GetModuleFileName(NULL, szFilename, sizeof(szFilename)) == 0) {
		_snprintf(szFilename, sizeof(szFilename), "{GetModuleFileName error %d}", GetLastError());
	}
	return szFilename;
}

//---------------------------------------------------------------------
// [�T�v]
// ����fopen
//---------------------------------------------------------------------

FILE *hs_fopen(const char *filename, const char *mode)
{
	// ���O�����̃`�F�b�N�ƃ��O
	{
		// assert(filename != NULL)
		if (filename == NULL) {
			HS_LOG0("Invalid argument: filename == NULL");
			errno = EINVAL;
			return NULL;
		}

		// assert(mode != NULL)
		if (mode == NULL) {
			HS_LOG0("Invalid argument: mode == NULL");
			errno = EINVAL;
			return NULL;
		}
	}

	FILE *fp;

	fp = fopen(filename, mode);

	// ���ʂ̃`�F�b�N�ƃ��O
	{
		// �G���[�����O�ɋL�^
		if (fp == NULL) {
			HS_ERRLOG2("fopen(\"%s\", \"%s\")", filename, mode);
		}
	}

	return fp;
}

//---------------------------------------------------------------------
// [�T�v]
// ����fputs
//---------------------------------------------------------------------

int hs_fputs(const char *string, FILE *stream)
{
	// ���O�����̃`�F�b�N�ƃ��O
	{
		// assert(string != NULL)
		if (string == NULL) {
			HS_LOG0("Invalid argument: string == NULL");
			errno = EINVAL;
			return EOF;
		}

		// assert(stream != NULL)
		if (stream == NULL) {
			HS_LOG0("Invalid argument: stream == NULL");
			errno = EINVAL;
			return EOF;
		}
	}

	int ret;
	
	ret = fputs(string, stream);

	// ���ʂ̃`�F�b�N�ƃ��O
	{
		// �G���[�����O�ɋL�^(�������A�G���[�ڍׂ͔���Ȃ�)
		if (ret == EOF) {
			HS_LOG2("fputs(\"%s\", %p) error", string, stream);
		}
	}

	return ret;
}

//---------------------------------------------------------------------
// [�T�v]
// ����fclose
//---------------------------------------------------------------------

int hs_fclose(FILE *stream)
{
	// ���O�����̃`�F�b�N�ƃ��O
	{
		// assert(stream != NULL)
		if (stream == NULL) {
			HS_LOG0("Invalid argument: stream == NULL");
			errno = EINVAL;
			return EOF;
		}
	}

	int ret;

	ret = fclose(stream);

	// ���ʂ̃`�F�b�N�ƃ��O
	{
		// �G���[�����O�ɋL�^(�������A�G���[�ڍׂ͔���Ȃ�)
		if (ret == EOF) {
			HS_LOG1("fclose(%d) error", stream);
		}
	}

	return ret;
}
