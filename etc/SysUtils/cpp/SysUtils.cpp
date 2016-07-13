#include <windows.h>
#include <shlwapi.h>
#include <string>
#include "SysUtils.h"

//---------------------------------------------------------------------
// �����Ŏg�p����l�Ȃ�
//---------------------------------------------------------------------

#define LWA_ALPHA		2

typedef BOOL (WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

//---------------------------------------------------------------------
// VC6�ł́A�P���Ɏg���Ȃ��֐��ł��B
//---------------------------------------------------------------------

static BOOL SetLayeredWindowAttributes(HWND hwnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags)
{
	BOOL ret = FALSE;

	HMODULE hlibUser32 = LoadLibrary("user32.dll");
	if (hlibUser32) {
		SLWA pfunc = (SLWA)GetProcAddress(hlibUser32, "SetLayeredWindowAttributes");
		if (pfunc) {
			ret = pfunc(hwnd, crKey, bAlpha, dwFlags);
		}
		FreeLibrary(hlibUser32);
	}

	return ret;
}

//---------------------------------------------------------------------
// [�T�v]
// �w�肵���E�B���h�E���A���������T�|�[�g���邩�`�F�b�N����
//
// [����]
// hwnd          �E�B���h�E�ւ̃n���h��
//
// [�߂�l]
// �T�|�[�g����      True
// �T�|�[�g���Ȃ�    False
//
// [��O]
// �G���[�����������ꍇ�AWin32�̃G���[�R�[�h�Ń����^�C���G���[�𔭐�������B
// �G���[�R�[�h�́AWINERROR.H�𒲂ׂĂ��������B
//
// [����]
// �������@�\���T�|�[�g���Ă��邩�ǂ����`�F�b�N���܂��B
//---------------------------------------------------------------------

bool IsLayered(HWND hwnd)
{
	// �E�B���h�E�X�^�C�����擾����
	LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
	if (exStyle == 0) {
		throw GetLastError();
	}

	// �������T�|�[�g���́Atrue, ���T�|�[�g����false��Ԃ�
	return ((exStyle & WS_EX_LAYERED) == WS_EX_LAYERED) ? true : false;
}

//---------------------------------------------------------------------
// [�T�v]
// �w�肵���E�B���h�E�́A�������T�|�[�g��ON/OFF���܂��B
//
// [����]
// hwnd      �E�B���h�E�ւ̃n���h��
// bLayered  �������T�|�[�g(True:ON False:OFF)
//
// [�߂�l]
// �ݒ�ł���        0
// �ݒ�ł��Ȃ�����  -1
// ���̑��̃G���[    Win32�̃G���[�R�[�h
//
// [����]
// �������@�\��ON/OFF���s���B
//---------------------------------------------------------------------

int SetLayered(HWND hWnd, bool bLayered)
{
	// �E�B���h�E�X�^�C�����擾����
	long exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
	if (exStyle == 0) {
		return GetLastError();
	}

    // �����ɉ����Ĕ������T�|�[�g�̃t���O��ω�������
	if (bLayered) {
		exStyle |= WS_EX_LAYERED;
	} else {
		exStyle &= ~WS_EX_LAYERED;
	}
    
    // �E�B���h�E�X�^�C����ݒ肷��B
	SetLastError(0);
	exStyle = SetWindowLong(hWnd, GWL_EXSTYLE, exStyle);
	DWORD err = GetLastError();
	if (exStyle == 0 && GetLastError() != 0) {
		return err;
	}

    try
	{
		// �������̃T�|�[�g��Ԃ��ݒ�ł������ǂ�����߂�
		return (IsLayered(hWnd) == bLayered) ? 0 : -1;
	}
	catch(DWORD e)
	{
		return e;
	}
}

//---------------------------------------------------------------------
// [�T�v]
// �E�B���h�E��s����������
//
// [����]
// hwnd      �E�B���h�E�ւ̃n���h��
// dOpacity  �s�����̊���(0.0�`1.0)
//
// [�߂�l]
// ����I��          0
// �G���[�I��        -1
// ���̑��̃G���[    Win32�G���[�R�[�h
//
// [����]
// �E�B���h�E��s���������܂��B
//---------------------------------------------------------------------

int SetOpacity(HWND hwnd, double dOpacity)
{
    // ���������T�|�[�g�E�B���h�E�𔼓����ɂ��悤�Ƃ����Ƃ��́A�t���O���Z�b�g����
    // �t���O���Z�b�g�ł��Ȃ��Ƃ��́A�G���[���^�[��
	try
	{
		if (IsLayered(hwnd) == false) {
			int ret = SetLayered(hwnd, true);
			if (ret != 0) {
				return ret;
			}
		}
	}
	catch(DWORD e)
	{
		return e;
	}
    
    // ��������(�s������)����
    BOOL ret = SetLayeredWindowAttributes(hwnd, 0, 255 * dOpacity, LWA_ALPHA);
	if (ret == FALSE) {
		return GetLastError();
	}

	return 0;
}

//---------------------------------------------------------------------
// [�T�v]
// �w��t�H���_�ȉ��ɂ���t�@�C�������ׂč폜����B
//
// [����]
// strFolder     ���̃t�H���_��艺�ɂ���t�@�C�����폜����
// strExcept     ���̖��O�Ɉ�v����t�@�C��/�t�H���_�͍폜���Ȃ��B
//
// [�߂�l]
// ����I��  0
// �G���[    0�ȊO�BError�֐��ŃG���[��������擾�\�B
//
// [����]
// strFolder�ȉ��ɂ���t�@�C�����AstrExcept�������Ă��ׂč폜����B
// strExcept�́A�P���ȕ�����B�t�H���_���ƈ�v����΁A�t�H���_��
// �ȉ����ׂĂ��폜�ΏۊO�ɂ��܂��B
//
// [���l]
// �Ԉ���āAc:\�Ƃ��w�肵����A������ρB
//---------------------------------------------------------------------

int RemoveSubFolders(const char *strFolder, const char *strExcept)
{
	int nRet = 0;

	char strPathName[MAX_PATH];		// strFolder + '\\'
	char strFileName[MAX_PATH];		// strPathName + '*.*'

	lstrcpyn(strPathName, strFolder, MAX_PATH);
	if (PathAddBackslash(strPathName) == NULL) {
		return -1;
	}

	lstrcpyn(strFileName, strPathName, MAX_PATH);
	if (PathAppend(strFileName, "*.*") == FALSE) {
		return -2;
	}

	// �t�@�C�������J�n
	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile(strFileName, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		std::string strFrom;
		BOOL ret;
		do {
			if (lstrcmp(data.cFileName, ".") && lstrcmp(data.cFileName, "..")) {
				if (lstrcmpi(data.cFileName, strExcept)) {
					strFrom += strPathName;
					strFrom += data.cFileName;
					strFrom += '\0';
				}
			}

			ret = FindNextFile(hFind, &data);
		} while (ret);
		FindClose(hFind);

		// �폜����t�@�C�����������Ƃ��́A�폜�I
		if (!strFrom.empty()) {
			SHFILEOPSTRUCT fo;

			memset(&fo, 0, sizeof(fo));
			fo.wFunc = FO_DELETE;
			fo.pFrom = strFrom.c_str();
			fo.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;

			nRet = SHFileOperation(&fo);
		}
	} else {
		nRet = GetLastError();
	}

	return nRet;
}

//---------------------------------------------------------------------
// [�T�v]
// �������u������
//
// [����]
// pszSrc			�������u������O�̕�����
// pszFind			pszSrc�̒�����T��������
// pszReplaceWith	pszFind��u�������镶����
//
// [�߂�l]
// �u����̕�����(std::string)�B
// pszSrc����pszFind��������Ȃ��Ƃ��́Astd::string(pszSrc)��Ԃ�
// �������G���[�̂Ƃ��́Aempty() �������Ԃ�
//
// [����]
// VB��Replace�֐����C�N�Ȋ֐��B�ׂ�������͂ł��Ȃ����A�P���ȕ�����
// �u�����T�|�[�g
//---------------------------------------------------------------------

std::string ReplaceStr(const char* pszSrc, const char* pszFind, const char* pszReplaceWith, int nStart/* = 1*/, int nCount/* = -1*/)
{
	// �������ُ�̂Ƃ��́A�󕶎����Ԃ�
	if (!pszSrc || !pszFind || !pszReplaceWith) {
		return std::string();
	}

	// ��������, ����������, �u���������wstring��
	std::wstring src = StrToWStr(pszSrc);
	std::wstring fnd = StrToWStr(pszFind);
	std::wstring rep = StrToWStr(pszReplaceWith);

	// �����������T���Č�����΁A�u��
	std::wstring::size_type n = (nStart < 1) ? 0 : nStart - 1;

	int nCntRep = 0;	// �u��������
	while ((n = src.find(fnd, n)) != std::wstring::npos) {
		src.replace(n, fnd.length(), rep);
		n += rep.length();

		// ���u�������Ǘ�����
		if (nCount > 0 && ++nCntRep >= nCount) {
			break;
		}
	}

	// ���ʂ�߂�
	return WStrToStr(src.c_str());
}

//---------------------------------------------------------------------
// [�T�v]
// ASCII�������Unicode������ɕϊ�
//
// [����]
// pszSrc	�ϊ���ASCII������ւ̃|�C���^
//
// [�߂�l]
// �ϊ����Unicode������ւ̃|�C���^�B�ϊ��ł��Ȃ��ꍇ�́A�󕶎����Ԃ��B
//---------------------------------------------------------------------

std::wstring StrToWStr(LPCSTR pszSrc)
{
	std::wstring wstr;

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(\0�܂�))���擾
	int nSizeNeeded = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nSizeNeeded == 0) {
		return wstr;
	}

	// �o�b�t�@���m��
	LPWSTR pwsz = (LPWSTR)malloc(sizeof(WCHAR) * nSizeNeeded);
	if (pwsz == NULL) {
		return wstr;
	}

	// �ϊ�
	int ret = MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwsz, nSizeNeeded);
	if (ret == 0) {
		free(pwsz);
		return wstr;
	}

	wstr = pwsz;
	free(pwsz);

	return wstr;
}

//---------------------------------------------------------------------
// [�T�v]
// Unicode�������ASCII������ɕϊ�
//
// [����]
// pwszSrc	�ϊ���Unicode������ւ̃|�C���^
//
// [�߂�l]
// �ϊ����ASCII������ւ̃|�C���^�B�ϊ��ł��Ȃ��ꍇ�́A�󕶎����Ԃ��B
//---------------------------------------------------------------------

std::string WStrToStr(LPCWSTR pwszSrc)
{
	std::string str;

	// �ϊ��ɕK�v�ȃo�b�t�@�T�C�Y(������(\0�܂�))���擾
	int nSizeNeeded = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nSizeNeeded == 0) {
		return str;
	}

	// �o�b�t�@���m��
	LPSTR psz = (LPSTR)malloc(sizeof(CHAR) * nSizeNeeded);
	if (psz == NULL) {
		return str;
	}

	// �ϊ�
	int ret = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, psz, nSizeNeeded, NULL, NULL);
	if (ret == 0) {
		free(psz);
		return str;
	}

	str = psz;
	free(psz);

	return str;
}
