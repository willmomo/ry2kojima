//=====================================================================
// VS2008�Ńr���h�������̂��A�C���X�g�[���Ȃ��ŁA�����^�C����������PC
// �œ��������߂ɂ́A
//
// C/C++ - �R�[�h���� - ���C���^�C�����C�u����
// ���A�u�}���`�X���b�h(/MT)�vor�u�}���`�X���b�h �f�o�b�O(/MTd)�v
// �ɂ��āA�r���h���܂�(�ÓI�����N���܂�)�B
//=====================================================================
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <queue>
#include "resource.h"
#include "SuDokuCell.h"

///#include "../../kjmsub/kjmsub-v0_2010_7_21/kjm_brush.h"
#include "kjmsub.h"

//---------------------------------------------------------------------
// �O���[�o���ϐ�
//---------------------------------------------------------------------
SuDokuGame g_game;
SuDokuGame g_game2;
int g_xCell, g_yCell;
int g_currentNum = 0;

//---------------------------------------------------------------------
// �o�[�W�������_�C�A���O
//---------------------------------------------------------------------
BOOL CALLBACK About_DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
		case WM_COMMAND:
			{
				WORD wNotifyCode = HIWORD(wParam);
				WORD wID = LOWORD(wParam);
				HWND hwndCtl = (HWND)lParam;

				if (wID == IDOK || wID == IDCANCEL) {
					EndDialog(hwnd, 0);
					return TRUE;
				}
			}
			break;
	}

	return FALSE;
}

//---------------------------------------------------------------------
// �`��
//---------------------------------------------------------------------
void OnDraw(HWND hwnd, HDC hdc, PAINTSTRUCT* pps) {
	TCHAR szNum[8];
	int candidateNum;
	int x, y;
	HBRUSH hbrOld;
	kjm::Brush hbr(RGB(255, 255, 255)), hbr2(RGB(200, 200, 200)), hbr3(RGB(150, 150, 250)), hbr4(RGB(250, 150, 150)), hbr5(RGB(150, 250, 150)), hbr6(RGB(240, 240, 100));
	HFONT hfont, hfontOld, hfontGui;
	LOGFONT lf;

	hfontGui = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	GetObject(hfontGui, sizeof(lf), &lf);
	lf.lfHeight = lf.lfHeight * 300 / 100;
	//lf.lfWeight = 700;
	hfont = CreateFontIndirect(&lf);
	hfontOld = (HFONT)SelectObject(hdc, hfont);

	for (y = 0; y < 9; y++) {
		for (x = 0; x < 9; x++) {
			if (g_game.m_cells[y][x].m_fixed != 0) {
				int bkOld;

				if (g_game.m_cells[y][x].m_sender == 1) {
					hbrOld = (HBRUSH)SelectObject(hdc, hbr2);
					bkOld = SetBkColor(hdc, RGB(200, 200, 200));	// �u���b�N���Ō�₪��ɂȂ�����
				}
				if (g_game.m_cells[y][x].m_sender == 2) {			// (��)��₪�ЂƂɂȂ����Ƃ�
					hbrOld = (HBRUSH)SelectObject(hdc, hbr3);
					bkOld = SetBkColor(hdc, RGB(150, 150, 250));
				}
				if (g_game.m_cells[y][x].m_sender == 3) {			// (��)�������ɂЂƂ�����₪�������Ƃ�
					hbrOld = (HBRUSH)SelectObject(hdc, hbr4);
					bkOld = SetBkColor(hdc, RGB(250, 150, 150));
				}
				if (g_game.m_cells[y][x].m_sender == 4) {			// (��)�c�����ɂЂƂ�����₪�������Ƃ�
					hbrOld = (HBRUSH)SelectObject(hdc, hbr5);
					bkOld = SetBkColor(hdc, RGB(150, 250, 150));
				}
				if (g_game.m_cells[y][x].m_sender == 5) {
					hbrOld = (HBRUSH)SelectObject(hdc, hbr6);
					bkOld = SetBkColor(hdc, RGB(240, 240, 100));
				}

				Rectangle(hdc, g_game.m_cells[y][x].m_rc.left,
							   g_game.m_cells[y][x].m_rc.top,
							   g_game.m_cells[y][x].m_rc.right,
							   g_game.m_cells[y][x].m_rc.bottom);

				SetTextColor(hdc, (g_currentNum == 0 || g_currentNum != g_game.m_cells[y][x].m_fixed) ? RGB(0, 0, 0) : RGB(255, 0, 0));
				wsprintf(szNum, _T("%d"), g_game.m_cells[y][x].m_fixed);
				DrawText(hdc, szNum, -1, &g_game.m_cells[y][x].m_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				if (g_game.m_cells[y][x].m_sender != 0) {
					SelectObject(hdc, hbrOld);
					SetBkColor(hdc, bkOld);
				}
			} else if (g_game.m_cells[y][x].GetCandidateCount(&candidateNum) == 1) {
				//hbrOld = (HBRUSH)SelectObject(hdc, hbr);

				Rectangle(hdc, g_game.m_cells[y][x].m_rc.left,
							   g_game.m_cells[y][x].m_rc.top,
							   g_game.m_cells[y][x].m_rc.right,
							   g_game.m_cells[y][x].m_rc.bottom);

				//SelectObject(hdc, hbrOld);

				//int bkOld = SetBkColor(hdc, RGB(180, 255, 180));
				wsprintf(szNum, _T("%d"), candidateNum);
				SetTextColor(hdc, (g_currentNum == 0 || g_currentNum != g_game.m_cells[y][x].m_fixed) ? RGB(180, 255, 180) : RGB(255, 0, 0));
				DrawText(hdc, szNum, -1, &g_game.m_cells[y][x].m_rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				//SetBkColor(hdc, bkOld);
			} else {
				Rectangle(hdc, g_game.m_cells[y][x].m_rc.left,
							   g_game.m_cells[y][x].m_rc.top,
							   g_game.m_cells[y][x].m_rc.right,
							   g_game.m_cells[y][x].m_rc.bottom);
			}
		}
	}

	for (int i = 0; i < 9; i++) {
		Rectangle(hdc, g_game.m_rcNums[i].left,
					   g_game.m_rcNums[i].top,
					   g_game.m_rcNums[i].right,
					   g_game.m_rcNums[i].bottom);

		wsprintf(szNum, _T("%d"), i + 1);
		SetTextColor(hdc, RGB(0, 0, 0));
		DrawText(hdc, szNum, -1, &g_game.m_rcNums[i], DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (g_game.m_nums[i] == 9) {
			MoveToEx(hdc, g_game.m_rcNums[i].right, g_game.m_rcNums[i].top, NULL);
			LineTo(hdc, g_game.m_rcNums[i].left, g_game.m_rcNums[i].bottom);
		}
	}

	SelectObject(hdc, hfontOld);

	DeleteObject(hfont);
}

//---------------------------------------------------------------------
// WM_CREATE���b�Z�[�W����
//---------------------------------------------------------------------
LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	g_game.InitCells();

	return 0;
}

//---------------------------------------------------------------------
// WM_PAINT���b�Z�[�W����
//---------------------------------------------------------------------
LRESULT OnPaint(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc;

	hdc= BeginPaint(hwnd, &ps);
	
	OnDraw(hwnd, hdc, &ps);

	EndPaint(hwnd, &ps);

	return 0;
}

//---------------------------------------------------------------------
// WM_LBUTTONDOWN���b�Z�[�W����
//---------------------------------------------------------------------
LRESULT OnLButtonDown(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	POINT ptScreen = pt;
	ClientToScreen(hwnd, &ptScreen);

	int x, y;
	SuDokuCell* pCell = g_game.PtInCells(pt, &x, &y);

	// �N���b�N�����Z���̌�₪�A�P���₾�����Ƃ��́A�m�肳����B
	if (pCell) {
		int num;
		if (pCell->m_fixed == 0 && pCell->GetCandidateCount(&num) == 1) {
			g_game.FixCell(y, x, num);
			InvalidateRect(hwnd, NULL, FALSE);
		} else {
			HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUP));
			HMENU hmenuSub = GetSubMenu(hmenu, 0);

			g_xCell = x;
			g_yCell = y;
			for (int i = 0; i < 9; i++) {
				if (pCell->m_fixed != 0 || pCell->m_candidate[i] == 0) {
					EnableMenuItem(hmenuSub, i, MF_BYPOSITION | MF_GRAYED);
				}
			}

			SetForegroundWindow(hwnd);
			TrackPopupMenu(hmenuSub, TPM_CENTERALIGN | TPM_VCENTERALIGN, ptScreen.x, ptScreen.y, 0, hwnd, NULL);
			SendMessage(hwnd, WM_NULL, 0, 0);
		}
	}

	return 0;
}

//---------------------------------------------------------------------
// WM_RBUTTONUP���b�Z�[�W����
//---------------------------------------------------------------------
LRESULT OnRButtonUp(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	POINT ptScreen = pt;
	ClientToScreen(hwnd, &ptScreen);

	int x, y;
	SuDokuCell* pCell = g_game.PtInCells(pt, &x, &y);

	// ��␔����I�����郁�j���[��\������B
	if (pCell) {
		HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUP));
		HMENU hmenuSub = GetSubMenu(hmenu, 0);

		g_xCell = x;
		g_yCell = y;
		for (int i = 0; i < 9; i++) {
			if (pCell->m_fixed != 0 || pCell->m_candidate[i] == 0) {
				EnableMenuItem(hmenuSub, i, MF_BYPOSITION | MF_GRAYED);
			}
		}

		SetForegroundWindow(hwnd);
		TrackPopupMenu(hmenuSub, TPM_LEFTALIGN | TPM_TOPALIGN, ptScreen.x, ptScreen.y, 0, hwnd, NULL);
		SendMessage(hwnd, WM_NULL, 0, 0);
	}

	return 0;
}

//---------------------------------------------------------------------
// WM_MOUSEMOVE ���b�Z�[�W����
//---------------------------------------------------------------------
LRESULT OnMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	POINT ptScreen = pt;
	ClientToScreen(hwnd, &ptScreen);

	int x, y;
	SuDokuCell* pCell = g_game.PtInCells(pt, &x, &y);

	// ��␔����I�����郁�j���[��\������B
	int prevNum = g_currentNum;
	if (pCell) {
		g_currentNum = pCell->m_fixed;
	} else {
		g_currentNum = 0;
	}

	if (prevNum != g_currentNum) {
		InvalidateRect(hwnd, NULL, FALSE);
	}

	return 0;
}

//---------------------------------------------------------------------
// �Q�[��������������
//---------------------------------------------------------------------
LRESULT OnGameInit(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	g_game.InitCells();

	InvalidateRect(hwnd, NULL, TRUE);

	return 0;
}

//---------------------------------------------------------------------
// �Q�[��������������
//---------------------------------------------------------------------
LRESULT OnGameOpen(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	OPENFILENAME ofn;
	TCHAR strFile[MAX_PATH];
	strFile[0] = 0;

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = _T("���ׂẴt�@�C��(*.*)\0*.*\0");
	ofn.lpstrFile = strFile;
	ofn.nMaxFile = MAX_PATH;
	//ofn.Flags = 

	if (GetOpenFileName(&ofn)) {
		g_game.InitCellsFromFile(strFile);
		InvalidateRect(hwnd, NULL, FALSE);
	}

	return 0;
}


//-----------------------------------------------------------------------------
// ��������
//-----------------------------------------------------------------------------
int SoAtari(HWND hwnd) {
	SuDokuGame save = g_game;

	int loop = 1;
	TCHAR buf[512];
	std::queue<SuDokuGame> games;

	games.push(g_game);

	while (!games.empty()) {
		SuDokuGame curGame = games.front();
		games.pop();

		// ��₪��̂��̂�T���āA��������V�����Q�[�������
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				int cnt = curGame.m_cells[y][x].GetCandidateCount(NULL);
				if (cnt == 2) {
					int n;
					for (n = 0; n < 9; n++) {
						if (curGame.m_cells[y][x].m_candidate[n] == 1) {
							SuDokuGame newGame(curGame);
							newGame.FixCell(y, x, n + 1);
							newGame.ReparseAll();
							if (!newGame.IsFinished()) {
								//OutputDebugString(_T("��l�܂�\n"));
							} else {
								OutputDebugString(_T("������!!\n"));
								g_game = newGame;
								return 1;
							}
							if (newGame.IsGameover() == false) {
								wsprintf(buf, _T("%2d: (%d,%d) = %d �̃p�^�[����o�^\n"), loop, y, x, n + 1);
								OutputDebugString(buf);
								games.push(newGame);
							} else {
								// �I���������ʁA�Q�[���I�[�o�[�ɂȂ���ꍇ�́A���̑I�������������ɗL���ȃp�^�[���̂͂��B
								OutputDebugString(_T("�Q�[���I�[�o�[!!\n"));
							}
							n++;
							break;
						}
					}
					for (; n< 9; n++) {
						if (g_game.m_cells[y][x].m_candidate[n] == 1) {
							SuDokuGame newGame(g_game);
							newGame.FixCell(y, x, n + 1);
							newGame.ReparseAll();
							if (!newGame.IsFinished()) {
								//OutputDebugString(_T("��l�܂�\n"));
							} else {
								OutputDebugString(_T("������!!\n"));
								g_game = newGame;
								return 1;
							}
							if (newGame.IsGameover() == false) {
								wsprintf(buf, _T("%2d: (%d,%d) = %d �̃p�^�[����o�^\n"), loop, y, x, n + 1);
								OutputDebugString(buf);
								games.push(newGame);
							}
							n++;
							wsprintf(buf, _T("---�c�� %4d -----------------------------------------------------\n"), games.size());
							OutputDebugString(buf);
							loop++;
							goto skip;
						}
					}
				}
			}
		}
skip:
		;
	}

	return 0;
}


//-----------------------------------------------------------------------------
// WM_COMMAND ���b�Z�[�W����
//-----------------------------------------------------------------------------
LRESULT OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	WORD wNotifyCode = HIWORD(wParam);
	WORD wID = LOWORD(wParam);
	HWND hwndCtl = (HWND)lParam;

	int fixed = 0;

	switch (wID) {
		case ID__1:
			fixed = 1;
			break;
		case ID__2:
			fixed = 2;
			break;
		case ID__3:
			fixed = 3;
			break;
		case ID__4:
			fixed = 4;
			break;
		case ID__5:
			fixed = 5;
			break;
		case ID__6:
			fixed = 6;
			break;
		case ID__7:
			fixed = 7;
			break;
		case ID__8:
			fixed = 8;
			break;
		case ID__9:
			fixed = 9;
			break;
		case ID__0:
			fixed = 0;
			break;
		case IDM_GAME_INIT:
			return OnGameInit(hwnd, wParam, lParam);
			
		case IDM_GAME_OPEN:
			return OnGameOpen(hwnd, wParam, lParam);

		case ID_GAME_PARSE:
			if (g_game.Reparse() == 0) {
				MessageBox(hwnd, _T("��l�܂�ł�"), _T("���ƃT�|�[�g"), MB_OK);
			}
			InvalidateRect(hwnd, NULL, FALSE);
			return 0;

		case ID_GAME_PARSE_ALL:
			g_game.ReparseAll();
			InvalidateRect(hwnd, NULL, FALSE);
			if (g_game.IsFinished()) {
				MessageBox(hwnd, _T("�����܂���"), _T("���ƃT�|�[�g"), MB_OK);
			} else {
				MessageBox(hwnd, _T("��l�܂�ł�"), _T("���ƃT�|�[�g"), MB_OK);
			}

			return 0;

		case ID_GAME_TOTAL:
			if (SoAtari(hwnd)) {
				InvalidateRect(hwnd, NULL, FALSE);
				MessageBox(hwnd, _T("�����܂���"), _T("���ƃT�|�[�g"), MB_OK);
			} else {
				MessageBox(hwnd, _T("����ς��l�܂�ł�"), _T("���ƃT�|�[�g"), MB_OK);
			}
			return 0;

		case IDM_GAME_EXIT:
			DestroyWindow(hwnd);
			return 0;

		case ID_HELP_ABOUT:
			DialogBox(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDD_ABOUT), hwnd, About_DlgProc);
			return 0;
	}

	if (fixed != 0) {
		g_game.FixCell(g_yCell, g_xCell, fixed);
	} else {
		g_game.ResetCell(g_yCell, g_xCell);
	}

	InvalidateRect(hwnd, NULL, FALSE);

	return 0;
}

//---------------------------------------------------------------------
//	�E�B���h�E�v���V�[�W��
//---------------------------------------------------------------------
LRESULT CALLBACK Main_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	LRESULT lRet = 0;

	switch (uMsg) {
	case WM_COMMAND:
		lRet = OnCommand(hwnd, wParam, lParam);
		break;

	case WM_CREATE:
		lRet = OnCreate(hwnd, wParam, lParam);
		break;

	case WM_PAINT:
		lRet = OnPaint(hwnd, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		lRet = OnLButtonDown(hwnd, wParam, lParam);
		break;

	case WM_RBUTTONUP:
		lRet = OnRButtonUp(hwnd, wParam, lParam);
		break;

	case WM_MOUSEMOVE:
		lRet = OnMouseMove(hwnd, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		lRet = DefWindowProc(hwnd, uMsg, wParam, lParam);
		break;
	}

	return lRet;
}

//---------------------------------------------------------------------
//	�v���O�����X�^�[�g
//---------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = Main_WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_DESKTOP + 1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN);
	wc.lpszClassName = _T("SUDOKU_MAIN");
	if (!RegisterClass(&wc)) {
		return -1;
	}

	RECT rcClient = {0, 0, 463, 516};

	AdjustWindowRect(&rcClient, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, TRUE);

	HWND hwnd = CreateWindow(_T("SUDOKU_MAIN"), _T("���ƃT�|�[�g"), 
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,
		NULL, NULL, hInstance, NULL);
	if (hwnd == NULL) {
		return -1;
	}

	ShowWindow(hwnd, nCmdShow);

	HACCEL hAccel = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR1));

	MSG msg;
	while (1) {
		BOOL bRet = GetMessage(&msg, NULL, 0, 0);
		if (bRet <= 0) {
			break;
		}

		if (!TranslateAccelerator(hwnd, hAccel, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
