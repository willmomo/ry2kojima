// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__28953CF9_CAB5_4A48_A9CC_71F84A47EF74__INCLUDED_)
#define AFX_STDAFX_H__28953CF9_CAB5_4A48_A9CC_71F84A47EF74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN // Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂�

#pragma warning(disable : 4786)

#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <string.h>
#include <vector>

//------------------------------------------------------------
//	Define�錾
//------------------------------------------------------------
#define MAX_PATH	260

#define	MAX_TELOP	40		// �e���b�v �S�O���܂�
#define MAX_MESSAGE	40		// �҂��󂯃��b�Z�[�W �S�O���܂�
#define	MAX_MESLINE	12		// �҂��󂯃��b�Z�[�W�̍s��	�P�Q�s�܂�
#define	MAX_MOJI	26		// �҂��󂯃��b�Z�[�W�̍ő啶���� �S�p�P�R����
#define	MAX_CORNER	160		// �e���b�v�E���b�Z�[�W�̍ő�R�[�i�[	�P�U�O�R�[�i�[
#define	MAX_SET		20		// �e���b�v�E���b�Z�[�W�̐ݒ�	�Q�O���܂�

#define INI_FILE	"tlp_msg.xml"	// INI�t�@�C��

//------------------------------------------------------------
//	�\���̐錾
//------------------------------------------------------------
typedef struct {				//
	char telop[100];		// �e���b�v���b�Z�[�W
} TELOP_MES_T, TELOP_MES_ALL[40];
	
typedef struct {
	int bg_R;				// �w�i�F �q			
	int bg_G;				// �w�i�F �f
	int bg_B;				// �w�i�F �a
	int title_R;			// �^�C�g�� �q
	int title_G;			// �^�C�g�� �f
	int title_B;			// �^�C�g�� �a
	char title[26];			// �^�C�g��
	struct {
		int font_R;			// ���b�Z�[�W �q
		int font_G;			// ���b�Z�[�W �f
		int font_B;			// ���b�Z�[�W �a
		char text[26];		// ���b�Z�[�W�i�S�p�P�R�����܂Łj
	} msg_line[12];
} MESSAGE_T, MESSAGE_ALL[40];
	
typedef struct {
	char day[10];				// ��
	char time[10];				// ���ԁi�I���̎��͂����ɓ���j
	char starttime[10];			// �J�n����
	char endtime[10];			// �I������
	struct {
		int rdb_begin;			// �J�n��ԍ�
		int rdb_end;			// �I����ԍ�
		int telop_id;			// �e���b�v�h�c
		int telop_id2;			// �`�r���M�p�e���b�v�h�c
		int msg_id;				// ���b�Z�[�W�h�c
		int msg_id2;			// �`�r���M�p���b�Z�[�W�h�c
	} corners[160];
} SET_MES_TLP_T, SET_MES_TLP_ALL[20];

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__28953CF9_CAB5_4A48_A9CC_71F84A47EF74__INCLUDED_)
