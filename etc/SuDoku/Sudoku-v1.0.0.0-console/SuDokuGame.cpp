#include "SuDokuCell.h"
#include <algorithm>
#include <functional>


//-----------------------------------------------------------------------------
// �R�s�[�R���X�g���N�^
//-----------------------------------------------------------------------------
SuDokuGame::SuDokuGame(const SuDokuGame& src) {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			this->m_cells[y][x] = src.m_cells[y][x];
		}
	}

	m_nums = src.m_nums;
	m_exGroups = src.m_exGroups;
}


//---------------------------------------------------------------------
// ������Z�q
//---------------------------------------------------------------------
SuDokuGame& SuDokuGame::operator =(const SuDokuGame& src) {
	if (this == &src) return *this;

	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			this->m_cells[y][x] = src.m_cells[y][x];
		}
	}

	m_nums = src.m_nums;
	m_exGroups = src.m_exGroups;

	return *this;
}


//---------------------------------------------------------------------
// �Z���̒l���m�肳����
//---------------------------------------------------------------------
void SuDokuGame::FixCell(int y, int x, int num, int sender) {
	int yTemp, xTemp;

	// ����s�̌�₩��ԍ����폜���Ă���
	for (xTemp = 0; xTemp < 9; xTemp++) {
		m_cells[y][xTemp].m_candidate[num - 1] = 0;
	}

	// �����̌�₩��ԍ����폜���Ă���
	for (yTemp = 0; yTemp < 9; yTemp++) {
		m_cells[yTemp][x].m_candidate[num - 1] = 0;
	}

	// ����u���b�N�̌�₩��ԍ����폜���Ă���
	for (yTemp = (y / 3) * 3; yTemp < ((y / 3) * 3) + 3; yTemp++) {
		for (xTemp = (x / 3) * 3; xTemp < ((x / 3) * 3) + 3; xTemp++) {
			m_cells[yTemp][xTemp].m_candidate[num - 1] = 0;
		}
	}

	// ����g���u���b�N�̌�₩��ԍ����폜���Ă���
	std::vector<std::vector<std::pair<int, int>>>::size_type nExGroup;
	for (/*int*/ nExGroup = 0; nExGroup < m_exGroups.size(); nExGroup++) {
		//int nPair;
		std::vector<std::vector<std::pair<int, int>>>::size_type nPair;

		// �g���O���[�v�̒��ɍ���̍��W���܂܂�邩�ǂ������ׂ�
		for (nPair = 0; nPair < m_exGroups[nExGroup].size(); nPair++) {
			if ((m_exGroups[nExGroup][nPair].first == (y+1)) && (m_exGroups[nExGroup][nPair].second == (x+1))) {
				break;
			}
		}

		// �g���O���[�v�̒��ɍ���̍��W���܂܂�Ă����ꍇ�A�g���O���[�v�̌�₩��ԍ����폜���Ă���
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
// �Z���̒l�𖢒�ɂ���
//---------------------------------------------------------------------
void SuDokuGame::ResetCell(int y, int x) {
	int yy, xx;

	// ��x���ׂĂ̌��𕜊�������
	for (yy = 0; yy < 9; yy++) {
		for (xx = 0; xx < 9; xx++) {
			std::fill(m_cells[yy][xx].m_candidate.begin(), m_cells[yy][xx].m_candidate.end(), 1);
		}
	}

	// �w�肳�ꂽ�Z���𖢊m��ɂ���
	m_cells[y][x].m_fixed = 0;

	// �m��ς݂̃Z����������x�m�肳����
	for (yy = 0; yy < 9; yy++) {
		for (xx = 0; xx < 9; xx++) {
			if (m_cells[yy][xx].m_fixed != 0) {
				FixCell(yy, xx, m_cells[yy][xx].m_fixed, m_cells[yy][xx].m_sender);
			}
		}
	}
}


//-----------------------------------------------------------------------------
// ��₪������ɂȂ����Z����T���Ċm�肳����
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
// �u���b�N���Ɉ�����̌���T���Ċm�肳����
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
// �������ɂɈ�����̌���T���Ċm�肳����
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
// �c�����ɂɈ�����̌���T���Ċm�肳����
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
// �u���b�N���̐����ŁA��₪ 2 �܂��� 3 �ŁA���̌�₪�c�܂��͉��ɘA�����Ă�ꏊ���Ȃ������ׂ�
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
					// X�����A��
					for (int y = 0; y < 9; y++) {
						if (y != ypos[0] && y != ypos[1] && m_cells[y][xpos[0]].m_candidate[num-1] == 1) {
							m_cells[y][xpos[0]].m_candidate[num-1] = 0;
							chg = 1;
						}
					}
				} else if (ypos[0] == ypos[1]) {
					// Y�����A��
					for (int x = 0; x < 9; x++) {
						if (x != xpos[0] && x != xpos[1] && m_cells[ypos[0]][x].m_candidate[num-1] == 1) {
							m_cells[ypos[0]][x].m_candidate[num-1] = 0;
							chg = 1;
						}
					}
				}
			} else if (cnt == 3) {
				if (xpos[0] == xpos[1] && xpos[1] == xpos[2]) {
					// X�����A��
					for (int y = 0; y < 9; y++) {
						if (y != ypos[0] && y != ypos[1] && y != ypos[2] && m_cells[y][xpos[0]].m_candidate[num-1] == 1) {
							m_cells[y][xpos[0]].m_candidate[num-1] = 0;
							chg = 1;
						}
					}
				} else if (ypos[0] == ypos[1] && ypos[1] == ypos[2]) {
					// Y�����A��
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
// ��₪��̃Z���ŁA�����ߎ��s���Ă݂�
// ���s���ʂ��A�Q�[���I�[�o�[�ɂȂ�Ȃ�A�Q�[���I�[�o�[�ɂȂ�Ȃ��ق��Ŋm��ł���B
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
// �g���u���b�N���ɂЂƂ����̌�₪�Ȃ������ׂ�
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
// ���m��Z���𑖍����Ċm��\�ȃZ�����m�肳����
//
// [�߂�l]
// �m��Z���ɕω����������ꍇ�A1 ��Ԃ�
//---------------------------------------------------------------------
int SuDokuGame::Reparse() {
	// ��₪������ɂȂ����Z����T���Ċm�肳����
	if (Parse01()) return 1;

	// �u���b�N���ɂЂƂ����̌�₪�Ȃ������ׂ�
	if (Parse02()) return 1;

	// �������Ɉ�����̌�₪�Ȃ������ׂ�
	if (Parse03()) return 1;

	// �c�����Ɉ�����̌�₪�Ȃ������ׂ�
	if (Parse04()) return 1;

	// �u���b�N���̐����ŁA��₪ 2 �܂��� 3 �ŁA���̌�₪�c�܂��͉��ɘA�����Ă�ꏊ���Ȃ������ׂ�
	if (Parse05()) return 1;

	// ��₪��̃Z���ŁA�����ߎ��s���Ă݂�
	// ���s���ʂ��A�Q�[���I�[�o�[�ɂȂ�Ȃ�A�Q�[���I�[�o�[�ɂȂ�Ȃ��ق��Ŋm��ł���B
	if (Parse06()) return 1;

	// �g���u���b�N���ɂЂƂ����̌�₪�Ȃ������ׂ�
	if (Parse11()) return 1;

	return 0;
}


//---------------------------------------------------------------------
// ���ׂẴZ��������������
//---------------------------------------------------------------------
void SuDokuGame::InitCells() {
	int x, y;

	for (y = 0; y < 9; y++) {
		for (x = 0; x < 9; x++) {
			m_cells[y][x].InitCell();
		}
	}

	std::fill(m_nums.begin(), m_nums.end(), 0);

	m_exGroups.clear();
}


//-----------------------------------------------------------------------------
// �Q�[�����I���������ǂ������ׂ�
//-----------------------------------------------------------------------------
bool SuDokuGame::IsFinished() {
	// �Ǘ����鐔(9)�ȊO����������B
	// �Ǘ����鐔(9)�ȊO��������Ȃ���΁A�Q�[���͐���ɏI����Ă���B

	return (std::find_if(m_nums.begin(), m_nums.end(), std::bind2nd(std::not_equal_to<int>(), m_nums.size())) == m_nums.end());
}


//-----------------------------------------------------------------------------
// �Q�[�������S�Ɏ�l�܂肩�ǂ������ׂ�
//-----------------------------------------------------------------------------
bool SuDokuGame::IsGameover() {
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			if (m_cells[y][x].m_fixed)
				continue;

			if (m_cells[y][x].GetCandidateCount(NULL) == 0)
				return true;	// �ǂ̌����I�ׂȂ��Ȃ��Ă���Ƃ��́A�Q�[���I�[�o�[
		}
	}

	return false;
}
