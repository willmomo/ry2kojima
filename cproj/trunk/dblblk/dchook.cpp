///////////////////////////////////////////////////////////////////////
// dchook.cpp

#include <windows.h>
#include "dchook.h"

//////// �O���[�o���ϐ� /////////

HINSTANCE g_hinst = NULL;
HHOOK g_hhk = NULL;

///////////////////////////////////////////////////////////////////////
// �}�E�X�t�b�N�v���V�[�W��

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0)
	{
		LRESULT ret = CallNextHookEx(g_hhk, nCode, wParam, lParam);

		// WM_LBUTTONDBLCLK �����́A�������ɏ����������Ƃɂ���
		// ����ŁA�A�v���P�[�V�����Ƀ_�u���N���b�N�����������Ȃ�

		return (wParam == WM_LBUTTONDBLCLK) ? 1 : ret;
	}

	return CallNextHookEx(g_hhk, nCode, wParam, lParam);
}

///////////////////////////////////////////////////////////////////////
// �}�E�X�t�b�N�J�n

DCHOOK_API void DcHook()
{
	if (g_hhk == NULL)
	{
		g_hhk = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hinst, 0);
	}
}

///////////////////////////////////////////////////////////////////////
// �}�E�X�t�b�N�I��

DCHOOK_API void DcUnhook()
{
	if (g_hhk)
	{
		UnhookWindowsHookEx(g_hhk);
		g_hhk = NULL;
	}
}

///////////////////////////////////////////////////////////////////////
// �G���g���[�|�C���g

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		g_hinst = hinstDLL;
	}

	return TRUE;
}
