// SHBrowseForFolderDlg.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CSHBrowseForFolderDlg dialog

class CSHBrowseForFolderDlg : public CDialog
{
// �\�z
public:
	CSHBrowseForFolderDlg(CWnd* pParent = NULL);	// �W���̃R���X�g���N�^

// Dialog Data
	//{{AFX_DATA(CSHBrowseForFolderDlg)
	enum { IDD = IDD_SHBROWSEFORFOLDER_DIALOG };
		// ����: ���̈ʒu�� ClassWizard �ɂ���ăf�[�^ �����o���ǉ�����܂��B
	//}}AFX_DATA

	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CSHBrowseForFolderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �̃T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:
	HICON m_hIcon;

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CSHBrowseForFolderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSHBrowseForFolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

