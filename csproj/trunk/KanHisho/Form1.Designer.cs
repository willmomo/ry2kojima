namespace KanHisho {
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
			this.ni = new System.Windows.Forms.NotifyIcon(this.components);
			this.cms = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.秘書官選択SToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.終了XToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.timer1 = new System.Windows.Forms.Timer(this.components);
			this.worker = new System.ComponentModel.BackgroundWorker();
			this.cms.SuspendLayout();
			this.SuspendLayout();
			// 
			// ni
			// 
			this.ni.ContextMenuStrip = this.cms;
			this.ni.Icon = ((System.Drawing.Icon)(resources.GetObject("ni.Icon")));
			this.ni.Text = "艦これ秘書";
			this.ni.Visible = true;
			this.ni.Click += new System.EventHandler(this.ni_Click);
			this.ni.DoubleClick += new System.EventHandler(this.ni_DoubleClick);
			this.ni.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ni_MouseUp);
			// 
			// cms
			// 
			this.cms.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.秘書官選択SToolStripMenuItem,
            this.終了XToolStripMenuItem});
			this.cms.Name = "cms";
			this.cms.Size = new System.Drawing.Size(158, 48);
			// 
			// 秘書官選択SToolStripMenuItem
			// 
			this.秘書官選択SToolStripMenuItem.Name = "秘書官選択SToolStripMenuItem";
			this.秘書官選択SToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
			this.秘書官選択SToolStripMenuItem.Text = "秘書艦選択(&S)...";
			this.秘書官選択SToolStripMenuItem.Click += new System.EventHandler(this.秘書官選択SToolStripMenuItem_Click);
			// 
			// 終了XToolStripMenuItem
			// 
			this.終了XToolStripMenuItem.Name = "終了XToolStripMenuItem";
			this.終了XToolStripMenuItem.Size = new System.Drawing.Size(157, 22);
			this.終了XToolStripMenuItem.Text = "終了(&X)";
			this.終了XToolStripMenuItem.Click += new System.EventHandler(this.終了XToolStripMenuItem_Click);
			// 
			// timer1
			// 
			this.timer1.Interval = 1000;
			this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
			// 
			// worker
			// 
			this.worker.WorkerReportsProgress = true;
			this.worker.DoWork += new System.ComponentModel.DoWorkEventHandler(this.worker_DoWork);
			this.worker.ProgressChanged += new System.ComponentModel.ProgressChangedEventHandler(this.worker_ProgressChanged);
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(284, 262);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "Form1";
			this.ShowInTaskbar = false;
			this.Text = "Form1";
			this.TopMost = true;
			this.Load += new System.EventHandler(this.Form1_Load);
			this.Click += new System.EventHandler(this.Form1_Click);
			this.Paint += new System.Windows.Forms.PaintEventHandler(this.Form1_Paint);
			this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Form1_MouseMove);
			this.cms.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.NotifyIcon ni;
		private System.Windows.Forms.Timer timer1;
		private System.ComponentModel.BackgroundWorker worker;
		private System.Windows.Forms.ContextMenuStrip cms;
		private System.Windows.Forms.ToolStripMenuItem 終了XToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem 秘書官選択SToolStripMenuItem;
	}
}

