namespace bfkjmw {
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
			this.label1 = new System.Windows.Forms.Label();
			this.txtData = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.txtKey = new System.Windows.Forms.TextBox();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.btnEncrypt = new System.Windows.Forms.Button();
			this.btnDecrypt = new System.Windows.Forms.Button();
			this.chkCopy = new System.Windows.Forms.CheckBox();
			this.chkQuit = new System.Windows.Forms.CheckBox();
			this.chkMemory = new System.Windows.Forms.CheckBox();
			this.tableLayoutPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(13, 13);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(45, 16);
			this.label1.TabIndex = 3;
			this.label1.Text = "データ";
			// 
			// txtData
			// 
			this.txtData.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtData.Location = new System.Drawing.Point(13, 33);
			this.txtData.Name = "txtData";
			this.txtData.Size = new System.Drawing.Size(401, 23);
			this.txtData.TabIndex = 4;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(13, 63);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(82, 16);
			this.label2.TabIndex = 0;
			this.label2.Text = "パスフレーズ";
			// 
			// txtKey
			// 
			this.txtKey.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.txtKey.Location = new System.Drawing.Point(13, 83);
			this.txtKey.Name = "txtKey";
			this.txtKey.Size = new System.Drawing.Size(401, 23);
			this.txtKey.TabIndex = 1;
			this.txtKey.UseSystemPasswordChar = true;
			this.txtKey.TextChanged += new System.EventHandler(this.txtKey_TextChanged);
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel1.ColumnCount = 2;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.Controls.Add(this.btnEncrypt, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this.btnDecrypt, 1, 0);
			this.tableLayoutPanel1.Enabled = false;
			this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 210);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 1;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(402, 71);
			this.tableLayoutPanel1.TabIndex = 2;
			// 
			// btnEncrypt
			// 
			this.btnEncrypt.Dock = System.Windows.Forms.DockStyle.Fill;
			this.btnEncrypt.Location = new System.Drawing.Point(3, 3);
			this.btnEncrypt.Name = "btnEncrypt";
			this.btnEncrypt.Size = new System.Drawing.Size(195, 65);
			this.btnEncrypt.TabIndex = 0;
			this.btnEncrypt.Text = "暗号化(&E)";
			this.btnEncrypt.UseVisualStyleBackColor = true;
			this.btnEncrypt.Click += new System.EventHandler(this.btnEncrypt_Click);
			// 
			// btnDecrypt
			// 
			this.btnDecrypt.Dock = System.Windows.Forms.DockStyle.Fill;
			this.btnDecrypt.Location = new System.Drawing.Point(204, 3);
			this.btnDecrypt.Name = "btnDecrypt";
			this.btnDecrypt.Size = new System.Drawing.Size(195, 65);
			this.btnDecrypt.TabIndex = 1;
			this.btnDecrypt.Text = "復号化(&D)";
			this.btnDecrypt.UseVisualStyleBackColor = true;
			this.btnDecrypt.Click += new System.EventHandler(this.btnDecrypt_Click);
			// 
			// chkCopy
			// 
			this.chkCopy.AutoSize = true;
			this.chkCopy.Location = new System.Drawing.Point(16, 113);
			this.chkCopy.Name = "chkCopy";
			this.chkCopy.Size = new System.Drawing.Size(278, 20);
			this.chkCopy.TabIndex = 5;
			this.chkCopy.Text = "変換結果をクリップボードにコピーする(&C)";
			this.chkCopy.UseVisualStyleBackColor = true;
			this.chkCopy.CheckedChanged += new System.EventHandler(this.chkCopy_CheckedChanged);
			// 
			// chkQuit
			// 
			this.chkQuit.AutoSize = true;
			this.chkQuit.Location = new System.Drawing.Point(16, 139);
			this.chkQuit.Name = "chkQuit";
			this.chkQuit.Size = new System.Drawing.Size(180, 20);
			this.chkQuit.TabIndex = 6;
			this.chkQuit.Text = "変換後、即終了する(&X)";
			this.chkQuit.UseVisualStyleBackColor = true;
			this.chkQuit.CheckedChanged += new System.EventHandler(this.chkQuit_CheckedChanged);
			// 
			// chkMemory
			// 
			this.chkMemory.AutoSize = true;
			this.chkMemory.Location = new System.Drawing.Point(16, 166);
			this.chkMemory.Name = "chkMemory";
			this.chkMemory.Size = new System.Drawing.Size(198, 20);
			this.chkMemory.TabIndex = 7;
			this.chkMemory.Text = "パスフレーズを覚えておく(&M)";
			this.chkMemory.UseVisualStyleBackColor = true;
			this.chkMemory.CheckedChanged += new System.EventHandler(this.chkMemory_CheckedChanged);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 16F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(426, 293);
			this.Controls.Add(this.chkMemory);
			this.Controls.Add(this.chkQuit);
			this.Controls.Add(this.chkCopy);
			this.Controls.Add(this.tableLayoutPanel1);
			this.Controls.Add(this.txtKey);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.txtData);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("MS UI Gothic", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(128)));
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.Margin = new System.Windows.Forms.Padding(4, 4, 4, 4);
			this.MaximizeBox = false;
			this.Name = "Form1";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "暗号化/復号化";
			this.Activated += new System.EventHandler(this.Form1_Activated);
			this.Load += new System.EventHandler(this.Form1_Load);
			this.Shown += new System.EventHandler(this.Form1_Shown);
			this.tableLayoutPanel1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txtData;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox txtKey;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.Button btnEncrypt;
		private System.Windows.Forms.Button btnDecrypt;
		private System.Windows.Forms.CheckBox chkCopy;
		private System.Windows.Forms.CheckBox chkQuit;
		private System.Windows.Forms.CheckBox chkMemory;


	}
}

