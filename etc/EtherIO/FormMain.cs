using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace EtherIO {
	public partial class FormMain : Form {
		private ComboBox[] m_cbMode = new ComboBox[4];
		private ComboBox[] m_cbStatus = new ComboBox[4];
		private Button[] m_btnWrite = new Button[4];

		public FormMain() {
			InitializeComponent();

			m_cbMode[0] = cbMode1;
			m_cbMode[1] = cbMode2;
			m_cbMode[2] = cbMode3;
			m_cbMode[3] = cbMode4;

			m_cbStatus[0] = cbStatus1;
			m_cbStatus[1] = cbStatus2;
			m_cbStatus[2] = cbStatus3;
			m_cbStatus[3] = cbStatus4;

			m_btnWrite[0] = btnWrite1;
			m_btnWrite[1] = btnWrite2;
			m_btnWrite[2] = btnWrite3;
			m_btnWrite[3] = btnWrite4;
		}

		private void btnReadAll_Click(object sender, EventArgs e) {
			MiiNePortE3 mnp = new MiiNePortE3("192.168.127.254");
			for (byte channel = 1; channel <= 4; channel++) {
				byte mode, status;
				mnp.ReadSingleDIO(channel, out mode, out status);

				m_cbMode[channel - 1].SelectedIndex = mode;
				m_cbStatus[channel - 1].SelectedIndex = status;
			}
		}

		private void btnRead_Click(object sender, EventArgs e) {
			Button obj = (Button)sender;

			MiiNePortE3 mnp = new MiiNePortE3("192.168.127.254");
			byte channel = Convert.ToByte(obj.Tag), mode, status;
			mnp.ReadSingleDIO(channel, out mode, out status);

			m_cbMode[channel - 1].SelectedIndex = mode;
			m_cbStatus[channel - 1].SelectedIndex = status;
		}

		private void btnWrite_Click(object sender, EventArgs e) {
			Button obj = (Button)sender;

			MiiNePortE3 mnp = new MiiNePortE3("192.168.127.254");
			byte channel = Convert.ToByte(obj.Tag);
			byte mode = Convert.ToByte(m_cbMode[channel - 1].SelectedIndex);
			byte status = Convert.ToByte(m_cbStatus[channel - 1].SelectedIndex);

			mnp.WriteSingleDIO(channel, mode, status);
		}
	}
}
