/////////////////////////////////////////////////////////////////////////////
//	エクスターン
//	・common.cpp以外のすべてのソースにインクルードする

#ifndef __EXT_H
#define __EXT_H

extern HINSTANCE ghinst;	// インスタンス
extern char gszAppName[64];	// アプリケーション名
extern char gszWindowName[64];	// ウィンドウ タイトル
extern char gszBaudRate[16];	// 転送レート
extern char gszDataBit[16];	// データビット
extern char gszParityBit[16];	// パリティビット
extern char gszStopBit[16];	// ストップビット
extern char gszComPort[16];	// 使用通信ポート

extern HWND ghwndStatusDlg;

#endif
