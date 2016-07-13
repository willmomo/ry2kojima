using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

namespace KanHisho {
	public partial class Balloon : Form {
		public Balloon() {
			InitializeComponent();
		}

		private void Balloon_Load(object sender, EventArgs e) {
			Bitmap img = new Bitmap(Path.Combine(Path.GetDirectoryName(Application.ExecutablePath), @"res\balloon.png"));
			img.MakeTransparent();

			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
			this.BackgroundImageLayout = ImageLayout.Stretch;
			this.BackgroundImage = img;
			this.TransparencyKey = this.BackColor;

			Kjm.User32.SetNotActiveWindow(this);

			using (Graphics g = this.CreateGraphics()) {
				Font font = new Font(FontFamily.GenericSansSerif, 10);
				SizeF size = g.MeasureString(m_msg, font);
				//this.Width = (int)(size.Width + 80.0f);
				this.Height = (int)(size.Height + 40.0f);
			}

			System.Diagnostics.Debug.WriteLine(string.Format("this.Owner.Bottom: {0}", this.Owner.Bottom));

			this.Top = this.Owner.Bottom - this.Height;

			// こちらは、ワークエリアの右下
			/*
			Kjm.WorkArea wa = new Kjm.WorkArea();
			this.Top = wa.Bottom - this.Height - 1;
			 */

			this.Opacity = 0.9;
			timer1.Interval = 5000;
			timer1.Enabled = true;
		}

		private void Balloon_Click(object sender, EventArgs e) {
			this.Close();
		}

		private string m_msg;

		public void ShowMessage(string msg, Form owner) {
			m_msg = msg;

			Kjm.WorkArea wa = new Kjm.WorkArea();
			this.Left = owner.Left;
			this.Width = owner.Width;
			this.Height = 1;
			//this.Top = wa.Bottom - this.Height - 1;
			this.Top = owner.Bottom - this.Height;

			System.Diagnostics.Debug.WriteLine(string.Format("owner.Bottom: {0}", owner.Bottom));

			this.Show(owner);
		}

		private void Balloon_Paint(object sender, PaintEventArgs e) {
			Font font = new Font(FontFamily.GenericSansSerif, 12);
			Brush brush = new SolidBrush(Color.Black);
			PointF point = new PointF(0.0f, 0.0f);

			SizeF size = e.Graphics.MeasureString(m_msg, font);
			RectangleF rect = new RectangleF(new PointF(0, 0), new SizeF(this.Width, this.Height));

			StringFormat sf = new StringFormat();
			sf.Alignment = StringAlignment.Center;
			sf.LineAlignment = StringAlignment.Center;

			e.Graphics.DrawString(m_msg, font, brush, rect, sf);
		}

		private void timer1_Tick(object sender, EventArgs e) {
			timer1.Enabled = false;
			this.Close();
		}
	}
}
