//---------------------------------------------------------------------
// Array クラスの拡張
//---------------------------------------------------------------------

// Array クラスの複製
Array.prototype.clone = function() {
	var dst = [];
	for (var i = 0; i < this.length; i++) {
		if (this[i] instanceof Array) {
			dst[i] = this[i].clone();
		} else if (this[i] instanceof SudokuCell) {
			dst[i] = this[i].clone();
		} else {
			dst[i] = this[i];
		}
	}
	return dst;
}

// 要素がすべて同じかどうか調べる
Array.prototype.sameAll = function() {
	if (this.length <= 1) return true;
	for (var i = 1; i < this.length; i++) {
		if (this[0] != this[i]) return false;
	}
	return true;
}

//---------------------------------------------------------------------
// 数独CELLクラス
//---------------------------------------------------------------------
function SudokuCell() {
	this.number = 0;
	this.candidates = [1, 2, 3, 4, 5, 6, 7, 8, 9];
}

// SudokuCell クラスの各種メンバ関数
SudokuCell.prototype = {
	// 自分と同じクラスを複製する
	clone : function() {
		var cpy = new SudokuCell();
		cpy.number = this.number;
		cpy.candidates = this.candidates.clone();
		return cpy;
	},
	// セルの数値を確定する
	fixNumber_ : function(number_) {
		this.number = number_;
		this.candidates = [];
	},
	// セルから候補を削除する
	removeCandidate : function(number_) {
		var index = this.candidates.indexOf(number_);
		if (index >= 0) {
			this.candidates.splice(index, 1);
		}
	}
}

//---------------------------------------------------------------------
// 数独BOARDクラス
//---------------------------------------------------------------------
function SudokuBoard() {
	this.clone = function() {
		var cpy = new SudokuBoard();
		cpy.cells = this.cells.clone();
		return cpy;
	}
	
	this.init = function() {
		this.cells = [
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()],
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()],
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()],
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()],
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()],
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()],
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()],
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()],
			[new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell(), new SudokuCell()]
		];
	}

	this.isFinished = function() {
		for (var row = 0; row < 9; row++) {
			for (var col = 0; col < 9; col++) {
				if (this.cells[row][col].number == 0) {
					return false;
				}
			}
		}
		return true;
	}
	
	this.isFailed = function() {
		for (var row = 0; row < 9; row++) {
			for (var col = 0; col < 9; col++) {
				if (this.cells[row][col].number == 0 && this.cells[row][col].candidates.length == 0) {
					return true;
				}
			}
		}
		return false;
	}
	
	this.fixNumber = function(rrr, ccc, num) {
		rrr = parseInt(rrr);
		ccc = parseInt(ccc);
		num = parseInt(num);
		
		if (this.cells[rrr][ccc].candidates.indexOf(num) == -1) {
			//console.log('(' + rrr + ',' + ccc + ') ' + num + ' は、入力できません。');
			return;
		}

		this.cells[rrr][ccc].fixNumber_(num);

		// 行から候補文字列を削除
		for (var col = 0; col < 9; col++) {
			this.cells[rrr][col].removeCandidate(num);
		}

		// 列から候補文字列を削除
		for (var row = 0; row < 9; row++) {
			this.cells[row][ccc].removeCandidate(num);
		}

		// ブロック内から候補文字列を削除
		row_top = Math.floor(rrr / 3) * 3;
		col_top = Math.floor(ccc / 3) * 3;
		for (var row = row_top; row < row_top + 3; row++) {
			for (var col = col_top; col < col_top + 3; col++) {
				this.cells[row][col].removeCandidate(num);
			}
		}
	}
}

//---------------------------------------------------------------------
// 数独クラス
//---------------------------------------------------------------------
function SudokuGame() {
	this.boardStack = [];
	this.board = new SudokuBoard();
	
	this.init = function() {
		this.board.init();
		this.boardStack = [];
	}

	this.serialize = function() {
		var result = '';
		for (var row = 0; row < 9; row++) {
			for (var col = 0; col < 9; col++) {
				result += String(this.board.cells[row][col].number);
			}
		}
		return result;
	}
	
	this.deserialize = function(data) {
		var ar = data.split('');
		for (var row = 0; row < 9; row++) {
			for (var col = 0; col < 9; col++) {
				if (ar[row * 9 + col] > 0) {
					this.saveGame();
					this.board.fixNumber(row, col, ar[row * 9 + col]);
				}
			}
		}
	}
	
	this.saveGame = function() {
		var cpy = this.board.clone();
		this.boardStack.push(cpy);
	}
	
	this.loadGame = function() {
		if (this.boardStack.length > 0) {
			this.board = this.boardStack[this.boardStack.length - 1];
			this.boardStack.pop();
		}
	}
	
	// 候補がひとつしかないセルを確定する
	this.parse00 = function() {
		for (var row = 0; row < 9; row++) {
			for (var col = 0; col < 9; col++) {
				if (this.board.cells[row][col].candidates.length == 1) {
					this.board.fixNumber(row, col, this.board.cells[row][col].candidates[0]);
					return true;
				}
			}
		}
		return false;
	}
	
	// 横方向グループ内で、候補がひとつしかない数字を確定させる
	this.parse01 = function() {
		for (var row = 0; row < 9; row++) {
			for (var num = 1; num <= 9; num++) {
				var cnt = 0;
				var foundCol = 0;
				for (var col = 0; col < 9; col++) {
					if (this.board.cells[row][col].candidates.indexOf(num) >= 0) {
						cnt++
						foundCol = col;
					}
				}

				if (cnt == 1) {
					this.board.fixNumber(row, foundCol, num);
					return true;
				}
			}
		}
		return false;
	}

	// 横方向グループ内で、候補がひとつしかない数字を確定させる
	this.parse02 = function() {
		for (var col = 0; col < 9; col++) {
			for (var num = 1; num <= 9; num++) {
				var cnt = 0;
				var foundRow = 0;
				for (var row = 0; row < 9; row++) {
					if (this.board.cells[row][col].candidates.indexOf(num) >= 0) {
						cnt++
						foundRow = row;
					}
				}

				if (cnt == 1) {
					this.board.fixNumber(foundRow, col, num);
					return true;
				}
			}
		}
		return false;
	}

	// ブロックグループ内で、候補がひとつしかない数字を確定させる
	this.parse03 = function() {
		for (var blk = 0; blk < 9; blk++) {
			for (var num = 1; num <= 9; num++) {
				var cnt = 0;
				var foundRow = 0;
				var foundCol = 0;
				for (var row = Math.floor(blk / 3) * 3; row < Math.floor(blk / 3) * 3 + 3; row++) {
					for (var col = (blk % 3) * 3; col < (blk % 3) * 3 + 3; col++) {
						if (this.board.cells[row][col].candidates.indexOf(num) >= 0) {
							cnt++
							foundRow = row;
							foundCol = col;
						}
					}
				}

				if (cnt == 1) {
					this.board.fixNumber(foundRow, foundCol, num);
					return true;
				}
			}
		}
		return false;
	}
	
	// ブロック内で候補が二個しかなく、rowまたはcol方向に並んでいる場合
	// 他ブロックの row または col から、削除する
	
	this.countNumInBlock = function(blk, num, rowList, colList) {
		var debug = [];
		var count = 0;
		
		for (var row = Math.floor(blk / 3) * 3; row < Math.floor(blk / 3) * 3 + 3; row++) {
			for (var col = (blk % 3) * 3; col < (blk % 3) * 3 + 3; col++) {
				if (this.board.cells[row][col].candidates.indexOf(num) >= 0) {
					++count;
					rowList.push(row);
					colList.push(col);
				}
			}
		}
		return count;
	}

	this.parse12 = function() {
		var result = false;
		
		for (var blk = 0; blk < 9; blk++) {
			for (var num = 1; num <= 9; num++) {
				var rowList = [];
				var colList = [];
				var count = this.countNumInBlock(blk, num, rowList, colList);
				
				if (count == 2 || count == 3) {
					if (rowList.sameAll()) {
						for (var col = 0; col < 9; col++) {
							if (colList.indexOf(col) == -1) {
								if (this.board.cells[rowList[0]][col].candidates.indexOf(num) >= 0) {
									this.board.cells[rowList[0]][col].removeCandidate(num);
									result = true;
								}
							}
						}
					} else if (colList.sameAll()) {
						for (var row = 0; row < 9; row++) {
							if (rowList.indexOf(row) == -1) {
								if (this.board.cells[row][colList[0]].candidates.indexOf(num) >= 0) {
									this.board.cells[row][colList[0]].removeCandidate(num);
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
	
	// 未確定セルが二つしかないブロックを探す
	this.findBlock = function() {
		var r1, c1, r2, c2;
		for (var blk = 0; blk < 9; blk++) {
			var count = 0;
			for (var row = Math.floor(blk / 3) * 3; row < Math.floor(blk / 3) * 3 + 3; row++) {
				for (var col = (blk % 3) * 3; col < (blk % 3) * 3 + 3; col++) {
					if (this.board.cells[row][col].number == 0) {
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
				return [blk, r1, c1, r2, c2];
			}
		}
		return [-1, r1, c1, r2, c2];
	}

	// 未確定セルが二つしかないブロックは、
	// 候補も二つしかなく、どちらかは必ず正解であるはずだから、
	// ゲームを複製して、エラー状態にならないかチェックする。
	this.parse30 = function() {
		var ret = this.findBlock(r1, c1, r2, c2);
		var blk = ret[0];
		var r1 = ret[1];
		var c1 = ret[2];
		var r2 = ret[3];
		var c2 = ret[4];

		if (blk >= 0) {
			var tempGame = new SudokuGame();
			tempGame.board = this.board.clone();
			tempGame.board.fixNumber(r1, c1, tempGame.board.cells[r1][c1].candidates[0]);

			while (
				tempGame.parse00() ||
				tempGame.parse01() ||
				tempGame.parse02() ||
				tempGame.parse03() ||
				tempGame.parse12())
				;

			if (tempGame.board.isFinished()) {
				this.board.fixNumber(r1, c1, this.board.cells[r1][c1].candidates[0]);
				return true;
			} else if (tempGame.board.isFailed()) {
				this.board.cells[r1][c1].removeCandidate(this.board.cells[r1][c1].candidates[0]);
				return true;
			} else {
				return false;
			}
		}
	}
}
