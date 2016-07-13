/////////////////////////////////////////////////////////////////////////////
//	SDI Generic

#include "main.h"
#include "ext.h"

/////////////////////////////////////////////////////////////////////////////
//	About�E�_�C�A���O�E�v���V�[�W��

BOOL CALLBACK AboutDialogProc(HWND hwndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (wParam == IDOK) {
			EndDialog(hwndDlg, 0);
			return TRUE;
		} else if (wParam == IDCANCEL) {
			EndDialog(hwndDlg, -1);
			return TRUE;
		}
		break;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// ���C���E�E�B���h�E�E�v���V�[�W��

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static XTTY * ptty;

	switch (msg) {
	case WM_CREATE:
		ptty = new XTTY(ghinst, hwnd);
		if (ptty == NULL)
			return -1;

		return 0;

	case WM_COMMAND:
		if (wParam == ID_HELP_ABOUT) {
			DialogBox(ghinst, "About", hwnd, (DLGPROC)AboutDialogProc);
		} else if (wParam == IDM_ONLINE) {
			ptty->Connect(TRUE);
		} else if (wParam == IDM_OFFLINE) {
			ptty->Connect(FALSE);
		} else {
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		break;

	case WM_PAINT:
		ptty->OnPaint();
		break;

	case WM_CHAR:
		ptty->OnChar(wParam, lParam);
		break;

	case WM_COMMNOTIFY:
		ptty->OnCommNotify(wParam, lParam);
		break;

	case WM_DESTROY:
		delete ptty;
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	�C���X�^���X�̏�����

BOOL InitInstance(int nCmdShow)
{
	HWND hwnd = CreateWindow(
		gszAppName,
		gszWindowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		ghinst,
		0);

	if (!hwnd)
		return FALSE;

	ShowWindow(hwnd, nCmdShow);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	�A�v���P�[�V�����̏�����

BOOL InitApplication()
{
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = ghinst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = "DEFAULT";
	wc.lpszClassName = gszAppName;

	return RegisterClass(&wc);
}

/////////////////////////////////////////////////////////////////////////////
//	�O���[�o���ϐ��̏�����

void InitGlobal(HINSTANCE hinst)
{
	ghinst = hinst;
	LoadString(ghinst, IDS_APPNAME, gszAppName, sizeof(gszAppName));
	LoadString(ghinst, IDS_WINDOWNAME, gszWindowName, sizeof(gszWindowName));
}

/////////////////////////////////////////////////////////////////////////////
//	�v���O�����E�X�^�[�g

int PASCAL WinMain(HINSTANCE hinstCurrent, HINSTANCE hinstPrevious, LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;

	InitGlobal(hinstCurrent);		// �O���[�o���ϐ��̏�����

	if (hinstPrevious == NULL)		// �ق��̃C���X�^���X?
		if (!InitApplication())		// ���L�̍���
			return FALSE;		// �������Ɏ��s

	if (!InitInstance(nCmdShow))		// ���̃C���X�^���X�̏�����
		return FALSE;

	// WM_QUIT���b�Z�[�W��������܂�
	// ���b�Z�[�W�̎擾�ƃf�B�X�p�b�`���J��Ԃ�

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);		// ���z�L�[ �R�[�h��ϊ�
		DispatchMessage(&msg);		// ���b�Z�[�W���E�B���h�E�Ƀf�B�X�p�b�`
	}

	return (int)msg.wParam;			// PostQuitMessage�̖߂�l
}
