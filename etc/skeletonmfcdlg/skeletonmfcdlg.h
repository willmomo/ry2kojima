// skeletonmfcdlg.h : SKELETONMFCDLG �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_SKELETONMFCDLG_H__85E8EE74_1E09_4AB9_A9C8_88E9E4035200__INCLUDED_)
#define AFX_SKELETONMFCDLG_H__85E8EE74_1E09_4AB9_A9C8_88E9E4035200__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CSkeletonmfcdlgApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� skeletonmfcdlg.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CSkeletonmfcdlgApp : public CWinApp
{
public:
	CSkeletonmfcdlgApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CSkeletonmfcdlgApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CSkeletonmfcdlgApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SKELETONMFCDLG_H__85E8EE74_1E09_4AB9_A9C8_88E9E4035200__INCLUDED_)
