#include <windows.h>
#include <windowsx.h>
#include "resource.h"

static BOOL CALLBACK Main_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	char data[512];
	char errmsg[512];

	switch (uMsg) {
	case WM_COMMAND:
		{
			WORD wNotifyCode = HIWORD(wParam);
			WORD wID = LOWORD(wParam);
			HWND hwndCtl = (HWND)lParam;

			if (wID == IDOK) {
				HANDLE hCom = CreateFile("COM1", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hCom != INVALID_HANDLE_VALUE) {
					DCB dcb;

					if (GetCommState(hCom, &dcb) == FALSE) {
						wsprintf(errmsg, "GetCommState error: %d", GetLastError());
						MessageBox(hwnd, errmsg, NULL, MB_OK);

						goto close_handle;
					}
					
					dcb.BaudRate = CBR_9600;
					dcb.Parity = NOPARITY;
					dcb.ByteSize = 8;
					dcb.StopBits = ONESTOPBIT;
					dcb.fDtrControl = DTR_CONTROL_DISABLE;
					dcb.fRtsControl = RTS_CONTROL_DISABLE;

					if (SetCommState(hCom, &dcb) == FALSE) {
						wsprintf(errmsg, "SetCommState error: %d", GetLastError());
						MessageBox(hwnd, errmsg, NULL, MB_OK);

						goto close_handle;
					}

					COMMTIMEOUTS to;
					if (GetCommTimeouts(hCom, &to) == FALSE) {
						wsprintf(errmsg, "GetCommTimeouts error: %d", GetLastError());
						MessageBox(hwnd, errmsg, NULL, MB_OK);

						goto close_handle;
					}

					to.ReadIntervalTimeout = 0;
					to.ReadTotalTimeoutMultiplier = 0;
					to.ReadTotalTimeoutConstant = 3000;
					to.WriteTotalTimeoutMultiplier = 0;
					to.WriteTotalTimeoutConstant = 1000;
					
					if (SetCommTimeouts(hCom, &to) == FALSE) {
						wsprintf(errmsg, "SetCommTimeouts error: %d", GetLastError());
						MessageBox(hwnd, errmsg, NULL, MB_OK);

						goto close_handle;
					}

					DWORD dwWritten;
					dwWritten = 0;
					if (WriteFile(hCom, "\x02PON\x03", lstrlen("\x02PON\x03"), &dwWritten, NULL) == FALSE) {
						wsprintf(errmsg, "WriteFile error: %d", GetLastError());
						MessageBox(hwnd, errmsg, NULL, MB_OK);

						goto close_handle;
					}

					Sleep(200);

					DWORD dwRead;
					dwRead = 0;
					if (ReadFile(hCom, data, 5, &dwRead, NULL) == FALSE) {
						wsprintf(errmsg, "ReadFile error: %d", GetLastError());
						MessageBox(hwnd, errmsg, NULL, MB_OK);
						
						goto close_handle;
					} else {
						wsprintf(errmsg, "%d bytes Read: %d", dwRead, GetLastError());
						MessageBox(hwnd, errmsg, NULL, MB_OK);

						MessageBox(hwnd, data, "Read", MB_OK);
					}

close_handle:
					CloseHandle(hCom);
				} else {
					MessageBox(hwnd, "'COM1' がオープンできません。", NULL, MB_OK);
				}
				return TRUE;
			} else if (wID == IDCANCEL) {
				EndDialog(hwnd, 1);
				return TRUE;
			}
		}
		break;

	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return TRUE;
	}

	return FALSE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, Main_DlgProc);
}
