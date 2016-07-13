namespace VideoSlice {
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
			this.button1 = new System.Windows.Forms.Button();
			this.progressBar1 = new System.Windows.Forms.ProgressBar();
			this.progressBar2 = new System.Windows.Forms.ProgressBar();
			this.backgroundWorker1 = new System.ComponentModel.BackgroundWorker();
			this.tbYoko = new System.Windows.Forms.TextBox();
			this.tbTate = new System.Windows.Forms.TextBox();
			this.tbFPS = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.checkBox1 = new System.Windows.Forms.CheckBox();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.button1.Location = new System.Drawing.Point(12, 89);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(357, 128);
			this.button1.TabIndex = 0;
			this.button1.Text = "動画を選んで変換";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// progressBar1
			// 
			this.progressBar1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.progressBar1.Location = new System.Drawing.Point(12, 236);
			this.progressBar1.Name = "progressBar1";
			this.progressBar1.Size = new System.Drawing.Size(357, 23);
			this.progressBar1.TabIndex = 1;
			// 
			// progressBar2
			// 
			this.progressBar2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.progressBar2.Location = new System.Drawing.Point(12, 265);
			this.progressBar2.Name = "progressBar2";
			this.progressBar2.Size = new System.Drawing.Size(357, 23);
			this.progressBar2.TabIndex = 2;
			// 
			// backgroundWorker1
			// 
			this.backgroundWorker1.DoWork += new System.ComponentModel.DoWorkEventHandler(this.backgroundWorker1_DoWork);
			// 
			// tbYoko
			// 
			this.tbYoko.Location = new System.Drawing.Point(13, 13);
			this.tbYoko.Name = "tbYoko";
			this.tbYoko.Size = new System.Drawing.Size(100, 19);
			this.tbYoko.TabIndex = 1;
			this.tbYoko.Text = "64";
			// 
			// tbTate
			// 
			this.tbTate.Location = new System.Drawing.Point(119, 13);
			this.tbTate.Name = "tbTate";
			this.tbTate.Size = new System.Drawing.Size(100, 19);
			this.tbTate.TabIndex = 2;
			this.tbTate.Text = "32";
			// 
			// tbFPS
			// 
			this.tbFPS.Location = new System.Drawing.Point(119, 39);
			this.tbFPS.Name = "tbFPS";
			this.tbFPS.Size = new System.Drawing.Size(100, 19);
			this.tbFPS.TabIndex = 3;
			this.tbFPS.Text = "30";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(226, 45);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(21, 12);
			this.label1.TabIndex = 6;
			this.label1.Text = "fps";
			// 
			// checkBox1
			// 
			this.checkBox1.AutoSize = true;
			this.checkBox1.Location = new System.Drawing.Point(119, 65);
			this.checkBox1.Name = "checkBox1";
			this.checkBox1.Size = new System.Drawing.Size(130, 16);
			this.checkBox1.TabIndex = 4;
			this.checkBox1.Text = "古いフォーマットで出力";
			this.checkBox1.UseVisualStyleBackColor = true;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(381, 300);
			this.Controls.Add(this.checkBox1);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.tbFPS);
			this.Controls.Add(this.tbTate);
			this.Controls.Add(this.tbYoko);
			this.Controls.Add(this.progressBar2);
			this.Controls.Add(this.progressBar1);
			this.Controls.Add(this.button1);
			this.Name = "Form1";
			this.Text = "VideoSlice";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.ProgressBar progressBar1;
		private System.Windows.Forms.ProgressBar progressBar2;
		private System.ComponentModel.BackgroundWorker backgroundWorker1;
		private System.Windows.Forms.TextBox tbYoko;
		private System.Windows.Forms.TextBox tbTate;
		private System.Windows.Forms.TextBox tbFPS;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.CheckBox checkBox1;
	}
}

