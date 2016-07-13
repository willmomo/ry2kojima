using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace bfkjmw {
	public partial class Form1 : Form {
		public Form1() {
			InitializeComponent();
		}

		private void Form1_Load(object sender, EventArgs e) {
			IDataObject data = Clipboard.GetDataObject();
			if (data.GetDataPresent(DataFormats.Text)) {
				txtData.Text = (string)data.GetData(DataFormats.Text);
			}

			using (Kjm.XmlPreference opt = new Kjm.XmlPreference()) {
				chkCopy.Checked = (opt.GetString("option", "QuickCopy", "off") == "on" ? true : false);
				chkQuit.Checked = (opt.GetString("option", "QuickQuit", "off") == "on" ? true : false);
				chkMemory.Checked = (opt.GetString("option", "MemoryPass", "off") == "on" ? true : false);

				BlowFishCS.BlowFish bf2 = new BlowFishCS.BlowFish(System.Text.Encoding.ASCII.GetBytes("GAT-X105Strike"));
				string key = opt.GetString("option", "Key", "");
				if (key.Length > 0) {
					txtKey.Text = bf2.Decrypt_ECB(key);
				}
			}
		}

		private void CopyData() {
			Clipboard.SetDataObject(txtData.Text, true);
		}

		private void btnEncrypt_Click(object sender, EventArgs e) {
			BlowFishCS.BlowFish bf = new BlowFishCS.BlowFish(System.Text.Encoding.ASCII.GetBytes(txtKey.Text));
			txtData.Text = bf.Encrypt_ECB(txtData.Text);
			txtData.Focus();
			txtData.SelectAll();
			if (chkCopy.Checked) {
				CopyData();
			}

			if (chkMemory.Checked) {
				using (Kjm.XmlPreference opt = new Kjm.XmlPreference()) {
					BlowFishCS.BlowFish bf2 = new BlowFishCS.BlowFish(System.Text.Encoding.ASCII.GetBytes("GAT-X105Strike"));
					opt.SetString("option", "Key", bf2.Encrypt_ECB(txtKey.Text));
				}
			}

			if (chkQuit.Checked) {
				if (chkMemory.Checked) {
					this.Hide();
				} else {
					this.Close();
				}
			}
		}

		private void btnDecrypt_Click(object sender, EventArgs e) {
			BlowFishCS.BlowFish bf = new BlowFishCS.BlowFish(System.Text.Encoding.ASCII.GetBytes(txtKey.Text));
			txtData.Text = bf.Decrypt_ECB(txtData.Text);
			txtData.Focus();
			txtData.SelectAll();
			if (chkCopy.Checked) {
				CopyData();
			}

			if (chkMemory.Checked) {
				using (Kjm.XmlPreference opt = new Kjm.XmlPreference()) {
					BlowFishCS.BlowFish bf2 = new BlowFishCS.BlowFish(System.Text.Encoding.ASCII.GetBytes("GAT-X105Strike"));
					opt.SetString("option", "Key", bf2.Encrypt_ECB(txtKey.Text));
				}
			}
			
			if (chkQuit.Checked) {
				if (chkMemory.Checked) {
					this.Hide();
				} else {
					this.Close();
				}
			}
		}

		private void txtKey_TextChanged(object sender, EventArgs e) {
			TextBox obj = (TextBox)sender;
			tableLayoutPanel1.Enabled = (obj.TextLength == 0 ? false : true);
		}

		private void chkCopy_CheckedChanged(object sender, EventArgs e) {
			CheckBox obj = (CheckBox)sender;
			using (Kjm.XmlPreference opt = new Kjm.XmlPreference()) {
				opt.SetString("option", "QuickCopy", (obj.Checked ? "on" : "off"));
			}
		}

		private void chkQuit_CheckedChanged(object sender, EventArgs e) {
			CheckBox obj = (CheckBox)sender;
			using (Kjm.XmlPreference opt = new Kjm.XmlPreference()) {
				opt.SetString("option", "QuickQuit", (obj.Checked ? "on" : "off"));
			}
		}

		private void chkMemory_CheckedChanged(object sender, EventArgs e) {
			CheckBox obj = (CheckBox)sender;
			using (Kjm.XmlPreference opt = new Kjm.XmlPreference()) {
				opt.SetString("option", "MemoryPass", (obj.Checked ? "on" : "off"));
			}
		}

		private void Form1_Shown(object sender, EventArgs e) {
			//MessageBox.Show("Shown");
		}

		private void Form1_Activated(object sender, EventArgs e) {
			//MessageBox.Show("Form1_Activated");
			IDataObject data = Clipboard.GetDataObject();
			if (data.GetDataPresent(DataFormats.Text)) {
				txtData.Text = (string)data.GetData(DataFormats.Text);
			}
		}
	}
}
