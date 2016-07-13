// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__28953CF9_CAB5_4A48_A9CC_71F84A47EF74__INCLUDED_)
#define AFX_STDAFX_H__28953CF9_CAB5_4A48_A9CC_71F84A47EF74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN // Windows ヘッダーから殆ど使用されないスタッフを除外します

#pragma warning(disable : 4786)

#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <string.h>
#include <vector>

//------------------------------------------------------------
//	Define宣言
//------------------------------------------------------------
#define MAX_PATH	260

#define	MAX_TELOP	40		// テロップ ４０件まで
#define MAX_MESSAGE	40		// 待ち受けメッセージ ４０件まで
#define	MAX_MESLINE	12		// 待ち受けメッセージの行数	１２行まで
#define	MAX_MOJI	26		// 待ち受けメッセージの最大文字数 全角１３文字
#define	MAX_CORNER	160		// テロップ・メッセージの最大コーナー	１６０コーナー
#define	MAX_SET		20		// テロップ・メッセージの設定	２０件まで

#define INI_FILE	"tlp_msg.xml"	// INIファイル

//------------------------------------------------------------
//	構造体宣言
//------------------------------------------------------------
typedef struct {				//
	char telop[100];		// テロップメッセージ
} TELOP_MES_T, TELOP_MES_ALL[40];
	
typedef struct {
	int bg_R;				// 背景色 Ｒ			
	int bg_G;				// 背景色 Ｇ
	int bg_B;				// 背景色 Ｂ
	int title_R;			// タイトル Ｒ
	int title_G;			// タイトル Ｇ
	int title_B;			// タイトル Ｂ
	char title[26];			// タイトル
	struct {
		int font_R;			// メッセージ Ｒ
		int font_G;			// メッセージ Ｇ
		int font_B;			// メッセージ Ｂ
		char text[26];		// メッセージ（全角１３文字まで）
	} msg_line[12];
} MESSAGE_T, MESSAGE_ALL[40];
	
typedef struct {
	char day[10];				// 日
	char time[10];				// 時間（終日の時はここに入る）
	char starttime[10];			// 開始時間
	char endtime[10];			// 終了時間
	struct {
		int rdb_begin;			// 開始台番号
		int rdb_end;			// 終了台番号
		int telop_id;			// テロップＩＤ
		int telop_id2;			// ＡＳ送信用テロップＩＤ
		int msg_id;				// メッセージＩＤ
		int msg_id2;			// ＡＳ送信用メッセージＩＤ
	} corners[160];
} SET_MES_TLP_T, SET_MES_TLP_ALL[20];

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__28953CF9_CAB5_4A48_A9CC_71F84A47EF74__INCLUDED_)
