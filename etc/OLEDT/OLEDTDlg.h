// OLEDTDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_OLEDTDLG_H__DC8B677B_DD95_4AB1_9A16_A058AEE1CFCD__INCLUDED_)
#define AFX_OLEDTDLG_H__DC8B677B_DD95_4AB1_9A16_A058AEE1CFCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COLEDTDlg �_�C�A���O

class COLEDTDlg : public CDialog
{
// �\�z
public:
	COleDateTime m_now;
	COLEDTDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(COLEDTDlg)
	enum { IDD = IDD_OLEDT_DIALOG };
	CString	m_lblNow;
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(COLEDTDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(COLEDTDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_OLEDTDLG_H__DC8B677B_DD95_4AB1_9A16_A058AEE1CFCD__INCLUDED_)
