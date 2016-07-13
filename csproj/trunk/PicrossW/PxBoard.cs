using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace pxw {
	class HintInfo {
		public int m_count = 0;
		public bool m_fix = false;

		public HintInfo(int cnt) {
			m_count = cnt;
			m_fix = false;
		}
	}

	class PxBoard {
		public int m_height = 25;
		public int m_width = 25;
		public int[] m_board;
		public List<HintInfo>[] m_hintRow;
		public List<HintInfo>[] m_hintCol;

		public PxBoard() {
			m_board = new int[m_height * m_width];
			m_hintRow = new List<HintInfo>[m_height];
			m_hintCol = new List<HintInfo>[m_width];
		}

		public PxBoard(int h, int w) {
			m_height = h;
			m_width = w;
			m_board = new int[m_height * m_width];
			m_hintRow = new List<HintInfo>[m_height];
			m_hintCol = new List<HintInfo>[m_width];
		}

		public void Load(StreamReader sr) {
			m_height = Convert.ToInt32(sr.ReadLine());
			m_width = Convert.ToInt32(sr.ReadLine());
			m_board = new int[m_height * m_width];
			m_hintRow = new List<HintInfo>[m_height];
			m_hintCol = new List<HintInfo>[m_width];

			for (int i = 0; i < m_hintRow.Length; i++) {
				string temp = sr.ReadLine();
				if (temp.Length > 0) {
					set_HintRow(i, temp);
				}
			}

			for (int i = 0; i < m_hintCol.Length; i++) {
				string temp = sr.ReadLine();
				if (temp.Length > 0) {
					set_HintCol(i, temp);
				}
			}
		}

		public void Save(StreamWriter sw) {
			sw.WriteLine(m_height);
			sw.WriteLine(m_width);

			for (int i = 0; i < m_hintRow.Length; i++) {
				if (m_hintRow[i] == null) {
					sw.WriteLine("");
				} else {
					sw.WriteLine(String.Join(",", m_hintRow[i]));
				}
			}

			for (int i = 0; i < m_hintCol.Length; i++) {
				if (m_hintCol[i] == null) {
					sw.WriteLine("");
				} else {
					sw.WriteLine(String.Join(",", m_hintCol[i]));
				}
			}
		}

		private int get_MaxHint(List<HintInfo>[] hint) {
			int ret = 0;
			for (int i = 0; i < hint.Length; i++) {
				if (hint[i] != null && hint[i].Count() > ret) {
					ret = hint[i].Count();
				}
			}
			return ret;
		}

		public int get_MaxHintCol() {
			return get_MaxHint(m_hintCol);
		}

		public int get_MaxHintRow() {
			return get_MaxHint(m_hintRow);
		}

		public void set_HintRow(int row, string hints) {
			if (m_hintRow[row] == null) {
				m_hintRow[row] = new List<HintInfo>();
			}
			m_hintRow[row].Clear();
			foreach (string hint in hints.Split(new char[] {',','.',' '})) {
				m_hintRow[row].Add(new HintInfo(Convert.ToInt32(hint)));
			}
		}

		public void set_HintCol(int col, string hints) {
			if (m_hintCol[col] == null) {
				m_hintCol[col] = new List<HintInfo>();
			}
			m_hintCol[col].Clear();
			foreach (string hint in hints.Split(new char[] { ',', '.', ' ' })) {
				m_hintCol[col].Add(new HintInfo(Convert.ToInt32(hint)));
			}
		}

		private void DebugArray(int[] array) {
			for (int i = 0; i < array.Length; i++) {
				System.Diagnostics.Debug.Write(array[i]);
			}
			System.Diagnostics.Debug.WriteLine("");
		}

		private bool Step1_1(List<HintInfo> hint, int[] fix) {
			int[] fix1 = new int[fix.Length];
			int[] fix2 = new int[fix.Length];

			// 左から配置
			int index = 0;
			for (int i = 0; i < hint.Count; i++) {
				if (i != 0) {
					fix1[index++] = 0;
				}
				for (int j = 0; j < hint[i].m_count; j++) {
					fix1[index++] = i + 1;
				}
			}

			// 右から配置
			index = fix2.Length - 1;
			for (int i = hint.Count - 1; i >= 0; i--) {
				if (i != hint.Count - 1) {
					fix2[index--] = 0;
				}
				for (int j = 0; j < hint[i].m_count; j++) {
					fix2[index--] = i + 1;
				}
			}

			for (int i = 0; i < fix1.Length; i++) {
				if (fix1[i] != 0 && fix2[i] != 0 && fix1[i] == fix2[i]) {
					fix[i] = 1;
				}
			}

			return fix1.SequenceEqual(fix2);
		}

		public bool IsFixHint(List<HintInfo> hint) {
			bool ret = true;
			foreach (HintInfo hi in hint) {
				if (hi.m_fix == false) {
					ret = false;
					break;
				}
			}
			return ret;
		}

		public bool IsFixHitRow(int index) {
			bool ret = false;
			if (m_hintRow[index] != null) {
				ret = IsFixHint(m_hintRow[index]);
			}
			return ret;
		}

		public bool IsFixHitCol(int index) {
			bool ret = false;
			if (m_hintRow[index] != null) {
				ret = IsFixHint(m_hintRow[index]);
			}
			return ret;
		}

		private void FixHint(List<HintInfo> hint) {
			foreach (HintInfo hi in hint) {
				hi.m_fix = true;
			}
		}

		public void Step1() {
			for (int i = 0; i < m_hintRow.Length; i++) {
				if (m_hintRow[i] != null) {
					int[] fix = new int[m_width];
					bool b = Step1_1(m_hintRow[i], fix);

					if (b) {
						FixHint(m_hintRow[i]);
					}

					DebugArray(fix);
					for (int j = 0; j < m_width; j++) {
						if (fix[j] != 0) {
							m_board[i * m_width + j] = 1;
						} else {
							if (b) {
								m_board[i * m_width + j] = -1;
							}
						}
					}
				}
			}
			for (int i = 0; i < m_hintCol.Length; i++) {
				if (m_hintCol[i] != null) {
					int[] fix = new int[m_height];
					bool b = Step1_1(m_hintCol[i], fix);

					if (b) {
						FixHint(m_hintRow[i]);
					}

					DebugArray(fix);
					for (int j = 0; j < m_height; j++) {
						if (fix[j] != 0) {
							m_board[j * m_height + i] = 1;
						} else {
							if (b) {
								m_board[j * m_height + i] = -1;
							}
						}
					}
				}
			}
		}
	}
}
