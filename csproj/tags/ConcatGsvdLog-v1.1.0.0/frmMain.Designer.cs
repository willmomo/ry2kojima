namespace ConcatGsvdLog {
	partial class frmMain {
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
			this.label1 = new System.Windows.Forms.Label();
			this.txtSrc = new System.Windows.Forms.TextBox();
			this.btnSrcBrowse = new System.Windows.Forms.Button();
			this.btnConcat = new System.Windows.Forms.Button();
			this.lblOutput = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.txtDst = new System.Windows.Forms.TextBox();
			this.btnDstBrowse = new System.Windows.Forms.Button();
			this.bgWorker = new System.ComponentModel.BackgroundWorker();
			this.btnCancel = new System.Windows.Forms.Button();
			this.progBar = new System.Windows.Forms.ProgressBar();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(15, 14);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(223, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "gsvdlogのあるフォルダ（通常：C:\\gsvdlog）";
			// 
			// txtSrc
			// 
			this.txtSrc.AllowDrop = true;
			this.txtSrc.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtSrc.Location = new System.Drawing.Point(15, 31);
			this.txtSrc.Name = "txtSrc";
			this.txtSrc.Size = new System.Drawing.Size(626, 20);
			this.txtSrc.TabIndex = 1;
			// 
			// btnSrcBrowse
			// 
			this.btnSrcBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnSrcBrowse.Location = new System.Drawing.Point(649, 26);
			this.btnSrcBrowse.Name = "btnSrcBrowse";
			this.btnSrcBrowse.Size = new System.Drawing.Size(87, 25);
			this.btnSrcBrowse.TabIndex = 2;
			this.btnSrcBrowse.Text = "参照(&B)";
			this.btnSrcBrowse.UseVisualStyleBackColor = true;
			this.btnSrcBrowse.Click += new System.EventHandler(this.btnSrcBrowse_Click);
			// 
			// btnConcat
			// 
			this.btnConcat.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnConcat.Location = new System.Drawing.Point(554, 194);
			this.btnConcat.Name = "btnConcat";
			this.btnConcat.Size = new System.Drawing.Size(87, 25);
			this.btnConcat.TabIndex = 7;
			this.btnConcat.Text = "結合(&C)";
			this.btnConcat.UseVisualStyleBackColor = true;
			this.btnConcat.Click += new System.EventHandler(this.btnConcat_Click);
			// 
			// lblOutput
			// 
			this.lblOutput.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.lblOutput.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.lblOutput.Location = new System.Drawing.Point(13, 123);
			this.lblOutput.Name = "lblOutput";
			this.lblOutput.Size = new System.Drawing.Size(723, 66);
			this.lblOutput.TabIndex = 6;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(12, 54);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(46, 13);
			this.label3.TabIndex = 3;
			this.label3.Text = "出力先";
			// 
			// txtDst
			// 
			this.txtDst.AllowDrop = true;
			this.txtDst.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.txtDst.Location = new System.Drawing.Point(12, 70);
			this.txtDst.Name = "txtDst";
			this.txtDst.Size = new System.Drawing.Size(626, 20);
			this.txtDst.TabIndex = 4;
			// 
			// btnDstBrowse
			// 
			this.btnDstBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnDstBrowse.Location = new System.Drawing.Point(651, 67);
			this.btnDstBrowse.Name = "btnDstBrowse";
			this.btnDstBrowse.Size = new System.Drawing.Size(87, 25);
			this.btnDstBrowse.TabIndex = 5;
			this.btnDstBrowse.Text = "参照(&R)";
			this.btnDstBrowse.UseVisualStyleBackColor = true;
			this.btnDstBrowse.Click += new System.EventHandler(this.btnDstBrowse_Click);
			// 
			// bgWorker
			// 
			this.bgWorker.WorkerReportsProgress = true;
			this.bgWorker.WorkerSupportsCancellation = true;
			this.bgWorker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.bgWorker_DoWork);
			this.bgWorker.RunWorkerCompleted += new System.ComponentModel.RunWorkerCompletedEventHandler(this.bgWorker_RunWorkerCompleted);
			this.bgWorker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.bgWorker_ProgressChanged);
			// 
			// btnCancel
			// 
			this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnCancel.Enabled = false;
			this.btnCancel.Location = new System.Drawing.Point(647, 194);
			this.btnCancel.Name = "btnCancel";
			this.btnCancel.Size = new System.Drawing.Size(87, 25);
			this.btnCancel.TabIndex = 8;
			this.btnCancel.Text = "キャンセル";
			this.btnCancel.UseVisualStyleBackColor = true;
			this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
			// 
			// progBar
			// 
			this.progBar.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.progBar.Location = new System.Drawing.Point(12, 97);
			this.progBar.Name = "progBar";
			this.progBar.Size = new System.Drawing.Size(724, 23);
			this.progBar.TabIndex = 9;
			// 
			// frmMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(750, 231);
			this.Controls.Add(this.progBar);
			this.Controls.Add(this.btnCancel);
			this.Controls.Add(this.btnDstBrowse);
			this.Controls.Add(this.txtDst);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.lblOutput);
			this.Controls.Add(this.btnConcat);
			this.Controls.Add(this.btnSrcBrowse);
			this.Controls.Add(this.txtSrc);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("MS UI Gothic", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.Name = "frmMain";
			this.Text = "gsvdlogの結合";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtSrc;
		private System.Windows.Forms.Button btnSrcBrowse;
		private System.Windows.Forms.Button btnConcat;
		private System.Windows.Forms.Label lblOutput;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox txtDst;
		private System.Windows.Forms.Button btnDstBrowse;
		private System.ComponentModel.BackgroundWorker bgWorker;
		private System.Windows.Forms.Button btnCancel;
		private System.Windows.Forms.ProgressBar progBar;
	}
}

