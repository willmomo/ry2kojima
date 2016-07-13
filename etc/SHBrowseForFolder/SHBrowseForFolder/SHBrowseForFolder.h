// SHBrowseForFolder.h : SHBROWSEFORFOLDER アプリケーションのメイン ヘッダー ファイルです。
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSHBrowseForFolderApp:
// このクラスの動作の定義に関しては SHBrowseForFolder.cpp ファイルを参照してください。
//

class CSHBrowseForFolderApp : public CWinApp
{
public:
	CSHBrowseForFolderApp();

// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CSHBrowseForFolderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CSHBrowseForFolderApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

