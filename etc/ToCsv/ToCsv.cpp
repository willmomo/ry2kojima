#include "../kjmlib/kjmlib.h"
#if defined(_DEBUG)
#pragma comment(lib, "../kjmlib/library/kjmlibd.lib")
#else
#pragma comment(lib, "../kjmlib/library/kjmlib.lib")
#endif

//---------------------------------------------------------------------
// 営業履歴用の構造体
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
// 顔認証で表示対象になる発生かどうか調べる
//
// [戻り値]
//  = 1 表示対象の発生。
//  = 0 表示しない発生。
//---------------------------------------------------------------------
int IsFxSupport(int code1, int code2) {
	int c1 = code1 & 0x7f;	// 復旧ビットを落とす
	int c1c2 = ((c1 > 0x07 ? c1 : 0) << 8) | code2;

	int ret = 0;	// [表示しない]で初期化

	switch (c1c2) {
	case 0x003b:	// 不正
	case 0x003c:	// 不正2
	case 0x0040:	// 不正1断線
	case 0x0041:	// 不正2断線
	case 0x0042:	// ドア断線
	case 0x0043:	// ドア2断線
	case 0x0044:	// セーフ異常
	case 0x0045:	// 特賞回数異常
	case 0x0046:	// 特賞出玉異常
	case 0x0047:	// ドア特賞
	case 0x0048:	// ドア異常
	case 0x0049:	// スタート異常
	case 0x004a:	// ドアオープン
	case 0x004b:	// ドア2オープン
	case 0x004c:	// アウト点検
	case 0x004d:	// セーフ点検
	case 0x004e:	// 特賞出玉点検
	case 0x004f:	// 補給点検
	case 0x0050:	// スタート点検
	case 0x0051:	// ドア点検
	case 0x0052:	// 会員非稼動点検
	case 0x0053:	// 現金1断線
	case 0x0054:	// 現金2断線
	case 0x0055:	// カード断線
	case 0x0056:	// 売上個数異常
	case 0x0057:	// 売上異常
	case 0x0058:	// 特賞売上異常
	case 0x007b:	// 再プレイ断線
	case 0x007c:	// 券売機断線
	case 0x007d:	// サンドトラブル
	case 0x007e:	// ドア3オープン
	case 0x007f:	// 不正3
	case 0x0bf1:	// 機器点検	
	case 0x0bf6:	// 台ランプ通信異常
	case 0x0bf7:	// サンド払出エラー
	case 0x0cf2:	// TN通信異常
	case 0x0cf3:	// TNIO通信異常
	case 0x0cf4:	// リプレイ通信異常
	case 0x0cf5:	// コインセレクタ点検
	case 0x1101:	// 補給異常(サンド)
	case 0x1201:	// 補給機点検
		ret = 1;	// [表示する]に変更
		break;
	}

	return ret;
}

//---------------------------------------------------------------------
// 営業履歴をCSV化する
//---------------------------------------------------------------------
int ToCsv_Eireki(LPCTSTR pszFname) {
	FILE* fp = _tfopen(pszFname, _T("rb"));
	if (fp == NULL) {
		_ftprintf(stderr, _T("ファイル '%s' が、オープンできません。error %d\n"), pszFname, errno);
		return -1;
	}

	eigyou_rireki_A buf;

	// 1レコード目は、件数なのでスキップ
	if (fread(&buf, sizeof(buf), 1, fp) != 1) {
		return -1;
	}
	// ヘッダを画面に出力
	_tprintf(_T("rno,code1,code2,hhmms0,data1,data2,data3,data4,ldata5,顔認証\n"));

	int rno = 1;
	while (1) {
		if (fread(&buf, sizeof(buf), 1, fp) != 1) {
			break;
		}

		// CSV変換して画面に出力
		_tprintf(_T("%d,%u,%u,%u,%u,%u,%u,%u,%d,%d\n"), 
			rno++, buf.code1, buf.code2, buf.hhmms0, buf.data1, buf.data2, buf.data3, buf.data4, buf.ldata5, IsFxSupport(buf.code1, buf.code2));
	}

	fclose(fp);

	return 0;
}

//---------------------------------------------------------------------
// プログラムスタート
//---------------------------------------------------------------------
int _tmain(int argc, TCHAR** argv) {
	for (int i = 1; i < argc; i++) {
		ToCsv_Eireki(argv[i]);
	}

	return 0;
}
