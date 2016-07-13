// skeletonmfcdlgDlg.h : ヘッダー ファイル
//

#if !defined(AFX_SKELETONMFCDLGDLG_H__73CB1297_5EC8_4FAF_B2B2_E7814B7CEC93__INCLUDED_)
#define AFX_SKELETONMFCDLGDLG_H__73CB1297_5EC8_4FAF_B2B2_E7814B7CEC93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSkeletonmfcdlgDlg ダイアログ

class CSkeletonmfcdlgDlg : public CDialog
{
// 構築
public:
	CSkeletonmfcdlgDlg(CWnd* pParent = NULL);	// 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CSkeletonmfcdlgDlg)
	enum { IDD = IDD_SKELETONMFCDLG_DIALOG };
		// メモ: この位置に ClassWizard によってデータ メンバが追加されます。
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CSkeletonmfcdlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	HICON m_hIcon;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CSkeletonmfcdlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SKELETONMFCDLGDLG_H__73CB1297_5EC8_4FAF_B2B2_E7814B7CEC93__INCLUDED_)
