namespace kancVoiceGet {
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
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
			this.button1 = new System.Windows.Forms.Button();
			this.listBox1 = new System.Windows.Forms.ListBox();
			this.tbReferenceURL = new System.Windows.Forms.TextBox();
			this.textBox2 = new System.Windows.Forms.TextBox();
			this.lbDestinationFolder = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.button1.Location = new System.Drawing.Point(12, 125);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(465, 62);
			this.button1.TabIndex = 5;
			this.button1.Text = "音声取得(&G)";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// listBox1
			// 
			this.listBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.listBox1.FormattingEnabled = true;
			this.listBox1.IntegralHeight = false;
			this.listBox1.ItemHeight = 12;
			this.listBox1.Location = new System.Drawing.Point(12, 206);
			this.listBox1.Name = "listBox1";
			this.listBox1.Size = new System.Drawing.Size(465, 167);
			this.listBox1.TabIndex = 7;
			// 
			// tbReferenceURL
			// 
			this.tbReferenceURL.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tbReferenceURL.Location = new System.Drawing.Point(12, 24);
			this.tbReferenceURL.Name = "tbReferenceURL";
			this.tbReferenceURL.Size = new System.Drawing.Size(465, 19);
			this.tbReferenceURL.TabIndex = 1;
			this.tbReferenceURL.Enter += new System.EventHandler(this.textBox1_Enter);
			// 
			// textBox2
			// 
			this.textBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.textBox2.Location = new System.Drawing.Point(12, 65);
			this.textBox2.Name = "textBox2";
			this.textBox2.Size = new System.Drawing.Size(464, 19);
			this.textBox2.TabIndex = 3;
			this.textBox2.TextChanged += new System.EventHandler(this.textBox2_TextChanged);
			this.textBox2.Enter += new System.EventHandler(this.textBox2_Enter);
			// 
			// lbDestinationFolder
			// 
			this.lbDestinationFolder.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.lbDestinationFolder.Location = new System.Drawing.Point(12, 90);
			this.lbDestinationFolder.Name = "lbDestinationFolder";
			this.lbDestinationFolder.Size = new System.Drawing.Size(463, 32);
			this.lbDestinationFolder.TabIndex = 4;
			this.lbDestinationFolder.Text = "(ファイルの保存先)";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 9);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(67, 12);
			this.label2.TabIndex = 0;
			this.label2.Text = "参考URL(&U)";
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(14, 50);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(64, 12);
			this.label3.TabIndex = 2;
			this.label3.Text = "艦むす名(&K)";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(12, 191);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(45, 12);
			this.label4.TabIndex = 6;
			this.label4.Text = "結果(&R)";
			// 
			// timer1
			// 
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(489, 385);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.lbDestinationFolder);
			this.Controls.Add(this.textBox2);
			this.Controls.Add(this.tbReferenceURL);
			this.Controls.Add(this.listBox1);
			this.Controls.Add(this.button1);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Form1";
			this.Text = "艦むす音声取得";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.ListBox listBox1;
		private System.Windows.Forms.TextBox tbReferenceURL;
		private System.Windows.Forms.TextBox textBox2;
		private System.Windows.Forms.Label lbDestinationFolder;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Timer timer1;
	}
}

