// keyhookDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_KEYHOOKDLG_H__E1958A4A_47C5_4522_8E51_2BD61A6CD99A__INCLUDED_)
#define AFX_KEYHOOKDLG_H__E1958A4A_47C5_4522_8E51_2BD61A6CD99A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeyhookDlg �_�C�A���O

#define ID_DISABLE_ALTESC	101
#define ID_DISABLE_CTRLALTESC	102

class CKeyhookDlg : public CDialog
{
// �\�z
public:
	CKeyhookDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CKeyhookDlg)
	enum { IDD = IDD_KEYHOOK_SAMPLE_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CKeyhookDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
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
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_KEYHOOKDLG_H__E1958A4A_47C5_4522_8E51_2BD61A6CD99A__INCLUDED_)
