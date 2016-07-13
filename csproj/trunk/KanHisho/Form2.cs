using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace KanHisho {
	public partial class Form2 : Form {
		public Form2() {
			InitializeComponent();
		}

		private void Form2_Load(object sender, EventArgs e) {
			using (Graphics g = this.CreateGraphics()) {
				System.Diagnostics.Debug.WriteLine("DpiX = {0}", g.DpiX);
			}
		}
	}
}
