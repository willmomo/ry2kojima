
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define MAINFRM_CLASSNAME	"MainFrm_SndPlay"

static WORD g_mciDeviceID = 0;

static void OnMciNotify(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	//mciSendCommand(g_mciDeviceID, MCI_STOP, MCI_WAIT, 0);
	//mciSendCommand(g_mciDeviceID, MCI_CLOSE, MCI_WAIT, 0);

	char alias[MAX_PATH + 10];
	MCI_OPEN_PARMS mciOpenParms;
	memset(&mciOpenParms, 0, sizeof(mciOpenParms));
	mciOpenParms.lpstrElementName = "tada.wav";
	mciOpenParms.lpstrAlias = alias;
	DWORD fdwCommand = MCI_WAIT | MCI_OPEN_ALIAS | MCI_OPEN_ELEMENT;

	mciSendCommand(0, MCI_OPEN, fdwCommand, (DWORD)&mciOpenParms);

	g_mciDeviceID = mciOpenParms.wDeviceID;

	MCI_PLAY_PARMS mciPlayParms;
	memset(&mciPlayParms, 0, sizeof(mciPlayParms));
	mciPlayParms.dwCallback = (DWORD)hwnd;
	mciPlayParms.dwFrom = 0;

//	mciSendCommand(mciOpenParms.wDeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)&mciPlayParms);
}

static void OnLButtonDown(HWND hwnd, int fwKeys, int xPos, int yPos)
{
	char alias[MAX_PATH + 10];
	MCI_OPEN_PARMS mciOpenParms;
	memset(&mciOpenParms, 0, sizeof(mciOpenParms));
	mciOpenParms.lpstrElementName = "b10.wav";
	mciOpenParms.lpstrAlias = alias;
	DWORD fdwCommand = MCI_WAIT | MCI_OPEN_ALIAS | MCI_OPEN_ELEMENT;

	mciSendCommand(0, MCI_OPEN, fdwCommand, (DWORD)&mciOpenParms);

	g_mciDeviceID = mciOpenParms.wDeviceID;

	MCI_SET_PARMS mciSetParms;
	memset(&mciSetParms, 0, sizeof(mciSetParms));
	mciSetParms.dwAudio = MCI_SET_AUDIO_LEFT;
	
	mciSendCommand(mciOpenParms.wDeviceID, MCI_SET, MCI_WAIT | MCI_SET_AUDIO | MCI_SET_OFF, (DWORD)&mciSetParms);

	MCI_PLAY_PARMS mciPlayParms;
	memset(&mciPlayParms, 0, sizeof(mciPlayParms));
	mciPlayParms.dwCallback = (DWORD)hwnd;
	mciPlayParms.dwFrom = 0;

	mciSendCommand(mciOpenParms.wDeviceID, MCI_PLAY, MCI_FROM | MCI_NOTIFY, (DWORD)&mciPlayParms);

	OnMciNotify(hwnd, 0, 0);
}

static void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

LRESULT CALLBACK MainFrm_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case MM_MCINOTIFY:
		//OnMciNotify(hwnd, wParam, lParam);
		return 0;

	case WM_LBUTTONDOWN:
		OnLButtonDown(hwnd, wParam, LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_DESTROY:
		OnDestroy(hwnd);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ウィンドウクラスの登録
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainFrm_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = MAINFRM_CLASSNAME;
	if (RegisterClass(&wc) == 0) {
		return -1;
	}

	// メインフレームの作成
	HWND hwnd = CreateWindow(
		MAINFRM_CLASSNAME,
		"SndPlay1",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		return -1;
	}
	ShowWindow(hwnd, nCmdShow);

	// メッセージループ
	MSG msg;
	while (true) {
		BOOL ret = GetMessage(&msg, NULL, 0, 0);
		if (ret <= 0) {
			break;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
