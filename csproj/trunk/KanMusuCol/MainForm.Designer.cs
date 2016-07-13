namespace KanMusuCol {
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
			this.apistart2jsonの読み込みRToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.終了XToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.ツールTToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.オプションOToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.label2 = new System.Windows.Forms.Label();
			this.textBoxClearButton = new System.Windows.Forms.Button();
			this.filterTextBox = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.dataGridView1 = new System.Windows.Forms.DataGridView();
			this.chkUpdate = new System.Windows.Forms.DataGridViewCheckBoxColumn();
			this.id = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.sortid = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.name = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.filename = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.voicef = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.lastUpdate = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.btnUpdate = new System.Windows.Forms.DataGridViewButtonColumn();
			this.更新UToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.チェックした艦むすを更新UToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.すべての艦むすを更新AToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.menuStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
			this.SuspendLayout();
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ファイルFToolStripMenuItem,
            this.更新UToolStripMenuItem,
            this.ツールTToolStripMenuItem});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(666, 24);
			this.menuStrip1.TabIndex = 0;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// ファイルFToolStripMenuItem
			// 
			this.ファイルFToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.apistart2jsonの読み込みRToolStripMenuItem,
            this.toolStripSeparator1,
            this.終了XToolStripMenuItem});
			this.ファイルFToolStripMenuItem.Name = "ファイルFToolStripMenuItem";
			this.ファイルFToolStripMenuItem.Size = new System.Drawing.Size(66, 20);
			this.ファイルFToolStripMenuItem.Text = "ファイル(&F)";
			// 
			// apistart2jsonの読み込みRToolStripMenuItem
			// 
			this.apistart2jsonの読み込みRToolStripMenuItem.Name = "apistart2jsonの読み込みRToolStripMenuItem";
			this.apistart2jsonの読み込みRToolStripMenuItem.Size = new System.Drawing.Size(232, 22);
			this.apistart2jsonの読み込みRToolStripMenuItem.Text = "api_start2.json の読み込み(&R)...";
			this.apistart2jsonの読み込みRToolStripMenuItem.Click += new System.EventHandler(this.apistart2jsonの読み込みRToolStripMenuItem_Click);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(229, 6);
			// 
			// 終了XToolStripMenuItem
			// 
			this.終了XToolStripMenuItem.Name = "終了XToolStripMenuItem";
			this.終了XToolStripMenuItem.Size = new System.Drawing.Size(232, 22);
			this.終了XToolStripMenuItem.Text = "終了(&X)";
			this.終了XToolStripMenuItem.Click += new System.EventHandler(this.終了XToolStripMenuItem_Click);
			// 
			// ツールTToolStripMenuItem
			// 
			this.ツールTToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.オプションOToolStripMenuItem});
			this.ツールTToolStripMenuItem.Name = "ツールTToolStripMenuItem";
			this.ツールTToolStripMenuItem.Size = new System.Drawing.Size(60, 20);
			this.ツールTToolStripMenuItem.Text = "ツール(&T)";
			// 
			// オプションOToolStripMenuItem
			// 
			this.オプションOToolStripMenuItem.Name = "オプションOToolStripMenuItem";
			this.オプションOToolStripMenuItem.Size = new System.Drawing.Size(143, 22);
			this.オプションOToolStripMenuItem.Text = "オプション(&O)...";
			this.オプションOToolStripMenuItem.Click += new System.EventHandler(this.オプションOToolStripMenuItem_Click);
			// 
			// splitContainer1
			// 
			this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.splitContainer1.Location = new System.Drawing.Point(0, 24);
			this.splitContainer1.Name = "splitContainer1";
			this.splitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.label2);
			this.splitContainer1.Panel1.Controls.Add(this.textBoxClearButton);
			this.splitContainer1.Panel1.Controls.Add(this.filterTextBox);
			this.splitContainer1.Panel1.Controls.Add(this.label1);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.dataGridView1);
			this.splitContainer1.Size = new System.Drawing.Size(666, 545);
			this.splitContainer1.SplitterDistance = 53;
			this.splitContainer1.TabIndex = 2;
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(3, 4);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(660, 18);
			this.label2.TabIndex = 3;
			this.label2.Text = "label2";
			this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			// 
			// textBoxClearButton
			// 
			this.textBoxClearButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.textBoxClearButton.FlatAppearance.BorderSize = 0;
			this.textBoxClearButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
			this.textBoxClearButton.Image = global::KanMusuCol.Properties.Resources.TextBoxClearButton21;
			this.textBoxClearButton.Location = new System.Drawing.Point(641, 25);
			this.textBoxClearButton.Name = "textBoxClearButton";
			this.textBoxClearButton.Size = new System.Drawing.Size(19, 19);
			this.textBoxClearButton.TabIndex = 2;
			this.textBoxClearButton.UseVisualStyleBackColor = true;
			this.textBoxClearButton.Click += new System.EventHandler(this.textBoxClearButton_Click);
			// 
			// filterTextBox
			// 
			this.filterTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.filterTextBox.Location = new System.Drawing.Point(88, 25);
			this.filterTextBox.Name = "filterTextBox";
			this.filterTextBox.Size = new System.Drawing.Size(547, 19);
			this.filterTextBox.TabIndex = 1;
			this.filterTextBox.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
			this.filterTextBox.Enter += new System.EventHandler(this.filterTextBox_Enter);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(9, 29);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(73, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "名前フィルタ(&I)";
			// 
			// dataGridView1
			// 
			this.dataGridView1.AllowUserToAddRows = false;
			this.dataGridView1.AllowUserToDeleteRows = false;
			this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.chkUpdate,
            this.id,
            this.sortid,
            this.name,
            this.filename,
            this.voicef,
            this.lastUpdate,
            this.btnUpdate});
			this.dataGridView1.Dock = System.Windows.Forms.DockStyle.Fill;
			this.dataGridView1.Location = new System.Drawing.Point(0, 0);
			this.dataGridView1.Name = "dataGridView1";
			this.dataGridView1.ReadOnly = true;
			this.dataGridView1.RowTemplate.Height = 21;
			this.dataGridView1.Size = new System.Drawing.Size(666, 488);
			this.dataGridView1.TabIndex = 3;
			this.dataGridView1.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellContentClick);
			// 
			// chkUpdate
			// 
			this.chkUpdate.FalseValue = "false";
			this.chkUpdate.HeaderText = "更新";
			this.chkUpdate.Name = "chkUpdate";
			this.chkUpdate.ReadOnly = true;
			this.chkUpdate.TrueValue = "true";
			this.chkUpdate.Width = 50;
			// 
			// id
			// 
			this.id.DataPropertyName = "id";
			this.id.HeaderText = "ID";
			this.id.Name = "id";
			this.id.ReadOnly = true;
			this.id.Width = 50;
			// 
			// sortid
			// 
			this.sortid.DataPropertyName = "sortid";
			this.sortid.HeaderText = "sortid";
			this.sortid.Name = "sortid";
			this.sortid.ReadOnly = true;
			this.sortid.Width = 50;
			// 
			// name
			// 
			this.name.DataPropertyName = "name";
			this.name.HeaderText = "名前";
			this.name.Name = "name";
			this.name.ReadOnly = true;
			// 
			// filename
			// 
			this.filename.DataPropertyName = "filename";
			this.filename.HeaderText = "ファイル名";
			this.filename.Name = "filename";
			this.filename.ReadOnly = true;
			// 
			// voicef
			// 
			this.voicef.DataPropertyName = "voicef";
			this.voicef.HeaderText = "voicef";
			this.voicef.Name = "voicef";
			this.voicef.ReadOnly = true;
			this.voicef.Width = 50;
			// 
			// lastUpdate
			// 
			this.lastUpdate.DataPropertyName = "lastUpdate";
			this.lastUpdate.HeaderText = "更新日";
			this.lastUpdate.Name = "lastUpdate";
			this.lastUpdate.ReadOnly = true;
			// 
			// btnUpdate
			// 
			this.btnUpdate.HeaderText = "更新ボタン";
			this.btnUpdate.Name = "btnUpdate";
			this.btnUpdate.ReadOnly = true;
			// 
			// 更新UToolStripMenuItem
			// 
			this.更新UToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.チェックした艦むすを更新UToolStripMenuItem,
            this.すべての艦むすを更新AToolStripMenuItem});
			this.更新UToolStripMenuItem.Name = "更新UToolStripMenuItem";
			this.更新UToolStripMenuItem.Size = new System.Drawing.Size(59, 20);
			this.更新UToolStripMenuItem.Text = "更新(&U)";
			// 
			// チェックした艦むすを更新UToolStripMenuItem
			// 
			this.チェックした艦むすを更新UToolStripMenuItem.Name = "チェックした艦むすを更新UToolStripMenuItem";
			this.チェックした艦むすを更新UToolStripMenuItem.Size = new System.Drawing.Size(205, 22);
			this.チェックした艦むすを更新UToolStripMenuItem.Text = "チェックした艦むすを更新(&U)";
			this.チェックした艦むすを更新UToolStripMenuItem.Click += new System.EventHandler(this.チェックした艦むすを更新UToolStripMenuItem_Click);
			// 
			// すべての艦むすを更新AToolStripMenuItem
			// 
			this.すべての艦むすを更新AToolStripMenuItem.Name = "すべての艦むすを更新AToolStripMenuItem";
			this.すべての艦むすを更新AToolStripMenuItem.Size = new System.Drawing.Size(205, 22);
			this.すべての艦むすを更新AToolStripMenuItem.Text = "すべての艦むすを更新(&A)";
			this.すべての艦むすを更新AToolStripMenuItem.Click += new System.EventHandler(this.すべての艦むすを更新AToolStripMenuItem_Click);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(666, 569);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.menuStrip1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "MainForm";
			this.Text = "艦娘コレクション";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel1.PerformLayout();
			this.splitContainer1.Panel2.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
			this.splitContainer1.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem ファイルFToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem apistart2jsonの読み込みRToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripMenuItem 終了XToolStripMenuItem;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.TextBox filterTextBox;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.DataGridView dataGridView1;
		private System.Windows.Forms.ToolStripMenuItem ツールTToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem オプションOToolStripMenuItem;
		private System.Windows.Forms.Button textBoxClearButton;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.DataGridViewCheckBoxColumn chkUpdate;
		private System.Windows.Forms.DataGridViewTextBoxColumn id;
		private System.Windows.Forms.DataGridViewTextBoxColumn sortid;
		private System.Windows.Forms.DataGridViewTextBoxColumn name;
		private System.Windows.Forms.DataGridViewTextBoxColumn filename;
		private System.Windows.Forms.DataGridViewTextBoxColumn voicef;
		private System.Windows.Forms.DataGridViewTextBoxColumn lastUpdate;
		private System.Windows.Forms.DataGridViewButtonColumn btnUpdate;
		private System.Windows.Forms.ToolStripMenuItem 更新UToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem チェックした艦むすを更新UToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem すべての艦むすを更新AToolStripMenuItem;
	}
}

