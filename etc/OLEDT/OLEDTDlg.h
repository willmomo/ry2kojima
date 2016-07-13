// OLEDTDlg.h : ヘッダー ファイル
//

#if !defined(AFX_OLEDTDLG_H__DC8B677B_DD95_4AB1_9A16_A058AEE1CFCD__INCLUDED_)
#define AFX_OLEDTDLG_H__DC8B677B_DD95_4AB1_9A16_A058AEE1CFCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COLEDTDlg ダイアログ

class COLEDTDlg : public CDialog
{
// 構築
public:
	COleDateTime m_now;
	COLEDTDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(COLEDTDlg)
	enum { IDD = IDD_OLEDT_DIALOG };
	CString	m_lblNow;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(COLEDTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(COLEDTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_OLEDTDLG_H__DC8B677B_DD95_4AB1_9A16_A058AEE1CFCD__INCLUDED_)
