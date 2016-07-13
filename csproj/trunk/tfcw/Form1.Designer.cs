namespace tfcw {
	partial class Form1 {
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
			this.label1 = new System.Windows.Forms.Label();
			this.tbFile1 = new System.Windows.Forms.TextBox();
			this.btnBrowse1 = new System.Windows.Forms.Button();
			this.btnBrowse2 = new System.Windows.Forms.Button();
			this.tbFile2 = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.btnCompare = new System.Windows.Forms.Button();
			this.tbOutput = new System.Windows.Forms.TextBox();
			this.pb = new System.Windows.Forms.ProgressBar();
			this.ofDlg = new System.Windows.Forms.OpenFileDialog();
			this.bgWorker = new System.ComponentModel.BackgroundWorker();
			this.lblVer = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(13, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(45, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "ファイル1";
			// 
			// tbFile1
			// 
			this.tbFile1.AllowDrop = true;
			this.tbFile1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tbFile1.Location = new System.Drawing.Point(15, 29);
			this.tbFile1.Name = "tbFile1";
			this.tbFile1.Size = new System.Drawing.Size(480, 19);
			this.tbFile1.TabIndex = 1;
			this.tbFile1.DragDrop += new System.Windows.Forms.DragEventHandler(this.tbFile_DragDrop);
			this.tbFile1.DragEnter += new System.Windows.Forms.DragEventHandler(this.tbFile_DragEnter);
			// 
			// btnBrowse1
			// 
			this.btnBrowse1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnBrowse1.Location = new System.Drawing.Point(501, 24);
			this.btnBrowse1.Name = "btnBrowse1";
			this.btnBrowse1.Size = new System.Drawing.Size(75, 23);
			this.btnBrowse1.TabIndex = 2;
			this.btnBrowse1.Text = "参照";
			this.btnBrowse1.UseVisualStyleBackColor = true;
			this.btnBrowse1.Click += new System.EventHandler(this.btnBrowse1_Click);
			// 
			// btnBrowse2
			// 
			this.btnBrowse2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnBrowse2.Location = new System.Drawing.Point(500, 62);
			this.btnBrowse2.Name = "btnBrowse2";
			this.btnBrowse2.Size = new System.Drawing.Size(75, 23);
			this.btnBrowse2.TabIndex = 5;
			this.btnBrowse2.Text = "参照";
			this.btnBrowse2.UseVisualStyleBackColor = true;
			this.btnBrowse2.Click += new System.EventHandler(this.bnBrowse2_Click);
			// 
			// tbFile2
			// 
			this.tbFile2.AllowDrop = true;
			this.tbFile2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tbFile2.Location = new System.Drawing.Point(14, 67);
			this.tbFile2.Name = "tbFile2";
			this.tbFile2.Size = new System.Drawing.Size(480, 19);
			this.tbFile2.TabIndex = 4;
			this.tbFile2.DragDrop += new System.Windows.Forms.DragEventHandler(this.tbFile_DragDrop);
			this.tbFile2.DragEnter += new System.Windows.Forms.DragEventHandler(this.tbFile_DragEnter);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 51);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(45, 12);
			this.label2.TabIndex = 3;
			this.label2.Text = "ファイル2";
			// 
			// btnCompare
			// 
			this.btnCompare.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnCompare.Location = new System.Drawing.Point(501, 92);
			this.btnCompare.Name = "btnCompare";
			this.btnCompare.Size = new System.Drawing.Size(75, 23);
			this.btnCompare.TabIndex = 6;
			this.btnCompare.Text = "比較";
			this.btnCompare.UseVisualStyleBackColor = true;
			this.btnCompare.Click += new System.EventHandler(this.btnCompare_Click);
			// 
			// tbOutput
			// 
			this.tbOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tbOutput.Location = new System.Drawing.Point(13, 121);
			this.tbOutput.Multiline = true;
			this.tbOutput.Name = "tbOutput";
			this.tbOutput.ReadOnly = true;
			this.tbOutput.Size = new System.Drawing.Size(563, 61);
			this.tbOutput.TabIndex = 8;
			// 
			// pb
			// 
			this.pb.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.pb.Location = new System.Drawing.Point(13, 91);
			this.pb.Name = "pb";
			this.pb.Size = new System.Drawing.Size(482, 23);
			this.pb.TabIndex = 7;
			// 
			// ofDlg
			// 
			this.ofDlg.FileName = "openFileDialog1";
			// 
			// bgWorker
			// 
			this.bgWorker.WorkerReportsProgress = true;
			this.bgWorker.WorkerSupportsCancellation = true;
			this.bgWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.bgWorker_DoWork);
			this.bgWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.bgWorker_ProgressChanged);
			this.bgWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.bgWorker_RunWorkerCompleted);
			// 
			// lblVer
			// 
			this.lblVer.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.lblVer.AutoSize = true;
			this.lblVer.Location = new System.Drawing.Point(11, 185);
			this.lblVer.Name = "lblVer";
			this.lblVer.Size = new System.Drawing.Size(59, 12);
			this.lblVer.TabIndex = 9;
			this.lblVer.Text = "Ver. 0.0.0.0";
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(588, 206);
			this.Controls.Add(this.lblVer);
			this.Controls.Add(this.pb);
			this.Controls.Add(this.tbOutput);
			this.Controls.Add(this.btnCompare);
			this.Controls.Add(this.btnBrowse2);
			this.Controls.Add(this.tbFile2);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.btnBrowse1);
			this.Controls.Add(this.tbFile1);
			this.Controls.Add(this.label1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Form1";
			this.Text = "Tiny File Compare for Windows";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox tbFile1;
		private System.Windows.Forms.Button btnBrowse1;
		private System.Windows.Forms.Button btnBrowse2;
		private System.Windows.Forms.TextBox tbFile2;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button btnCompare;
		private System.Windows.Forms.TextBox tbOutput;
		private System.Windows.Forms.ProgressBar pb;
		private System.Windows.Forms.OpenFileDialog ofDlg;
		private System.ComponentModel.BackgroundWorker bgWorker;
		private System.Windows.Forms.Label lblVer;
	}
}

