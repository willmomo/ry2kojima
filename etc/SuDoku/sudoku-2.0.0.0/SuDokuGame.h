//=====================================================================
//
//=====================================================================
#pragma once

#include <windows.h>
#include <tchar.h>

#include <vector>

#include <stdio.h>

#include "./kjmlib-20121208/kjmlib.h"

#if !defined(_countof)
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#define CELL_SIZE	50

class SuDokuCell {
public:
	// コンストラクタ
	SuDokuCell() : m_candidate(9, 1), m_fixed(0), m_sender(0) {}

	// コピーコンストラクタ
	SuDokuCell(const SuDokuCell& src) : m_candidate(src.m_candidate), m_fixed(src.m_fixed), m_sender(src.m_sender) {}

	// デストラクタ
	virtual ~SuDokuCell() {}

	// セルの座標とデータを初期化
	void InitCell() {
		m_fixed = 0;	// データを未確定
		m_sender = 0;	// 人間が値を設定
		// すべての数値を候補状態で初期化
		std::fill(m_candidate.begin(), m_candidate.end(), 1);
	}

	// 候補の数
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

	// 指定した値が、候補に含まれるかどうか調べる
	// 確定済みのセルのときは、常に false が返る
	bool IsCandidateNum(int number) { return ((m_fixed == 0) && (m_candidate[number - 1] != 0)); };

	bool get_Candidate(int n) { return m_candidate[n]; }

public:
	std::vector<int> m_candidate;
	int m_fixed;
	int m_sender;
};


class SuDokuGame {
public:
	// デフォルトコンストラクタ
	SuDokuGame() : m_nums(9) { InitCells(); }

	// コピーコンストラクタ
	SuDokuGame(const SuDokuGame& src);

	// デストラクタ
	virtual ~SuDokuGame() {};

	// デバッグ用にログ出力
	void DebugPrint();

	// セルの値を取得する
	int get_Value(int row, int col) const {
		return m_cells[row][col].m_fixed;
	}

	// 値を確定させた人を取得する(0=人間)
	int get_Sender(int row, int col) const {
		return m_cells[row][col].m_sender;
	}

	int get_Candidate(int row, int col, int n) {
		return m_cells[row][col].get_Candidate(n);
	}

	int GetCandidateCount(int row, int col, int* candidateNum) {
		return m_cells[row][col].GetCandidateCount(candidateNum);
	}

	int get_Num(int i) {
		return m_nums[i];
	}

	// セルの値を確定させる
	void FixCell(int y, int x, int num, int sender = 0);

	// セルの値を未定にする
	void ResetCell(int y, int x);

	// 未確定セルを走査して確定可能なセルを確定させる
	int Reparse();

	int Parse01();
	int Parse02();
	int Parse03();
	int Parse04();
	int Parse05();
	int Parse06();
	int Parse11();

	void ReparseAll() {
		if ((GetTickCount() - m_reparseStart) > 5000)
			return;

		while (Reparse()) {
			;
		}
	}

	void ReparseStart() {
		m_reparseStart = GetTickCount();
		ReparseAll();
	}

	// すべてのセルを初期化する
	void InitCells();

	// 拡張グループを追加する
	void AddExGroup(const std::vector<std::pair<int, int> >& oneGroup) {
		m_exGroups.push_back(oneGroup);
	}

	// ゲームが終了したかどうか調べる
	bool IsFinished();

	// ゲームが完全に手詰まりかどうか調べる
	bool IsGameover();

	// 代入演算子
	SuDokuGame& operator=(const SuDokuGame& rhs);

private:
	DWORD m_reparseStart;
	SuDokuCell m_cells[9][9];
	std::vector<int> m_nums;

	// 数独のローカル拡張で、任意のセルグループの中に、数値がかぶらないという
	// ルールを再現するための情報
	//
	// pairでx,y座標を保存、それを 9 個の配列として 1 ブロックを管理
	// そのブロックを複数管理。
	//
	std::vector<std::vector<std::pair<int, int>>> m_exGroups;
};


class SuDokuView {
public:
	SuDokuView() : m_rcNums(9) {}

	~SuDokuView() {}

	void InitView() {
		int ofs1[] = {3, 156, 309};

		// 表示位置を初期化
		for (int y = 0; y < 9; y++) {
			for (int x = 0; x < 9; x++) {
				int left = ofs1[x / 3] + ((x % 3) * CELL_SIZE);
				int top  = ofs1[y / 3] + ((y % 3) * CELL_SIZE);

				SetRect(&m_rc[y][x], left, top, left + CELL_SIZE + 1, top + CELL_SIZE + 1);
			}
		}

		int ofs[] = {3, 156, 309, 309 + CELL_SIZE * 3 + 3};
		for (int i = 0; i < 9; i++) {
			// 表示位置を初期化
			int left = ofs[i / 3] + ((i % 3) * CELL_SIZE);
			int top  = ofs[9 / 3] + ((9 % 3) * CELL_SIZE);

			SetRect(&m_rcNums[i], left, top, left + CELL_SIZE + 1, top + CELL_SIZE + 1);
		}
	}


	//---------------------------------------------------------------------
	// 座標から対応するセルへのポインタを返す
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

	const RECT& get_Rect(int row, int col) const { return m_rc[row][col]; }
	RECT get_Rect(int row, int col) { return m_rc[row][col]; }

	const RECT& get_NumRect(int n) const { return m_rcNums[n]; }
	RECT get_NumRect(int n) { return m_rcNums[n]; }

private:
	RECT m_rc[9][9];	// 各セルの表示位置
	std::vector<RECT> m_rcNums;
};
