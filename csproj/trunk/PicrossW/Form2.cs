using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace pxw {
	public partial class Form2 : Form {
		public string m_text = "";

		public Form2() {
			InitializeComponent();
		}

		private void acceptButton_Click(object sender, EventArgs e) {
			m_text = textBox1.Text;
			this.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.Close();
		}
	}
}
