// ReadCheck.h : READCHECK �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_READCHECK_H__CC3CFCC6_3954_4529_BD1B_C32824E3525E__INCLUDED_)
#define AFX_READCHECK_H__CC3CFCC6_3954_4529_BD1B_C32824E3525E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CReadCheckApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� ReadCheck.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CReadCheckApp : public CWinApp
{
public:
	CReadCheckApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CReadCheckApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CReadCheckApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_READCHECK_H__CC3CFCC6_3954_4529_BD1B_C32824E3525E__INCLUDED_)
