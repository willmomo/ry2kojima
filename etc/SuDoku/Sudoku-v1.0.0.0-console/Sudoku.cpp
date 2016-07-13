#include <windows.h>
#include <tchar.h>
#include <stack>

#include "kjmsub.h"

#include "SuDokuGame.h"


//---------------------------------------------------------------------
// すべてのセルを初期化する
//---------------------------------------------------------------------
void InitCellsFromFile(SuDokuGame& sudoku, LPCTSTR fname) {
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

void dispGame(const SuDokuGame& sudoku) {
	_tcout << _T("  1 2 3 4 5 6 7 8 9\n");

	// ゲーム版の情報を画面に表示
	for (int y = 0; y < 9; y++) {
		_tcout << (y + 1) << _T(":");
		for (int x = 0; x < 9; x++) {
			if (sudoku.get_Value(y, x) == 0) {
				// 未確定セル
				_tcout << _T("_ ");
			} else {
				// 確定セル
				_tcout << sudoku.get_Value(y, x) << (sudoku.get_Sender(y, x) == 0 ?  _T(" ") : _T("*"));
			}
		}
		_tcout << _T("\n");
	}
}

int _tmain(int argc, TCHAR** argv) {

	// 数独のゲーム盤を生成
	SuDokuGame sudoku;

	std::stack<SuDokuGame> games;
	
	dispGame(sudoku);

	// ゲーム版にコマンドを入力
	while (1) {
		_tprintf(_T("#"));

		TCHAR buf[80];
		_getts(buf);

		_tstring t = buf;
		if (t == _T("q")) break;
		if (t == _T("p")) {
			sudoku.ReparseAll();
			dispGame(sudoku);
			break;
		}
		if (t == _T(".")) {
			sudoku = games.top();
			games.pop();
			continue;
		}

		if (t.size() != 3) continue;

		int row = t[0] - '0';
		int col = t[1] - '0';
		int val = t[2] - '0';

		games.push(sudoku);	// 今の状態を記録して次に進める

		sudoku.FixCell(row - 1, col - 1, val);

		dispGame(sudoku);
	}

	return 0;
}
