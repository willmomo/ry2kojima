//////////////////////////////////////////////////////////////////////
//	[概要]
//	プロセス管理クラス
//
//	[作成者]
//	Ryota Kojima
//
//	[作成日
//	2003/10/15

#include "rkProcess.h"

//////////////////////////////////////////////////////////////////////
//	[概要]
//	GetProcessWindow関数で使用するコールバック
//
//	[引数]
//	HWND hwnd		列挙されたウィンドウへのハンドル
//	LPARAM lParam	rkProcessへのポインタ。キャストして使う
//
//	[備考]
//	列挙されたウィンドウと同じプロセスIDを持つものを見つけた場合
//	そのハンドルをメンバー変数に保存し、列挙を終了する。

BOOL CALLBACK rkProcess::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	rkProcess* p = reinterpret_cast<rkProcess*>(lParam);

	DWORD dwProcessId;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == p->m_pi.dwProcessId) {
		p->m_hwnd = hwnd;
		return FALSE;
	}

	return TRUE;
}
