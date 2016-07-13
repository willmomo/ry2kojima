// viewverDlg.h : ヘッダー ファイル
//

#if !defined(AFX_VIEWVERDLG_H__4853686E_327B_4D9C_80B5_198921BA01E7__INCLUDED_)
#define AFX_VIEWVERDLG_H__4853686E_327B_4D9C_80B5_198921BA01E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CViewverDlg ダイアログ

class CViewverDlg : public CDialog
{
// 構築
public:
	CString GetVersionIni(LPCTSTR pszIniFile);
	CViewverDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CViewverDlg)
	enum { IDD = IDD_VIEWVER_DIALOG };
	CListCtrl	m_lcVersion;
	CString	m_strWsuVersion;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CViewverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CViewverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnReload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_VIEWVERDLG_H__4853686E_327B_4D9C_80B5_198921BA01E7__INCLUDED_)
