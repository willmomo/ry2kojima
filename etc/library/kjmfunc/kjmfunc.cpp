//---------------------------------------------------------------------
// KJMFUNC.CPP -- ���[�e�B���e�B�I�֐�
//                (c) Ryouta Kojima, 2004
//---------------------------------------------------------------------

#include "kjmfunc.h"

namespace kjm
{
	//-----------------------------------------------------------------
	// [�T�v]
	// KTime�R���X�g���N�^
	//
	// [���l]
	// FILETIME�^���g�����R���X�g���N�^
	//-----------------------------------------------------------------

	KTime::KTime(FILETIME t)
	{
		FILETIME lft;
		FileTimeToLocalFileTime(&t, &lft);
		SYSTEMTIME st;
		FileTimeToSystemTime(&lft, &st);

		tm stm;
		memset(&stm, 0, sizeof(stm));
		stm.tm_year = st.wYear - 1900;
		stm.tm_mon = st.wMonth - 1;
		stm.tm_mday = st.wDay;
		stm.tm_hour = st.wHour;
		stm.tm_min = st.wMinute;
		stm.tm_sec = st.wSecond;

		m_time = mktime(&stm);
	}

	//-----------------------------------------------------------------
	// [�T�v]
	// �t�H���_���폜���܂��B
	//-----------------------------------------------------------------

	BOOL DeleteFolder(LPCTSTR lpszPathName)
	{
		BOOL bRet = TRUE;

		// �����p�̕�������쐬
		std::vector<TCHAR> szFileSpec(MAX_PATH);
		lstrcpy(szFileSpec.begin(), lpszPathName);
		PathAppend(szFileSpec.begin(), _T("*.*"));

		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile(szFileSpec.begin(), &data);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// "." or ".." �́A�X�L�b�v
				if (kjm::IsDots(data.cFileName))	continue;

				std::vector<TCHAR> szFullName(MAX_PATH);
				lstrcpy(szFullName.begin(), lpszPathName);
				PathAppend(szFullName.begin(), data.cFileName);

				if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					if ((bRet = DeleteFolder(szFullName.begin())) == FALSE) {
						break;
					}
				} else {
					if ((bRet = DeleteFile(szFullName.begin())) == FALSE) {
						break;
					}
				}
			} while (FindNextFile(hFind, &data));
			FindClose(hFind);

			// �Ō�Ƀt�H���_���폜(��ō폜�Ɏ��s���Ă���ƁA��������Ɏ��s)
			bRet = RemoveDirectory(lpszPathName);
		}

		return bRet;
	}

	//-----------------------------------------------------------------
	// [�T�v]
	// �Â��t�@�C��/�t�H���_���폜
	//
	// [���l]
	// lpszPathName�Ŏw�肵���t�H���_�̒��̃t�@�C��/�t�H���_�ŁA�ŏI�X
	// �V�������AnDays�����Â����̂��폜����B
	//
	// lpszPathName�ȉ��̃t�H���_���폜�ΏۂɂȂ����ꍇ�A���̉��̃t�@�C
	// ��/�t�H���_�̍ŏI�X�V�����́A�`�F�b�N����Ȃ��B
	//-----------------------------------------------------------------

	BOOL DeleteOldFile(LPCTSTR lpszPathName, int nDays)
	{
		BOOL bRet = TRUE;

		// �����p�̕�������쐬
		std::vector<TCHAR> szFileSpec(MAX_PATH);
		lstrcpy(szFileSpec.begin(), lpszPathName);
		PathAppend(szFileSpec.begin(), _T("*.*"));

		// ���ݓ������擾
		kjm::KTime now = kjm::KTime::GetCurrentTime();

		WIN32_FIND_DATA data;
		HANDLE hFind = FindFirstFile(szFileSpec.begin(), &data);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// "." or ".." �́A�X�L�b�v
				if (kjm::IsDots(data.cFileName))	continue;

				// �t�@�C���̓��t���w��������Â��Ƃ��͍폜����
				kjm::KTime ft(data.ftLastWriteTime);
				if ((now - ft).GetDays() > nDays) {
					std::vector<TCHAR> szFullName(MAX_PATH);
					lstrcpy(szFullName.begin(), lpszPathName);
					PathAppend(szFullName.begin(), data.cFileName);

					if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						if ((bRet = DeleteFolder(szFullName.begin())) == FALSE) {
							break;
						}
					} else {
						if ((bRet = DeleteFile(szFullName.begin())) == FALSE) {
							break;
						}
					}
				}
			} while (FindNextFile(hFind, &data));
			FindClose(hFind);
		}

		return bRet;
	}

	//-----------------------------------------------------------------
	// ���t��i�߂�(�߂�) ... time_t�^�o�[�W����
	//
	// tSrc		�ϊ��O���t
	// nDays	�i�߂�(�߂�)����
	// tDst		�ϊ�����t��ۑ�����o�b�t�@�ւ̃|�C���^
	//-----------------------------------------------------------------
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

	//-----------------------------------------------------------------
	// ���t��i�߂�(�߂�) ... tm�^�o�[�W����
	//
	// tSrc		�ϊ��O���t
	// nDays	�i�߂�(�߂�)����
	// tDst		�ϊ�����t��ۑ�����o�b�t�@�ւ̃|�C���^
	//-----------------------------------------------------------------
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

	//-----------------------------------------------------------------
	// ���t��i�߂�(�߂�) ... "YYYYMMDD"�o�[�W����
	//
	// tSrc		�ϊ��O���t
	//			�������"YYYYMMDD"�̌`�ɂȂ��Ă������
	// nDays	�i�߂�(�߂�)����
	// tDst		�ϊ�����t��ۑ�����o�b�t�@�ւ̃|�C���^
	//			�������"YYYYMMDD"�̌`�ɂȂ�܂�
	// nSize	nDst�̕�����(countof�}�N�����g�p���Ă�������)
	//-----------------------------------------------------------------
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

	//---------------------------------------------------------------
	// �����̉��s����菜��
	//
	// lpBuffer	���s����菜��������
	//			�o�b�t�@�̒��g�𒼐ڕύX���܂��B
	//---------------------------------------------------------------
	LPTSTR Chomp(LPTSTR lpBuffer)
	{
		for (int i = _tcslen(lpBuffer) - 1;
			i >= 0 && (lpBuffer[i] == TEXT('\r') || lpBuffer[i] == TEXT('\n')); i--) {

			lpBuffer[i] = TEXT('\0');
		}

		return lpBuffer;
	}

	//-----------------------------------------------------------------
	// �V�X�e���G���[�𕶎���ɂ���
	//
	// dwError	�V�X�e���G���[�R�[�h(GetLastError�̖߂�l)
	// lpBuffer	�G���[���b�Z�[�W��ۑ�����o�b�t�@�ւ̃|�C���^
	// nSize	lpBuffer�̕�����(countof�}�N�����g�p���Ă�������)
	//-----------------------------------------------------------------
	DWORD FormatMessageFromSystem(DWORD dwError, LPTSTR lpBuffer, DWORD nSize)
	{
		DWORD dwRet;
		
		dwRet = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwError, 0, lpBuffer, nSize, NULL);
		Chomp(lpBuffer);

		return dwRet;
	}

	//-----------------------------------------------------------------
	// [�T�v]
	// �w�肳�ꂽ������ "." or ".." ���𒲂ׂ�
	//
	// [�߂�l]
	// �w�肳�ꂽ������ "." or ".." �̂Ƃ� true�A�Ⴄ�Ƃ� false��߂�
	//-----------------------------------------------------------------

	bool IsDots(LPCTSTR psz)
	{
		return (lstrcmp(psz, _T(".")) == 0) || (lstrcmp(psz, _T("..")) == 0);
	}

	//-----------------------------------------------------------------
	// �r�b�g�t�B�[���h745�^�̓��t�𕶎���ɕϊ�����
	// 745�^�Ƃ́A{yyyy yyym mmmd dddd}�̃r�b�g�t�B�[���h�ŁA
	// �N�̊J�n����1910����B���A���̊J�n����1����B
	//
	// YMD745	���t������킷�r�b�g�t�B�[���h�l
	// lpBuffer	�ϊ���̕�������i�[����o�b�t�@(�Œ�11������)
	//			�ϊ���̕�����́A"YYYY/MM/DD"�̌`�ɂȂ�܂��B
	// nSize	lpBuffer�̕�����(countof�}�N�����g�p���Ă�������)
	//-----------------------------------------------------------------
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

	//---------------------------------------------------------------
	// �r�b�g�t�B�[���h745�^�̓��t��time_t�^�ɕϊ�����
	// 745�^�Ƃ́A{yyyy yyym mmmd dddd}�̃r�b�g�t�B�[���h�ŁA
	// �N�̊J�n����1910����B���A���̊J�n����1����B
	//
	// YMD745	���t������킷�r�b�g�t�B�[���h�l
	//
	// [�߂�l]
	// �G���[�̂Ƃ��́Atime_t�^�ɃL���X�g����-1��Ԃ��܂��B
	// (mktime�֐��̃G���[�ł�)
	//---------------------------------------------------------------
	time_t ToTimeYMD745(WORD YMD745)
	{
		struct _ymd745 {
			unsigned d:5;
			unsigned m:4;
			unsigned y:7;
		} *p = reinterpret_cast<struct _ymd745*>(&YMD745);

		tm tmBuf;

		memset(&tmBuf, 0, sizeof(tmBuf));
		tmBuf.tm_year = (p->y + 1910) - 1900;
		tmBuf.tm_mon = p->m - 1;
		tmBuf.tm_mday = p->d;

		return mktime(&tmBuf);
	}

	//-----------------------------------------------------------------
	// ps_tm�^�̓��t��time_t�^�ɕϊ�����
	// ps_tm�^�Ƃ́A{yyyy yyym mmmd dddd hhhh hnnn nnns ssss}�̃r�b�g
	// �t�B�[���h�ŁA�N�̊J�n����1910����B���A���̊J�n����1����B
	// �b�́A1/2�B
	//
	// pstm		���t������킷�r�b�g�t�B�[���h�l
	//
	// [�߂�l]
	// �G���[�̂Ƃ��́Atime_t�^�ɃL���X�g����-1��Ԃ��܂��B
	// (mktime�֐��̃G���[�ł�)
	//-----------------------------------------------------------------
	time_t ToTimePSTM(DWORD pstm)
	{
		struct _pstm {
			unsigned s:5;
			unsigned n:6;
			unsigned h:5;
			unsigned d:5;
			unsigned m:4;
			unsigned y:7;
		} *p = reinterpret_cast<struct _pstm*>(&pstm);

		tm tmBuf;

		memset(&tmBuf, 0, sizeof(tmBuf));
		tmBuf.tm_year = (p->y + 1910) - 1900;
		tmBuf.tm_mon = p->m - 1;
		tmBuf.tm_mday = p->d;
		tmBuf.tm_hour = p->h;
		tmBuf.tm_min = p->n;
		tmBuf.tm_sec = p->s * 2;

		return mktime(&tmBuf);
	}
};
