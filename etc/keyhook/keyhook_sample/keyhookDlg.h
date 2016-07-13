// keyhookDlg.h : ヘッダー ファイル
//

#if !defined(AFX_KEYHOOKDLG_H__E1958A4A_47C5_4522_8E51_2BD61A6CD99A__INCLUDED_)
#define AFX_KEYHOOKDLG_H__E1958A4A_47C5_4522_8E51_2BD61A6CD99A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeyhookDlg ダイアログ

#define ID_DISABLE_ALTESC	101
#define ID_DISABLE_CTRLALTESC	102

class CKeyhookDlg : public CDialog
{
// 構築
public:
	CKeyhookDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CKeyhookDlg)
	enum { IDD = IDD_KEYHOOK_SAMPLE_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CKeyhookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CKeyhookDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnKeyHook(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_KEYHOOKDLG_H__E1958A4A_47C5_4522_8E51_2BD61A6CD99A__INCLUDED_)
