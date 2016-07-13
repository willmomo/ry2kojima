using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Threading;
using System.Diagnostics;
using System.IO;

namespace GlobalHook {
	public partial class Form1 : Form {
		private static Kjm.MouseHook s_mouseHook;

		public Form1() {
			InitializeComponent();

			s_mouseHook = new Kjm.MouseHook();
			s_mouseHook.MouseProc += new Kjm.MouseHook.MouseProcEventHandler(OnMouseProc);
		}

		private void button1_Click(object sender, EventArgs e) {
			if (s_mouseHook.IsHooked) {
				s_mouseHook.SetHook();
				this.button1.Text = "Unhook Windows Hook";
			} else {
				s_mouseHook.RemoveHook();
				this.button1.Text = "Set Windwos Hook";
			}
		}

		public void OnMouseProc(object sender, Kjm.MouseProcEventArgs e) {
			String strCaption = "x = " + e.X.ToString("d") + " : y = " + e.Y.ToString("d");
			this.Text = strCaption;
		}

		private void Form1_FormClosed(object sender, FormClosedEventArgs e) {
			s_mouseHook.RemoveHook();
		}
	}
}
