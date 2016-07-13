using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace pxw {
	public partial class Form1 : Form {
		private const int BOX_SIZE = 18;
		private const int FONT_SIZE_L = 14;
		private const int FONT_SIZE_S = 8;

		private PxBoard m_pxb = new PxBoard();

		public Form1() {
			InitializeComponent();
		}

		private void exitToolStripMenuItem_Click(object sender, EventArgs e) {
			this.Close();
		}

		private void pictureBox1_Paint(object sender, PaintEventArgs e) {
			int ofsx = (m_pxb.get_MaxHintRow() + 1) * BOX_SIZE;
			int ofsy = (m_pxb.get_MaxHintCol() + 1) * BOX_SIZE;
			DrawMain(e.Graphics, ofsx, ofsy);
			DrawHint(e.Graphics, ofsx, ofsy);
		}

		private void DrawHint(Graphics g, int ofsx, int ofsy) {
			Pen pen = null;
			Pen pen2 = null;
			Font font = null;
			Font font2 = null;
			Brush brush = null;
			Brush brush2 = null;

			try {
				pen = new Pen(Color.Black);
				pen2 = new Pen(Color.Black, 2);
				font = new Font(FontFamily.GenericMonospace, FONT_SIZE_L);
				font2 = new Font(FontFamily.GenericMonospace, FONT_SIZE_S);
				brush = new SolidBrush(Color.Blue);
				brush2 = new SolidBrush(Color.LightYellow);

				// 列のヒントを描画
				for (int x = 0; x < m_pxb.m_width; x++) {
					bool b = m_pxb.IsFixHitCol(x);
					Rectangle rc = new Rectangle((x * BOX_SIZE) + ofsx, ofsy - BOX_SIZE, BOX_SIZE, BOX_SIZE);
					for (int y = 0; m_pxb.m_hintCol[x] != null && y < m_pxb.m_hintCol[x].Count(); y++) {
						if (b) {
							Rectangle rcTemp = rc;
							rcTemp.Inflate(-2, -2);
							g.FillRectangle(brush2, rcTemp);
						}
						g.DrawRectangle(pen, rc);
						string text = m_pxb.m_hintCol[x][m_pxb.m_hintCol[x].Count() - y - 1].m_count.ToString();
						g.DrawString(text, (text.Length == 1 ? font : font2), brush, rc.Left, rc.Top);
						rc.Offset(0, -BOX_SIZE);
					}
					g.DrawRectangle(pen, rc);
				}

				// 行のヒントを描画
				for (int y = 0; y < m_pxb.m_width; y++) {
					bool b = m_pxb.IsFixHitRow(y);
					Rectangle rc = new Rectangle(ofsx - BOX_SIZE, (y * BOX_SIZE) + ofsy, BOX_SIZE, BOX_SIZE);
					for (int x = 0; m_pxb.m_hintRow[y] != null && x < m_pxb.m_hintRow[y].Count(); x++) {
						if (b) {
							Rectangle rcTemp = rc;
							rcTemp.Inflate(-2, -2);
							g.FillRectangle(brush2, rcTemp);
						}
						g.DrawRectangle(pen, rc);
						string text = m_pxb.m_hintRow[y][m_pxb.m_hintRow[y].Count() - x - 1].m_count.ToString();
						g.DrawString(text, (text.Length == 1 ? font : font2), brush, rc.Left, rc.Top);
						rc.Offset(-BOX_SIZE, 0);
					}
					g.DrawRectangle(pen, rc);
				}
			} finally {
				if (pen != null) pen.Dispose();
				if (pen2 != null) pen2.Dispose();
				if (font != null) font.Dispose();
				if (font2 != null) font2.Dispose();
				if (brush != null) brush.Dispose();
				if (brush2 != null) brush2.Dispose();
			}
		}

		private void DrawMain(Graphics g, int ofsx, int ofsy) {
			Pen pen = null;
			Pen pen2 = null;
			Brush brush = null;
			Brush brush2 = null;

			try {
				pen = new Pen(Color.Black);
				pen2 = new Pen(Color.Black, 2);
				brush = new SolidBrush(Color.DarkBlue);
				brush2 = new SolidBrush(Color.LightGray);

				for (int x = 0; x <= m_pxb.m_width; x++) {
					Point pt1 = new Point(x * BOX_SIZE, 0);
					Point pt2 = new Point(x * BOX_SIZE, m_pxb.m_height * BOX_SIZE);
					pt1.Offset(ofsx, ofsy);
					pt2.Offset(ofsx, ofsy);

					if ((x % 5) == 0) {
						g.DrawLine(pen2, pt1, pt2);
					} else {
						g.DrawLine(pen, pt1, pt2);
					}
				}
				for (int y = 0; y <= m_pxb.m_height; y++) {
					Point pt1 = new Point(0, y * BOX_SIZE);
					Point pt2 = new Point(m_pxb.m_width * BOX_SIZE, y * BOX_SIZE);
					pt1.Offset(ofsx, ofsy);
					pt2.Offset(ofsx, ofsy);

					if ((y % 5) == 0) {
						g.DrawLine(pen2, pt1, pt2);
					} else {
						g.DrawLine(pen, pt1, pt2);
					}
				}
				for (int y = 0; y < m_pxb.m_height; y++) {
					for (int x = 0; x < m_pxb.m_width; x++) {
						Rectangle rc = new Rectangle((x * BOX_SIZE) + ofsx, (y * BOX_SIZE) + ofsy, BOX_SIZE, BOX_SIZE);
						rc.Inflate(-2, -2);
						if (m_pxb.m_board[y * m_pxb.m_width + x] == 1) {
							g.FillRectangle(brush, rc);
						}
						else if (m_pxb.m_board[y * m_pxb.m_width + x] == -1) {
							g.FillRectangle(brush2, rc);
						}
					}
				}
			} finally {
				if (pen != null) pen.Dispose();
				if (pen2 != null) pen2.Dispose();
			}
		}

		private void Form1_Load(object sender, EventArgs e) {
			this.Width = (m_pxb.m_width + 5) * BOX_SIZE;
			this.Height = (m_pxb.m_height + 10) * BOX_SIZE;
		}

		private void pictureBox1_Click(object sender, EventArgs e) {
			int ofsx = (m_pxb.get_MaxHintRow() + 1) * BOX_SIZE;
			int ofsy = (m_pxb.get_MaxHintCol() + 1) * BOX_SIZE;

			Point pt = pictureBox1.PointToClient(Cursor.Position);
			System.Diagnostics.Debug.WriteLine("{0} {1}", pt.X, pt.Y);

			if (pt.X < ofsx) {
				// 行ヒントクリック
				if (pt.Y > ofsy) {
					int row = (pt.Y - ofsy) / BOX_SIZE;
					System.Diagnostics.Debug.WriteLine("行ヒント : {0}", row);

					using (Form2 f = new Form2()) {
						if (f.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
							m_pxb.set_HintRow(row, f.m_text);
							Refresh();
						}
					}
				}
			} else if (pt.Y < ofsy) {
				// 列ヒントクリック
				if (pt.X > ofsx) {
					int col = (pt.X - ofsx) / BOX_SIZE;
					System.Diagnostics.Debug.WriteLine("列ヒント : {0}", col);

					using (Form2 f = new Form2()) {
						if (f.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
							m_pxb.set_HintCol(col, f.m_text);
							Refresh();
						}
					}
				}
			} else {
				// 盤面内クリック
				int row = (pt.Y - ofsy) / BOX_SIZE;
				int col = (pt.X - ofsx) / BOX_SIZE;
				System.Diagnostics.Debug.WriteLine("行,列 : {0},{1}", row, col);
			}
		}

		private void newToolStripMenuItem_Click(object sender, EventArgs e) {
			using (Form3 f = new Form3()) {
				if (f.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
					m_pxb = new PxBoard(f.m_height, f.m_width);
					Refresh();
				}
			}
		}

		private void saveToolStripMenuItem_Click(object sender, EventArgs e) {
			using (StreamWriter sw = new StreamWriter("pxboard.txt")) {
				m_pxb.Save(sw);
			}
		}

		private void openToolStripMenuItem_Click(object sender, EventArgs e) {
			using (StreamReader sr = new StreamReader("pxboard.txt")) {
				m_pxb.Load(sr);
				Refresh();
			}
		}

		private void step1ToolStripMenuItem_Click(object sender, EventArgs e) {
			m_pxb.Step1();
			Refresh();
		}
	}
}
