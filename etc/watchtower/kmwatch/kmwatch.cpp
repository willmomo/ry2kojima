// kmwatch.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#define _WIN32_WINNT	0x0400

#define STRICT
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "kmwatch.h"

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
HHOOK g_hhkKeybd = NULL;
HHOOK g_hhkMouse = NULL;
#pragma data_seg()

HINSTANCE g_hinstDLL;
UINT WM_KMWATCH;
UINT WM_KMWATCH_MOUSE;

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

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0) {
		if (code == HC_ACTION) {
			switch (wParam) {
			case WM_NCMOUSEMOVE:	// 0x00A0
			case WM_MOUSEMOVE:		// 0x0200
				// �}�E�X�̈ړ��͖������܂��B
				break;

			default:
				// ���������邱�Ƃ��m�F�ł��Ă���}�E�X�C�x���g
				{
					MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;

					FILE* fp = fopen("mouse.log", "a+");
					if (fp) {
						fprintf(fp, "[%04X] wParam = 0x%08X {pt = %u, mouseData = %u, flags = %08X, time = %u, dwExtraInfo = %u}\n",
							GetCurrentProcessId(),
							wParam,
							p->pt, p->mouseData, p->flags, p->time, p->dwExtraInfo);
						fclose(fp);
					}
				}
				
				PostMessage(g_hwndTgt, WM_KMWATCH_MOUSE, wParam, lParam);
				break;
			}
		}
	}

	return CallNextHookEx(g_hhkMouse, code, wParam, lParam);
}

const char* getEventName(WPARAM wParam) {
	static char s_msg[80];

	switch (wParam) {
	case WM_LBUTTONDOWN:
		lstrcpy(s_msg, _T("WM_LBUTTONDOWN"));
		break;
	
	case WM_LBUTTONUP:
		lstrcpy(s_msg, _T("WM_LBUTTONUP"));
		break;

	case WM_RBUTTONDOWN:
		lstrcpy(s_msg, _T("WM_RBUTTONDOWN"));
		break;

	case WM_RBUTTONUP:
		lstrcpy(s_msg, _T("WM_RBUTTONUP"));
		break;

	case WM_MOUSEWHEEL:
		lstrcpy(s_msg, _T("WM_MOUSEWHEEL"));
		break;

	default:
		wsprintf(s_msg, _T("0x%04X"), wParam);
		break;
	}

	return s_msg;

}

LRESULT CALLBACK MouseProcLL(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0) {
		if (code == HC_ACTION) {
			switch (wParam) {
			case WM_NCMOUSEMOVE:	// 0x00A0
			case WM_MOUSEMOVE:		// 0x0200
			case WM_MOUSEWHEEL:		// 0x020A
				// �}�E�X�̈ړ��͖������܂��B
				break;

			default:
				// ���������邱�Ƃ��m�F�ł��Ă���}�E�X�C�x���g
				{
					MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;

					FILE* fp = fopen("mouse.log", "a+");
					if (fp) {
						fprintf(fp, "[%4d] wParam(%-14s) {pt(%4d,%4d), mouseData(%u), flags(%08X), time(%u), dwExtraInfo(%u)}\n",
							GetCurrentProcessId(),
							getEventName(wParam),
							wParam,
							p->pt.x, p->pt.y, p->mouseData, p->flags, p->time, p->dwExtraInfo);
						fclose(fp);
					}
				}
				
				PostMessage(g_hwndTgt, WM_KMWATCH_MOUSE, wParam, lParam);
				break;
			}
		}
	}

	return CallNextHookEx(g_hhkMouse, code, wParam, lParam);
}

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
			PostMessage(g_hwndTgt, WM_KMWATCH, wParam, lParam);
		}
	}

	return CallNextHookEx(g_hhkKeybd, code, wParam, lParam);
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

BOOL KMWATCH_API SetHook(HWND hwnd)
{
	BOOL ret = FALSE;

	// ��hook�̂Ƃ�����hook
	if (!g_hhkKeybd) {
		g_hwndTgt = hwnd;
		g_hhkKeybd = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hinstDLL, 0);
		///g_hhkMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hinstDLL, 0);
		g_hhkMouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProcLL, g_hinstDLL, 0);
		if (g_hhkKeybd && g_hhkMouse) {
			ret = TRUE;
		} else {
			FreeHook();
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

void KMWATCH_API FreeHook()
{
	// hook�ς݂̎�����unhook
	if (g_hhkKeybd) {
		UnhookWindowsHookEx(g_hhkKeybd);
		g_hhkKeybd = NULL;
	}

	if (g_hhkMouse) {
		UnhookWindowsHookEx(g_hhkMouse);
		g_hhkMouse = NULL;
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
		WM_KMWATCH = RegisterWindowMessage(KMWATCH_MESSAGE);
		WM_KMWATCH_MOUSE = RegisterWindowMessage(KMWATCH_MOUSE_MESSAGE);
	}

	return TRUE;
}
