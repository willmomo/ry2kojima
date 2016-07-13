//=====================================================================
//
//=====================================================================
#pragma once

#include <windows.h>
#include <tchar.h>

#include <vector>

#include <stdio.h>

#if !defined(_countof)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#define CELL_SIZE	50

class SuDokuCell {
public:
	// �R���X�g���N�^
	SuDokuCell() : m_candidate(9, 1), m_fixed(0), m_sender(0) {}

	// �R�s�[�R���X�g���N�^
	SuDokuCell(const SuDokuCell& src) : m_candidate(src.m_candidate), m_fixed(src.m_fixed), m_sender(src.m_sender) {}

	// �f�X�g���N�^
	virtual ~SuDokuCell() {}

	// �Z���̍��W�ƃf�[�^��������
	void InitCell() {
		m_fixed = 0;	// �f�[�^�𖢊m��
		m_sender = 0;	// �l�Ԃ��l��ݒ�
		// ���ׂĂ̐��l������Ԃŏ�����
		std::fill(m_candidate.begin(), m_candidate.end(), 1);
	}

	// ���̐�
	int GetCandidateCount(int* candidateNum) {
		if (this->m_fixed != 0) {
			return 0;
		}
		int nCnt = 0;
		for (int i = 0; i < 9; i++) {
			if (m_candidate[i] != 0) {
				++nCnt;
				if (candidateNum) *candidateNum = i + 1;
			}
		}
		return nCnt;
	};

	// �w�肵���l���A���Ɋ܂܂�邩�ǂ������ׂ�
	// �m��ς݂̃Z���̂Ƃ��́A��� false ���Ԃ�
	bool IsCandidateNum(int number) { return ((m_fixed == 0) && (m_candidate[number - 1] != 0)); };

public:
	std::vector<int> m_candidate;
	int m_fixed;
	int m_sender;
};


class SuDokuGame {
public:
	// �f�t�H���g�R���X�g���N�^
	SuDokuGame() : m_nums(9) { InitCells(); }

	// �R�s�[�R���X�g���N�^
	SuDokuGame(const SuDokuGame& src);

	// �f�X�g���N�^
	virtual ~SuDokuGame() {};

	// �f�o�b�O�p�Ƀ��O�o��
	void DebugPrint();

	// �Z���̒l���擾����
	int get_Value(int row, int col) const {
		return m_cells[row][col].m_fixed;
	}

	// �l���m�肳�����l���擾����(0=�l��)
	int get_Sender(int row, int col) const {
		return m_cells[row][col].m_sender;
	}

	// �Z���̒l���m�肳����
	void FixCell(int y, int x, int num, int sender = 0);

	// �Z���̒l�𖢒�ɂ���
	void ResetCell(int y, int x);

	// ���m��Z���𑖍����Ċm��\�ȃZ�����m�肳����
	int Reparse();

	int Parse01();
	int Parse02();
	int Parse03();
	int Parse04();
	int Parse05();
	int Parse06();
	int Parse11();

	void ReparseAll() { while (Reparse()) ; };

	// ���ׂẴZ��������������
	void InitCells();

	// �g���O���[�v��ǉ�����
	void AddExGroup(const std::vector<std::pair<int, int> >& oneGroup) {
		m_exGroups.push_back(oneGroup);
	}

	// �Q�[�����I���������ǂ������ׂ�
	bool IsFinished();

	// �Q�[�������S�Ɏ�l�܂肩�ǂ������ׂ�
	bool IsGameover();

	// ������Z�q
	SuDokuGame& operator=(const SuDokuGame& rhs);

private:
	SuDokuCell m_cells[9][9];
	std::vector<int> m_nums;

	// ���Ƃ̃��[�J���g���ŁA�C�ӂ̃Z���O���[�v�̒��ɁA���l�����Ԃ�Ȃ��Ƃ���
	// ���[�����Č����邽�߂̏��
	//
	// pair��x,y���W��ۑ��A����� 9 �̔z��Ƃ��� 1 �u���b�N���Ǘ�
	// ���̃u���b�N�𕡐��Ǘ��B
	//
	std::vector<std::vector<std::pair<int, int>>> m_exGroups;
};


class SuDokuView {
public:
	SuDokuView() : m_rcNums(9) {}

	~SuDokuView() {}

	void InitView() {
		int ofs1[] = {3, 156, 309};

		// �\���ʒu��������
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				int left = ofs1[x / 3] + ((x % 3) * CELL_SIZE);
				int top  = ofs1[y / 3] + ((y % 3) * CELL_SIZE);

				SetRect(&m_rc[y][x], left, top, left + CELL_SIZE + 1, top + CELL_SIZE + 1);
			}
		}

		int ofs[] = {3, 156, 309, 309 + CELL_SIZE * 3 + 3};
		for (int i = 0; i < 9; i++) {
			// �\���ʒu��������
			int left = ofs[i / 3] + ((i % 3) * CELL_SIZE);
			int top  = ofs[9 / 3] + ((9 % 3) * CELL_SIZE);

			SetRect(&m_rcNums[i], left, top, left + CELL_SIZE + 1, top + CELL_SIZE + 1);
		}
	}


	//---------------------------------------------------------------------
	// ���W����Ή�����Z���ւ̃|�C���^��Ԃ�
	//---------------------------------------------------------------------
	bool PtInCells(POINT pt, int* pX, int* pY) {
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				if (PtInRect(&m_rc[y][x], pt)) {
					*pY = y;
					*pX = x;
					return true;
				}
			}
		}

		return false;
	}

private:
	RECT m_rc[9][9];	// �e�Z���̕\���ʒu
	std::vector<RECT> m_rcNums;
};
