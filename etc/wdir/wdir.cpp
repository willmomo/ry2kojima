#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")


typedef struct {
	SIZE windowSize;	// �E�B���h�E�̕��ƍ���
	SIZE paddingLT;		// ���Ə�̃p�f�B���O
	SIZE paddingRB;		// �E�Ɖ��̃p�f�B���O
	RECT rcLogical;		// �p�f�B���O���l�������R���g���[���̘_���I��`

	void set_logical_rect(LPCRECT prc) {
		rcLogical.left = paddingLT.cx;
		rcLogical.top = paddingLT.cy;
		rcLogical.right = rcLogical.left + (prc->right - prc->left) + paddingRB.cx;
		rcLogical.bottom = rcLogical.top + (prc->bottom - prc->top) + paddingRB.cy;
	};

	/* �p�f�B���O���܂߂��� */
	int get_logical_width() {
		return paddingLT.cx + windowSize.cx + paddingRB.cx;
	};

	/* �p�f�B���O���܂߂����� */
	int get_logical_height() {
		return paddingLT.cy + windowSize.cy + paddingRB.cy;
	};
} window_position_ext;


/* ID */
#define IDC_STATIC1		1
#define IDC_STATIC2		2
#define IDC_EDIT1		3
#define IDC_EDIT2		4
#define IDC_BUTTON1		5
#define IDC_BUTTON2		6
#define IDC_LISTVIEW1	7


/* �O���[�o���ϐ� */
HWND g_hwndMainFrm;
HWND g_hwndStatic1;		// "�t�H���_: "��\������R���g���[��
HWND g_hwndStatic2;		// "���C���h�J�[�h: "��\������R���g���[��
HWND g_hwndEdit1;		// �t�H���_����͂���R���g���[��
HWND g_hwndEdit2;		// ���C���h�J�[�h����͂���R���g���[��
HWND g_hwndButton1;		// "���s" �{�^��
HWND g_hwndButton2;		// "�t�@�C���ɕۑ�" �{�^��
HWND g_hwndListView1;	// dir �̌��ʕ\���R���g���[��


/* �E�B���h�E�̈ʒu�����߂�g����� */
window_position_ext g_wpStatic1;
window_position_ext g_wpStatic2;


/* MainFrm �� WM_CREATE ���b�Z�[�W���� */
LRESULT MainFrm_OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	// ListView �R���g���[�����g�p�ł���悤�ɂ���
	InitCommonControls();

	/* "�t�H���_: "��\�����郉�x�� */
	g_hwndStatic1 = CreateWindow(_T("STATIC"), _T("�t�H���_: "),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 0, 0,
		hwnd, (HMENU)IDC_STATIC1, GetWindowInstance(hwnd), NULL);
	if (g_hwndStatic1 == NULL) {
		return -1;
	}

	/* "���C���h�J�[�h: "��\�����郉�x�� */
	g_hwndStatic2 = CreateWindow(_T("STATIC"), _T("���C���h�J�[�h: "),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 0, 0, 0,
		hwnd, (HMENU)IDC_STATIC2, GetWindowInstance(hwnd), NULL);
	if (g_hwndStatic2 == NULL) {
		return -1;
	}

	/* �t�H���_����͂���G�f�B�b�g */
	g_hwndEdit1 = CreateWindow(_T("EDIT"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		130, 0, 260, 20,
		hwnd, (HMENU)IDC_EDIT1, GetWindowInstance(hwnd), NULL);
	if (g_hwndEdit1 == NULL) {
		return -1;
	}

	/* �t�H���_����͂���G�f�B�b�g */
	g_hwndEdit2 = CreateWindow(_T("EDIT"), _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		130, 20, 260, 20,
		hwnd, (HMENU)IDC_EDIT2, GetWindowInstance(hwnd), NULL);
	if (g_hwndEdit2 == NULL) {
		return -1;
	}

	// ���s�{�^��
	g_hwndButton1 = CreateWindow(_T("BUTTON"), _T("���s"),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		0, 40, 130, 20,
		hwnd, (HMENU)IDC_BUTTON1, GetWindowInstance(hwnd), NULL);
	if (g_hwndButton1 == NULL) {
		return -1;
	}

	// ���ʕ\���R���g���[��
	g_hwndListView1 = CreateWindow(WC_LISTVIEW, _T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_REPORT,
		0, 60, 390, 100,
		hwnd, (HMENU)IDC_LISTVIEW1, GetWindowInstance(hwnd), NULL);
	if (g_hwndListView1 == NULL) {
		return -1;
	}

	// �t�@�C���ɕۑ��{�^��
	g_hwndButton2 = CreateWindow(_T("BUTTON"), _T("�t�@�C���ɕۑ�"),
		WS_CHILD | WS_VISIBLE,
		0, 160, 130, 20,
		hwnd, (HMENU)IDC_BUTTON2, GetWindowInstance(hwnd), NULL);
	if (g_hwndButton2 == NULL) {
		return -1;
	}

	return 0;
}


/* MainFrm �� WM_SIZE ���b�Z�[�W���� */
LRESULT MainFrm_OnSize(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	TCHAR windowText[512];
	HDC hdc;

	// Static1 ��K�؂ȃT�C�Y�Ƀ��T�C�Y
	GetWindowText(g_hwndStatic1, windowText, 512);
	hdc = GetWindowDC(g_hwndStatic1);
	GetTextExtentPoint32(hdc, windowText, lstrlen(windowText), &g_wpStatic1.windowSize);
	ReleaseDC(g_hwndStatic1, hdc);
	g_wpStatic1.paddingLT.cx = g_wpStatic1.paddingLT.cy = 3;
	g_wpStatic1.paddingRB.cx = g_wpStatic1.paddingRB.cy = 3;
	MoveWindow(g_hwndStatic1, 
		g_wpStatic1.paddingLT.cx,
		g_wpStatic1.paddingLT.cy,
		g_wpStatic1.windowSize.cx,
		g_wpStatic1.windowSize.cy,
		TRUE);

	// Static2 ��K�؂ȃT�C�Y�Ƀ��T�C�Y
	GetWindowText(g_hwndStatic2, windowText, 512);
	hdc = GetWindowDC(g_hwndStatic2);
	GetTextExtentPoint32(hdc, windowText, lstrlen(windowText), &g_wpStatic2.windowSize);
	ReleaseDC(g_hwndStatic2, hdc);
	g_wpStatic2.paddingLT.cx = g_wpStatic2.paddingLT.cy = 3;
	g_wpStatic2.paddingRB.cx = g_wpStatic2.paddingRB.cy = 3;
	MoveWindow(g_hwndStatic2, 
		g_wpStatic2.paddingLT.cx,
		g_wpStatic1.get_logical_height() + g_wpStatic2.paddingLT.cy,
		g_wpStatic2.windowSize.cx,
		g_wpStatic2.windowSize.cy,
		TRUE);

	return 0;
}


/* MainFrm �̃E�B���h�E�E�v���V�[�W�� */
LRESULT CALLBACK MainFrm_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT nResult;

	switch (uMsg) {
	case WM_CREATE:
		nResult = MainFrm_OnCreate(hwnd, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		nResult = 0;
		break;

	case WM_SIZE:
		nResult = MainFrm_OnSize(hwnd, wParam, lParam);
		break;

	default:
		nResult = DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}

	return nResult;
}


/* �v���O�����E�X�^�[�g */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = MainFrm_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = _T("MAINFRM_WDIR");

	if (!RegisterClass(&wc)) {
		return -1;
	}

	g_hwndMainFrm = CreateWindow(_T("MAINFRM_WDIR"), _T("wdir"), WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (g_hwndMainFrm == NULL) {
		return -1;
	}

	ShowWindow(g_hwndMainFrm, nCmdShow);

	MSG msg;
	while (1) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet == 0 || bRet == -1) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
