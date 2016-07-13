//=====================================================================
// VS2008でビルドしたものを、インストールなしで、ランタイム環境無しのPC
// で動かすためには、
//
// C/C++ - コード生成 - ラインタイムライブラリ
// を、「マルチスレッド(/MT)」or「マルチスレッド デバッグ(/MTd)」
// にして、ビルドします(静的リンクします)。
//=====================================================================
#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <queue>
#include <stack>
#include "resource.h"
#include "SuDokuGame.h"

///#include "../../kjmsub/kjmsub-v0_2010_7_21/kjm_brush.h"
//#include "kjmsub.h"
#include "./kjmlib-20121208/kjmlib.h"

//---------------------------------------------------------------------
// グローバル変数
//---------------------------------------------------------------------
SuDokuView g_view;
SuDokuGame g_game;
SuDokuGame g_game2;
int g_xCell, g_yCell;
int g_currentNum = 0;
std::stack<SuDokuGame> g_gameStack;

//---------------------------------------------------------------------
// バージョン情報ダイアログ
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
// 描画
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
			if (g_game.get_Value(y, x) != 0) {
				int bkOld;

				if (g_game.get_Sender(y, x) == 1) {
					hbrOld = (HBRUSH)SelectObject(hdc, hbr2);
					bkOld = SetBkColor(hdc, RGB(200, 200, 200));	// ブロック内で候補が一つになった時
				}
				if (g_game.get_Sender(y, x) == 2) {			// (青)候補がひとつになったとき
					hbrOld = (HBRUSH)SelectObject(hdc, hbr3);
					bkOld = SetBkColor(hdc, RGB(150, 150, 250));
				}
				if (g_game.get_Sender(y, x) == 3) {			// (赤)横方向にひとつだけ候補があったとき
					hbrOld = (HBRUSH)SelectObject(hdc, hbr4);
					bkOld = SetBkColor(hdc, RGB(250, 150, 150));
				}
				if (g_game.get_Sender(y, x) == 4) {			// (緑)縦方向にひとつだけ候補があったとき
					hbrOld = (HBRUSH)SelectObject(hdc, hbr5);
					bkOld = SetBkColor(hdc, RGB(150, 250, 150));
				}
				if (g_game.get_Sender(y, x) == 5) {
					hbrOld = (HBRUSH)SelectObject(hdc, hbr6);
					bkOld = SetBkColor(hdc, RGB(240, 240, 100));
				}

				Rectangle(hdc, g_view.get_Rect(y, x).left,
							   g_view.get_Rect(y, x).top,
							   g_view.get_Rect(y, x).right,
							   g_view.get_Rect(y, x).bottom);

				SetTextColor(hdc, (g_currentNum == 0 || g_currentNum != g_game.get_Value(y, x)) ? RGB(0, 0, 0) : RGB(255, 0, 0));
				wsprintf(szNum, _T("%d"), g_game.get_Value(y, x));
				DrawText(hdc, szNum, -1, &g_view.get_Rect(y, x), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

				if (g_game.get_Sender(y, x) != 0) {
					SelectObject(hdc, hbrOld);
					SetBkColor(hdc, bkOld);
				}
			} else if (g_game.GetCandidateCount(y, x, &candidateNum) == 1) {
				//hbrOld = (HBRUSH)SelectObject(hdc, hbr);

				Rectangle(hdc, g_view.get_Rect(y, x).left,
							   g_view.get_Rect(y, x).top,
							   g_view.get_Rect(y, x).right,
							   g_view.get_Rect(y, x).bottom);

				//SelectObject(hdc, hbrOld);

				//int bkOld = SetBkColor(hdc, RGB(180, 255, 180));
				wsprintf(szNum, _T("%d"), candidateNum);
				SetTextColor(hdc, (g_currentNum == 0 || g_currentNum != g_game.get_Value(y, x)) ? RGB(180, 255, 180) : RGB(255, 0, 0));
				DrawText(hdc, szNum, -1, &g_view.get_Rect(y, x), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				//SetBkColor(hdc, bkOld);
			} else {
				Rectangle(hdc, g_view.get_Rect(y, x).left,
							   g_view.get_Rect(y, x).top,
							   g_view.get_Rect(y, x).right,
							   g_view.get_Rect(y, x).bottom);
			}
		}
	}

	for (int i = 0; i < 9; i++) {
		Rectangle(hdc, g_view.get_NumRect(i).left,
					   g_view.get_NumRect(i).top,
					   g_view.get_NumRect(i).right,
					   g_view.get_NumRect(i).bottom);

		wsprintf(szNum, _T("%d"), i + 1);
		SetTextColor(hdc, RGB(0, 0, 0));
		DrawText(hdc, szNum, -1, &g_view.get_NumRect(i), DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		if (2 == 9) {
			MoveToEx(hdc, g_view.get_NumRect(i).right, g_view.get_NumRect(i).top, NULL);
			LineTo(hdc, g_view.get_NumRect(i).left, g_view.get_NumRect(i).bottom);
		}
	}

	SelectObject(hdc, hfontOld);

	DeleteObject(hfont);
}

//---------------------------------------------------------------------
// WM_CREATEメッセージ処理
//---------------------------------------------------------------------
LRESULT OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	g_game.InitCells();
	g_view.InitView();

	return 0;
}

//---------------------------------------------------------------------
// WM_PAINTメッセージ処理
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
// WM_LBUTTONDOWNメッセージ処理
//---------------------------------------------------------------------
LRESULT OnLButtonDown(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	POINT ptScreen = pt;
	ClientToScreen(hwnd, &ptScreen);

	int x, y;

	// クリックしたセルの候補が、単一候補だったときは、確定させる。
	if (g_view.PtInCells(pt, &x, &y)) {
		int num;
		if (g_game.get_Value(y, x) == 0 && g_game.GetCandidateCount(y, x, &num) == 1) {
			g_game.FixCell(y, x, num);
			InvalidateRect(hwnd, NULL, FALSE);
		} else {
			HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUP));
			HMENU hmenuSub = GetSubMenu(hmenu, 0);

			g_xCell = x;
			g_yCell = y;
			for (int i = 0; i < 9; i++) {
				if (g_game.get_Value(y, x) != 0 || g_game.get_Candidate(y, x, i) == 0) {
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
// WM_RBUTTONUPメッセージ処理
//---------------------------------------------------------------------
LRESULT OnRButtonUp(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	POINT ptScreen = pt;
	ClientToScreen(hwnd, &ptScreen);

	int x, y;

	// 候補数字を選択するメニューを表示する。
	if (g_view.PtInCells(pt, &x, &y)) {
		HMENU hmenu = LoadMenu(GetWindowInstance(hwnd), MAKEINTRESOURCE(IDR_POPUP));
		HMENU hmenuSub = GetSubMenu(hmenu, 0);

		g_xCell = x;
		g_yCell = y;
		for (int i = 0; i < 9; i++) {
			if (g_game.get_Value(y, x) != 0 || g_game.get_Candidate(y, x, i) == 0) {
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
// WM_MOUSEMOVE メッセージ処理
//---------------------------------------------------------------------
LRESULT OnMouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	POINT ptScreen = pt;
	ClientToScreen(hwnd, &ptScreen);

	int x, y;

	// 候補数字を選択するメニューを表示する。
	int prevNum = g_currentNum;
	if (g_view.PtInCells(pt, &x, &y)) {
		g_currentNum = g_game.get_Value(y, x);
	} else {
		g_currentNum = 0;
	}

	if (prevNum != g_currentNum) {
		InvalidateRect(hwnd, NULL, FALSE);
	}

	return 0;
}

//---------------------------------------------------------------------
// ゲーム板を初期化する
//---------------------------------------------------------------------
LRESULT OnGameInit(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	while (!g_gameStack.empty())
		g_gameStack.pop();

	g_game.InitCells();

	InvalidateRect(hwnd, NULL, TRUE);

	return 0;
}

//---------------------------------------------------------------------
// すべてのセルを初期化する
//---------------------------------------------------------------------
void InitCellsFromFile(SuDokuGame& sudoku, LPCTSTR fname) {
	while (!g_gameStack.empty())
		g_gameStack.pop();

	sudoku.InitCells();

	FILE* fp = _tfopen(fname, _T("r"));
	if (fp) {
		TCHAR buf[512];
		for (int y = 0; y < 9; y++) {
			if (_fgetts(buf, _countof(buf), fp) == NULL) {
				break;
			}

			for (int x = 0; x < 9; x++) {
				int num = buf[x] - '0';
				if (num != 0) {
					sudoku.FixCell(y, x, num);
				}
			}
		}

		while (1) {
			if (_fgetts(buf, _countof(buf), fp) == NULL) {
				break;
			}

			if (_tcsnicmp(_T("ExGroup="), buf, _tcslen(_T("ExGroup="))) == 0) {
				std::pair<int, int> onePair;
				std::vector<std::pair<int, int>> oneGroup;

				LPTSTR p = _tcstok(&buf[_tcslen(_T("ExGroup="))], _T(","));
				onePair.first = _tstoi(p);	// y 座標

				p = _tcstok(NULL, _T(","));
				onePair.second = _tstoi(p);	// x座標

				oneGroup.push_back(onePair);

				while ((p = _tcstok(NULL, _T(","))) != NULL) {
					onePair.first = _tstoi(p);
					p = _tcstok(NULL, _T(","));
					onePair.second = _tstoi(p);
					oneGroup.push_back(onePair);
				}

				sudoku.AddExGroup(oneGroup);
			}
		}

		fclose(fp);
	}
}

//---------------------------------------------------------------------
// ゲーム板を初期化する
//---------------------------------------------------------------------
LRESULT OnGameOpen(HWND hwnd, WPARAM wParam, LPARAM lParam) {
	OPENFILENAME ofn;
	TCHAR strFile[MAX_PATH];
	strFile[0] = 0;

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = _T("すべてのファイル(*.*)\0*.*\0");
	ofn.lpstrFile = strFile;
	ofn.nMaxFile = MAX_PATH;
	//ofn.Flags = 

	if (GetOpenFileName(&ofn)) {
		InitCellsFromFile(g_game, strFile);
		InvalidateRect(hwnd, NULL, FALSE);
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 総当たり
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

		// 候補が二個のものを探して、そこから新しいゲームを作る
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				int cnt = curGame.GetCandidateCount(y, x, NULL);
				if (cnt == 2) {
					int n;
					for (n = 0; n < 9; n++) {
						if (curGame.get_Candidate(y, x, n) == 1) {
							SuDokuGame newGame(curGame);
							newGame.FixCell(y, x, n + 1);
							newGame.ReparseAll();
							if (!newGame.IsFinished()) {
								//OutputDebugString(_T("手詰まり\n"));
							} else {
								OutputDebugString(_T("解けた!!\n"));
								g_game = newGame;
								return 1;
							}
							if (newGame.IsGameover() == false) {
								wsprintf(buf, _T("%2d: (%d,%d) = %d のパターンを登録\n"), loop, y, x, n + 1);
								OutputDebugString(buf);
								games.push(newGame);
							} else {
								// 選択した結果、ゲームオーバーにつながる場合は、次の選択肢が無条件に有効なパターンのはず。
								OutputDebugString(_T("ゲームオーバー!!\n"));
							}
							n++;
							break;
						}
					}
					for (; n< 9; n++) {
						if (g_game.get_Candidate(y, x, n) == 1) {
							SuDokuGame newGame(g_game);
							newGame.FixCell(y, x, n + 1);
							newGame.ReparseAll();
							if (!newGame.IsFinished()) {
								//OutputDebugString(_T("手詰まり\n"));
							} else {
								OutputDebugString(_T("解けた!!\n"));
								g_game = newGame;
								return 1;
							}
							if (newGame.IsGameover() == false) {
								wsprintf(buf, _T("%2d: (%d,%d) = %d のパターンを登録\n"), loop, y, x, n + 1);
								OutputDebugString(buf);
								games.push(newGame);
							}
							n++;
							wsprintf(buf, _T("---残り %4d -----------------------------------------------------\n"), games.size());
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
// WM_COMMAND メッセージ処理
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
				MessageBox(hwnd, _T("手詰まりです"), _T("数独サポート"), MB_OK);
			}
			InvalidateRect(hwnd, NULL, FALSE);
			return 0;

		case ID_GAME_PARSE_ALL:
			g_game.ReparseAll();
			InvalidateRect(hwnd, NULL, FALSE);
			if (g_game.IsFinished()) {
				MessageBox(hwnd, _T("解けました"), _T("数独サポート"), MB_OK);
			} else {
				MessageBox(hwnd, _T("手詰まりです"), _T("数独サポート"), MB_OK);
			}

			return 0;

		case ID_GAME_TOTAL:
			if (SoAtari(hwnd)) {
				InvalidateRect(hwnd, NULL, FALSE);
				MessageBox(hwnd, _T("解けました"), _T("数独サポート"), MB_OK);
			} else {
				MessageBox(hwnd, _T("やっぱり手詰まりです"), _T("数独サポート"), MB_OK);
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
		g_gameStack.push(g_game);
		g_game.FixCell(g_yCell, g_xCell, fixed);
		g_game.ReparseStart();
	} else {
		g_game = g_gameStack.top();
		g_gameStack.pop();
		g_game.ReparseStart();
	}

	if (g_game.IsFinished()) {
		MessageBox(hwnd, _T("解けました"), _T("数独サポート"), MB_OK);
	}

	InvalidateRect(hwnd, NULL, FALSE);

	return 0;
}

//---------------------------------------------------------------------
//	ウィンドウプロシージャ
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
//	プログラムスタート
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

	HWND hwnd = CreateWindow(_T("SUDOKU_MAIN"), _T("数独サポート"), 
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
