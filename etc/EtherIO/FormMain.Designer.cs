namespace EtherIO {
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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.btnWrite1 = new System.Windows.Forms.Button();
			this.btnRead1 = new System.Windows.Forms.Button();
			this.cbStatus1 = new System.Windows.Forms.ComboBox();
			this.label2 = new System.Windows.Forms.Label();
			this.cbMode1 = new System.Windows.Forms.ComboBox();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox2 = new System.Windows.Forms.GroupBox();
			this.btnWrite2 = new System.Windows.Forms.Button();
			this.btnRead2 = new System.Windows.Forms.Button();
			this.cbStatus2 = new System.Windows.Forms.ComboBox();
			this.label3 = new System.Windows.Forms.Label();
			this.cbMode2 = new System.Windows.Forms.ComboBox();
			this.label4 = new System.Windows.Forms.Label();
			this.groupBox3 = new System.Windows.Forms.GroupBox();
			this.btnWrite3 = new System.Windows.Forms.Button();
			this.btnRead3 = new System.Windows.Forms.Button();
			this.cbStatus3 = new System.Windows.Forms.ComboBox();
			this.label5 = new System.Windows.Forms.Label();
			this.cbMode3 = new System.Windows.Forms.ComboBox();
			this.label6 = new System.Windows.Forms.Label();
			this.groupBox4 = new System.Windows.Forms.GroupBox();
			this.btnWrite4 = new System.Windows.Forms.Button();
			this.btnRead4 = new System.Windows.Forms.Button();
			this.cbStatus4 = new System.Windows.Forms.ComboBox();
			this.label7 = new System.Windows.Forms.Label();
			this.cbMode4 = new System.Windows.Forms.ComboBox();
			this.label8 = new System.Windows.Forms.Label();
			this.btnReadAll = new System.Windows.Forms.Button();
			this.groupBox1.SuspendLayout();
			this.groupBox2.SuspendLayout();
			this.groupBox3.SuspendLayout();
			this.groupBox4.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.btnWrite1);
			this.groupBox1.Controls.Add(this.btnRead1);
			this.groupBox1.Controls.Add(this.cbStatus1);
			this.groupBox1.Controls.Add(this.label2);
			this.groupBox1.Controls.Add(this.cbMode1);
			this.groupBox1.Controls.Add(this.label1);
			this.groupBox1.Location = new System.Drawing.Point(12, 12);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(522, 55);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "channel #1";
			// 
			// btnWrite1
			// 
			this.btnWrite1.Location = new System.Drawing.Point(437, 16);
			this.btnWrite1.Name = "btnWrite1";
			this.btnWrite1.Size = new System.Drawing.Size(75, 23);
			this.btnWrite1.TabIndex = 5;
			this.btnWrite1.Tag = "1";
			this.btnWrite1.Text = "Write";
			this.btnWrite1.UseVisualStyleBackColor = true;
			this.btnWrite1.Click += new System.EventHandler(this.btnWrite_Click);
			// 
			// btnRead1
			// 
			this.btnRead1.Location = new System.Drawing.Point(356, 17);
			this.btnRead1.Name = "btnRead1";
			this.btnRead1.Size = new System.Drawing.Size(75, 23);
			this.btnRead1.TabIndex = 4;
			this.btnRead1.Tag = "1";
			this.btnRead1.Text = "Read";
			this.btnRead1.UseVisualStyleBackColor = true;
			this.btnRead1.Click += new System.EventHandler(this.btnRead_Click);
			// 
			// cbStatus1
			// 
			this.cbStatus1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbStatus1.FormattingEnabled = true;
			this.cbStatus1.Items.AddRange(new object[] {
            "0:low",
            "1:high"});
			this.cbStatus1.Location = new System.Drawing.Point(217, 19);
			this.cbStatus1.Name = "cbStatus1";
			this.cbStatus1.Size = new System.Drawing.Size(121, 20);
			this.cbStatus1.TabIndex = 3;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(174, 21);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(37, 12);
			this.label2.TabIndex = 2;
			this.label2.Text = "status";
			// 
			// cbMode1
			// 
			this.cbMode1.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbMode1.FormattingEnabled = true;
			this.cbMode1.Items.AddRange(new object[] {
            "0:input",
            "1:output"});
			this.cbMode1.Location = new System.Drawing.Point(46, 19);
			this.cbMode1.Name = "cbMode1";
			this.cbMode1.Size = new System.Drawing.Size(121, 20);
			this.cbMode1.TabIndex = 1;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(8, 22);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(32, 12);
			this.label1.TabIndex = 0;
			this.label1.Text = "mode";
			// 
			// groupBox2
			// 
			this.groupBox2.Controls.Add(this.btnWrite2);
			this.groupBox2.Controls.Add(this.btnRead2);
			this.groupBox2.Controls.Add(this.cbStatus2);
			this.groupBox2.Controls.Add(this.label3);
			this.groupBox2.Controls.Add(this.cbMode2);
			this.groupBox2.Controls.Add(this.label4);
			this.groupBox2.Location = new System.Drawing.Point(12, 73);
			this.groupBox2.Name = "groupBox2";
			this.groupBox2.Size = new System.Drawing.Size(522, 55);
			this.groupBox2.TabIndex = 1;
			this.groupBox2.TabStop = false;
			this.groupBox2.Text = "channel #2";
			// 
			// btnWrite2
			// 
			this.btnWrite2.Location = new System.Drawing.Point(437, 16);
			this.btnWrite2.Name = "btnWrite2";
			this.btnWrite2.Size = new System.Drawing.Size(75, 23);
			this.btnWrite2.TabIndex = 5;
			this.btnWrite2.Tag = "2";
			this.btnWrite2.Text = "Write";
			this.btnWrite2.UseVisualStyleBackColor = true;
			this.btnWrite2.Click += new System.EventHandler(this.btnWrite_Click);
			// 
			// btnRead2
			// 
			this.btnRead2.Location = new System.Drawing.Point(356, 17);
			this.btnRead2.Name = "btnRead2";
			this.btnRead2.Size = new System.Drawing.Size(75, 23);
			this.btnRead2.TabIndex = 4;
			this.btnRead2.Tag = "2";
			this.btnRead2.Text = "Read";
			this.btnRead2.UseVisualStyleBackColor = true;
			this.btnRead2.Click += new System.EventHandler(this.btnRead_Click);
			// 
			// cbStatus2
			// 
			this.cbStatus2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbStatus2.FormattingEnabled = true;
			this.cbStatus2.Items.AddRange(new object[] {
            "0:low",
            "1:high"});
			this.cbStatus2.Location = new System.Drawing.Point(217, 19);
			this.cbStatus2.Name = "cbStatus2";
			this.cbStatus2.Size = new System.Drawing.Size(121, 20);
			this.cbStatus2.TabIndex = 3;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(174, 21);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(37, 12);
			this.label3.TabIndex = 2;
			this.label3.Text = "status";
			// 
			// cbMode2
			// 
			this.cbMode2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbMode2.FormattingEnabled = true;
			this.cbMode2.Items.AddRange(new object[] {
            "0:input",
            "1:output"});
			this.cbMode2.Location = new System.Drawing.Point(46, 19);
			this.cbMode2.Name = "cbMode2";
			this.cbMode2.Size = new System.Drawing.Size(121, 20);
			this.cbMode2.TabIndex = 1;
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(8, 22);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(32, 12);
			this.label4.TabIndex = 0;
			this.label4.Text = "mode";
			// 
			// groupBox3
			// 
			this.groupBox3.Controls.Add(this.btnWrite3);
			this.groupBox3.Controls.Add(this.btnRead3);
			this.groupBox3.Controls.Add(this.cbStatus3);
			this.groupBox3.Controls.Add(this.label5);
			this.groupBox3.Controls.Add(this.cbMode3);
			this.groupBox3.Controls.Add(this.label6);
			this.groupBox3.Location = new System.Drawing.Point(12, 134);
			this.groupBox3.Name = "groupBox3";
			this.groupBox3.Size = new System.Drawing.Size(522, 55);
			this.groupBox3.TabIndex = 2;
			this.groupBox3.TabStop = false;
			this.groupBox3.Text = "channel #3";
			// 
			// btnWrite3
			// 
			this.btnWrite3.Location = new System.Drawing.Point(437, 16);
			this.btnWrite3.Name = "btnWrite3";
			this.btnWrite3.Size = new System.Drawing.Size(75, 23);
			this.btnWrite3.TabIndex = 5;
			this.btnWrite3.Tag = "3";
			this.btnWrite3.Text = "Write";
			this.btnWrite3.UseVisualStyleBackColor = true;
			this.btnWrite3.Click += new System.EventHandler(this.btnWrite_Click);
			// 
			// btnRead3
			// 
			this.btnRead3.Location = new System.Drawing.Point(356, 17);
			this.btnRead3.Name = "btnRead3";
			this.btnRead3.Size = new System.Drawing.Size(75, 23);
			this.btnRead3.TabIndex = 4;
			this.btnRead3.Tag = "3";
			this.btnRead3.Text = "Read";
			this.btnRead3.UseVisualStyleBackColor = true;
			this.btnRead3.Click += new System.EventHandler(this.btnRead_Click);
			// 
			// cbStatus3
			// 
			this.cbStatus3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbStatus3.FormattingEnabled = true;
			this.cbStatus3.Items.AddRange(new object[] {
            "0:low",
            "1:high"});
			this.cbStatus3.Location = new System.Drawing.Point(217, 19);
			this.cbStatus3.Name = "cbStatus3";
			this.cbStatus3.Size = new System.Drawing.Size(121, 20);
			this.cbStatus3.TabIndex = 3;
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(174, 21);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(37, 12);
			this.label5.TabIndex = 2;
			this.label5.Text = "status";
			// 
			// cbMode3
			// 
			this.cbMode3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbMode3.FormattingEnabled = true;
			this.cbMode3.Items.AddRange(new object[] {
            "0:input",
            "1:output"});
			this.cbMode3.Location = new System.Drawing.Point(46, 19);
			this.cbMode3.Name = "cbMode3";
			this.cbMode3.Size = new System.Drawing.Size(121, 20);
			this.cbMode3.TabIndex = 1;
			// 
			// label6
			// 
			this.label6.AutoSize = true;
			this.label6.Location = new System.Drawing.Point(8, 22);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(32, 12);
			this.label6.TabIndex = 0;
			this.label6.Text = "mode";
			// 
			// groupBox4
			// 
			this.groupBox4.Controls.Add(this.btnWrite4);
			this.groupBox4.Controls.Add(this.btnRead4);
			this.groupBox4.Controls.Add(this.cbStatus4);
			this.groupBox4.Controls.Add(this.label7);
			this.groupBox4.Controls.Add(this.cbMode4);
			this.groupBox4.Controls.Add(this.label8);
			this.groupBox4.Location = new System.Drawing.Point(12, 195);
			this.groupBox4.Name = "groupBox4";
			this.groupBox4.Size = new System.Drawing.Size(522, 55);
			this.groupBox4.TabIndex = 3;
			this.groupBox4.TabStop = false;
			this.groupBox4.Text = "channel #4";
			// 
			// btnWrite4
			// 
			this.btnWrite4.Location = new System.Drawing.Point(437, 16);
			this.btnWrite4.Name = "btnWrite4";
			this.btnWrite4.Size = new System.Drawing.Size(75, 23);
			this.btnWrite4.TabIndex = 5;
			this.btnWrite4.Tag = "4";
			this.btnWrite4.Text = "Write";
			this.btnWrite4.UseVisualStyleBackColor = true;
			this.btnWrite4.Click += new System.EventHandler(this.btnWrite_Click);
			// 
			// btnRead4
			// 
			this.btnRead4.Location = new System.Drawing.Point(356, 17);
			this.btnRead4.Name = "btnRead4";
			this.btnRead4.Size = new System.Drawing.Size(75, 23);
			this.btnRead4.TabIndex = 4;
			this.btnRead4.Tag = "4";
			this.btnRead4.Text = "Read";
			this.btnRead4.UseVisualStyleBackColor = true;
			this.btnRead4.Click += new System.EventHandler(this.btnRead_Click);
			// 
			// cbStatus4
			// 
			this.cbStatus4.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbStatus4.FormattingEnabled = true;
			this.cbStatus4.Items.AddRange(new object[] {
            "0:low",
            "1:high"});
			this.cbStatus4.Location = new System.Drawing.Point(217, 19);
			this.cbStatus4.Name = "cbStatus4";
			this.cbStatus4.Size = new System.Drawing.Size(121, 20);
			this.cbStatus4.TabIndex = 3;
			// 
			// label7
			// 
			this.label7.AutoSize = true;
			this.label7.Location = new System.Drawing.Point(174, 21);
			this.label7.Name = "label7";
			this.label7.Size = new System.Drawing.Size(37, 12);
			this.label7.TabIndex = 2;
			this.label7.Text = "status";
			// 
			// cbMode4
			// 
			this.cbMode4.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
			this.cbMode4.FormattingEnabled = true;
			this.cbMode4.Items.AddRange(new object[] {
            "0:input",
            "1:output"});
			this.cbMode4.Location = new System.Drawing.Point(46, 19);
			this.cbMode4.Name = "cbMode4";
			this.cbMode4.Size = new System.Drawing.Size(121, 20);
			this.cbMode4.TabIndex = 1;
			// 
			// label8
			// 
			this.label8.AutoSize = true;
			this.label8.Location = new System.Drawing.Point(8, 22);
			this.label8.Name = "label8";
			this.label8.Size = new System.Drawing.Size(32, 12);
			this.label8.TabIndex = 0;
			this.label8.Text = "mode";
			// 
			// btnReadAll
			// 
			this.btnReadAll.Location = new System.Drawing.Point(236, 256);
			this.btnReadAll.Name = "btnReadAll";
			this.btnReadAll.Size = new System.Drawing.Size(75, 23);
			this.btnReadAll.TabIndex = 4;
			this.btnReadAll.Text = "Read All";
			this.btnReadAll.UseVisualStyleBackColor = true;
			this.btnReadAll.Click += new System.EventHandler(this.btnReadAll_Click);
			// 
			// FormMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(546, 291);
			this.Controls.Add(this.btnReadAll);
			this.Controls.Add(this.groupBox4);
			this.Controls.Add(this.groupBox3);
			this.Controls.Add(this.groupBox2);
			this.Controls.Add(this.groupBox1);
			this.Name = "FormMain";
			this.Text = "MiiNePort E3 controller";
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.groupBox2.ResumeLayout(false);
			this.groupBox2.PerformLayout();
			this.groupBox3.ResumeLayout(false);
			this.groupBox3.PerformLayout();
			this.groupBox4.ResumeLayout(false);
			this.groupBox4.PerformLayout();
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.ComboBox cbMode1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button btnWrite1;
		private System.Windows.Forms.Button btnRead1;
		private System.Windows.Forms.ComboBox cbStatus1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.Button btnWrite2;
		private System.Windows.Forms.Button btnRead2;
		private System.Windows.Forms.ComboBox cbStatus2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ComboBox cbMode2;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.Button btnWrite3;
		private System.Windows.Forms.Button btnRead3;
		private System.Windows.Forms.ComboBox cbStatus3;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.ComboBox cbMode3;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.Button btnWrite4;
		private System.Windows.Forms.Button btnRead4;
		private System.Windows.Forms.ComboBox cbStatus4;
		private System.Windows.Forms.Label label7;
		private System.Windows.Forms.ComboBox cbMode4;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.Button btnReadAll;
	}
}

