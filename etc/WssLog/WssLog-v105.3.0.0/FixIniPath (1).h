//---------------------------------------------------------------------
// $Header: /GLw3.0.0.x/GetIniFIle/FixIniPath.h 2     07/09/07 16:11 Kojima $ 
//
// アプリケーション名とINIファイルの読込先を決定する関数の提供。
// GetIniFile.dll と wsslog.dll が、process attach のタイミングで、使用する。
//---------------------------------------------------------------------
#pragma once


// プロトタイプ宣言

void FixIniPath(char* app_name, char* app_path);	// アプリケーション名とINIファイルの読込先を決定する


/**

$History: FixIniPath.h $ 
 * 
 * *****************  Version 2  *****************
 * User: Kojima       Date: 07/09/07   Time: 16:11
 * Updated in $/GLw3.0.0.x/GetIniFIle
 * GetIniFile, WssLog共通の初期パス取得関数 FixIniPath 追加。

**/
