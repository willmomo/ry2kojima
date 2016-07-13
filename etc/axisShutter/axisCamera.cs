using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net;

namespace axisShutter {
	class axisCamera {
		public bool Shoot(string txtHost, out string txtResult) {
			txtResult = "";

			System.Net.HttpWebRequest webreq = 
				(System.Net.HttpWebRequest)System.Net.WebRequest.Create(@"http://" + txtHost + @"/axis-cgi/io/virtualinput.cgi?action=1:/1000\");

			//認証の設定
			webreq.Credentials = new System.Net.NetworkCredential("root", "12345678");

			try {
				System.Net.HttpWebResponse webres = (System.Net.HttpWebResponse)webreq.GetResponse();

				Stream stm = webres.GetResponseStream();
				if (stm != null) {
					StreamReader reader = new StreamReader(stm, System.Text.Encoding.GetEncoding("Shift_JIS"));
					txtResult = reader.ReadToEnd();
					stm.Close();
				}
				webres.Close();
			} catch (WebException ex) {
				txtResult = ex.Message;
			}

			return true;
		}
	}
}
