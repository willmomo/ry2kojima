// viewverDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_VIEWVERDLG_H__4853686E_327B_4D9C_80B5_198921BA01E7__INCLUDED_)
#define AFX_VIEWVERDLG_H__4853686E_327B_4D9C_80B5_198921BA01E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CViewverDlg �_�C�A���O

class CViewverDlg : public CDialog
{
// �\�z
public:
	CString GetVersionIni(LPCTSTR pszIniFile);
	CViewverDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CViewverDlg)
	enum { IDD = IDD_VIEWVER_DIALOG };
	CListCtrl	m_lcVersion;
	CString	m_strWsuVersion;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CViewverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_VIEWVERDLG_H__4853686E_327B_4D9C_80B5_198921BA01E7__INCLUDED_)
