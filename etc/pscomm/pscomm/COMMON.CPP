/////////////////////////////////////////////////////////////////////////////
//	グローバル変数

#include "main.h"

HINSTANCE ghinst;	// インスタンス
char gszAppName[64];	// アプリケーション名
char gszWindowName[64];	// ウィンドウ タイトル
char gszBaudRate[16];	// 転送レート
char gszDataBit[16];	// データビット
char gszParityBit[16];	// パリティビット
char gszStopBit[16];	// ストップビット
char gszComPort[16];	// 使用通信ポート

HWND ghwndStatusDlg;
