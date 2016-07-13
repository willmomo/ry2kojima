/*-------------------------------------------------------------------
    KJMFUNC.CPP -- ���[�e�B���e�B�I�֐�
	               (c) Ryouta Kojima, 2004
  -------------------------------------------------------------------*/

#include "kjmfunc.h"

namespace kjm
{
	/*---------------------------------------------------------------
	    ���t��i�߂�(�߂�) ... time_t�^�o�[�W����
	    
	    tSrc    �ϊ��O���t
	    nDays   �i�߂�(�߂�)����
	    tDst    �ϊ�����t��ۑ�����o�b�t�@�ւ̃|�C���^
	-----------------------------------------------------------------*/
	BOOL DateAdd(time_t tSrc, int nDays, time_t* tDst)
	{
		// time_t�^�̓��t��tm�^�ɕϊ�
		tm* ptm = localtime(&tSrc);
		if (ptm == NULL) {
			return FALSE;
		}

		tm tmBuf = *ptm;
		tmBuf.tm_mday += nDays;		// nDays�i�߂�

		*tDst = mktime(&tmBuf);		// ���t�����

		return (*tDst == (time_t)-1) ? FALSE : TRUE;
	}

	/*---------------------------------------------------------------
	    ���t��i�߂�(�߂�) ... tm�^�o�[�W����
	    
	    tSrc    �ϊ��O���t
	    nDays   �i�߂�(�߂�)����
	    tDst    �ϊ�����t��ۑ�����o�b�t�@�ւ̃|�C���^
	-----------------------------------------------------------------*/
	BOOL DateAdd(const tm* tSrc, int nDays, tm* tDst)
	{
		tm tmBuf = *tSrc;
		tmBuf.tm_mday += nDays;		// nDays�i�߂�

		time_t t = mktime(&tmBuf);		// ���t�����
		if (t == (time_t)-1) {
			return FALSE;
		}

		tm* ptm = localtime(&t);
		if (ptm == NULL) {
			return FALSE;
		}

		*tDst = *ptm;

		return TRUE;
	}

	/*---------------------------------------------------------------
	    ���t��i�߂�(�߂�) ... "YYYYMMDD"�o�[�W����

	    tSrc    �ϊ��O���t
	            �������"YYYYMMDD"�̌`�ɂȂ��Ă������
	    nDays   �i�߂�(�߂�)����
	    tDst    �ϊ�����t��ۑ�����o�b�t�@�ւ̃|�C���^
	            �������"YYYYMMDD"�̌`�ɂȂ�܂�
	    nSize   nDst�̕�����(countof�}�N�����g�p���Ă�������)
	-----------------------------------------------------------------*/
	BOOL DateAdd(LPCTSTR tSrc, int nDays, LPTSTR tDst, DWORD nSize)
	{
		int y, m, d;
		tm tmBuf;
		memset(&tmBuf, 0, sizeof(tmBuf));
		_stscanf(tSrc, TEXT("%04d%02d%02d"), &y, &m, &d);
		tmBuf.tm_year = y - 1900;
		tmBuf.tm_mon = m - 1;
		tmBuf.tm_mday = d + nDays;

		time_t t = mktime(&tmBuf);		// ���t�����
		if (t == (time_t)-1) {
			return FALSE;
		}

		tm* ptm = localtime(&t);
		if (ptm == NULL) {
			return FALSE;
		}

		_sntprintf(tDst, nSize, TEXT("%04d%02d%02d"), ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday);

		return TRUE;
	}

	/*---------------------------------------------------------------
	    �����̉��s����菜��
	    
	    lpBuffer    ���s����菜��������
	                �o�b�t�@�̒��g�𒼐ڕύX���܂�
	  ---------------------------------------------------------------*/
	LPTSTR Chomp(LPTSTR lpBuffer)
	{
		for (int i = _tcslen(lpBuffer) - 1;
			i >= 0 && (lpBuffer[i] == TEXT('\r') || lpBuffer[i] == TEXT('\n')); i--) {

			lpBuffer[i] = TEXT('\0');
		}

		return lpBuffer;
	}

	/*---------------------------------------------------------------
		�V�X�e���G���[�𕶎���ɂ���
		
		dwError    �V�X�e���G���[�R�[�h(GetLastError�̖߂�l)
		lpBuffer   �G���[���b�Z�[�W��ۑ�����o�b�t�@�ւ̃|�C���^
		nSize      lpBuffer�̕�����(countof�}�N�����g�p���Ă�������)
	  ---------------------------------------------------------------*/
	DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize)
	{
		DWORD dwRet;
		
		dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, lpBuffer, nSize, NULL);
		Chomp(lpBuffer);

		return dwRet;
	}

	/*---------------------------------------------------------------
		�r�b�g�t�B�[���h745�^�̓��t�𕶎���ɕϊ�����
		745�^�Ƃ́A{yyyy yyym mmmd dddd}�̃r�b�g�t�B�[���h�ŁA
		�N�̊J�n����1910����B���A���̊J�n����1����B
		
		YMD745    ���t������킷�r�b�g�t�B�[���h�l
		lpBuffer  �ϊ���̕�������i�[����o�b�t�@(�Œ�11������)
		          �ϊ���̕�����́A"YYYY/MM/DD"�̌`�ɂȂ�܂�
		nSize     lpBuffer�̕�����(countof�}�N�����g�p���Ă�������)
	  ---------------------------------------------------------------*/
	LPTSTR ToStringYMD745(WORD YMD745, LPTSTR lpBuffer, DWORD nSize)
	{
		struct _ymd745 {
			unsigned d:5;
			unsigned m:4;
			unsigned y:7;
		} *p = reinterpret_cast<struct _ymd745*>(&YMD745);

		_sntprintf(lpBuffer, nSize, TEXT("%04d/%02d/%02d"), p->y + 1910, p->m, p->d);

		return lpBuffer;
	}
};
