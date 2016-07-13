
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include "sys_hed.h"
#include "set_sys.h"
#include "dat_file.h"

typedef struct {
    int out;
    int safe;
    int start;
    int gen1uri;
    int gen2uri;
} DAI_RELATIVE_DATA;

typedef struct {
	int urikin;
} ZEN_RELATIVE_DATA;

typedef struct {
	int rdb;
	int err;
} BUILD_ERROR;

typedef struct {
	int idx;
	int result_data;
} BUILD_RESULT;

static bool is_valid(int new_val, int rel)
{
	if (rel == 0)	return true;		// 修正値が無いときは、有効
	if (new_val > 0)	return true;	// 修正後が 0 になることは許さない
	return false;
}

//
// 金額を種別の設定に応じて玉(メダル)数に変換する
//
static int kingaku_to_tamasu(int kingaku, const fno1007_t* pas)
{
	assert(pas != NULL);

	if (pas->urikin == 0 || pas->inpscl == 0)
		return 0;

	return ((kingaku * pas->urikos) / (pas->urikin * 100)) / pas->inpscl;
}

int __stdcall fno640X_build(FDATA_DAI* buf6407,
							size_t size6407,
							FDATA_SMCON* buf6411,
							size_t size6411,
							const fno1000_t* buf1000,
							const fno1001_t* buf1001,
							const fno1002_t* buf1002,
							const fno1007_t* buf1007,
							const FDATA_DAI* buf4102,
							const FDATA_SMCON* buf4103,
							const DAI_RELATIVE_DATA* relDai,
							const ZEN_RELATIVE_DATA* relZen,
							BUILD_ERROR* build_error,
							int* err_count,
							BUILD_RESULT* build_result)
{
	if (size6407 < sizeof(FDATA_DAI[DAISU]))		return -1;
	if (size6411 < sizeof(FDATA_SMCON[ZENTENSU]))	return -1;

	int new_out, new_safe, new_start, new_gen1uri, new_gen2uri, new_indata;
	int i;
	int wp[3] = {0, 0, 0};
	FDATA_SMCON zen[ZENTENSU];
	ZEN_RELATIVE_DATA relZen2[ZENTENSU];

	*err_count = 0;
	memset(zen, 0, sizeof(zen));
	memmove(relZen2, relZen, sizeof(relZen2));

	for (i = 0; i < ntohs(buf1000->dai); i++) {
		int rs = ((buf1001[i].rsdn_nid & 0xe0) >> 5) - 1;

		int idx6407 = (512 * rs) + wp[rs];
		buf6407[idx6407] = buf4102[i];

		new_out = ntohs(buf6407[idx6407].out) + relDai[i].out;						// アウト再計算
		new_safe = ntohs(buf6407[idx6407].saf) 
			+ ntohs(buf6407[idx6407].chk_dat) + relDai[i].safe;						// セーフ再計算
		new_start = ntohs(buf6407[idx6407].start) + relDai[i].start;				// スタート再計算
		new_gen1uri = ntohs(buf6407[idx6407].gen1uri) + relDai[i].gen1uri / 100;	// 現金1売上再計算
		new_gen2uri = ntohs(buf6407[idx6407].gen2uri) + relDai[i].gen2uri / 100;	// 現金2売上再計算

		// 復旧後のすべてのデータが整数のときは、正常なので適用する。
		if (is_valid(new_out, relDai[i].out) && 
			is_valid(new_safe, relDai[i].safe) &&
			is_valid(new_start, relDai[i].start) &&
			is_valid(new_gen1uri, relDai[i].gen1uri) &&
			is_valid(new_gen2uri, relDai[i].gen2uri)) {

			buf6407[idx6407].out = htons(new_out);
			if (new_safe <= 65535) {
				buf6407[idx6407].saf = htons(new_safe);
				buf6407[idx6407].chk_dat = 0;
			} else {
				buf6407[idx6407].saf = 65535;
				buf6407[idx6407].chk_dat = htons(new_safe - 65535);
			}
			buf6407[idx6407].start = htons(new_start);
			buf6407[idx6407].gen1uri = htons(new_gen1uri);
			buf6407[idx6407].gen2uri = htons(new_gen2uri);
		} else {
			memset(&build_error[*err_count], 0, sizeof(BUILD_ERROR));
			build_error[*err_count].rdb = i + 1;	// エラーの発生した台番号
			if (!is_valid(new_out, relDai[i].out))		build_error[*err_count].err |= (int)pow(2, 0);	// 復旧後アウト異常
			if (!is_valid(new_safe, relDai[i].safe))		build_error[*err_count].err |= (int)pow(2, 1);	// 復旧後セーフ異常
			if (!is_valid(new_start, relDai[i].start))		build_error[*err_count].err |= (int)pow(2, 2);	// 復旧後スタート異常
			if (!is_valid(new_gen1uri, relDai[i].gen1uri))	build_error[*err_count].err |= (int)pow(2, 3);	// 復旧後現金1売上異常
			if (!is_valid(new_gen2uri, relDai[i].gen2uri))	build_error[*err_count].err |= (int)pow(2, 4);	// 復旧後現金2売上異常

			if (buf1001[i].urisyu1 == 1 && buf1001[i].kinko > 0) {
				relZen2[buf1001[i].kinko - 1].urikin += -relDai[i].gen1uri;
			}
			if (buf1001[i].urisyu2 == 1 && buf1001[i].kinko > 0) {
				relZen2[buf1001[i].kinko - 1].urikin += -relDai[i].gen2uri;
			}

			++(*err_count);
		}

		++wp[rs];
	}

	for (i = 0; i < ZENTENSU; i++) {
		buf6411[i] = buf4103[i];

		switch (buf1002[i].kind) {
		case 2:		// 景品機
			new_indata = ntohl(buf6411[i].indata) +
				kingaku_to_tamasu(relZen2[i].urikin, &buf1007[buf1002[i].pas - 1]) + zen[buf1001[i].kinko-1].indata;
			break;
		default:
			new_indata = ntohl(buf6411[i].indata) + (relZen2[i].urikin / 100) + (zen[buf1001[i].kinko-1].indata / 100);
			break;
		}

		build_result[i].idx = i + 1;
		build_result[i].result_data = new_indata;

		buf6411[i].indata = htonl(new_indata);
	}

	return 0;
}

int __stdcall fno640X_build_nolink(FDATA_DAI* buf6407,
								   size_t size6407,
								   FDATA_SMCON* buf6411,
								   size_t size6411,
								   const fno1000_t* buf1000,
								   const fno1001_t* buf1001,
								   const fno1002_t* buf1002,
								   const fno1007_t* buf1007,
								   const FDATA_DAI* buf4102,
								   const FDATA_SMCON* buf4103,
								   const DAI_RELATIVE_DATA* relDai,
								   const ZEN_RELATIVE_DATA* relZen,
								   BUILD_ERROR* build_error,
								   int* err_count,
								   BUILD_RESULT* build_result)
{
	if (size6407 < sizeof(FDATA_DAI[DAISU]))		return -1;
	if (size6411 < sizeof(FDATA_SMCON[ZENTENSU]))	return -1;

	int new_out, new_safe, new_start, new_gen1uri, new_gen2uri, new_indata;
	int i;
	int wp[3] = {0, 0, 0};
	FDATA_SMCON zen[ZENTENSU];

	*err_count = 0;
	memset(zen, 0, sizeof(zen));

	for (i = 0; i < ntohs(buf1000->dai); i++) {
		int rs = ((buf1001[i].rsdn_nid & 0xe0) >> 5) - 1;

		int idx6407 = (512 * rs) + wp[rs];
		buf6407[idx6407] = buf4102[i];

		new_out = ntohs(buf6407[idx6407].out) + relDai[i].out;						// アウト再計算
		new_safe = ntohs(buf6407[idx6407].saf) 
			+ ntohs(buf6407[idx6407].chk_dat) + relDai[i].safe;						// セーフ再計算
		new_start = ntohs(buf6407[idx6407].start) + relDai[i].start;				// スタート再計算
		new_gen1uri = ntohs(buf6407[idx6407].gen1uri) + relDai[i].gen1uri / 100;	// 現金1売上再計算
		new_gen2uri = ntohs(buf6407[idx6407].gen2uri) + relDai[i].gen2uri / 100;	// 現金2売上再計算

		//
		// 復旧後のデータの正当性はチェックしない。
		// 間違った値を書けば、間違った情報になる。
		//
		buf6407[idx6407].out = htons(new_out);
		if (new_safe <= 65535) {
			buf6407[idx6407].saf = htons(new_safe);
			buf6407[idx6407].chk_dat = 0;
		} else {
			buf6407[idx6407].saf = 65535;
			buf6407[idx6407].chk_dat = htons(new_safe - 65535);
		}
		buf6407[idx6407].start = htons(new_start);
		buf6407[idx6407].gen1uri = htons(new_gen1uri);
		buf6407[idx6407].gen2uri = htons(new_gen2uri);

		++wp[rs];
	}

	for (i = 0; i < ZENTENSU; i++) {
		buf6411[i] = buf4103[i];

		switch (buf1002[i].kind) {
		case 2:		// 景品機
			new_indata = ntohl(buf6411[i].indata) + kingaku_to_tamasu(relZen[i].urikin, &buf1007[buf1002[i].pas - 1]);
			break;
		default:
			new_indata = ntohl(buf6411[i].indata) + (relZen[i].urikin / 100);
			break;
		}

		build_result[i].idx = i + 1;
		build_result[i].result_data = new_indata;

		buf6411[i].indata = htonl(new_indata);
	}

	return 0;
}

int __stdcall fno1000_init(const char* fname, fno1000_t* buffer, size_t bufsize)
{
	int ret = -1;

	if (bufsize < sizeof(fno1000_t))	return ret;

	FILE* fp = fopen(fname, "rb");
	if (fp) {
		if (fread(buffer, sizeof(fno1000_t), 1, fp) == 1) {
			ret = 0;
		}
		fclose(fp);
	}

	return ret;
}

int __stdcall fno1000_get_daisu(const fno1000_t* buffer)
{
	return ntohs(buffer->dai);
}

int __stdcall fno1000_get_rs(const fno1000_t* buffer)
{
	return ntohs(buffer->rs);
}

int __stdcall fno1001_init(const char* fname, fno1001_t* buffer, size_t bufsize)
{
	int ret = -1;

	if (bufsize < sizeof(fno1001_t[DAISU]))	return ret;

	FILE* fp = fopen(fname, "rb");
	if (fp) {
		if (fread(buffer, sizeof(fno1001_t), DAISU, fp) == DAISU) {
			ret = 0;
		}
		fclose(fp);
	}

	return ret;
}

int __stdcall fno1001_get_hdb(const fno1001_t* buffer, int rdb)
{
	return ntohs(buffer[rdb-1].dno);
}

int __stdcall fno1001_get_sbt(const fno1001_t* buffer, int rdb)
{
	return buffer[rdb-1].pas;
}

int __stdcall fno1001_get_kinko(const fno1001_t* buffer, int rdb)
{
	return buffer[rdb-1].kinko;
}

int __stdcall fno1002_init(const char* fname, fno1002_t* buffer, size_t bufsize)
{
	int ret = -1;

	if (bufsize < sizeof(fno1002_t[ZENTENSU]))	return ret;

	FILE* fp = fopen(fname, "rb");
	if (fp) {
		if (fread(buffer, sizeof(fno1002_t), ZENTENSU, fp) == ZENTENSU) {
			ret = 0;
		}
		fclose(fp);
	}

	return ret;
}

int __stdcall fno1002_get_sbt(const fno1002_t* buffer, int idx)
{
	return buffer[idx-1].pas;
}

int __stdcall fno1002_get_disp1(const fno1002_t* buffer, int idx)
{
	return ntohs(buffer[idx-1].disp1);
}

int __stdcall fno1002_get_disp2(const fno1002_t* buffer, int idx)
{
	return ntohs(buffer[idx-1].disp2);
}

int __stdcall fno1002_get_kind(const fno1002_t* buffer, int idx)
{
	return buffer[idx-1].kind;
}

int __stdcall fno1007_init(const char* fname, fno1007_t* buffer, size_t bufsize)
{
	int ret = -1;

	if (bufsize < sizeof(fno1007_t[SHUBETSU]))	return ret;

	FILE* fp = fopen(fname, "rb");
	if (fp) {
		if (fread(buffer, sizeof(fno1007_t), SHUBETSU, fp) == SHUBETSU) {
			ret = 0;
		}
		fclose(fp);
	}

	return ret;
}

int __stdcall fno1007_get_urikin(const fno1007_t* buffer, int sbt)
{
	return buffer[sbt-1].urikin * 100;
}

int __stdcall fno1007_get_urikos(const fno1007_t* buffer, int sbt)
{
	return buffer[sbt-1].urikos;
}

int __stdcall fno1007_get_inpscl(const fno1007_t* buffer, int sbt)
{
	return buffer[sbt-1].inpscl;
}

int __stdcall fno1008_init(const char* fname, fno1008_t* buffer, size_t bufsize)
{
	int ret = -1;

	if (bufsize < sizeof(fno1008_t))	return ret;

	FILE* fp = fopen(fname, "rb");
	if (fp) {
		if (fread(buffer, sizeof(fno1008_t), 1, fp) == 1) {
			ret = 0;
		}
		fclose(fp);
	}

	return ret;
}

int __stdcall fno1008_get_sbtname(const fno1008_t* buffer, int sbt, char* sbtname)
{
	memset(sbtname, 0, 13);
	strncpy(sbtname, (char*)&buffer->sbt1[(sbt - 1) * 12], 12);

	return 0;
}

int __stdcall fno4102_init(const char* fname, FDATA_DAI* buffer, size_t bufsize)
{
	int ret = -1;

	if (bufsize < sizeof(FDATA_DAI[DAISU]))	return -1;

	FILE* fp = fopen(fname, "rb");
	if (fp) {
		if (fread(buffer, sizeof(FDATA_DAI), DAISU, fp) == DAISU) {
			ret = 0;
		}
		fclose(fp);
	}

	return ret;
}

int __stdcall fno4102_get_out(const FDATA_DAI* buffer, int rdb)
{
	return ntohs(buffer[rdb-1].out);
}

int __stdcall fno4102_get_safe(const FDATA_DAI* buffer, int rdb)
{
	return ntohs(buffer[rdb-1].saf) + ntohs(buffer[rdb-1].chk_dat);
}

int __stdcall fno4102_get_start(const FDATA_DAI* buffer, int rdb)
{
	return ntohs(buffer[rdb-1].start);
}

int __stdcall fno4102_get_gen1uri(const FDATA_DAI* buffer, int rdb)
{
	return ntohs(buffer[rdb-1].gen1uri) * 100;
}

int __stdcall fno4102_get_gen2uri(const FDATA_DAI* buffer, int rdb)
{
	return ntohs(buffer[rdb-1].gen2uri) * 100;
}

int __stdcall fno4103_init(const char* fname, FDATA_SMCON* buffer, size_t bufsize)
{
	int ret = -1;

	if (bufsize < sizeof(FDATA_SMCON[ZENTENSU]))	return -1;

	FILE* fp = fopen(fname, "rb");
	if (fp) {
		if (fread(buffer, sizeof(FDATA_SMCON), ZENTENSU, fp) == ZENTENSU) {
			ret = 0;
		}
		fclose(fp);
	}

	return ret;
}

int __stdcall fno4103_get_indata(const FDATA_SMCON* buffer, int idx)
{
	return ntohl(buffer[idx-1].indata);
}

int __stdcall CreateGUID(GUID* guid)
{
	int ret = -1;

	HRESULT hr = CoCreateGuid(guid);
	if (SUCCEEDED(hr))
		ret = 0;

	return ret;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
