//---------------------------------------------------------------------
// �T�v
//	AngelHiLow���ʂ̃w�b�_
//
// �쐬��
//	HAL����
//
// ���l
//	AngelHiLow.DLL�𗘗p����l�͂��̃t�@�C�����C���N���[�h
//---------------------------------------------------------------------

#pragma once

#if !defined(__ANGELHILOW_H)
#define __ANGELHILOW_H

#include <stdio.h>
#include <assert.h>

#if defined(ANGELHILOW_EXPORTS)
#define ANGELHILOW_API	__declspec(dllexport) __stdcall
#else
#define ANGELHILOW_API	__declspec(dllimport) __stdcall
#endif

#if defined(_DEBUG)
#define AHL_W(N)	(assert(sizeof(N) == sizeof(short)) , ((((N) & 0x00ff) << 8) | (((N) & 0xff00) >> 8)))
#define AHL_L(N)	(assert(sizeof(N) == sizeof(long)) , ((((N) & 0x000000ff) << 24) | (((N) & 0x0000ff00) << 8) | (((N) & 0x00ff0000) >> 8) | (((N) & 0xff000000) >> 24)))
#else
#define AHL_W(N)	((((N) & 0x00ff) << 8) | (((N) & 0xff00) >> 8))
#define AHL_L(N)	((((N) & 0x000000ff) << 24) | (((N) & 0x0000ff00) << 8) | (((N) & 0x00ff0000) >> 8) | (((N) & 0xff000000) >> 24))
#endif

double AHL_D(double src);

// AngelHiLow�p�I�v�V����

#define HLOPT_BODY	0	// �ʏ�̕ϊ�
#define HLOPT_HEAD	1	// �����̃C���f�b�N�X�ϊ��ȂǂŎg�p
#define HLOPT_FULL	2	// �擪���R�[�h�{�ʏ탌�R�[�h�S�̂�ϊ�����ꍇ(fno6001�Ȃ�)

// ConvertCSV�p�I�v�V����

#define CVOPT_NORMAL			0x0000	// �ʏ��CSV�ϊ�
//#define CVOPT_ROW				0x0001	// �\���̐��ϊ�(���Ή�)

#define CVOPT_NOHEADER			0x0100	// ��w�b�_���o�͂��܂���
#define CVOPT_NORECNO			0x0200	// ��ԍ����o�͂��܂���
#define CVOPT_NOCOMMENT			0x0400	// �R�����g�s���o�͂��܂���
#define CVOPT_ALLOCATEBUFFER	0x0800	// �֐����o�b�t�@���m�ۂ��܂�
										// (pdst�ɂ́A�|�C���^�ւ̃A�h���X��n���܂�)
										// (�o�b�t�@��GlobalFree�ŊJ�����Ă�������)

// �v���g�^�C�v

BOOL ANGELHILOW_API AngelHiLow(int fno, const void* psrc, void* pdst, int rec, int opt);
BOOL ANGELHILOW_API ConvertCSV(int fno, const void* psrc, const char* pfname, int rec, int opt);

#endif
