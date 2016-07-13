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
	// コンストラクタ
	SuDokuCell() {
		m_candidate.resize(9);
	};

	// コピーコンストラクタ
	SuDokuCell(const SuDokuCell& src) : m_candidate(src.m_candidate) {};

	// デストラクタ
	virtual ~SuDokuCell() {};

	// セルの座標とデータを初期化
	void InitCell(int y, int x) {
		int ofs[] = {3, 156, 309};

		m_sender = 0;

		// データを未確定で初期化
		m_fixed = 0;
		
		// すべての数値が候補状態で初期化
		std::fill(m_candidate.begin(), m_candidate.end(), 1);

		// 表示位置を初期化
		int left = ofs[x / 3] + ((x % 3) * CELL_SIZE);
		int top  = ofs[y / 3] + ((y % 3) * CELL_SIZE);

		SetRect(&m_rc, left, top, left + CELL_SIZE + 1, top + CELL_SIZE + 1);
	};

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

public:
	RECT m_rc;
	int m_sender;
	std::vector<int> m_candidate;
	int m_fixed;
};


class SuDokuGame {
public:
	// デフォルトコンストラクタ
	SuDokuGame();

	// コピーコンストラクタ
	SuDokuGame(const SuDokuGame& src);

	// デストラクタ
	virtual ~SuDokuGame() {};

	// デバッグ用にログ出力
	void DebugPrint();

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

	void ReparseAll() { while (Reparse()) ; };

	// すべてのセルを初期化する
	void InitCells();

	// すべてのセルを初期化する
	void InitCellsFromFile(LPCTSTR fname);

	// 座標から対応するセルへのポインタを返す
	SuDokuCell* PtInCells(POINT pt, int* pX, int* pY);

	// ゲームが終了したかどうか調べる
	bool IsFinished();

	// ゲームが完全に手詰まりかどうか調べる
	bool IsGameover();

	// 代入演算子
	SuDokuGame& operator=(const SuDokuGame& rhs);

	SuDokuCell m_cells[9][9];
	std::vector<RECT> m_rcNums;
	std::vector<int> m_nums;

	// 数独のローカル拡張で、任意のセルグループの中に、数値がかぶらないという
	// ルールを再現するための情報
	//
	// pairでx,y座標を保存、それを 9 個の配列として 1 ブロックを管理
	// そのブロックを複数管理。
	//
	std::vector<std::vector<std::pair<int, int>>> m_exGroups;
};
