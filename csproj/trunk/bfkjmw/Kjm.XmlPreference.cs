//
// version 0.2016.6.28
//
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Reflection;
using System.Threading;

namespace Kjm {
	public class XmlPreference : IDisposable {
		// Flag: Has Dispose already been called?
		bool m_disposed = false;

		// 真のdisposer
		// デストラクタから呼ばれたときは、disposing = true
		// Disposeから呼ばれたときは、disposing = false
		protected virtual void Dispose(bool disposing) {
			if (!m_disposed) {

				// unmanaged や IDisposable なオブジェクトは破棄は、ここで処理をする
				if (m_mutex != null) {
					m_mutex.Close();
					m_mutex = null;
				}

				if (disposing) {
					// IDisposable ではない managed なオブジェクトの破棄は、ここで処理する
				}
				m_disposed = true;
			}
		}

		// Mutexオブジェクト
		// XmlFileNameに書込みアクセスしたとき、同名のMutexを作成する
		private Mutex m_mutex;

		// XMLのパス
		private string m_xmlPathName;
		public string XmlPathName {
			get {
				return m_xmlPathName;
			}
			set {
				m_xmlPathName = value;
			}
		}

		// XMLのファイル名
		private string m_xmlFileName = Path.GetFileName(Path.ChangeExtension(Assembly.GetEntryAssembly().Location, ".xml"));
		public string XmlFileName {
			get {
				return m_xmlFileName;
			}
			set {
				m_xmlFileName = value;
				if (m_mutex != null) {
					m_mutex.Close();
				}
				m_mutex = new Mutex(false, m_xmlFileName);
			}
		}

		// XMLのフルパス
		public string XmlFullName {
			get {
				return Path.Combine(m_xmlPathName, m_xmlFileName);
			}
			set {
				XmlPathName = Path.GetDirectoryName(value);
				XmlFileName = Path.GetFileName(value);
			}
		}

		// コンストラクタ
		public XmlPreference() {
			XmlFullName = Path.ChangeExtension(Assembly.GetEntryAssembly().Location, ".xml");
		}

		// デストラクタ
		~XmlPreference() {
			Dispose(false);
		}

		// disposer
		public void Dispose() {
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		public bool CreateEmptyFile(bool force = false) {
			bool created = false;
			try {
				m_mutex.WaitOne();
				if (force || !File.Exists(XmlFullName)) {
					XmlDocument doc = new XmlDocument();
					try {
						//doc.Load(XmlFullName);
						doc.AppendChild(doc.CreateXmlDeclaration("1.0", "utf-8", "yes"));
						doc.AppendChild(doc.CreateElement("root"));
					} catch {
					}
					doc.Save(XmlFullName);
					created = true;
				}
			} finally {
				m_mutex.ReleaseMutex();
			}
			return created;
		}

		public string GetString(string section, string key, string defValue) {
			string result = defValue;
			try {
				m_mutex.WaitOne();
				XmlDocument doc = new XmlDocument();
				doc.Load(XmlFullName);
				XmlNode node = doc.DocumentElement.SelectSingleNode(string.Format("{0}/{1}", section, key));
				if (node != null) {
					result = node.InnerText;
				}
			} catch {
				// 特にすることありません。
			} finally {
				m_mutex.ReleaseMutex();
			}
			return result;
		}

		public void SetString(string section, string key, string newValue) {
			try {
				m_mutex.WaitOne();
				XmlDocument doc = new XmlDocument();
				try {
					doc.Load(XmlFullName);
				} catch {
					doc.AppendChild(doc.CreateXmlDeclaration("1.0", "utf-8", "yes"));
					doc.AppendChild(doc.CreateElement("root"));
				}
				XmlNode node = doc.DocumentElement;
				foreach (var s in new[] { section, key }) {
					XmlNode nextNode = node.SelectSingleNode(s);
					node = (nextNode == null) ? node.AppendChild(doc.CreateElement(s)) : nextNode;
				}
				node.InnerText = newValue;
				doc.Save(XmlFullName);
			} finally {
				m_mutex.ReleaseMutex();
			}
		}
	}
}
