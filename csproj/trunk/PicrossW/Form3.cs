using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace pxw {
	public partial class Form3 : Form {
		public int m_height;
		public int m_width;

		public Form3() {
			InitializeComponent();
		}

		private void acceptButton_Click(object sender, EventArgs e) {
			m_height = Convert.ToInt32(textBox1.Text);
			m_width = Convert.ToInt32(textBox2.Text);
			DialogResult = System.Windows.Forms.DialogResult.OK;
			this.Close();
		}
	}
}
