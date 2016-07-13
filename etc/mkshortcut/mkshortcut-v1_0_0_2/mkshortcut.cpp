//-----------------------------------------------------------------------------
// ショートカットを作成するプログラム
//
// 使用法：mkshortcut <ショートカットのフルパス> <実行ファイルのフルパス>
//-----------------------------------------------------------------------------
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <intshcut.h>
#include <shlobj.h>

#include <comutil.h>
#pragma comment(lib, "comsuppw.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "resource.h"

//-----------------------------------------------------------------------------
// ショートカットの作成
//
// CoInitialize(NULL)/CoUninitialize()は、アプリケーションが行ってください。
//-----------------------------------------------------------------------------
bool createShortcut(LPCTSTR  strFileName, LPCTSTR strFile, LPCTSTR strArgs = NULL, LPCTSTR strDir = NULL) {
	HRESULT hRes;
	IShellLink *isLink = NULL; 
	IPersistFile *ipFile = NULL;

	//IShellLinkを取得
	hRes = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID *)&isLink);
	if (SUCCEEDED(hRes)) {
		//パスの設定
		isLink->SetPath(strFile);

		//実行時引数（スイッチ）の設定
		if (strArgs)	isLink->SetArguments(strArgs);

		//作業フォルダの設定
		if (strDir)		isLink->SetWorkingDirectory(strDir);

		//IPersistFileを取得
		hRes = isLink->QueryInterface(IID_IPersistFile, (void**)&ipFile);
		if (SUCCEEDED(hRes)) {
			//ディスクに保存する
			_bstr_t path = strFileName;
			hRes = ipFile->Save((wchar_t*)path, TRUE);

			//IPersistFileの破棄
			ipFile->Release();
		}

		//IShellLinkを破棄
		isLink->Release();
	}else{
		//失敗
	}

	return (SUCCEEDED(hRes) ? true : false);
}


//-----------------------------------------------------------------------------
// プログラム スタート
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	if (__argc != 3) {
		MessageBox(NULL, _T("使用法：mkshortcut <ショートカットファイル名> <リンク先ファイル名>"), _T("mkshortcut"), MB_OK);
		return 0;
	}

	CoInitialize(NULL);
	
	TCHAR workDir[MAX_PATH];
	PathRemoveFileSpec(lstrcpy(workDir, __argv[2]));
	int ret = (createShortcut(__argv[1], __argv[2], NULL, workDir)) ? 0 : 1;

	CoUninitialize();
	return 0;
}
