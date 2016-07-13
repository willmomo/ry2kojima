#if !defined(__WSS_KADO_H)
#define __WSS_KADO_H

// 詳細稼動のためにとりあえず、ヘッダだけ準備

#include "dat_file.h"
#include "sharedobjs.h"
#include "GetIniFile.h"
#include "../WinSousaCalc/Calc/stt_prt.h"
#include "hscp.h"
#include "../wssasup/minitool.h"

#define ALLTIME	24	/* 24時間 */
#define KADOFILE_RECORDSIZE		(ALLTIME * (60 / 5))	/* 機種別定時稼動ファイル（詳細）の１レコードサイズ	*/

#define BackUpInterval	(60 / 10)	/* １分に１回 */
// extern
extern int EigyoZyoutai_Check();


// 稼動情報ファイル構造体
typedef struct {
	unsigned char kadoflg;
	unsigned char dmy[3];
} Kado_Info_t;

typedef std::vector<unsigned long>	ulong_vec;
typedef std::vector<Kado_Info_t> kdinfo_vec;

BOOL ShosaiKadoMain();

// 稼動詳細データテーブルに使用
typedef struct{
	int time;			// 時間
	int as_ofs;			// ＡＳの稼動データオフセット
	int syosai_idx;		// 稼動詳細のインデックス
	char yobi[2];
} SyosaiIndex_t;

#endif

