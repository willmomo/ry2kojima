// SHBrowseForFolder.h : SHBROWSEFORFOLDER �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CSHBrowseForFolderApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� SHBrowseForFolder.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSHBrowseForFolderApp : public CWinApp
{
public:
	CSHBrowseForFolderApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��𐶐����I�[�o�[���C�h���܂��B
	//{{AFX_VIRTUAL(CSHBrowseForFolderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CSHBrowseForFolderApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

