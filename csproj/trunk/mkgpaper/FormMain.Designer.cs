namespace mkgpaper {
    partial class FormMain {
        /// <summary>
        /// 必要なデザイナ変数です。
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

        #region Windows フォーム デザイナで生成されたコード

        /// <summary>
        /// デザイナ サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディタで変更しないでください。
        /// </summary>
        private void InitializeComponent() {
			this.pd = new System.Drawing.Printing.PrintDocument();
			this.ms = new System.Windows.Forms.MenuStrip();
			this.smiFile = new System.Windows.Forms.ToolStripMenuItem();
			this.smiFileSetup = new System.Windows.Forms.ToolStripMenuItem();
			this.smiFilePrint = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.終了XToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.ヘルプHToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.バージョン情報AToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.label1 = new System.Windows.Forms.Label();
			this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
			this.button1 = new System.Windows.Forms.Button();
			this.checkBox1 = new System.Windows.Forms.CheckBox();
			this.ms.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
			this.SuspendLayout();
			// 
			// pd
			// 
			this.pd.DocumentName = "方眼紙";
			this.pd.OriginAtMargins = true;
			this.pd.PrintPage += new System.Drawing.Printing.PrintPageEventHandler(this.pd_PrintPage);
			// 
			// ms
			// 
			this.ms.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.smiFile,
            this.ヘルプHToolStripMenuItem});
			this.ms.Location = new System.Drawing.Point(0, 0);
			this.ms.Name = "ms";
			this.ms.Size = new System.Drawing.Size(211, 26);
			this.ms.TabIndex = 0;
			this.ms.Text = "menuStrip1";
			// 
			// smiFile
			// 
			this.smiFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.smiFileSetup,
            this.smiFilePrint,
            this.toolStripMenuItem1,
            this.終了XToolStripMenuItem});
			this.smiFile.Name = "smiFile";
			this.smiFile.Size = new System.Drawing.Size(85, 22);
			this.smiFile.Text = "ファイル(&F)";
			// 
			// smiFileSetup
			// 
			this.smiFileSetup.Name = "smiFileSetup";
			this.smiFileSetup.Size = new System.Drawing.Size(175, 22);
			this.smiFileSetup.Text = "ページ設定(&U)...";
			this.smiFileSetup.Click += new System.EventHandler(this.smiFileSetup_Click);
			// 
			// smiFilePrint
			// 
			this.smiFilePrint.Name = "smiFilePrint";
			this.smiFilePrint.ShortcutKeyDisplayString = "";
			this.smiFilePrint.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
			this.smiFilePrint.Size = new System.Drawing.Size(175, 22);
			this.smiFilePrint.Text = "印刷(&P)...";
			this.smiFilePrint.Click += new System.EventHandler(this.button1_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(172, 6);
			// 
			// 終了XToolStripMenuItem
			// 
			this.終了XToolStripMenuItem.Name = "終了XToolStripMenuItem";
			this.終了XToolStripMenuItem.Size = new System.Drawing.Size(175, 22);
			this.終了XToolStripMenuItem.Text = "終了(&X)";
			this.終了XToolStripMenuItem.Click += new System.EventHandler(this.終了XToolStripMenuItem_Click);
			// 
			// ヘルプHToolStripMenuItem
			// 
			this.ヘルプHToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.バージョン情報AToolStripMenuItem});
			this.ヘルプHToolStripMenuItem.Name = "ヘルプHToolStripMenuItem";
			this.ヘルプHToolStripMenuItem.Size = new System.Drawing.Size(75, 22);
			this.ヘルプHToolStripMenuItem.Text = "ヘルプ(&H)";
			// 
			// バージョン情報AToolStripMenuItem
			// 
			this.バージョン情報AToolStripMenuItem.Name = "バージョン情報AToolStripMenuItem";
			this.バージョン情報AToolStripMenuItem.Size = new System.Drawing.Size(178, 22);
			this.バージョン情報AToolStripMenuItem.Text = "バージョン情報(&A)";
			this.バージョン情報AToolStripMenuItem.Click += new System.EventHandler(this.バージョン情報AToolStripMenuItem_Click);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(57, 42);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(56, 12);
			this.label1.TabIndex = 2;
			this.label1.Text = "mm方眼を";
			// 
			// numericUpDown1
			// 
			this.numericUpDown1.Location = new System.Drawing.Point(12, 40);
			this.numericUpDown1.Name = "numericUpDown1";
			this.numericUpDown1.Size = new System.Drawing.Size(39, 19);
			this.numericUpDown1.TabIndex = 3;
			this.numericUpDown1.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(119, 37);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 4;
			this.button1.Text = "印刷(&P)";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// checkBox1
			// 
			this.checkBox1.AutoSize = true;
			this.checkBox1.Location = new System.Drawing.Point(12, 76);
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Size = new System.Drawing.Size(126, 16);
			this.checkBox1.TabIndex = 5;
			this.checkBox1.Text = "縦線を印刷しない(&V)";
			this.checkBox1.UseVisualStyleBackColor = true;
			// 
			// FormMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(211, 107);
			this.Controls.Add(this.checkBox1);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.numericUpDown1);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.ms);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
			this.MainMenuStrip = this.ms;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FormMain";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "方眼紙印刷";
			this.ms.ResumeLayout(false);
			this.ms.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion

        private System.Drawing.Printing.PrintDocument pd;
        private System.Windows.Forms.MenuStrip ms;
        private System.Windows.Forms.ToolStripMenuItem smiFile;
        private System.Windows.Forms.ToolStripMenuItem smiFilePrint;
        private System.Windows.Forms.ToolStripMenuItem smiFileSetup;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem 終了XToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ヘルプHToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem バージョン情報AToolStripMenuItem;
        private System.Windows.Forms.CheckBox checkBox1;
    }
}

