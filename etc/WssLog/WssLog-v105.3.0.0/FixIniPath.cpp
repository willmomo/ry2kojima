//---------------------------------------------------------------------
// $Header: /GLw3.0.0.x/GetIniFIle/FixIniPath.cpp 2     07/09/07 16:11 Kojima $ 
//
// アプリケーション名とINIファイルの読込先を決定する関数の提供。
// GetIniFile.dll と wsslog.dll が、process attach のタイミングで、使用する。
//
// INIファイルの読込先は、実行ファイル名から .exe を取り除いたもの。
//
// INIファイルの読込先は、実行ファイルのある場所。
// ただし、アプリケーション名が 'vb6' のときは、カレントディレクトリをINIファイルの読込先にする。
// さらに、レジストリ HKLM\Software\Sanko\Wss\IniPath が、あるときは、そちらをINIファイルの読込先にする。
//---------------------------------------------------------------------
#include "FixIniPath.h"
#include "kjmlib.h"


//---------------------------------------------------------------------
// アプリケーション名とINIファイルの読込先を決定する
//
// [引数]
// app_name  アプリケーション名が入るバッファ。MAX_PATHバイト必要。
// app_path  INIファイルの読込先が入るバッファ。MAX_PATHバイト必要。
//---------------------------------------------------------------------
void FixIniPath(char* app_name, char* app_path) {

	char work[MAX_PATH];

	// モジュールのフルパスを取得する。 
	GetModuleFileName (NULL, work, MAX_PATH);

	// 実行ファイル名を取得する
	PathRemoveExtension(strcpy(app_name, PathFindFileName(work)));

	// 実行ファイルのパス名を取得する
	PathRemoveFileSpec(strcpy(app_path, work));

	// VBの場合、AppPathをカレントディレクトリとする。
	// "C:\Program Files\Microsoft Visual Studio\VB98\VB6.EXE"
	// "C:\Program Files\Microsoft Visual Studio\"のファイルを読みにいくのを防ぐ。
	if (_stricmp(app_name, "vb6") == 0) {

		// カレントディレクトリをセットする。
		GetCurrentDirectory(MAX_PATH, app_path);
	}

	// !! Embedded 対応 !!
	// レジストリに設定があるときは、そちらをINIファイルの読込先とする
	//
	kjm::registry reg;
	if (reg.reg_open_key_ex(HKEY_LOCAL_MACHINE, _T("Software\\Sanko\\Wss"), KEY_READ) == ERROR_SUCCESS) {
		std::string path = reg.get_string(_T("IniPath"), _T(""));
		if (!path.empty()) {
			strcpy(app_path, path.c_str());
		}
	}
}

/**

$History: FixIniPath.cpp $ 
 * 
 * *****************  Version 2  *****************
 * User: Kojima       Date: 07/09/07   Time: 16:11
 * Updated in $/GLw3.0.0.x/GetIniFIle
 * GetIniFile, WssLog共通の初期パス取得関数 FixIniPath 追加。

**/
