#include "hsutil.h"

// HsFindWindowEnumProc�ɓn���p�����[�^�p�\����

typedef struct {
	LPCTSTR lpClassName;	// HsFindWindow�ɓn���ꂽ����
	LPCTSTR lpWindowName;	//  .. ���� ..
	DWORD dwFlags;			//  .. ���� ..
	HWND hwnd;				// �}�b�`�����E�B���h�E�ւ̃n���h��
} HSFW_ENUM_PARAM, *PHSFW_ENUM_PARAM;

#define MAX_CLASS_NAME	2048
#define MAX_WINDOW_NAME	2048

//---------------------------------------------------------------------
// [�T�v]
// HsFindWindow����Ăяo�����񋓊֐�
//
// [����]
// �p�����[�^�Ŏw�肳�ꂽ�t���O�ɏ]���āA��������r�B
// �������s��v�Ɣ��f�����Ƃ��́A�񋓂��p���B
//---------------------------------------------------------------------

static BOOL CALLBACK HsFindWindowEnumProc(HWND hwnd, LPARAM lParam)
{
	PHSFW_ENUM_PARAM pParam = (PHSFW_ENUM_PARAM)lParam;

	// �N���X�����w�肳��Ă���Ƃ��́A�t���O�ɏ]���Ĕ�r
	if (pParam->lpClassName) {
		TCHAR szClassName[MAX_CLASS_NAME];
		GetClassName(hwnd, szClassName, MAX_CLASS_NAME);

		// �������낢��Ȕ�r���@�ɑΉ�(���邩��)
		if (pParam->dwFlags == HSFW_FLAG_MATCHHEAD) {
			if (_tcsnicmp(pParam->lpClassName, szClassName, _tcslen(pParam->lpClassName)) != 0) {
				return TRUE;
			}
		}
	}

	// �E�B���h�E�����w�肳��Ă���Ƃ��́A�t���O�ɏ]���Ĕ�r
	if (pParam->lpWindowName) {
		TCHAR szWindowName[MAX_WINDOW_NAME];
		GetWindowText(hwnd, szWindowName, MAX_WINDOW_NAME);

		// �������낢��Ȕ�r���@�ɑΉ�(���邩��)
		if (pParam->dwFlags == HSFW_FLAG_MATCHHEAD) {
			if (_tcsnicmp(pParam->lpWindowName, szWindowName, _tcslen(pParam->lpWindowName)) != 0) {
				return TRUE;
			}
		}
	}

	pParam->hwnd = hwnd;	// ��v�����E�B���h�E�n���h����߂�

	return FALSE;
}

//---------------------------------------------------------------------
// [�T�v]
// ������̕����}�b�`�Ή���FindWindow
//
// [����]
// lpClassName	: FindWindow�Q��
// lpWindowName	: FindWindow�Q��
// dwFlags		: HSFW_FLAG_SAMEFINDWINDOW	: ���S��v�Ō���
//				: HSFW_FLAG_MATCHHEAD		: �擪������v�Ō���
//
// [�߂�l]
// �E�B���h�E�����������Ƃ��́A�E�B���h�E�n���h����߂��B
// �E�B���h�E��������Ȃ������Ƃ��́ANULL��߂��B
//
// [����]
// FindWindow�֐���������̊��S��v(�������Aignore-case)�Ȃ̂ŁA
// ���I�ɕω�����E�B���h�E��T�����߂ɐ擪������v�Ɋg���B
//---------------------------------------------------------------------

HSUTIL_API HWND WINAPI HsFindWindow(LPCTSTR lpClassName, LPCTSTR lpWindowName, DWORD dwFlags)
{
	// �t���O���w�肳��Ȃ��Ƃ��́AFindWindow�Ɠ���
	if (dwFlags == 0) {
		return FindWindow(lpClassName, lpWindowName);
	}

	// �t���O���w�肳��Ă���Ƃ��́AEnumWindow���g���Ĉ�v����E�B���h�E��T��
	HSFW_ENUM_PARAM param = {lpClassName, lpWindowName, dwFlags, NULL};
	EnumWindows(HsFindWindowEnumProc, (LPARAM)&param);

	return param.hwnd;
}

//---------------------------------------------------------------------
// [�T�v]
// DLL entry point
//---------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}
