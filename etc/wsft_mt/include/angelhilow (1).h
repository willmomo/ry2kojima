//---------------------------------------------------------------------
// 概要
//	AngelHiLow共通のヘッダ
//
// 作成者
//	HAL小島
//
// 備考
//	AngelHiLow.DLLを利用する人はこのファイルをインクルード
//---------------------------------------------------------------------

#pragma once

#if !defined(__ANGELHILOW_H)
#define __ANGELHILOW_H

#include <stdio.h>
#include <assert.h>

#if defined(ANGELHILOW_EXPORTS)
#define ANGELHILOW_API	__declspec(dllexport) __stdcall
#else
#define ANGELHILOW_API	__declspec(dllimport) __stdcall
#endif

#if defined(_DEBUG)
#define AHL_W(N)	(assert(sizeof(N) == sizeof(short)) , ((((N) & 0x00ff) << 8) | (((N) & 0xff00) >> 8)))
#define AHL_L(N)	(assert(sizeof(N) == sizeof(long)) , ((((N) & 0x000000ff) << 24) | (((N) & 0x0000ff00) << 8) | (((N) & 0x00ff0000) >> 8) | (((N) & 0xff000000) >> 24)))
#else
#define AHL_W(N)	((((N) & 0x00ff) << 8) | (((N) & 0xff00) >> 8))
#define AHL_L(N)	((((N) & 0x000000ff) << 24) | (((N) & 0x0000ff00) << 8) | (((N) & 0x00ff0000) >> 8) | (((N) & 0xff000000) >> 24))
#endif

double AHL_D(double src);

// AngelHiLow用オプション

#define HLOPT_BODY	0	// 通常の変換
#define HLOPT_HEAD	1	// 履歴のインデックス変換などで使用
#define HLOPT_FULL	2	// 先頭レコード＋通常レコード全体を変換する場合(fno6001など)

// ConvertCSV用オプション

#define CVOPT_NORMAL			0x0000	// 通常のCSV変換
//#define CVOPT_ROW				0x0001	// 構造体生変換(未対応)

#define CVOPT_NOHEADER			0x0100	// 列ヘッダを出力しません
#define CVOPT_NORECNO			0x0200	// 列番号を出力しません
#define CVOPT_NOCOMMENT			0x0400	// コメント行を出力しません
#define CVOPT_ALLOCATEBUFFER	0x0800	// 関数がバッファを確保します
										// (pdstには、ポインタへのアドレスを渡します)
										// (バッファはGlobalFreeで開放してください)

// プロトタイプ

BOOL ANGELHILOW_API AngelHiLow(int fno, const void* psrc, void* pdst, int rec, int opt);
BOOL ANGELHILOW_API ConvertCSV(int fno, const void* psrc, const char* pfname, int rec, int opt);

#endif
