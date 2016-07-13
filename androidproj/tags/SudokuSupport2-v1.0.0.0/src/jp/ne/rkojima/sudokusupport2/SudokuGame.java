package jp.ne.rkojima.sudokusupport2;

import java.util.ArrayList;

import android.util.Log;

public class SudokuGame {
	private int m_baseNum;
	private ArrayList<SudokuBoard> m_boardStack;
	private SudokuBoard m_board;

	// �R���X�g���N�^
	public SudokuGame() {
		m_baseNum = 3;
		m_boardStack = new ArrayList<SudokuBoard>();
		m_board = new SudokuBoard(m_baseNum);
	}
	
	// ���݂̔Ֆʂ��擾
	public SudokuBoard getBoard() {
		return this.m_board;
	}
	
	/** �Ֆʂ̃X�^�b�N����Ԃ� */
	public int getBoardCount() {
		return m_boardStack.size();
	}
	
	// ���݂̔Ֆʂ��X�^�b�N�ɕۑ�
	public void pushBoard() {
		m_boardStack.add(0, new SudokuBoard(this.m_board));
	}

	// �����Ƃ��V�����Ֆʂ��X�^�b�N���畜��
	public void popBoard() {
		m_board = m_boardStack.get(0);
		m_boardStack.remove(0);
	}

	public int blockCellCount() {
		return this.m_baseNum * this.m_baseNum;
	}
	
	/**
	 * �Q�[���̏�����
	 */
	public void init() {
		this.m_board.init();
		this.m_boardStack.clear();
	}

	/**
	 * ��₪������Ȃ��Z����T���A���̒l�Ŋm�肳����B
	 * @return �Ֆʂ��ω������Ƃ��́Atrue��Ԃ��B�ω����������́Afalse��Ԃ��B
	 */
	public boolean parse00() {
		for (int row = 0; row < this.blockCellCount(); row++) {
			for (int col = 0; col < this.blockCellCount(); col++) {
				SudokuCell cell = this.m_board.getCell(row, col);
				if (cell.getCandidates().size() == 1) {
					this.m_board.fixNumber(row, col, cell.getCandidates().get(0));
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * �������O���[�v�̒��ŁA���Ƃ��Ĉ�����o�Ă��Ȃ��l��T���m�肳����B
	 * @return �Ֆʂ��ω������Ƃ��́Atrue��Ԃ��B�ω����������́Afalse��Ԃ��B
	 */
	public boolean parse01() {
		for (int row = 0; row < this.blockCellCount(); row++) {
			for (int num = 1; num <= this.blockCellCount(); num++) {
				int cnt = 0;
				int foundCol = 0;
				for (int col = 0; col < this.blockCellCount(); col++) {
					if (this.m_board.getCell(row, col).getCandidates().indexOf(num) >= 0) {
						cnt++;
						foundCol = col;	// �Č������Ȃ��Ă��ǂ��悤�ɁA�������ꏊ���o���Ă���
					}
				}
				
				if (cnt == 1) {
					this.m_board.fixNumber(row, foundCol, num);
					return true;
				}
			}
		}
		return false;
	}

	/**
	 * �c�����O���[�v�̒��ŁA���Ƃ��Ĉ�����o�Ă��Ȃ��l��T���m�肳����B
	 * @return �Ֆʂ��ω������Ƃ��́Atrue��Ԃ��B�ω����������́Afalse��Ԃ��B
	 */
	public boolean parse02() {
		for (int col = 0; col < this.blockCellCount(); col++) {
			for (int num = 1; num <= this.blockCellCount(); num++) {
				int cnt = 0;
				int foundRow = 0;
				for (int row = 0; row < this.blockCellCount(); row++) {
					if (this.m_board.getCell(row, col).getCandidates().indexOf(num) >= 0) {
						cnt++;
						foundRow = row;
					}
				}
				
				if (cnt == 1) {
					this.m_board.fixNumber(foundRow, col, num);
					return true;
				}
			}
		}
		return false;
	}
	
	/**
	 * �u���b�N�O���[�v�̒��ŁA���Ƃ��Ĉ�����o�Ă��Ȃ��l��T���m�肳����B
	 * @return �Ֆʂ��ω������Ƃ��́Atrue��Ԃ��B�ω����������́Afalse��Ԃ��B
	 */
	public boolean parse03() {
		for (int blk = 0; blk < this.blockCellCount(); blk++) {
			for (int num = 0; num <= this.blockCellCount(); num++) {
				int cnt = 0;
				int foundRow = 0;
				int foundCol = 0;
				
				int topRow = (int) (Math.floor(blk / this.m_baseNum) * this.m_baseNum);
				int topCol = (blk % this.m_baseNum) * this.m_baseNum;
				
				for (int row = topRow; row < topRow + this.m_baseNum; row++) {
					for (int col = topCol; col < topCol + this.m_baseNum; col++) {
						if (this.m_board.getCell(row, col).getCandidates().indexOf(num) >= 0) {
							cnt++;
							foundRow = row;
							foundCol = col;
						}
					}
				}
				
				if (cnt == 1) {
					this.m_board.fixNumber(foundRow, foundCol, num);
					return true;
				}
			}
		}
		return false;
	}
	
	// �u���b�N���Ō�₪������Ȃ��Arow�܂���col�����ɕ���ł���ꍇ
	// ���u���b�N�� row �܂��� col ����A�폜����
	private int countNumInBlock(int blk, int num, ArrayList<Integer> rowList, ArrayList<Integer> colList) {
		int count = 0;
		
		for (int row = (int) (Math.floor(blk / this.m_baseNum) * this.m_baseNum); row < Math.floor(blk / this.m_baseNum) * this.m_baseNum + this.m_baseNum; row++) {
			for (int col = (blk % this.m_baseNum) * this.m_baseNum; col < (blk % this.m_baseNum) * this.m_baseNum + this.m_baseNum; col++) {
				if (this.m_board.getCell(row, col).getCandidates().indexOf(num) >= 0) {
					++count;
					rowList.add(row);
					colList.add(col);
				}
			}
		}
		return count;
	}

	/**
	 * ArrayList<Integer>�̗v�f���A���ׂē����l���ǂ������ׂ�B
	 * @param ar
	 * @return ���ׂē����l�̎��́Atrue��Ԃ��B��ł�����Ă���΁Afalse��Ԃ��B
	 */
	private boolean sameAll(ArrayList<Integer> ar) {
		if (ar.size() <= 1) {
			return true;
		}
		for (int i = 1; i < ar.size(); i++) {
			if (ar.get(0) != ar.get(i)) {
				return false;
			}
		}
		return true;
	}

	/**
	 * 
	 * @return
	 */
	public boolean parse12() {
		boolean result = false;
		
		for (int blk = 0; blk < this.blockCellCount(); blk++) {
			for (int num = 1; num <= this.blockCellCount(); num++) {
				ArrayList<Integer> rowList = new ArrayList<Integer>();
				ArrayList<Integer> colList = new ArrayList<Integer>();
				int count = this.countNumInBlock(blk, num, rowList, colList);
				
				if (count == 2 || count == 3) {
					if (sameAll(rowList)) {
						for (int col = 0; col < this.blockCellCount(); col++) {
							if (colList.indexOf(col) == -1) {
								if (this.m_board.getCell(rowList.get(0), col).getCandidates().indexOf(num) >= 0) {
									this.m_board.getCell(rowList.get(0), col).removeCandidate(num);
									result = true;
								}
							}
						}
					} else if (sameAll(colList)) {
						for (int row = 0; row < this.blockCellCount(); row++) {
							if (rowList.indexOf(row) == -1) {
								if (this.m_board.getCell(row, colList.get(0)).getCandidates().indexOf(num) >= 0) {
									this.m_board.getCell(row, colList.get(0)).removeCandidate(num);
									result = true;
								}
							}
						}
					} else {
					}
				}
			}
		}
		
		return result;
	}
	
	class FindBlockResult {
		public int m_count = 0;
		public int m_findRow1 = -1;
		public int m_findCol1 = -1;
		public int m_findRow2 = -1;
		public int m_findCol2 = -1;
		
		public FindBlockResult() {
		}
		
		public FindBlockResult(int count, int findRow1, int findCol1, int findRow2, int findCol2) {
			m_count = count;
			m_findRow1 = findRow1;
			m_findCol1 = findCol1;
			m_findRow2 = findRow2;
			m_findCol2 = findCol2;
		}
	}
	
	// ���m��Z����������Ȃ��u���b�N��T��
	private FindBlockResult findBlock() {
		int r1 = 0, c1 = 0, r2 = 0, c2 = 0;
		for (int blk = 0; blk < this.blockCellCount(); blk++) {
			int count = 0;
			int row_top = (int) (Math.floor(blk / this.m_baseNum) * this.m_baseNum);
			int col_top = (blk % this.m_baseNum) * this.m_baseNum;
			for (int row = row_top; row < row_top + this.m_baseNum; row++) {
				for (int col = col_top; col < col_top + this.m_baseNum; col++) {
					if (this.m_board.getCell(row, col).getNumber() == 0) {
						if (count == 0) {
							r1 = row;
							c1 = col;
							++count;
						} else if (count == 1) {
							r2 = row;
							c2 = col;
							++count;
						} else {
							++count;
							break;
						}
					}
				}
			}
			if (count == 2) {
				return new FindBlockResult(count, r1, c1, r2, c2);
			}
		}
		return new FindBlockResult();
	}

	/**
	 * �u���b�N�̒��ŁA���m��Z������̂��̂�T���Ă݂Ȃ��T������
	 * @return
	 */
	public boolean parse30() {
		FindBlockResult res = this.findBlock();
		if (res.m_count == 2) {
			int saveRow = res.m_findRow1;
			int saveCol = res.m_findCol1;
			
			Log.d("SS2", String.format("parse30 %d,%d ���ґ���", saveRow, saveCol));
			
			SudokuGame t1 = new SudokuGame();
			t1.m_board = new SudokuBoard(this.m_board);
			t1.m_board.fixNumber(saveRow, saveCol, t1.m_board.getCell(saveRow, saveCol).getCandidates().get(0));
			while (t1.parse00() ||
				   t1.parse01() ||
				   t1.parse02() ||
				   t1.parse03() ||
				   t1.parse12()) ;

			if (t1.m_board.isFinished()) {
				this.m_board.fixNumber(saveRow, saveCol, this.m_board.getCell(saveRow, saveCol).getCandidates().get(0));
				return true;
			} else if (t1.m_board.isFailed()) {
				this.m_board.fixNumber(saveRow, saveCol, this.m_board.getCell(saveRow, saveCol).getCandidates().get(1));
				return true;
			}
			
			SudokuGame t2 = new SudokuGame();
			t2.m_board = new SudokuBoard(this.m_board);
			t2.m_board.fixNumber(saveRow, saveCol, t2.m_board.getCell(saveRow,  saveCol).getCandidates().get(1));
			while (t2.parse00() || t2.parse01() || t2.parse02() || t2.parse03() || t2.parse12());
			if (t2.m_board.isFinished()) {
				this.m_board.fixNumber(saveRow,  saveCol, this.m_board.getCell(saveRow, saveCol).getCandidates().get(1));
				return true;
			} else if (t2.m_board.isFailed()) {
				this.m_board.fixNumber(saveRow,  saveCol, this.m_board.getCell(saveRow, saveCol).getCandidates().get(0));
				return true;
			}
			return false;
		}
		return false;
	}

	/**
	 * ���u���b�N�̒��ŁA���m��Z������̂��̂�T���Ă݂Ȃ��T������
	 * @return
	 */
	public boolean parse31() {
		for (int row = 0; row < 9; row++) {
			int count = 0;
			int saveCol = -1;
			for (int col = 0; col < 9; col++) {
				if (this.m_board.getCell(row, col).getNumber() == 0) {
					count++;
					saveCol = col;
				}
			}
			if (count == 2) {
				Log.d("SS2", String.format("parse31: row %d ���ґ���", row));
				
				SudokuGame t1 = new SudokuGame();
				t1.m_board = new SudokuBoard(this.m_board);
				t1.m_board.fixNumber(row, saveCol, t1.m_board.getCell(row, saveCol).getCandidates().get(0));
				while (t1.parse00() ||
					   t1.parse01() ||
					   t1.parse02() ||
					   t1.parse03() ||
					   t1.parse12()) ;

				if (t1.m_board.isFinished()) {
					this.m_board.fixNumber(row, saveCol, this.m_board.getCell(row, saveCol).getCandidates().get(0));
					return true;
				} else if (t1.m_board.isFailed()) {
					this.m_board.fixNumber(row, saveCol, this.m_board.getCell(row, saveCol).getCandidates().get(1));
					return true;
				}
				
				SudokuGame t2 = new SudokuGame();
				t2.m_board = new SudokuBoard(this.m_board);
				t2.m_board.fixNumber(row, saveCol, t2.m_board.getCell(row,  saveCol).getCandidates().get(1));
				while (t2.parse00() || t2.parse01() || t2.parse02() || t2.parse03() || t2.parse12());
				if (t2.m_board.isFinished()) {
					this.m_board.fixNumber(row,  saveCol, this.m_board.getCell(row, saveCol).getCandidates().get(1));
					return true;
				} else if (t2.m_board.isFailed()) {
					this.m_board.fixNumber(row,  saveCol, this.m_board.getCell(row, saveCol).getCandidates().get(0));
					return true;
				}
				return false;
			}
		}
		return false;
	}

	
	/**
	 * �c�u���b�N�̒��ŁA���m��Z������̂��̂�T���Ă݂Ȃ��T������
	 * @return
	 */
	public boolean parse32() {
		for (int col = 0; col < 9; col++) {
			int count = 0;
			int saveRow = -1;
			for (int row = 0; row < 9; row++) {
				if (this.m_board.getCell(row, col).getNumber() == 0) {
					count++;
					saveRow = row;
				}
			}
			if (count == 2) {
				Log.d("SS2", String.format("parse32: col %d ���ґ���", col));
				
				SudokuGame t1 = new SudokuGame();
				t1.m_board = new SudokuBoard(this.m_board);
				t1.m_board.fixNumber(saveRow, col, t1.m_board.getCell(saveRow, col).getCandidates().get(0));
				while (t1.parse00() ||
					   t1.parse01() ||
					   t1.parse02() ||
					   t1.parse03() ||
					   t1.parse12()) ;

				if (t1.m_board.isFinished()) {
					this.m_board.fixNumber(saveRow, col, this.m_board.getCell(saveRow, col).getCandidates().get(0));
					return true;
				} else if (t1.m_board.isFailed()) {
					this.m_board.fixNumber(saveRow, col, this.m_board.getCell(saveRow, col).getCandidates().get(1));
					return true;
				}
				
				SudokuGame t2 = new SudokuGame();
				t2.m_board = new SudokuBoard(this.m_board);
				t2.m_board.fixNumber(saveRow, col, t2.m_board.getCell(saveRow, col).getCandidates().get(1));
				while (t2.parse00() || t2.parse01() || t2.parse02() || t2.parse03() || t2.parse12());
				if (t2.m_board.isFinished()) {
					this.m_board.fixNumber(saveRow, col, this.m_board.getCell(saveRow, col).getCandidates().get(1));
					return true;
				} else if (t2.m_board.isFailed()) {
					this.m_board.fixNumber(saveRow, col, this.m_board.getCell(saveRow, col).getCandidates().get(0));
					return true;
				}
				return false;
			}
		}
		return false;
	}
}

class SudokuBoard {
	private int m_baseNum;
	private ArrayList<ArrayList<SudokuCell>> m_cells;

	// �R���X�g���N�^
	public SudokuBoard(int baseNumber) {
		this.m_baseNum = baseNumber;
		this.m_cells = new ArrayList<ArrayList<SudokuCell>>();
		this.init();
	}

	// �R�s�[�R���X�g���N�^
	public SudokuBoard(SudokuBoard src) {
		this.m_baseNum = src.m_baseNum;
		this.m_cells = new ArrayList<ArrayList<SudokuCell>>();
		for (int row = 0; row < src.m_cells.size(); row++) {
			ArrayList<SudokuCell> theRow = new ArrayList<SudokuCell>();
			for (int col = 0; col < src.m_cells.get(row).size(); col++) {
				theRow.add(new SudokuCell(src.getCell(row, col)));
			}
			this.m_cells.add(theRow);
		}
	}

	// �s�Ɨ�ɑΉ�����u�Z���v���擾
	// row:0�` col:0�`
	public SudokuCell getCell(int row, int col) {
		return this.m_cells.get(row).get(col);
	}
	
	public int blockCellCount() {
		return this.m_baseNum * this.m_baseNum;
	}

	// �{�[�h������������
	public void init() {
		this.m_cells.clear();
		for (int row = 0; row < this.blockCellCount(); row++) {
			ArrayList<SudokuCell> theRow = new ArrayList<SudokuCell>();
			for (int col = 0; col < this.blockCellCount(); col++) {
				theRow.add(new SudokuCell(this.blockCellCount()));
			}
			this.m_cells.add(theRow);
		}
	}

	// ���ׂẴZ�����m�肵�����ǂ����𒲂ׂ�
	public boolean isFinished() {
		for (int row = 0; row < this.blockCellCount(); row++) {
			for (int col = 0; col < this.blockCellCount(); col++) {
				if (this.getCell(row, col).getNumber() == 0) {
					return false;
				}
			}
		}
		return true;
	}

	// ���m��Ȃ̂ɁA��₪���݂��Ȃ��Z���𒲂ׂ�
	public boolean isFailed() {
		for (int row = 0; row < this.blockCellCount(); row++) {
			for (int col = 0; col < this.blockCellCount(); col++) {
				SudokuCell cell = this.getCell(row, col);
				if (cell.getNumber() == 0 && cell.getCandidates().isEmpty() == true) {
					return true;
				}
			}
		}
		return false;
	}
	
	// �w��Z�����m�肷��B
	// �w�萔�l���A�֘A�Z���̌�₩��A�O���B
	public void fixNumber(int row, int col, int num) {
		// �w�萔�l���Z���̌��ɂȂ���΁A���������ɏI��
		if (this.getCell(row, col).getCandidates().indexOf(num) == -1) {
			return;
		}
		
		// ���l���m��
		this.getCell(row, col).fixNumber(num);
		
		// �����s�̃Z���̌�₩���菜��
		for (int c = 0; c < this.blockCellCount(); c++) {
			this.getCell(row, c).removeCandidate(num);
		}

		// ������̃Z���̌�₩���菜��
		for (int r = 0; r < this.blockCellCount(); r++) {
			this.getCell(r, col).removeCandidate(num);
		}

		// �����u���b�N���̃Z���̌�₩���菜��
		int row_top = (int) Math.floor(row / this.m_baseNum) * this.m_baseNum;
		int col_top = (int) Math.floor(col / this.m_baseNum) * this.m_baseNum;
		for (int r = row_top; r < row_top + this.m_baseNum; r++) {
			for (int c = col_top; c < col_top + this.m_baseNum; c++) {
				this.getCell(r, c).removeCandidate(num);
			}
		}
	}
}

//���Ƃ̃Z�������\���N���X
class SudokuCell {
	private int m_number;
	private ArrayList<Integer> m_candidates;
	
	// �R���X�g���N�^
	public SudokuCell(int candidateCount) {
		this.m_number = 0;
		this.m_candidates = new ArrayList<Integer>();
		for (int i = 0; i < candidateCount; i++) {
			this.m_candidates.add(i + 1);
		}
	}

	// �R�s�[�R���X�g���N�^
	public SudokuCell(SudokuCell src) {
		this.m_number = src.m_number;
		this.m_candidates = new ArrayList<Integer>(src.m_candidates);
	}

	// �Z���̐��l���擾
	public int getNumber() {
		return m_number;
	}

	// ��␔������擾
	public ArrayList<Integer> getCandidates() {
		return this.m_candidates;
	}

	// �Z�����w�萔�l�Ŋm�肳����
	public void fixNumber(int number) {
		this.m_number = number;
		this.m_candidates.clear();
	}
	
	// �w�萔�l����₩���菜��
	public void removeCandidate(int number) {
		int index = this.m_candidates.indexOf(number);
		if (index >= 0) {
			this.m_candidates.remove(index);
		}
	}
}
