// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__D5CED52B_BB4B_4E0A_92E2_522536BC8F51__INCLUDED_)
#define AFX_STDAFX_H__D5CED52B_BB4B_4E0A_92E2_522536BC8F51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B

#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdisp.h>        // MFC �̃I�[�g���[�V���� �N���X
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxtempl.h>		// MFC �̃R���N�V���� �N���X
#include <afxmt.h>			// MFC �̃}���`�X���b�h�p �N���X

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "DriveInfo.h"
#include "kjmlib.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__D5CED52B_BB4B_4E0A_92E2_522536BC8F51__INCLUDED_)
