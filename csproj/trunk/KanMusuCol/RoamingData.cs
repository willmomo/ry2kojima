using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KanMusuCol {
	/*
	 * このアプリケーションのための設定にアクセスするためのクラス
	 */
	class RoamingData {
		private string m_basePath;
		private Kjm.IniFile m_iniFile = new Kjm.IniFile();

		public RoamingData() {
			m_basePath = System.Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
			m_basePath = System.IO.Path.Combine(new [] { m_basePath, "rkojima", "KanMusuCol" });

			m_iniFile.FileName = System.IO.Path.Combine(m_basePath, "KanMusuCol.ini");
		}

		public string DataPath {
			get {
				return m_iniFile.GetString("option", "DataPath", System.Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments));
			}
			set {
				m_iniFile.SetString("option", "DataPath", value);
			}
		}

		public string HugFlashPath {
			get {
				return m_iniFile.GetString("option", "HugFlashPath", "");
			}
			set {
				m_iniFile.SetString("option", "HugFlashPath", value);
			}
		}
	}
}
