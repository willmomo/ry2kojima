#include "SuDokuCell.h"
#include <algorithm>
#include <functional>


//-----------------------------------------------------------------------------
// デフォルトコンストラクタ
//-----------------------------------------------------------------------------
SuDokuGame::SuDokuGame() {
	m_rcNums.resize(9);
	m_nums.resize(9);
}


//-----------------------------------------------------------------------------
// コピーコンストラクタ
//-----------------------------------------------------------------------------
SuDokuGame::SuDokuGame(const SuDokuGame& src) {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			this->m_cells[y][x] = src.m_cells[y][x];
		}
	}

	m_rcNums = src.m_rcNums;
	m_nums = src.m_nums;
	m_exGroups = src.m_exGroups;
}


//---------------------------------------------------------------------
// 代入演算子
//---------------------------------------------------------------------
SuDokuGame& SuDokuGame::operator =(const SuDokuGame& src) {
	if (this == &src) return *this;

	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			this->m_cells[y][x] = src.m_cells[y][x];
		}
	}

	m_rcNums = m_rcNums;
	m_nums = src.m_nums;
	m_exGroups = src.m_exGroups;

	return *this;
}


//---------------------------------------------------------------------
// セルの値を確定させる
//---------------------------------------------------------------------
void SuDokuGame::FixCell(int y, int x, int num, int sender) {
	int yTemp, xTemp;

	// 同一行の候補から番号を削除していく
	for (xTemp = 0; xTemp < 9; xTemp++) {
		m_cells[y][xTemp].m_candidate[num - 1] = 0;
	}

	// 同一列の候補から番号を削除していく
	for (yTemp = 0; yTemp < 9; yTemp++) {
		m_cells[yTemp][x].m_candidate[num - 1] = 0;
	}

	// 同一ブロックの候補から番号を削除していく
	for (yTemp = (y / 3) * 3; yTemp < ((y / 3) * 3) + 3; yTemp++) {
		for (xTemp = (x / 3) * 3; xTemp < ((x / 3) * 3) + 3; xTemp++) {
			m_cells[yTemp][xTemp].m_candidate[num - 1] = 0;
		}
	}

	// 同一拡張ブロックの候補から番号を削除していく
	std::vector<std::vector<std::pair<int, int>>>::size_type nExGroup;
	for (/*int*/ nExGroup = 0; nExGroup < m_exGroups.size(); nExGroup++) {
		//int nPair;
		std::vector<std::vector<std::pair<int, int>>>::size_type nPair;

		// 拡張グループの中に今回の座標が含まれるかどうか調べる
		for (nPair = 0; nPair < m_exGroups[nExGroup].size(); nPair++) {
			if ((m_exGroups[nExGroup][nPair].first == (y+1)) && (m_exGroups[nExGroup][nPair].second == (x+1))) {
				break;
			}
		}

		// 拡張グループの中に今回の座標が含まれていた場合、拡張グループの候補から番号を削除していく
		if (nPair < m_exGroups[nExGroup].size()) {
			std::vector<std::vector<std::pair<int, int>>>::size_type i;
			for (/*int*/ i = 0; i < m_exGroups[nExGroup].size(); i++) {
				m_cells[m_exGroups[nExGroup][i].first - 1][m_exGroups[nExGroup][i].second - 1].m_candidate[num - 1] = 0;
			}
		}
	}

	m_cells[y][x].m_fixed = num;
	m_cells[y][x].m_sender = sender;

	m_nums[num - 1]++;
}


//---------------------------------------------------------------------
// セルの値を未定にする
//---------------------------------------------------------------------
void SuDokuGame::ResetCell(int y, int x) {
	int yy, xx;

	// 一度すべての候補を復活させる
	for (yy = 0; yy < 9; yy++) {
		for (xx = 0; xx < 9; xx++) {
			std::fill(m_cells[yy][xx].m_candidate.begin(), m_cells[yy][xx].m_candidate.end(), 1);
		}
	}

	// 指定されたセルを未確定にする
	m_cells[y][x].m_fixed = 0;

	// 確定済みのセルをもう一度確定させる
	for (yy = 0; yy < 9; yy++) {
		for (xx = 0; xx < 9; xx++) {
			if (m_cells[yy][xx].m_fixed != 0) {
				FixCell(yy, xx, m_cells[yy][xx].m_fixed, m_cells[yy][xx].m_sender);
			}
		}
	}
}


//-----------------------------------------------------------------------------
// 候補が一つだけになったセルを探して確定させる
//-----------------------------------------------------------------------------
int SuDokuGame::Parse01() {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			int candidateNum;
			if (m_cells[y][x].GetCandidateCount(&candidateNum) == 1) {
				FixCell(y, x, candidateNum, 2);
				return 1;
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// ブロック内に一つだけの候補を探して確定させる
//-----------------------------------------------------------------------------
int SuDokuGame::Parse02() {
	for (int block = 0; block < 9; block++) {
		for (int num = 0; num < 9; num++) {
			int ySave, xSave, cnt = 0;
			for (int y = (block / 3) * 3; y < ((block / 3) * 3) + 3; y++) {
				for (int x = (block % 3) * 3; x < ((block % 3) * 3) + 3; x++) {
					if (m_cells[y][x].IsCandidateNum(num + 1)) {
						ySave = y;
						xSave = x;
						++cnt;
					}
				}
			}
			if (cnt == 1) {
				FixCell(ySave, xSave, num + 1, 1);
				return 1;
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 横方向にに一つだけの候補を探して確定させる
//-----------------------------------------------------------------------------
int SuDokuGame::Parse03() {
	for (int y = 0; y < 9; y++) {
		for (int num = 0; num < 9; num++) {
			int yyPos, xxPos;
			int cnt = 0;

			for (int x = 0; x < 9; x++) {
				if (m_cells[y][x].IsCandidateNum(num + 1)) {
					yyPos = y;
					xxPos = x;
					++cnt;
				}
			}

			if (cnt == 1) {
				FixCell(yyPos, xxPos, num + 1, 3);
				return 1;
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 縦方向にに一つだけの候補を探して確定させる
//-----------------------------------------------------------------------------
int SuDokuGame::Parse04() {
	for (int x = 0; x < 9; x++) {
		for (int num = 0; num < 9; num++) {
			int yyPos, xxPos;
			int cnt = 0;

			for (int y = 0; y < 9; y++) {
				if (m_cells[y][x].IsCandidateNum(num + 1)) {
					yyPos = y;
					xxPos = x;
					++cnt;
				}
			}

			if (cnt == 1) {
				FixCell(yyPos, xxPos, num + 1, 4);
				return 1;
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// ブロック内の数字で、候補が 2 または 3 で、その候補が縦または横に連続してる場所がないか調べる
//-----------------------------------------------------------------------------
int SuDokuGame::Parse05() {
	int block;
	int chg = 0;
	for (block = 0; block < 9; block++) {
		for (int num = 1; num <= 9; num++) {
			std::vector<int> xpos(9), ypos(9);

			int cnt = 0;
			for (int y = (block / 3) * 3; y < ((block / 3) * 3) + 3; y++) {
				for (int x = (block % 3) * 3; x < ((block % 3) * 3)+ 3; x++) {
					if (m_cells[y][x].IsCandidateNum(num)) {
						xpos[cnt] = x;
						ypos[cnt] = y;
						++cnt;
					}
				}
			}

			chg = 0;
			if (cnt == 2) {
				if (xpos[0] == xpos[1]) {
					// X方向連続
					for (int y = 0; y < 9; y++) {
						if (y != ypos[0] && y != ypos[1] && m_cells[y][xpos[0]].m_candidate[num-1] == 1) {
							m_cells[y][xpos[0]].m_candidate[num-1] = 0;
							chg = 1;
						}
					}
				} else if (ypos[0] == ypos[1]) {
					// Y方向連続
					for (int x = 0; x < 9; x++) {
						if (x != xpos[0] && x != xpos[1] && m_cells[ypos[0]][x].m_candidate[num-1] == 1) {
							m_cells[ypos[0]][x].m_candidate[num-1] = 0;
							chg = 1;
						}
					}
				}
			} else if (cnt == 3) {
				if (xpos[0] == xpos[1] && xpos[1] == xpos[2]) {
					// X方向連続
					for (int y = 0; y < 9; y++) {
						if (y != ypos[0] && y != ypos[1] && y != ypos[2] && m_cells[y][xpos[0]].m_candidate[num-1] == 1) {
							m_cells[y][xpos[0]].m_candidate[num-1] = 0;
							chg = 1;
						}
					}
				} else if (ypos[0] == ypos[1] && ypos[1] == ypos[2]) {
					// Y方向連続
					for (int x = 0; x < 9; x++) {
						if (x != xpos[0] && x != xpos[1] && x != xpos[2] && m_cells[ypos[0]][x].m_candidate[num-1] == 1) {
							m_cells[ypos[0]][x].m_candidate[num-1] = 0;
							chg = 1;
						}
					}
				}
			}

			if (chg == 1) {
				TCHAR buf[512];
				wsprintf(buf, _T("cnt %d : num %d : (%d,%d),(%d,%d),(%d,%d) \n"), cnt, num, ypos[0], xpos[0], ypos[1], xpos[1], ypos[2], xpos[2]);
				//OutputDebugString(buf);
				return 1;
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 候補が二つのセルで、仮決め実行してみる
// 実行結果が、ゲームオーバーになるなら、ゲームオーバーにならないほうで確定できる。
//-----------------------------------------------------------------------------
int SuDokuGame::Parse06() {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			if (m_cells[y][x].m_fixed)
				continue;

			if (m_cells[y][x].GetCandidateCount(NULL) != 2)
				continue;

			std::vector<int> result;
			for (std::vector<int>::iterator top = m_cells[y][x].m_candidate.begin(); (top = std::find(top, m_cells[y][x].m_candidate.end(), 1)) != m_cells[y][x].m_candidate.end(); top++) {
				result.push_back(top - m_cells[y][x].m_candidate.begin() + 1);
			}

			SuDokuGame tempGame;
			
			tempGame = *this;
			tempGame.FixCell(y, x, result[0]);
			tempGame.ReparseAll();
			if (tempGame.IsGameover()) {
				FixCell(y, x, result[1], 5);
				return 1;
			} else if (tempGame.IsFinished()) {
				FixCell(y, x, result[0], 5);
				return 1;
			}
			
			tempGame = *this;
			tempGame.FixCell(y, x, result[1]);
			tempGame.ReparseAll();
			if (tempGame.IsGameover()) {
				FixCell(y, x, result[0], 5);
				return 1;
			} else if (tempGame.IsFinished()) {
				FixCell(y, x, result[1], 5);
				return 1;
			}
		}
	}

	return 0;
}


//-----------------------------------------------------------------------------
// 拡張ブロック内にひとつだけの候補がないか調べる
//-----------------------------------------------------------------------------
int SuDokuGame::Parse11() {
	int cnt;

	std::vector<std::vector<std::pair<int, int>>>::size_type nExGroup;
	for (/*int*/ nExGroup = 0; nExGroup < m_exGroups.size(); nExGroup++) {
		std::vector<std::vector<std::pair<int, int>>>::size_type nPair;
		for (/*int*/ nPair = 0; nPair < m_exGroups[nExGroup].size(); nPair++) {
			for (int num = 0; num < 9; num++) {
				int yyPos, xxPos;
				cnt = 0;

				std::vector<std::vector<std::pair<int, int>>>::size_type nn;
				for (/*int*/ nn = 0; nn < m_exGroups[nExGroup].size(); nn++) {
					if (m_cells[m_exGroups[nExGroup][nn].first-1][m_exGroups[nExGroup][nn].second-1].IsCandidateNum(num + 1)) {
						yyPos = m_exGroups[nExGroup][nn].first-1;
						xxPos = m_exGroups[nExGroup][nn].second-1;
						++cnt;
					}
				}

				if (cnt == 1) {
					FixCell(yyPos, xxPos, num + 1, 1);
					return 1;
				}
			}
		}
	}

	return 0;
}


//---------------------------------------------------------------------
// 未確定セルを走査して確定可能なセルを確定させる
//
// [戻り値]
// 確定セルに変化があった場合、1 を返す
//---------------------------------------------------------------------
int SuDokuGame::Reparse() {
	// 候補が一つだけになったセルを探して確定させる
	if (Parse01()) return 1;

	// ブロック内にひとつだけの候補がないか調べる
	if (Parse02()) return 1;

	// 横方向に一つだけの候補がないか調べる
	if (Parse03()) return 1;

	// 縦方向に一つだけの候補がないか調べる
	if (Parse04()) return 1;

	// ブロック内の数字で、候補が 2 または 3 で、その候補が縦または横に連続してる場所がないか調べる
	if (Parse05()) return 1;

	// 候補が二つのセルで、仮決め実行してみる
	// 実行結果が、ゲームオーバーになるなら、ゲームオーバーにならないほうで確定できる。
	if (Parse06()) return 1;

	// 拡張ブロック内にひとつだけの候補がないか調べる
	if (Parse11()) return 1;

	return 0;
}


//---------------------------------------------------------------------
// すべてのセルを初期化する
//---------------------------------------------------------------------
void SuDokuGame::InitCells() {
	int ofs[] = {3, 156, 309, 309 + CELL_SIZE * 3 + 3};
	int x, y;

	for (y = 0; y < 9; y++) {
		for (x = 0; x < 9; x++) {
			m_cells[y][x].InitCell(y, x);
		}
	}

	std::fill(m_nums.begin(), m_nums.end(), 0);

	for (int i = 0; i < 9; i++) {
		// 表示位置を初期化
		int left = ofs[i / 3] + ((i % 3) * CELL_SIZE);
		int top  = ofs[9 / 3] + ((9 % 3) * CELL_SIZE);

		SetRect(&m_rcNums[i], left, top, left + CELL_SIZE + 1, top + CELL_SIZE + 1);
	}

	{
		TCHAR buf[512];
		wsprintf(buf, _T("%d,%d\n"), m_rcNums[8].right, m_rcNums[8].bottom);
		OutputDebugString(buf);
	}

	m_exGroups.clear();
}


//---------------------------------------------------------------------
// すべてのセルを初期化する
//---------------------------------------------------------------------
void SuDokuGame::InitCellsFromFile(LPCTSTR fname) {
	InitCells();

	FILE* fp = _tfopen(fname, _T("r"));
	if (fp) {
		TCHAR buf[512];
		for (int y = 0; y < 9; y++) {
			if (_fgetts(buf, _countof(buf), fp) == NULL) {
				break;
			}

			for (int x = 0; x < 9; x++) {
				int num = buf[x] - '0';
				if (num != 0 && m_cells[y][x].m_fixed == 0) {
					this->FixCell(y, x, num);
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

				m_exGroups.push_back(oneGroup);
			}
		}

		fclose(fp);
	}
}


//---------------------------------------------------------------------
// 座標から対応するセルへのポインタを返す
//---------------------------------------------------------------------
SuDokuCell* SuDokuGame::PtInCells(POINT pt, int* pX, int* pY) {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			if (PtInRect(&m_cells[y][x].m_rc, pt)) {
				*pY = y;
				*pX = x;
				return &m_cells[y][x];
			}
		}
	}

	return NULL;
}


//-----------------------------------------------------------------------------
// ゲームが終了したかどうか調べる
//-----------------------------------------------------------------------------
bool SuDokuGame::IsFinished() {
	// 管理する数(9)以外を検索する。
	// 管理する数(9)以外が見つからなければ、ゲームは正常に終わっている。

	return (std::find_if(m_nums.begin(), m_nums.end(), std::bind2nd(std::not_equal_to<int>(), m_nums.size())) == m_nums.end());
}


//-----------------------------------------------------------------------------
// ゲームが完全に手詰まりかどうか調べる
//-----------------------------------------------------------------------------
bool SuDokuGame::IsGameover() {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			if (m_cells[y][x].m_fixed)
				continue;

			if (m_cells[y][x].GetCandidateCount(NULL) == 0)
				return true;	// どの候補も選べなくなっているときは、ゲームオーバー
		}
	}

	return false;
}
