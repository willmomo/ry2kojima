// PipeFormatDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_PIPEFORMATDLG_H__062F21D6_7336_11D3_B79F_00001C1ABF19__INCLUDED_)
#define AFX_PIPEFORMATDLG_H__062F21D6_7336_11D3_B79F_00001C1ABF19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPipeFormatDlg �_�C�A���O

class CPipeFormatDlg : public CDialog
{
// �\�z
public:
	CPipeFormatDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CPipeFormatDlg)
	enum { IDD = IDD_PIPEFORMAT_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CPipeFormatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CPipeFormatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnStart();
	afx_msg void OnStart2();
	afx_msg void OnStart3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_PIPEFORMATDLG_H__062F21D6_7336_11D3_B79F_00001C1ABF19__INCLUDED_)
