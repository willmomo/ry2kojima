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
	SuDokuCell() {
		m_candidate.resize(9);
	};

	// �R�s�[�R���X�g���N�^
	SuDokuCell(const SuDokuCell& src) : m_candidate(src.m_candidate) {};

	// �f�X�g���N�^
	virtual ~SuDokuCell() {};

	// �Z���̍��W�ƃf�[�^��������
	void InitCell(int y, int x) {
		int ofs[] = {3, 156, 309};

		m_sender = 0;

		// �f�[�^�𖢊m��ŏ�����
		m_fixed = 0;
		
		// ���ׂĂ̐��l������Ԃŏ�����
		std::fill(m_candidate.begin(), m_candidate.end(), 1);

		// �\���ʒu��������
		int left = ofs[x / 3] + ((x % 3) * CELL_SIZE);
		int top  = ofs[y / 3] + ((y % 3) * CELL_SIZE);

		SetRect(&m_rc, left, top, left + CELL_SIZE + 1, top + CELL_SIZE + 1);
	};

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
	RECT m_rc;
	int m_sender;
	std::vector<int> m_candidate;
	int m_fixed;
};


class SuDokuGame {
public:
	// �f�t�H���g�R���X�g���N�^
	SuDokuGame();

	// �R�s�[�R���X�g���N�^
	SuDokuGame(const SuDokuGame& src);

	// �f�X�g���N�^
	virtual ~SuDokuGame() {};

	// �f�o�b�O�p�Ƀ��O�o��
	void DebugPrint();

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

	// ���ׂẴZ��������������
	void InitCellsFromFile(LPCTSTR fname);

	// ���W����Ή�����Z���ւ̃|�C���^��Ԃ�
	SuDokuCell* PtInCells(POINT pt, int* pX, int* pY);

	// �Q�[�����I���������ǂ������ׂ�
	bool IsFinished();

	// �Q�[�������S�Ɏ�l�܂肩�ǂ������ׂ�
	bool IsGameover();

	// ������Z�q
	SuDokuGame& operator=(const SuDokuGame& rhs);

	SuDokuCell m_cells[9][9];
	std::vector<RECT> m_rcNums;
	std::vector<int> m_nums;

	// ���Ƃ̃��[�J���g���ŁA�C�ӂ̃Z���O���[�v�̒��ɁA���l�����Ԃ�Ȃ��Ƃ���
	// ���[�����Č����邽�߂̏��
	//
	// pair��x,y���W��ۑ��A����� 9 �̔z��Ƃ��� 1 �u���b�N���Ǘ�
	// ���̃u���b�N�𕡐��Ǘ��B
	//
	std::vector<std::vector<std::pair<int, int>>> m_exGroups;
};
