// keyhook_sample.h : KEYHOOK_SAMPLE �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C���ł��B
//

#if !defined(AFX_KEYHOOK_SAMPLE_H__CE1C2D8D_D3B3_4C4A_BA00_7714470B6D84__INCLUDED_)
#define AFX_KEYHOOK_SAMPLE_H__CE1C2D8D_D3B3_4C4A_BA00_7714470B6D84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CKeyhookApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� keyhook_sample.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CKeyhookApp : public CWinApp
{
public:
	CKeyhookApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CKeyhookApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CKeyhookApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_KEYHOOK_SAMPLE_H__CE1C2D8D_D3B3_4C4A_BA00_7714470B6D84__INCLUDED_)
