using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AxisView {
    public partial class FormMain : Form {
        private string[] m_files;
        private int m_fileTop;

        public FormMain() {
            InitializeComponent();

            this.MouseWheel += new MouseEventHandler(Form1_MouseWheel);

            this.pictureBox1.MouseWheel += new MouseEventHandler(pictureBox1_MouseWheel);

            this.trackBar1.MouseWheel += new MouseEventHandler(trackBar1_MouseWheel);
        }

        void trackBar1_MouseWheel(object sender, MouseEventArgs e) {
        }

        void pictureBox1_MouseWheel(object sender, MouseEventArgs e) {
            int prevFileTop = m_fileTop;

            m_fileTop += e.Delta;

            if (m_fileTop < -2) m_fileTop = -2;
            if (m_fileTop >= m_files.Length) m_fileTop = m_files.Length - 1;

            if (m_fileTop != prevFileTop) pictureBox1.Invalidate();
        }

        void Form1_MouseWheel(object sender, MouseEventArgs e) {
        }

        private void Form1_Paint(object sender, PaintEventArgs e) {
        }

        private void pictureBox1_Paint(object sender, PaintEventArgs e) {
            Graphics g = e.Graphics;

            //g.DrawLine(new Pen(Color.Black), new Point(10, 10), new Point(this.Width - 20, 10));

            //g.DrawString("17:00", new Font("ＭＳ Ｐゴシック", 10.0f), new SolidBrush(Color.Black), new Point(10, 10));

            //System.Random r = new System.Random();

			if (m_files == null) {
				// 描画するファイルが無いので終了
				return;
			}

            int index, relative, ftop = trackBar2.Value - 2;

            relative = 0; index = ftop + relative;
            if (0 <= index && index < m_files.Length) {
                System.Drawing.Imaging.ImageAttributes ia = new System.Drawing.Imaging.ImageAttributes();
                ia.SetGamma(0.25f);
                Image img = Image.FromFile(m_files[index]);
                g.DrawImage(img, new Rectangle(new Point(0, (pictureBox1.Height - 480) / 2 + 100), new Size(640, 480)),
                    0, 0, 640, 480, GraphicsUnit.Pixel, ia);
            }

            relative = 4; index = ftop + relative;
            if (0 <= index && index < m_files.Length) {
                System.Drawing.Imaging.ImageAttributes ia = new System.Drawing.Imaging.ImageAttributes();
                ia.SetGamma(0.25f);
                Image img = Image.FromFile(m_files[index]);
                g.DrawImage(img, new Rectangle(new Point((pictureBox1.Width - 640), (pictureBox1.Height - 480) / 2 + 100), new Size(640, 480)),
                    0, 0, 640, 480, GraphicsUnit.Pixel, ia);
            }

            relative = 1; index = ftop + relative;
            if (0 <= index && index < m_files.Length) {
                System.Drawing.Imaging.ImageAttributes ia = new System.Drawing.Imaging.ImageAttributes();
                ia.SetGamma(0.5f);
                Image img = Image.FromFile(m_files[index]);
                g.DrawImage(img, new Rectangle(new Point(120, (pictureBox1.Height - 480) / 2 + 50 - 50), new Size(640, 480)),
                    0, 0, 640, 480, GraphicsUnit.Pixel, ia);
            }

            relative = 3; index = ftop + relative;
            if (0 <= index && index < m_files.Length) {
                System.Drawing.Imaging.ImageAttributes ia = new System.Drawing.Imaging.ImageAttributes();
                ia.SetGamma(0.5f);
                Image img = Image.FromFile(m_files[index]);
                g.DrawImage(img, new Rectangle(new Point((pictureBox1.Width - 750), (pictureBox1.Height - 480) / 2 + 50 - 50), new Size(640, 480)),
                    0, 0, 640, 480, GraphicsUnit.Pixel, ia);
            }

            relative = 2; index = ftop + relative;
            if (0 <= index && index < m_files.Length) {
                Image img = Image.FromFile(m_files[index]);
                g.DrawImage(img, new Rectangle(new Point((pictureBox1.Width - 640) / 2, (pictureBox1.Height - 480) / 2 - 100), new Size(640, 480)));

                Rectangle rc = new Rectangle(new Point((pictureBox1.Width - 640) / 2, (pictureBox1.Height - 480) / 2 - 100), new Size(640, 480));
                rc.Inflate(5, 5);
                g.DrawRectangle(new Pen(Color.Black, 5.0f), rc);

                Rectangle rc2 = rc;
                rc2.Y -= 30;
                rc2.Height = 25;

                g.DrawString(System.IO.Path.GetFileName(m_files[index]), new Font("Arial", 16.0f), new SolidBrush(Color.Black), rc2);
            }
        }

        private void Form1_Load(object sender, EventArgs e) {
            trackBar1.Value = System.DateTime.Now.Hour;
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e) {
            this.UseWaitCursor = true;

            label1.Text = trackBar1.Value.ToString();

            string dataPath = System.IO.Path.Combine(System.IO.Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath), "axis");

            DateTime dt = this.dateTimePicker1.Value;
			try {
				m_files = System.IO.Directory.GetFiles(dataPath, "image" + (dt.Year % 100).ToString().PadLeft(2, '0') + "-" + dt.Month.ToString().PadLeft(2, '0') + "-" + dt.Day.ToString().PadLeft(2, '0') + "_" + label1.Text.PadLeft(2, '0') + "*.jpg");
			} catch (System.IO.DirectoryNotFoundException) {
				MessageBox.Show(
					"\"" + dataPath + "\" を作成してから実行してください。",
					"エラー");
				this.UseWaitCursor = false;
				return;
			}

            label2.Text = "ファイル数: " + m_files.Length.ToString();

            if (m_files.Length == 0) {
                trackBar2.Minimum = 0;
                trackBar2.Maximum = 0;
                trackBar2.Value = 0;
                trackBar2.Enabled = false;
            } else {
                trackBar2.Minimum = 0;
                trackBar2.Maximum = m_files.Length - 1;
                trackBar2.Value = 0;
                trackBar2.Enabled = true;
            }

            pictureBox1.Invalidate();

            this.UseWaitCursor = false;

            label1.Text = dt.ToShortDateString() + " " + trackBar1.Value.ToString() + " 時の画像";
        }

        private void trackBar1_Scroll(object sender, EventArgs e) {

        }

        private void trackBar2_Scroll(object sender, EventArgs e) {

        }

        private void trackBar2_ValueChanged(object sender, EventArgs e) {
            this.pictureBox1.Invalidate();
        }

        private void dateTimePicker1_ValueChanged(object sender, EventArgs e) {
            this.UseWaitCursor = true;

            label1.Text = trackBar1.Value.ToString();

            string dataPath = System.IO.Path.Combine(System.IO.Path.GetDirectoryName(System.Windows.Forms.Application.ExecutablePath), "axis");

            DateTime dt = this.dateTimePicker1.Value;
            m_files = System.IO.Directory.GetFiles(dataPath, "image" + (dt.Year % 100).ToString().PadLeft(2, '0') + "-" + dt.Month.ToString().PadLeft(2, '0') + "-" + dt.Day.ToString().PadLeft(2, '0') + "_" + label1.Text.PadLeft(2, '0') + "*.jpg");

            label2.Text = "ファイル数: " + m_files.Length.ToString();

            trackBar1.Focus();

            if (m_files.Length == 0) {
                trackBar2.Minimum = 0;
                trackBar2.Maximum = 0;
                trackBar2.Value = 0;
                trackBar2.Enabled = false;
            } else {
                trackBar2.Minimum = -1;
                trackBar2.Maximum = m_files.Length;
                trackBar2.Value = 0;
                trackBar2.Enabled = true;
            }

            pictureBox1.Invalidate();

            this.UseWaitCursor = false;

            label1.Text = dt.ToShortDateString() + " " + trackBar1.Value.ToString() + " 時の画像";
        }
    }
}
