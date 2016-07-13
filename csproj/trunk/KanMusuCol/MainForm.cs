using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KanMusuCol {
	public partial class MainForm : Form {

		private KanColle.KanMusuCollection m_kmc = new KanColle.KanMusuCollection();
		private DataTable m_table = new DataTable();
		BindingSource bs = new BindingSource();

		public MainForm() {
			InitializeComponent();
		}

		private void InitTable() {
			DataColumn pkey = new DataColumn("id", Type.GetType("System.Int32"));
			m_table.Columns.Add(pkey);
			m_table.Columns.Add(new DataColumn("sortid", Type.GetType("System.Int32")));
			m_table.Columns.Add(new DataColumn("name", Type.GetType("System.String")));
			m_table.Columns.Add(new DataColumn("filename", Type.GetType("System.String")));
			m_table.Columns.Add(new DataColumn("voicef", Type.GetType("System.Int32")));
			m_table.Columns.Add(new DataColumn("lastUpdate", Type.GetType("System.String")));

			m_table.PrimaryKey = new DataColumn[] { pkey };
		}

		private void Form1_Load(object sender, EventArgs e) {
			dataGridView1.AutoGenerateColumns = false;

			InitTable();

			bs.DataSource = m_table;
			dataGridView1.DataSource = bs;

			RoamingData rd = new RoamingData();
			label2.Text = "保存場所：" + (m_kmc.BasePath = rd.DataPath);
			m_kmc.m_cmdHugFlash = rd.HugFlashPath;
			m_kmc.LoadApiStart2Csv();
			// m_kmc.SaveApiStart2(); なんでLoadした直後にSaveしてたんだろう・・・不要なので削除
			UpdateData();
		}

		private void apistart2jsonの読み込みRToolStripMenuItem_Click(object sender, EventArgs e) {
			using (OpenFileDialog ofd = new OpenFileDialog()) {
				ofd.Filter = "jsonファイル(*.json)|*.json|すべてのファイル(*.*)|*.*";
				if (ofd.ShowDialog(this) == System.Windows.Forms.DialogResult.OK) {
					m_kmc.LoadApiStart2(ofd.FileName);
				}
			}
			m_kmc.SaveApiStart2();
			UpdateData();
		}

		private void UpdateData() {
			//m_table.Clear();
			foreach (var item in m_kmc.m_kanMusuList) {
				string[] cols = item.Value.Split(',');

				DataRow row = m_table.Rows.Find(cols[0]);
				if (row == null) {
					row = m_table.NewRow();
					row["id"] = cols[0];
					row["sortid"] = cols[1];
					row["name"] = cols[2];
					row["voicef"] = cols[3];
					row["filename"] = cols[4];
					if (cols.Length > 5) row["lastUpdate"] = cols[5];
					m_table.Rows.Add(row);
				}
				else {
					row["id"] = cols[0];
					row["sortid"] = cols[1];
					row["name"] = cols[2];
					row["voicef"] = cols[3];
					row["filename"] = cols[4];
					if (cols.Length > 5) row["lastUpdate"] = cols[5];
				}
			}
			bs.Sort = "id";			
		}

		private void 終了XToolStripMenuItem_Click(object sender, EventArgs e) {
			this.Close();
		}

		private void textBox1_TextChanged(object sender, EventArgs e) {

			if (filterTextBox.Text == "") {
				bs.Filter = "";
			}
			else {
				bs.Filter = "name like '%" + filterTextBox.Text + "%'";
			}

		}

		private void dataGridView1_CellContentClick(object sender, DataGridViewCellEventArgs e) {

			// sender は、DataGridView
			DataGridView view = (DataGridView)sender;

			// sender と e から DataGridViewCell を導き出せる
			DataGridViewCell cell = view[e.ColumnIndex, e.RowIndex];

			if (view.Columns[e.ColumnIndex].HeaderText == "更新") {
				DataGridViewCheckBoxCell checkCell = cell as DataGridViewCheckBoxCell;
				checkCell.Value = (checkCell.Value == checkCell.TrueValue ? checkCell.FalseValue : checkCell.TrueValue);
			} else if (view.Columns[e.ColumnIndex].HeaderText == "更新ボタン") {
				int id = (int)view.Rows[e.RowIndex].Cells["id"].Value;

				Cursor preCursor = Cursor.Current;
				Cursor.Current = Cursors.WaitCursor;
				m_kmc.GetSWF(id);
				Cursor.Current = preCursor;

				m_kmc.SaveApiStart2();
				UpdateData();
			}
		}

		private void オプションOToolStripMenuItem_Click(object sender, EventArgs e) {
			using (OptionForm of = new OptionForm()) {
				of.ShowDialog(this);
			}
		}

		private void textBoxClearButton_Click(object sender, EventArgs e) {
			filterTextBox.ResetText();
			filterTextBox.Focus();
		}

		private void filterTextBox_Enter(object sender, EventArgs e) {
			filterTextBox.SelectAll();
		}

		private void UpdateKanMusu() {
		}

		private void チェックした艦むすを更新UToolStripMenuItem_Click(object sender, EventArgs e) {
			// dataGridView1 を上から順に処理
			for (int i = 0; i < dataGridView1.RowCount; i++) {
				DataGridViewCheckBoxCell checkCell = dataGridView1[0, i] as DataGridViewCheckBoxCell;
				if (checkCell.Value == checkCell.TrueValue) {

					dataGridView1.FirstDisplayedScrollingRowIndex = i;

					int id = (int)dataGridView1.Rows[i].Cells["id"].Value;

					Cursor preCursor = Cursor.Current;
					Cursor.Current = Cursors.WaitCursor;
					m_kmc.GetSWF(id);
					Cursor.Current = preCursor;

					m_kmc.SaveApiStart2();

					checkCell.Value = checkCell.FalseValue;

					UpdateData();
				}
			}
		}

		private void すべての艦むすを更新AToolStripMenuItem_Click(object sender, EventArgs e) {
			// dataGridView1 を上から順に処理
			for (int i = 0; i < dataGridView1.RowCount; i++) {
				int voicef = (int)dataGridView1.Rows[i].Cells["voicef"].Value;
				DataGridViewCheckBoxCell checkCell = dataGridView1[0, i] as DataGridViewCheckBoxCell;
				if (voicef >= 0) {
					dataGridView1.FirstDisplayedScrollingRowIndex = i;

					int id = (int)dataGridView1.Rows[i].Cells["id"].Value;

					Cursor preCursor = Cursor.Current;
					Cursor.Current = Cursors.WaitCursor;
					m_kmc.GetSWF(id);
					Cursor.Current = preCursor;

					m_kmc.SaveApiStart2();

					checkCell.Value = checkCell.FalseValue;

					UpdateData();
				}
			}
		}
	}
}
