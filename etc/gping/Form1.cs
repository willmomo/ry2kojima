using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.NetworkInformation;
using MySql.Data.MySqlClient;

namespace gping {
	public partial class Form1 : Form {
		Ping m_ping = new Ping();
		bool m_pinging = false;
		int m_pingingRow = -1;

		public Form1() {
			InitializeComponent();

			m_ping.PingCompleted += new PingCompletedEventHandler(m_ping_PingCompleted);
		}

		void m_ping_PingCompleted(object sender, PingCompletedEventArgs e) {
			daiList.Rows[m_pingingRow].Cells[3].Value = "";

			if (e.Cancelled) {
				daiList.Rows[m_pingingRow].Cells[2].Value = "キャンセル";
			} else if (e.Error != null) {
				daiList.Rows[m_pingingRow].Cells[2].Value = "エラー:" + e.Error.Message;
			} else {
				if (e.Reply.Status == IPStatus.Success) {
					daiList.Rows[m_pingingRow].Cells[2].Value = e.Reply.RoundtripTime.ToString();
				} else {
					daiList.Rows[m_pingingRow].Cells[2].Value = "失敗";
				}
			}
			m_pinging = false;
		}

		private void button1_Click(object sender, EventArgs e) {
			button1.Enabled = false;

			//string connstr = "host=219.117.215.37;userid=root;password=Sanko0061;database=hcdb";
			string connstr = "host=172.22.101.254;userid=root;password=Sanko0061;database=hcdb";
			MySqlConnection conn = new MySqlConnection(connstr);
			conn.Open();

			MySqlCommand cmd = conn.CreateCommand();
			//cmd.CommandText = "select now()";
			//cmd.CommandText = "show tables";
			cmd.CommandText = "select HyoujiDaiNo from conf_dailamp_setsuzoku where ConfigNo=1";
			MySqlDataReader dr = cmd.ExecuteReader();

			while (dr.Read()) {
				int hdb = dr.GetInt32("HyoujiDaiNo");
				int ip3 = hdb / 100;
				int ip4 = hdb % 100;

				string ip;
				if (ip4 != 0) {
					ip = String.Format("172.22.{0}.{1}", ip3, ip4);
				} else {
					ip = String.Format("172.22.{0}.100", ip3);
				}
				//string ip = String.Format("192.168.120.{1}", (hdb / 100), (hdb % 100) + 100);
				daiList.Rows.Add(new[] { hdb.ToString(), ip });
			}

			conn.Close();

			button1.Enabled = true;
			MessageBox.Show(this, "IP読込み完了");
		}

		private void button2_Click(object sender, EventArgs e) {
			if (daiList.Rows.Count == 0) {
				MessageBox.Show("接続設定がありません。");
				return;
			}

			if (timer1.Enabled == false) {
				button1.Enabled = true;
				button2.Text = "PING終了";
				timer1.Start();
			} else {
				m_ping.SendAsyncCancel();

				button1.Enabled = false;
				button2.Text = "PING開始";
				timer1.Stop();
			}
		}

		private void timer1_Tick(object sender, EventArgs e) {
			if (m_pinging == false) {
				m_pinging = true;

				++m_pingingRow;
				if (!(m_pingingRow < daiList.Rows.Count)) {
					m_pingingRow = 0;
				}

				daiList.Rows[m_pingingRow].Cells[3].Value = "pinging";
				string ip = daiList.Rows[m_pingingRow].Cells[1].Value.ToString();
				m_ping.SendAsync(ip, 3000, null);
			}
		}

		private void daiList_CellContentClick(object sender, DataGridViewCellEventArgs e) {

		}
	}
}
