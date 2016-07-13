namespace mp3editor {
	partial class MainForm {
		/// <summary>
		/// 必要なデザイナー変数です。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 使用中のリソースをすべてクリーンアップします。
		/// </summary>
		/// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
		protected override void Dispose(bool disposing) {
			if (disposing && (components != null)) {
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows フォーム デザイナーで生成されたコード

		/// <summary>
		/// デザイナー サポートに必要なメソッドです。このメソッドの内容を
		/// コード エディターで変更しないでください。
		/// </summary>
		private void InitializeComponent() {
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.ファイルFToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.開くOToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.保存AToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.終了XToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.tabControl1 = new System.Windows.Forms.TabControl();
			this.tabPage1 = new System.Windows.Forms.TabPage();
			this.panel1 = new System.Windows.Forms.Panel();
			this.versionComboBox = new System.Windows.Forms.ComboBox();
			this.label3 = new System.Windows.Forms.Label();
			this.useID3v1TagCheckBox = new System.Windows.Forms.CheckBox();
			this.listView1 = new System.Windows.Forms.ListView();
			this.folderLabel = new System.Windows.Forms.Label();
			this.fileNameLabel = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.menuStrip1.SuspendLayout();
			this.tabControl1.SuspendLayout();
			this.tabPage1.SuspendLayout();
			this.panel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ファイルFToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(664, 26);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// ファイルFToolStripMenuItem
			// 
			this.ファイルFToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.開くOToolStripMenuItem,
            this.toolStripMenuItem1,
            this.保存AToolStripMenuItem,
            this.toolStripSeparator1,
            this.終了XToolStripMenuItem});
			this.ファイルFToolStripMenuItem.Name = "ファイルFToolStripMenuItem";
			this.ファイルFToolStripMenuItem.Size = new System.Drawing.Size(85, 22);
			this.ファイルFToolStripMenuItem.Text = "ファイル(&F)";
			// 
			// 開くOToolStripMenuItem
			// 
			this.開くOToolStripMenuItem.Name = "開くOToolStripMenuItem";
			this.開くOToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
			this.開くOToolStripMenuItem.Text = "開く(&O)...";
			this.開くOToolStripMenuItem.Click += new System.EventHandler(this.開くOToolStripMenuItem_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(128, 6);
			// 
			// 保存AToolStripMenuItem
			// 
			this.保存AToolStripMenuItem.Name = "保存AToolStripMenuItem";
			this.保存AToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
			this.保存AToolStripMenuItem.Text = "保存(&A)";
			this.保存AToolStripMenuItem.Click += new System.EventHandler(this.保存AToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(128, 6);
			// 
			// 終了XToolStripMenuItem
			// 
			this.終了XToolStripMenuItem.Name = "終了XToolStripMenuItem";
			this.終了XToolStripMenuItem.Size = new System.Drawing.Size(131, 22);
			this.終了XToolStripMenuItem.Text = "終了(&X)";
			this.終了XToolStripMenuItem.Click += new System.EventHandler(this.終了XToolStripMenuItem_Click);
			// 
			// tabControl1
			// 
			this.tabControl1.Controls.Add(this.tabPage1);
			this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.tabControl1.Location = new System.Drawing.Point(0, 26);
			this.tabControl1.Name = "tabControl1";
			this.tabControl1.SelectedIndex = 0;
			this.tabControl1.Size = new System.Drawing.Size(664, 453);
			this.tabControl1.TabIndex = 1;
			// 
			// tabPage1
			// 
			this.tabPage1.Controls.Add(this.panel1);
			this.tabPage1.Location = new System.Drawing.Point(4, 22);
			this.tabPage1.Name = "tabPage1";
			this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
			this.tabPage1.Size = new System.Drawing.Size(656, 427);
			this.tabPage1.TabIndex = 0;
			this.tabPage1.Text = "ファイル情報";
			this.tabPage1.UseVisualStyleBackColor = true;
			// 
			// panel1
			// 
			this.panel1.Controls.Add(this.versionComboBox);
			this.panel1.Controls.Add(this.label3);
			this.panel1.Controls.Add(this.useID3v1TagCheckBox);
			this.panel1.Controls.Add(this.listView1);
			this.panel1.Controls.Add(this.folderLabel);
			this.panel1.Controls.Add(this.fileNameLabel);
			this.panel1.Controls.Add(this.label2);
			this.panel1.Controls.Add(this.label1);
			this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.panel1.Location = new System.Drawing.Point(3, 3);
			this.panel1.Name = "panel1";
			this.panel1.Size = new System.Drawing.Size(650, 421);
			this.panel1.TabIndex = 0;
			// 
			// versionComboBox
			// 
			this.versionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.versionComboBox.FormattingEnabled = true;
			this.versionComboBox.Items.AddRange(new object[] {
            "ID3v1",
            "ID3v1.1"});
			this.versionComboBox.Location = new System.Drawing.Point(55, 110);
			this.versionComboBox.Name = "versionComboBox";
			this.versionComboBox.Size = new System.Drawing.Size(121, 20);
			this.versionComboBox.TabIndex = 8;
			this.versionComboBox.SelectedIndexChanged += new System.EventHandler(this.versionComboBox_SelectedIndexChanged);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(5, 113);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(44, 12);
			this.label3.TabIndex = 7;
			this.label3.Text = "Version";
			// 
			// useID3v1TagCheckBox
			// 
			this.useID3v1TagCheckBox.AutoSize = true;
			this.useID3v1TagCheckBox.Location = new System.Drawing.Point(7, 87);
			this.useID3v1TagCheckBox.Name = "useID3v1TagCheckBox";
			this.useID3v1TagCheckBox.Size = new System.Drawing.Size(104, 16);
			this.useID3v1TagCheckBox.TabIndex = 6;
			this.useID3v1TagCheckBox.Text = "ID3v1 Tagを使う";
			this.useID3v1TagCheckBox.UseVisualStyleBackColor = true;
			this.useID3v1TagCheckBox.CheckedChanged += new System.EventHandler(this.useID3v1TagCheckBox_CheckedChanged);
			// 
			// listView1
			// 
			this.listView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.listView1.FullRowSelect = true;
			this.listView1.GridLines = true;
			this.listView1.Location = new System.Drawing.Point(7, 136);
			this.listView1.Name = "listView1";
			this.listView1.Size = new System.Drawing.Size(638, 280);
			this.listView1.TabIndex = 5;
			this.listView1.UseCompatibleStateImageBehavior = false;
			this.listView1.View = System.Windows.Forms.View.Details;
			this.listView1.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.listView1_MouseDoubleClick);
			this.listView1.Resize += new System.EventHandler(this.listView1_Resize);
			// 
			// folderLabel
			// 
			this.folderLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.folderLabel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.folderLabel.Location = new System.Drawing.Point(63, 44);
			this.folderLabel.Name = "folderLabel";
			this.folderLabel.Size = new System.Drawing.Size(582, 40);
			this.folderLabel.TabIndex = 4;
			this.folderLabel.Text = "(ファイル名)";
			// 
			// fileNameLabel
			// 
			this.fileNameLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.fileNameLabel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.fileNameLabel.Location = new System.Drawing.Point(63, 12);
			this.fileNameLabel.Name = "fileNameLabel";
			this.fileNameLabel.Size = new System.Drawing.Size(582, 23);
			this.fileNameLabel.TabIndex = 2;
			this.fileNameLabel.Text = "(ファイル名)";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(5, 44);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(40, 12);
			this.label2.TabIndex = 1;
			this.label2.Text = "フォルダ";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(5, 12);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(51, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "ファイル名";
			// 
			// MainForm
			// 
			this.AllowDrop = true;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(664, 479);
			this.Controls.Add(this.tabControl1);
			this.Controls.Add(this.menuStrip1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "MainForm";
			this.Text = "MP3 Tag Editor";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResizeEnd += new System.EventHandler(this.Form1_ResizeEnd);
			this.DragDrop += new System.Windows.Forms.DragEventHandler(this.MainForm_DragDrop);
			this.DragEnter += new System.Windows.Forms.DragEventHandler(this.MainForm_DragEnter);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.tabControl1.ResumeLayout(false);
			this.tabPage1.ResumeLayout(false);
			this.panel1.ResumeLayout(false);
			this.panel1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem ファイルFToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem 開くOToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem 終了XToolStripMenuItem;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPage1;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label folderLabel;
		private System.Windows.Forms.Label fileNameLabel;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.ListView listView1;
		private System.Windows.Forms.CheckBox useID3v1TagCheckBox;
		private System.Windows.Forms.ToolStripMenuItem 保存AToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ComboBox versionComboBox;
		private System.Windows.Forms.Label label3;
	}
}

