using System;
using System.IO;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace BmpChk
{
	/// <summary>
	/// Form1 の概要の説明です。
	/// </summary>
	public class frmMain : System.Windows.Forms.Form
	{
		private System.Windows.Forms.MainMenu mnuMain;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.FolderBrowserDialog dlgFolder;
		private System.Windows.Forms.ListView lv;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem mnuFileExit;
		private System.Windows.Forms.MenuItem mnuFileOpen;
		/// <summary>
		/// 必要なデザイナ変数です。
		/// </summary>
		private System.ComponentModel.Container components = null;

		public frmMain()
		{
			//
			// Windows フォーム デザイナ サポートに必要です。
			//
			InitializeComponent();

			//
			// TODO: InitializeComponent 呼び出しの後に、コンストラクタ コードを追加してください。
			//
		}

		/// <summary>
		/// 使用されているリソースに後処理を実行します。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows フォーム デザイナで生成されたコード 
		/// <summary>
		/// デザイナ サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディタで変更しないでください。
		/// </summary>
		private void InitializeComponent()
		{
			this.mnuMain = new System.Windows.Forms.MainMenu();
			this.menuItem1 = new System.Windows.Forms.MenuItem();
			this.mnuFileOpen = new System.Windows.Forms.MenuItem();
			this.menuItem3 = new System.Windows.Forms.MenuItem();
			this.mnuFileExit = new System.Windows.Forms.MenuItem();
			this.dlgFolder = new System.Windows.Forms.FolderBrowserDialog();
			this.lv = new System.Windows.Forms.ListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.SuspendLayout();
			// 
			// mnuMain
			// 
			this.mnuMain.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.menuItem1});
			// 
			// menuItem1
			// 
			this.menuItem1.Index = 0;
			this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.mnuFileOpen,
																					  this.menuItem3,
																					  this.mnuFileExit});
			this.menuItem1.Text = "ファイル(&F)";
			// 
			// mnuFileOpen
			// 
			this.mnuFileOpen.Index = 0;
			this.mnuFileOpen.Shortcut = System.Windows.Forms.Shortcut.CtrlO;
			this.mnuFileOpen.Text = "フォルダを開く(&O)...";
			this.mnuFileOpen.Click += new System.EventHandler(this.mnuFileOpen_Click);
			// 
			// menuItem3
			// 
			this.menuItem3.Index = 1;
			this.menuItem3.Text = "-";
			// 
			// mnuFileExit
			// 
			this.mnuFileExit.Index = 2;
			this.mnuFileExit.Text = "終了(&X)";
			this.mnuFileExit.Click += new System.EventHandler(this.mnuFileExit_Click);
			// 
			// lv
			// 
			this.lv.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																				 this.columnHeader1,
																				 this.columnHeader2,
																				 this.columnHeader3});
			this.lv.Dock = System.Windows.Forms.DockStyle.Fill;
			this.lv.FullRowSelect = true;
			this.lv.GridLines = true;
			this.lv.Location = new System.Drawing.Point(0, 0);
			this.lv.Name = "lv";
			this.lv.Size = new System.Drawing.Size(624, 342);
			this.lv.TabIndex = 0;
			this.lv.View = System.Windows.Forms.View.Details;
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "file name";
			this.columnHeader1.Width = 140;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "horizontal rez";
			this.columnHeader2.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.columnHeader2.Width = 120;
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "vertical rez";
			this.columnHeader3.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
			this.columnHeader3.Width = 120;
			// 
			// frmMain
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 12);
			this.ClientSize = new System.Drawing.Size(624, 342);
			this.Controls.Add(this.lv);
			this.Menu = this.mnuMain;
			this.Name = "frmMain";
			this.Text = "BMP Checker";
			this.ResumeLayout(false);

		}
		#endregion

		private BmpChkDoc m_doc = new BmpChkDoc();

		/// <summary>
		/// アプリケーションのメイン エントリ ポイントです。
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new frmMain());
		}

		/// <summary>
		/// [ファイル]-[フォルダを開く]処理
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void mnuFileOpen_Click(object sender, System.EventArgs e)
		{
			//
			// フォルダ選択ダイアログを開く
			//
			dlgFolder.Description = "チェックするBMPファイルのあるフォルダを選択。";
			dlgFolder.ShowNewFolderButton = false;
			if (dlgFolder.ShowDialog(this) == DialogResult.OK) 
			{
				//
				// 砂時計カーソルに変更
				//
				this.Cursor = new Cursor(Cursors.WaitCursor.Handle);

				//
				// リストビュー、ドキュメントを空にして、選択されたフォルダでドキュメントを開く
				//
				lv.Items.Clear();
				m_doc.CloseDocument();
				m_doc.OpenDocument(dlgFolder.SelectedPath);
				this.Text = m_doc.Title + " - BMP Checker";

				//
				// BMPファイル名とレゾリューション(dpi)をリストに追加していく。
				//
				foreach (BmpFile oneFile in m_doc.FileArray)
				{
					ListViewItem item = lv.Items.Add(oneFile.Name);
					if (oneFile.IsBMPFile)
					{
						item.SubItems.Add(oneFile.XDotPerInch.ToString("F0") + " dpi");
						item.SubItems.Add(oneFile.YDotPerInch.ToString("F0") + " dpi");
					}
				}

				//
				// 標準マウスカーソルに変更
				//
				this.Cursor = new Cursor(Cursors.Default.Handle);
			}
		}

		/// <summary>
		/// [ファイル]-[終了]処理
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void mnuFileExit_Click(object sender, System.EventArgs e)
		{
			System.Environment.Exit(0);
		}
	}
}
