//---------------------------------------------------------------------
// 厳密チェック、ログつき関数群

#define HS_IMPREMENT

#include "libhs.h"

//---------------------------------------------------------------------
// [概要]
// 実行ファイル名を戻す関数(エラーは無いものと信じる)
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
// [概要]
// 厳密fopen
//---------------------------------------------------------------------

FILE *hs_fopen(const char *filename, const char *mode)
{
	// 事前条件のチェックとログ
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

	// 結果のチェックとログ
	{
		// エラーをログに記録
		if (fp == NULL) {
			HS_ERRLOG2("fopen(\"%s\", \"%s\")", filename, mode);
		}
	}

	return fp;
}

//---------------------------------------------------------------------
// [概要]
// 厳密fputs
//---------------------------------------------------------------------

int hs_fputs(const char *string, FILE *stream)
{
	// 事前条件のチェックとログ
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

	// 結果のチェックとログ
	{
		// エラーをログに記録(ただし、エラー詳細は判らない)
		if (ret == EOF) {
			HS_LOG2("fputs(\"%s\", %p) error", string, stream);
		}
	}

	return ret;
}

//---------------------------------------------------------------------
// [概要]
// 厳密fclose
//---------------------------------------------------------------------

int hs_fclose(FILE *stream)
{
	// 事前条件のチェックとログ
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

	// 結果のチェックとログ
	{
		// エラーをログに記録(ただし、エラー詳細は判らない)
		if (ret == EOF) {
			HS_LOG1("fclose(%d) error", stream);
		}
	}

	return ret;
}
