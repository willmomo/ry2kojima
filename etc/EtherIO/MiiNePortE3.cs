using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EtherIO {
	class MiiNePortE3 {

		private string m_hostname = "";
		private int m_port = 5001;

		public MiiNePortE3() {
			m_hostname = "";
		}

		public MiiNePortE3(string hostname) {
			m_hostname = hostname;
		}

		public byte ReadSingleDIO(byte channel, out byte mode, out byte status) {
			byte result = 0;
			System.Net.Sockets.TcpClient tcpCl = new System.Net.Sockets.TcpClient();

			tcpCl.Connect(this.m_hostname, this.m_port);
			System.Net.Sockets.NetworkStream stream = tcpCl.GetStream();

			byte[] data = new byte[5];
			data[0] = 0x01;
			data[1] = 0x02;
			data[2] = 0;
			data[3] = 0x01;
			data[4] = (byte)(channel - 1);

			stream.Write(data, 0, data.GetLength(0));

			//System.Threading.Thread.Sleep(1000);

			while (tcpCl.Available == 0) {
				System.Threading.Thread.Sleep(0);
			}

			if (tcpCl.Available > 0) {
				byte[] ans = new byte[tcpCl.Available];
				stream.Read(ans, 0, ans.GetLength(0));

				result = ans[2];
				mode = ans[5];
				status = ans[6];
			} else {
				mode = 0;
				status = 0;
			}

			tcpCl.Close();
			return result;
		}

		public void WriteSingleDIO(byte channel, byte mode, byte status) {
			System.Net.Sockets.TcpClient tcpCl = new System.Net.Sockets.TcpClient();

			tcpCl.Connect(this.m_hostname, this.m_port);
			System.Net.Sockets.NetworkStream stream = tcpCl.GetStream();

			byte[] data = new byte[7];
			data[0] = 0x02;
			data[1] = 0x02;
			data[2] = 0;
			data[3] = 0x03;
			data[4] = (byte)(channel - 1);
			data[5] = mode;
			data[6] = status;

			stream.Write(data, 0, data.GetLength(0));

			//System.Threading.Thread.Sleep(1000);

			while (tcpCl.Available == 0) {
				System.Threading.Thread.Sleep(0);
			}

			if (tcpCl.Available > 0) {
				byte[] ans = new byte[tcpCl.Available];
				stream.Read(ans, 0, ans.GetLength(0));
			}

			tcpCl.Close();
		}
	}
}
