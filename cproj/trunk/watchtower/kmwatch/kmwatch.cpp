// kmwatch.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
//

#define _WIN32_WINNT	0x0400

#define STRICT
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "kmwatch.h"
#include "kjm/kjm_logger.h"

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

CRITICAL_SECTION g_cs;

kjm::logger g_log;

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

					g_log.write_i(_T("[%04X] wParam = 0x%08X {pt = %u, mouseData = %u, flags = %08X, time = %u, dwExtraInfo = %u}\n"),
						GetCurrentProcessId(),
						wParam,
						p->pt, p->mouseData, p->flags, p->time, p->dwExtraInfo);
					/*
					FILE* fp = fopen("mouse.log", "a+");
					if (fp) {
						fprintf(fp, "[%04X] wParam = 0x%08X {pt = %u, mouseData = %u, flags = %08X, time = %u, dwExtraInfo = %u}\n",
							GetCurrentProcessId(),
							wParam,
							p->pt, p->mouseData, p->flags, p->time, p->dwExtraInfo);
						fclose(fp);
					}
					*/
				}
				
				PostMessage(g_hwndTgt, WM_KMWATCH_MOUSE, wParam, lParam);
				break;
			}
		}
	}

	return CallNextHookEx(g_hhkMouse, code, wParam, lParam);
}

// see also: https://msdn.microsoft.com/ja-jp/library/windows/desktop/dd375731%28v=vs.85%29.aspx?f=255&MSPPError=-2147217396

const char* getVKeyName(WPARAM wParam) {
	static char s_msg[80];

	switch (wParam) {
	case VK_LBUTTON:	lstrcpy(s_msg, _T("VK_LBUTTON"));	break;
	case VK_RBUTTON:	lstrcpy(s_msg, _T("VK_RBUTTON"));	break;
	case VK_CANCEL:	lstrcpy(s_msg, _T("VK_CANCEL"));	break;
	case VK_MBUTTON:	lstrcpy(s_msg, _T("VK_MBUTTON"));	break;
	//case VK_XBUTTON1:	lstrcpy(s_msg, _T("VK_XBUTTON1"));	break;
	//case VK_XBUTTON2:	lstrcpy(s_msg, _T("VK_XBUTTON2"));	break;
	case VK_BACK:	lstrcpy(s_msg, _T("VK_BACK"));	break;
	case VK_TAB:	lstrcpy(s_msg, _T("VK_TAB"));	break;
	case VK_CLEAR:	lstrcpy(s_msg, _T("VK_CLEAR"));	break;
	case VK_RETURN:	lstrcpy(s_msg, _T("VK_RETURN"));	break;
	case VK_SHIFT:	lstrcpy(s_msg, _T("VK_SHIFT"));	break;
	case VK_CONTROL:	lstrcpy(s_msg, _T("VK_CONTROL"));	break;
	case VK_MENU:	lstrcpy(s_msg, _T("VK_MENU"));	break;
	case VK_PAUSE:	lstrcpy(s_msg, _T("VK_PAUSE"));	break;
	case VK_CAPITAL:	lstrcpy(s_msg, _T("VK_CAPITAL"));	break;
	case VK_KANA:	lstrcpy(s_msg, _T("VK_KANA"));	break;
	//case VK_HANGUEL:	lstrcpy(s_msg, _T("VK_HANGUEL"));	break;
	//case VK_HANGUL:	lstrcpy(s_msg, _T("VK_HANGUL"));	break;
	case VK_JUNJA:	lstrcpy(s_msg, _T("VK_JUNJA"));	break;
	case VK_FINAL:	lstrcpy(s_msg, _T("VK_FINAL"));	break;
	case VK_HANJA:	lstrcpy(s_msg, _T("VK_HANJA"));	break;
	//case VK_KANJI:	lstrcpy(s_msg, _T("VK_KANJI"));	break;
	case VK_ESCAPE:	lstrcpy(s_msg, _T("VK_ESCAPE"));	break;
	case VK_CONVERT:	lstrcpy(s_msg, _T("VK_CONVERT"));	break;
	case VK_NONCONVERT:	lstrcpy(s_msg, _T("VK_NONCONVERT"));	break;
	case VK_ACCEPT:	lstrcpy(s_msg, _T("VK_ACCEPT"));	break;
	case VK_MODECHANGE:	lstrcpy(s_msg, _T("VK_MODECHANGE"));	break;
	case VK_SPACE:	lstrcpy(s_msg, _T("VK_SPACE"));	break;
	case VK_PRIOR:	lstrcpy(s_msg, _T("VK_PRIOR"));	break;
	case VK_NEXT:	lstrcpy(s_msg, _T("VK_NEXT"));	break;
	case VK_END:	lstrcpy(s_msg, _T("VK_END"));	break;
	case VK_HOME:	lstrcpy(s_msg, _T("VK_HOME"));	break;
	case VK_LEFT:	lstrcpy(s_msg, _T("VK_LEFT"));	break;
	case VK_UP:	lstrcpy(s_msg, _T("VK_UP"));	break;
	case VK_RIGHT:	lstrcpy(s_msg, _T("VK_RIGHT"));	break;
	case VK_DOWN:	lstrcpy(s_msg, _T("VK_DOWN"));	break;
	case VK_SELECT:	lstrcpy(s_msg, _T("VK_SELECT"));	break;
	case VK_PRINT:	lstrcpy(s_msg, _T("VK_PRINT"));	break;
	case VK_EXECUTE:	lstrcpy(s_msg, _T("VK_EXECUTE"));	break;
	case VK_SNAPSHOT:	lstrcpy(s_msg, _T("VK_SNAPSHOT"));	break;
	case VK_INSERT:	lstrcpy(s_msg, _T("VK_INSERT"));	break;
	case VK_DELETE:	lstrcpy(s_msg, _T("VK_DELETE"));	break;
	case VK_HELP:	lstrcpy(s_msg, _T("VK_HELP"));	break;
	case VK_LWIN:	lstrcpy(s_msg, _T("VK_LWIN"));	break;
	case VK_RWIN:	lstrcpy(s_msg, _T("VK_RWIN"));	break;
	case VK_APPS:	lstrcpy(s_msg, _T("VK_APPS"));	break;
	case VK_SLEEP:	lstrcpy(s_msg, _T("VK_SLEEP"));	break;
	case VK_NUMPAD0:	lstrcpy(s_msg, _T("VK_NUMPAD0"));	break;
	case VK_NUMPAD1:	lstrcpy(s_msg, _T("VK_NUMPAD1"));	break;
	case VK_NUMPAD2:	lstrcpy(s_msg, _T("VK_NUMPAD2"));	break;
	case VK_NUMPAD3:	lstrcpy(s_msg, _T("VK_NUMPAD3"));	break;
	case VK_NUMPAD4:	lstrcpy(s_msg, _T("VK_NUMPAD4"));	break;
	case VK_NUMPAD5:	lstrcpy(s_msg, _T("VK_NUMPAD5"));	break;
	case VK_NUMPAD6:	lstrcpy(s_msg, _T("VK_NUMPAD6"));	break;
	case VK_NUMPAD7:	lstrcpy(s_msg, _T("VK_NUMPAD7"));	break;
	case VK_NUMPAD8:	lstrcpy(s_msg, _T("VK_NUMPAD8"));	break;
	case VK_NUMPAD9:	lstrcpy(s_msg, _T("VK_NUMPAD9"));	break;
	case VK_MULTIPLY:	lstrcpy(s_msg, _T("VK_MULTIPLY"));	break;
	case VK_ADD:	lstrcpy(s_msg, _T("VK_ADD"));	break;
	case VK_SEPARATOR:	lstrcpy(s_msg, _T("VK_SEPARATOR"));	break;
	case VK_SUBTRACT:	lstrcpy(s_msg, _T("VK_SUBTRACT"));	break;
	case VK_DECIMAL:	lstrcpy(s_msg, _T("VK_DECIMAL"));	break;
	case VK_DIVIDE:	lstrcpy(s_msg, _T("VK_DIVIDE"));	break;
	case VK_NUMLOCK:	lstrcpy(s_msg, _T("VK_NUMLOCK"));	break;
	case VK_SCROLL:	lstrcpy(s_msg, _T("VK_SCROLL"));	break;
	case VK_LSHIFT:	lstrcpy(s_msg, _T("VK_LSHIFT"));	break;
	case VK_RSHIFT:	lstrcpy(s_msg, _T("VK_RSHIFT"));	break;
	case VK_LCONTROL:	lstrcpy(s_msg, _T("VK_LCONTROL"));	break;
	case VK_RCONTROL:	lstrcpy(s_msg, _T("VK_RCONTROL"));	break;
	case VK_LMENU:	lstrcpy(s_msg, _T("VK_LMENU"));	break;
	case VK_RMENU:	lstrcpy(s_msg, _T("VK_RMENU"));	break;
	default:
		if (0x30 <= wParam && wParam <= 0x39) {
			wsprintf(s_msg, _T("%c key"), wParam);
		} else if (0x41 <= wParam && wParam <= 0x5a) {
			wsprintf(s_msg, _T("%c key"), wParam);
		} else if (VK_F1 <= wParam && wParam <= VK_F24) {
			wsprintf(s_msg, _T("F%d key"), wParam - VK_F1 + 1);
		} else {
			wsprintf(s_msg, _T("0x%04X"), wParam);
		}
		break;
	}

	return s_msg;
}

const char* getKeyEventName(WPARAM wParam) {
	static char s_msg[80];

	switch (wParam) {
	case WM_KEYDOWN:
		lstrcpy(s_msg, _T("WM_KEYDOWN"));
		break;
	
	case WM_KEYUP:
		lstrcpy(s_msg, _T("WM_KEYUP"));
		break;

	case WM_SYSKEYDOWN:
		lstrcpy(s_msg, _T("WM_SYSKEYDOWN"));
		break;

	case WM_SYSKEYUP:
		lstrcpy(s_msg, _T("WM_SYSKEYUP"));
		break;

	default:
		wsprintf(s_msg, _T("0x%04X"), wParam);
		break;
	}

	return s_msg;

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
				SYSTEMTIME st;
				GetLocalTime(&st);
				EnterCriticalSection(&g_cs);
				{
					MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;

					g_log.write_i(_T("M w[%08X] l[%08X] wParam(%-14s) {pt(%4d,%4d), mouseData(%u), flags(%08X), time(%u), dwExtraInfo(%u)}"),
						wParam, lParam, getEventName(wParam),
						p->pt.x, p->pt.y, p->mouseData, p->flags, p->time, p->dwExtraInfo);
					/*
					FILE* fp = fopen("kmevent.log", "a+");
					if (fp) {
						fprintf(fp, "%04d/%02d/%02d %02d:%02d:%02d.%03d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
						fprintf(fp, "M w[%08X] l[%08X] ", wParam, lParam);
						fprintf(fp, "wParam(%-14s) {pt(%4d,%4d), mouseData(%u), flags(%08X), time(%u), dwExtraInfo(%u)}\n",
							getEventName(wParam),
							p->pt.x, p->pt.y, p->mouseData, p->flags, p->time, p->dwExtraInfo);
						fclose(fp);
					}
					*/
				}
				LeaveCriticalSection(&g_cs);
				
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

LRESULT CALLBACK KeyboardProcLL(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= 0) {
		if (code == HC_ACTION) {
			PostMessage(g_hwndTgt, WM_KMWATCH, wParam, lParam);
			
			SYSTEMTIME st;
			GetLocalTime(&st);
			EnterCriticalSection(&g_cs);
			{
				KBDLLHOOKSTRUCT* p = (KBDLLHOOKSTRUCT*)lParam;

				g_log.write_i(_T("K w[%08X] l[%08X] wParam(%-14s) {vkCode(%-10s) scanCode(%2d) flags(%08X) time(%d) dwExtraInfo(%u)}"),
					wParam, lParam, getKeyEventName(wParam),
					getVKeyName(p->vkCode), p->scanCode, p->flags, p->time, p->dwExtraInfo);
				/*
				FILE* fp = fopen("kmevent.log", "a+");
				if (fp) {
					fprintf(fp, "%04d/%02d/%02d %02d:%02d:%02d.%03d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
					fprintf(fp, "K w[%08X] l[%08X] ", wParam, lParam);
					fprintf(fp, "wParam(%-14s) {vkCode(%-10s) scanCode(%2d) flags(%08X) time(%d) dwExtraInfo(%u)}\n",
						getKeyEventName(wParam),
						getVKeyName(p->vkCode), p->scanCode, p->flags, p->time, p->dwExtraInfo
						);
					fclose(fp);
				}
				*/
			}
			LeaveCriticalSection(&g_cs);
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
		g_hhkKeybd = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProcLL, g_hinstDLL, 0);
		g_hhkMouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProcLL, g_hinstDLL, 0);

		///g_hhkKeybd = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hinstDLL, 0);
		///g_hhkMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hinstDLL, 0);
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
		InitializeCriticalSection(&g_cs);

		g_hinstDLL = hinstDLL;
		WM_KMWATCH = RegisterWindowMessage(KMWATCH_MESSAGE);
		WM_KMWATCH_MOUSE = RegisterWindowMessage(KMWATCH_MOUSE_MESSAGE);
	}

	return TRUE;
}
