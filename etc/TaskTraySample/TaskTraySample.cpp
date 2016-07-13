//-----------------------------------------------------------------------------
// タスクトレイにアイコンを表示するウィンドウを表示しないアプリケーションのサンプル
//
// !! 重要 !!
// 基本的な作業は、ワーカースレッドで行う。
// メインスレッドがもたつくと、エクスプローラが固めるため。
// 
// 2012/03/14 r.kojima
//-----------------------------------------------------------------------------
#include "mainFrm.h"

#include "resource.h"

//-----------------------------------------------------------------------------
// プログラムスタート
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// ウィンドウクラスの登録
	if (MainFrm_RegisterClass(hInstance) == FALSE) {
		return -1;
	}

	// ウィンドウの作成
	if (MainFrm_CreateWindow(hInstance, nCmdShow) == FALSE) {
		return -1;
	}

	// メッセージループ
	MSG msg;
	BOOL bRet;
	while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0) {
		if (bRet == -1) {
			msg.wParam = -1;
			break;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}
