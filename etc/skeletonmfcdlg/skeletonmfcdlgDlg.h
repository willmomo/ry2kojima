// skeletonmfcdlgDlg.h : �w�b�_�[ �t�@�C��
//

#if !defined(AFX_SKELETONMFCDLGDLG_H__73CB1297_5EC8_4FAF_B2B2_E7814B7CEC93__INCLUDED_)
#define AFX_SKELETONMFCDLGDLG_H__73CB1297_5EC8_4FAF_B2B2_E7814B7CEC93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSkeletonmfcdlgDlg �_�C�A���O

class CSkeletonmfcdlgDlg : public CDialog
{
// �\�z
public:
	CSkeletonmfcdlgDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CSkeletonmfcdlgDlg)
	enum { IDD = IDD_SKELETONMFCDLG_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSkeletonmfcdlgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSkeletonmfcdlgDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SKELETONMFCDLGDLG_H__73CB1297_5EC8_4FAF_B2B2_E7814B7CEC93__INCLUDED_)
