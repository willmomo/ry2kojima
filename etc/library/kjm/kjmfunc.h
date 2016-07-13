/*-------------------------------------------------------------------
    KJMFUNC.H -- ���[�e�B���e�B�I�֐�
                 (c) Ryouta Kojima, 2004
  -------------------------------------------------------------------*/

#pragma once

// �K�v�ȃw�b�_�t�@�C��

#include <windows.h>
#include <tchar.h>
#include <cstdio>
#include <ctime>

// �}�N��

#if !defined(countof)
#define countof(a)	(sizeof((a))/sizeof((a)[0]))
#endif

// �֐��v���g�^�C�v

namespace kjm
{
	BOOL DateAdd(time_t tSrc, int nDays, time_t* tDst);
	BOOL DateAdd(const tm* tSrc, int nDays, tm* tDst);
	BOOL DateAdd(LPCTSTR tSrc, int nDays, LPTSTR tDst, DWORD nSize);
	LPTSTR Chomp(LPTSTR lpBuffer);
	DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize);
	LPTSTR ToStringYMD745(WORD YMD745, LPTSTR lpBuffer, DWORD nSize);
};
