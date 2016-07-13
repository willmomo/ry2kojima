namespace gping {
	partial class Form1 {
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
			this.components = new System.ComponentModel.Container();
			this.button1 = new System.Windows.Forms.Button();
			this.daiList = new System.Windows.Forms.DataGridView();
			this.button2 = new System.Windows.Forms.Button();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.hdb = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ip = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.ping = new System.Windows.Forms.DataGridViewTextBoxColumn();
			this.stat = new System.Windows.Forms.DataGridViewTextBoxColumn();
			((System.ComponentModel.ISupportInitialize)(this.daiList)).BeginInit();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(12, 12);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(133, 41);
			this.button1.TabIndex = 0;
			this.button1.Text = "接続設定読込み";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// daiList
			// 
			this.daiList.AllowUserToAddRows = false;
			this.daiList.AllowUserToDeleteRows = false;
			this.daiList.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.daiList.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.daiList.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.hdb,
            this.ip,
            this.ping,
            this.stat});
			this.daiList.Location = new System.Drawing.Point(13, 59);
			this.daiList.Name = "daiList";
			this.daiList.ReadOnly = true;
			this.daiList.RowTemplate.Height = 21;
			this.daiList.Size = new System.Drawing.Size(656, 420);
			this.daiList.TabIndex = 1;
			this.daiList.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.daiList_CellContentClick);
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(151, 12);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(133, 41);
			this.button2.TabIndex = 2;
			this.button2.Text = "PING開始";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.button2_Click);
			// 
			// timer1
			// 
			this.timer1.Interval = 1000;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// hdb
			// 
			this.hdb.HeaderText = "表示台番号";
			this.hdb.Name = "hdb";
			this.hdb.ReadOnly = true;
			// 
			// ip
			// 
			this.ip.HeaderText = "IP";
			this.ip.Name = "ip";
			this.ip.ReadOnly = true;
			// 
			// ping
			// 
			this.ping.HeaderText = "Ping";
			this.ping.Name = "ping";
			this.ping.ReadOnly = true;
			this.ping.Width = 200;
			// 
			// stat
			// 
			this.stat.HeaderText = "Act";
			this.stat.Name = "stat";
			this.stat.ReadOnly = true;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(681, 491);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.daiList);
			this.Controls.Add(this.button1);
			this.Name = "Form1";
			this.Text = "Form1";
			((System.ComponentModel.ISupportInitialize)(this.daiList)).EndInit();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.DataGridView daiList;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Timer timer1;
		private System.Windows.Forms.DataGridViewTextBoxColumn hdb;
		private System.Windows.Forms.DataGridViewTextBoxColumn ip;
		private System.Windows.Forms.DataGridViewTextBoxColumn ping;
		private System.Windows.Forms.DataGridViewTextBoxColumn stat;
	}
}

