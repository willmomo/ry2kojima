// PipeFormat.h : PIPEFORMAT アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_PIPEFORMAT_H__062F21D4_7336_11D3_B79F_00001C1ABF19__INCLUDED_)
#define AFX_PIPEFORMAT_H__062F21D4_7336_11D3_B79F_00001C1ABF19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CPipeFormatApp:
// このクラスの動作の定義に関しては PipeFormat.cpp ファイルを参照してください。
//

class CPipeFormatApp : public CWinApp
{
public:
	CPipeFormatApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CPipeFormatApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CPipeFormatApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_PIPEFORMAT_H__062F21D4_7336_11D3_B79F_00001C1ABF19__INCLUDED_)
