// keyhook.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#define STRICT
#include <windows.h>
#include "keyhook.h"

// �������́I
// �ǂ��ɂ��A�悭�킩���̂����Ag_hwndTgt��share�ɂ��Ă����Ȃ��ƁA���b�Z�[�W���͂��Ȃ�
// �������A��������Ƃ���DLL���N�����Č�Ƀt�b�N�����v���O���������L���ɂȂ��E�E�E
//
// KeyboardProc�����̂Ƃ��A�N�e�B�u�ȃ�������ԂŌĂ΂��Ƃ�����A�\��������̂����A
// ��������A���̒��Ŏg�p���Ă���O���[�o���ϐ��S�Ă��Ashare�ɂ��Ȃ��Ƃ����Ȃ��Ǝv���A
// ���������ۂ́Ag_hwndTgt����share����Γ����E�E�E�Ȃ��B�N�������āH
//
// �Ώ��@�Ƃ��ẮAg_hhk��share���āA��߈ڍs�̃v���Z�X����̃t�b�N�����s�����邩
// g_hwndTgt�𕡐����Ă�悤�ɉ��ǂ��邩�E�E�E
// ����́Asample�Ȃ̂�g_hhk��share���āA��l�ڂ̃v���Z�X�̂݃t�b�N�\�Ƃ����B

#pragma data_seg(".sharedata")
HWND g_hwndTgt = NULL;	// ���b�Z�[�W���M��E�B���h�E
HHOOK g_hhk = NULL;
#pragma data_seg()

HINSTANCE g_hinstDLL;
UINT WM_KEYHOOK;

//---------------------------------------------------------------------
// �T�v
//	�L�[�{�[�h�t�b�N�v���V�[�W��
//
// ����
//	�}�j���A���Q�Ƃ�
//
// �߂�l
//	��ɁACallNextHookEx�̖߂�l
//---------------------------------------------------------------------

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0) {
		if (code == HC_ACTION) {
			BYTE keyState[256];
			GetKeyboardState(keyState);

//			lParam = 
//				((keyState[VK_CONTROL] & 0x80) ? 0x00ff0000 : 0x00cc0000) |
//				((keyState[VK_MENU] & 0x80) ? 0x0000ff00 : 0x0000cc00) |
//				((keyState['P'] & 0x80) ? 0x000000ff : 0x000000cc);

			PostMessage(g_hwndTgt, WM_KEYHOOK, wParam, lParam);
		}
	}

	return CallNextHookEx(g_hhk, code, wParam, lParam);
}

//---------------------------------------------------------------------
// �T�v
//	�L�[�{�[�h�t�b�N�̊J�n
//
// ����
//	hwnd	hook�������b�Z�[�W�̑��M��
//
// �߂�l
//	TRUE:	����I��
//	FALSE:	�ُ�I��
//---------------------------------------------------------------------

BOOL KEYHOOK_API SetHook(HWND hwnd)
{
	BOOL ret = FALSE;

	// ��hook�̂Ƃ�����hook
	if (!g_hhk) {
		g_hwndTgt = hwnd;
		g_hhk = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hinstDLL, 0);
		if (g_hhk) {
			ret = TRUE;
		}
	}

	return ret;
}

//---------------------------------------------------------------------
// �T�v
//	�L�[�{�[�h�t�b�N�̏I��
//
// ����
//	�Ȃ�
//
// �߂�l
//	�Ȃ�
//---------------------------------------------------------------------

void KEYHOOK_API FreeHook()
{
	// hook�ς݂̎�����unhook
	if (g_hhk) {
		UnhookWindowsHookEx(g_hhk);
		g_hhk = NULL;
	}
}

//---------------------------------------------------------------------
// ����
//	hinstDLL	DLL�̃n���h��
//	fdwReason	DLL�̌Ăяo�����R
//	lpvReserved	reserved�����Ă�!
//
// �߂�l
//	���TRUE
//---------------------------------------------------------------------

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID plvReserved)
{
	// ����Ăяo���ŁA�n���h���̕ۑ��Ƌ��L���郁�b�Z�[�W��o�^
	if (fdwReason == DLL_PROCESS_ATTACH) {
		g_hinstDLL = hinstDLL;
		WM_KEYHOOK = RegisterWindowMessage(KEYHOOK_MESSAGE);
	}

	return TRUE;
}
