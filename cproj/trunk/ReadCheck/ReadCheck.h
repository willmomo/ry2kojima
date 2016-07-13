// ReadCheck.h : READCHECK アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_READCHECK_H__CC3CFCC6_3954_4529_BD1B_C32824E3525E__INCLUDED_)
#define AFX_READCHECK_H__CC3CFCC6_3954_4529_BD1B_C32824E3525E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CReadCheckApp:
// このクラスの動作の定義に関しては ReadCheck.cpp ファイルを参照してください。
//

class CReadCheckApp : public CWinApp
{
public:
	CReadCheckApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CReadCheckApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CReadCheckApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_READCHECK_H__CC3CFCC6_3954_4529_BD1B_C32824E3525E__INCLUDED_)
