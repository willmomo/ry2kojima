// ReadCheckDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_READCHECKDLG_H__E7158BEB_0A43_4ADC_A737_F73BC74AA3FE__INCLUDED_)
#define AFX_READCHECKDLG_H__E7158BEB_0A43_4ADC_A737_F73BC74AA3FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_USER_THREAD_NOTIFY	(WM_USER + 1)	// (MFC WM_USER) ���b�Z�[�W���`

/////////////////////////////////////////////////////////////////////////////
// CReadCheckDlg �_�C�A���O

class CReadCheckDlg : public CDialog
{
	// �����o�ϐ�
private:
	CWinThread* m_readCheckThread;

public:
	bool m_readCheckAbort;
	CCriticalSection m_cs;

	// �X���b�h�̃X�e�[�^�X
	CString m_ts_strMsg;
	int m_ts_onePos;
	int m_ts_totalPos;
	CList<CString, CString&> m_ts_resultList;
	bool m_ts_has_error;
	__int64 m_ts_totalSize;

// �\�z
public:
	CReadCheckDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CReadCheckDlg)
	enum { IDD = IDD_READCHECK_DIALOG };
	CComboBox	m_drives;
	CProgressCtrl	m_progressOne;
	CProgressCtrl	m_progressTotal;
	CButton	m_btnOK;
	CListBox	m_resultList;
	CStatic	m_statusLabel;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CReadCheckDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CReadCheckDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	//}}AFX_MSG
	afx_msg LRESULT OnUserThreadNotify(WPARAM wParam, LPARAM lParam);	// (MFC WM_USER) ���\�b�h���`
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonSave();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_READCHECKDLG_H__E7158BEB_0A43_4ADC_A737_F73BC74AA3FE__INCLUDED_)
