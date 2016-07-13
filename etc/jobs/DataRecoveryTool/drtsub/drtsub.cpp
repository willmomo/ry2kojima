
#include <windows.h>
#include <stdio.h>
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

int __stdcall fno640X_build(FDATA_DAI* buf6408,
							size_t size6408,
							FDATA_SMCON* buf6409,
							size_t size6409,
							const fno1000_t* buf1000,
							const fno1001_t* buf1001,
							const fno1002_t* buf1002,
							const FDATA_DAI* buf4102,
							const FDATA_SMCON* buf4103,
							const DAI_RELATIVE_DATA* relDai,
							const ZEN_RELATIVE_DATA* relZen)
{
	if (size6408 < sizeof(FDATA_DAI[DAISU]))		return -1;
	if (size6409 < sizeof(FDATA_SMCON[ZENTENSU]))	return -1;

	int i;
	int wp[3] = {0, 0, 0};

	for (i = 0; i < ntohs(buf1000->dai); i++) {
		int rs = ((buf1001[i].rsdn_nid & 0xe0) >> 5) - 1;

		int idx6408 = (512 * rs) + wp[rs];
		buf6408[idx6408] = buf4102[i];

		buf6408[idx6408].out = htons(ntohs(buf6408[idx6408].out) + relDai[i].out);
		buf6408[idx6408].saf = htons(ntohs(buf6408[idx6408].saf) + relDai[i].safe);
		buf6408[idx6408].start = htons(ntohs(buf6408[idx6408].start) + relDai[i].start);
		buf6408[idx6408].gen1uri = htons(ntohs(buf6408[idx6408].gen1uri) + relDai[i].gen1uri);
		buf6408[idx6408].gen2uri = htons(ntohs(buf6408[idx6408].gen2uri) + relDai[i].gen2uri);

		++wp[rs];
	}

	for (i = 0; i < ZENTENSU; i++) {
		buf6409[i] = buf4103[i];

		buf6409[i].indata = htonl(ntohl(buf6409[i].indata) + relZen[i].urikin);
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
	return ntohl(buffer[idx-1].indata) * 100;
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
