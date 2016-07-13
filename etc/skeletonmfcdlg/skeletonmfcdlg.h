// skeletonmfcdlg.h : SKELETONMFCDLG アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_SKELETONMFCDLG_H__85E8EE74_1E09_4AB9_A9C8_88E9E4035200__INCLUDED_)
#define AFX_SKELETONMFCDLG_H__85E8EE74_1E09_4AB9_A9C8_88E9E4035200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSkeletonmfcdlgApp:
// このクラスの動作の定義に関しては skeletonmfcdlg.cpp ファイルを参照してください。
//

class CSkeletonmfcdlgApp : public CWinApp
{
public:
	CSkeletonmfcdlgApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSkeletonmfcdlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CSkeletonmfcdlgApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SKELETONMFCDLG_H__85E8EE74_1E09_4AB9_A9C8_88E9E4035200__INCLUDED_)
