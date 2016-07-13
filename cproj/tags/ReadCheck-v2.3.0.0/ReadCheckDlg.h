// ReadCheckDlg.h : ヘッダー ファイル
//

#if !defined(AFX_READCHECKDLG_H__E7158BEB_0A43_4ADC_A737_F73BC74AA3FE__INCLUDED_)
#define AFX_READCHECKDLG_H__E7158BEB_0A43_4ADC_A737_F73BC74AA3FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_THREAD_NOTIFY	(WM_USER + 1)	// (MFC WM_USER) メッセージを定義

/////////////////////////////////////////////////////////////////////////////
// CReadCheckDlg ダイアログ

class CReadCheckDlg : public CDialog
{
	// メンバ変数
private:
	CWinThread* m_readCheckThread;

public:
	bool m_readCheckAbort;
	CCriticalSection m_cs;

	// スレッドのステータス
	CString m_ts_strMsg;
	int m_ts_onePos;
	int m_ts_totalPos;
	CList<CString, CString&> m_ts_resultList;
	bool m_ts_has_error;
	__int64 m_ts_totalSize;

// 構築
public:
	CReadCheckDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CReadCheckDlg)
	enum { IDD = IDD_READCHECK_DIALOG };
	CComboBox	m_drives;
	CProgressCtrl	m_progressOne;
	CProgressCtrl	m_progressTotal;
	CButton	m_btnOK;
	CListBox	m_resultList;
	CStatic	m_statusLabel;
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CReadCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CReadCheckDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LRESULT OnUserThreadNotify(WPARAM wParam, LPARAM lParam);	// (MFC WM_USER) メソッドを定義
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonSave();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_READCHECKDLG_H__E7158BEB_0A43_4ADC_A737_F73BC74AA3FE__INCLUDED_)
