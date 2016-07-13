using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace axisShutter {
	public partial class Form1 : Form {
		private System.Threading.Thread t;
		private volatile bool m_abort = false;

		public Form1() {
			InitializeComponent();
		}

		private void button1_Click(object sender, EventArgs e) {
		}

		private void button1_Click_1(object sender, EventArgs e) {
			if (button1.Text == "シャッターを切る") {
				if (radioButton1.Checked) {
					string strResult = "";

					//textBox1.Focus();

					button1.Enabled = false;
					axisCamera cam = new axisCamera();
					cam.Shoot(textBox2.Text, out strResult);
					//textBox1.Text += "[" + System.DateTime.Now.ToLongTimeString() + "] " + strResult + "\x0d\x0a";
					//textBox1.SelectionStart = textBox1.TextLength;
					textBox1.HideSelection = false;
					textBox1.AppendText("[" + System.DateTime.Now.ToLongTimeString() + "] " + strResult + "\x0d\x0a");
					button1.Enabled = true;
				} else {
					//textBox1.Focus();

					button1.Text = "連写を止める";
					t = new System.Threading.Thread(new System.Threading.ThreadStart(worker));
					m_abort = false;
					t.Start();
				}
			} else {
				button1.Enabled = false;
				m_abort = true;
			}
		}

		delegate void SetTextBox1Delegate(string value);

		void SetTextBox1(string value) {
			if (InvokeRequired) {
				Invoke(new SetTextBox1Delegate(SetTextBox1), new object[]{value});
				return;
			} else {
				//textBox1.Text += value + "\x0d\x0a";
				//textBox1.SelectionStart = textBox1.TextLength;
				textBox1.HideSelection = false;
				textBox1.AppendText(value + "\x0d\x0a");
			}
		}

		delegate void SetButton1TextDelegate(string value);

		void SetButton1Text(string value) {
			if (InvokeRequired) {
				Invoke(new SetButton1TextDelegate(SetButton1Text), new object[] { value });
				return;
			} else {
				button1.Text = value;
			}
		}

		delegate void SetButton1EnableDelegate(bool value);

		void SetButton1Enable(bool value) {
			if (InvokeRequired) {
				Invoke(new SetButton1EnableDelegate(SetButton1Enable), new object[] { value });
				return;
			} else {
				button1.Enabled = value;
			}
		}

		void worker() {
			axisCamera cam = new axisCamera();
			string strResult = "";
			int prev = System.Environment.TickCount;

			int i = 0;
			while (true) {
				if (m_abort) {
					break;
				}

				int tbox4 = Convert.ToInt32(textBox4.Text);
				if ((tbox4 != 0) && ((System.Environment.TickCount - prev) < tbox4)) {
					System.Threading.Thread.Sleep(0);
					continue;
				}

				int tbox3 = Convert.ToInt32(textBox3.Text);
				if ((tbox3 != 0) && (i >= tbox3)) {
					break;
				}

				cam.Shoot(textBox2.Text, out strResult);
				SetTextBox1("[" + System.DateTime.Now.ToLongTimeString() + "] [" + (i + 1).ToString() + "] " + strResult);

				++i;
				prev = System.Environment.TickCount;
			}

			SetButton1Text("シャッターを切る");
			SetButton1Enable(true);
		}
	}
}
