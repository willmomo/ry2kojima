using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace mp3editor {
	public partial class MainForm : Form {
		public MP3Doc m_doc = null;

		public MainForm() {
			InitializeComponent();
		}

		// コントロールの初期化
		private void InitControls() {
			listView1.Columns.Add("項目");
			listView1.Columns.Add("値");

			foreach (ColumnHeader c in listView1.Columns) {
				c.Width = -2;
			}
		}

		// フォームの情報を更新する
		private void UpdateData() {
			if (m_doc == null) return;
			if (m_doc.ID3v1Tag == null) return;

			fileNameLabel.Text = System.IO.Path.GetFileName(m_doc.FileName);
			folderLabel.Text = System.IO.Path.GetDirectoryName(m_doc.FileName);

			listView1.Items.Clear();

			listView1.Enabled = useID3v1TagCheckBox.Checked = m_doc.HasID3v1Tag;
			listView1.Items.Add(
				new ListViewItem(new string[] { "SongTitle", m_doc.ID3v1Tag.SongTitle }));
			listView1.Items.Add(
				new ListViewItem(new string[] { "Artist", m_doc.ID3v1Tag.Artist }));
			listView1.Items.Add(
				new ListViewItem(new string[] { "Album", m_doc.ID3v1Tag.Album }));
			listView1.Items.Add(
				new ListViewItem(new string[] { "Year", m_doc.ID3v1Tag.Year }));
			listView1.Items.Add(
				new ListViewItem(new string[] { "Comment", m_doc.ID3v1Tag.Comment }));
			listView1.Items.Add(
				new ListViewItem(new string[] { "Track", m_doc.ID3v1Tag.AlbumTrack.ToString() }));
			listView1.Items.Add(
				new ListViewItem(new string[] { "Genre", m_doc.ID3v1Tag.Genre.ToString() }));

			if (m_doc.ID3v1Tag.Version == 0) {
				// ID3v1 のときは、Track が無いので行をグレーにする
				versionComboBox.SelectedIndex = 0;
				listView1.Items[5].BackColor = Color.DarkGray;
			} else {
				versionComboBox.SelectedIndex = 1;
				listView1.Items[5].BackColor = listView1.Items[4].BackColor;
			}

			foreach (ColumnHeader c in listView1.Columns) {
				c.Width = -2;
			}
		}

		// [ファイル]-[開く]処理
		private void 開くOToolStripMenuItem_Click(object sender, EventArgs e) {
			OpenFileDialog ofd = new OpenFileDialog();
			if (ofd.ShowDialog() == System.Windows.Forms.DialogResult.OK) {
				m_doc = new MP3Doc();
				m_doc.LoadFile(ofd.FileName);

				this.UpdateData();
			}
		}

		// Loadした時の処理
		private void Form1_Load(object sender, EventArgs e) {
			InitControls();
		}

		private void listView1_MouseDoubleClick(object sender, MouseEventArgs e) {
			ListViewHitTestInfo info = listView1.HitTest(e.X, e.Y);
			if (info.SubItem != null && e.Button == MouseButtons.Left) {
				ListViewInputBox input = new ListViewInputBox(listView1, info.Item, 1);
				input.FinishInput += new ListViewInputBox.InputEventHandler(input_FinishInput);
				input.Show();
			}
		}

		void input_FinishInput(object sender, ListViewInputBox.InputEventArgs e) {
			listView1.Items[e.Index].SubItems[1].Text = e.NewName;
		}

		private void listView1_Resize(object sender, EventArgs e) {
		}

		private void Form1_ResizeEnd(object sender, EventArgs e) {
			// リストビューの中身を再調整する
			foreach (ColumnHeader c in listView1.Columns) {
				c.Width = -2;
			}
		}

		private void useID3v1TagCheckBox_CheckedChanged(object sender, EventArgs e) {
			listView1.Enabled = useID3v1TagCheckBox.Checked;
		}

		private void 保存AToolStripMenuItem_Click(object sender, EventArgs e) {
			// フォームに入力した内容を反映させる
			m_doc.HasID3v1Tag = useID3v1TagCheckBox.Checked;
			m_doc.ID3v1Tag.Version = versionComboBox.SelectedIndex == 0 ? 0 : 1;
			m_doc.ID3v1Tag.SongTitle = listView1.Items[0].SubItems[1].Text;
			m_doc.ID3v1Tag.Artist = listView1.Items[1].SubItems[1].Text;
			m_doc.ID3v1Tag.Album = listView1.Items[2].SubItems[1].Text;
			m_doc.ID3v1Tag.Year = listView1.Items[3].SubItems[1].Text;
			m_doc.ID3v1Tag.Comment = listView1.Items[4].SubItems[1].Text;

			short result = 0;
			if (Int16.TryParse(listView1.Items[5].SubItems[1].Text, out result)) {
				m_doc.ID3v1Tag.AlbumTrack = result;
			}
			if (Int16.TryParse(listView1.Items[6].SubItems[1].Text, out result)) {
				m_doc.ID3v1Tag.Genre = result;
			}

			try {
				m_doc.SaveFile();
			} catch (Exception ex) {
				MessageBox.Show(this, "保存できませんでした。\n\n" + ex.Message, "保存できません", MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		private void 終了XToolStripMenuItem_Click(object sender, EventArgs e) {
			this.Close();
		}

		private void versionComboBox_SelectedIndexChanged(object sender, EventArgs e) {
			if (versionComboBox.SelectedIndex == 0) {
				listView1.Items[5].BackColor = Color.DarkGray;
			} else {
				listView1.Items[5].BackColor = listView1.Items[4].BackColor;
			}
		}

		private void MainForm_DragEnter(object sender, DragEventArgs e) {
			e.Effect = DragDropEffects.Copy;
		}

		private void MainForm_DragDrop(object sender, DragEventArgs e) {
			if (e.Data.GetDataPresent(DataFormats.FileDrop)) {
				string[] fname = (string[])e.Data.GetData(DataFormats.FileDrop, false);
				m_doc = new MP3Doc();
				m_doc.LoadFile(fname[0]);

				this.UpdateData();
			}
		}
	}

	/// <summary>
	/// リストビューにオーバーラップさせてアイテムを編集するためのTextBox
	/// </summary>
	public class ListViewInputBox : TextBox {
		public class InputEventArgs : EventArgs {
			public int Index = 0;
			public string Path = "";
			public string NewName = "";
		}

		public delegate void InputEventHandler(object sender, InputEventArgs e);

		//イベントデリゲートの宣言
		public event InputEventHandler FinishInput;

		private InputEventArgs args = new InputEventArgs();
		private bool finished = false;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="parent">対象となるListViewコントロール</param>
		/// <param name="item">編集対象のアイテム</param>
		/// <param name="subitem_index">編集する対象の列</param>
		public ListViewInputBox(ListView parent, ListViewItem item, int subitem_index) : base() {
			args.Index = item.Index;
			args.Path = item.SubItems[0].Text;
			args.NewName = item.SubItems[1].Text;

			int left = 0;
			for (int i = 0; i < subitem_index; i++) {
				left += parent.Columns[i].Width;
			}
			int width = item.SubItems[subitem_index].Bounds.Width;
			int height = item.SubItems[subitem_index].Bounds.Height - 4;

			this.Parent = parent;
			this.Size = new Size(width, height);
			this.Left = left;
			this.Top = item.Position.Y - 1;
			this.Text = item.SubItems[subitem_index].Text;
			this.LostFocus += new EventHandler(textbox_LostFocus);
			this.ImeMode = ImeMode.NoControl;
			this.Multiline = false;
			this.KeyDown += new KeyEventHandler(textbox_KeyDown);
			this.Focus();
		}

		void Finish(string new_name) {
			// Enterで入力を完了した場合はKeyDownが呼ばれた後に
			// さらにLostFocusが呼ばれるため，二回Finishが呼ばれる
			if (!finished) {
				// textbox.Hide()すると同時にLostFocusするため，
				// finished=trueを先に呼び出しておかないと，
				// このブロックが二回呼ばれてしまう．
				finished = true;
				this.Hide();
				args.NewName = new_name;
				FinishInput(this, args);
			}
		}

		void textbox_KeyDown(object sender, KeyEventArgs e) {
			if (e.KeyCode == Keys.Enter) {
				Finish(this.Text);
			} else if (e.KeyCode == Keys.Escape) {
				Finish(args.NewName);
			}
		}

		void textbox_LostFocus(object sender, EventArgs e) {
			Finish(this.Text);
		}
	}
}
