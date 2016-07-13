using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.IO;

namespace Kjm {
	class ForFiles {

		public delegate void ForFilesCallback(bool isdir, string fullName);

		public string Path { get; set; }
		public string Mask { get; set; }
		public bool SubDir { get; set; }
		public ForFilesCallback CallbackFunc { get; set; }

		public ForFiles() {
		}

		public ForFiles(string path, string mask, bool subdir) {
			Path = path;
			Mask = mask;
			SubDir = subdir;
			CallbackFunc = null;
		}

		public ForFiles(string path, string mask, bool subdir, ForFilesCallback callbackFunc) {
			Path = path;
			Mask = mask;
			SubDir = subdir;
			CallbackFunc = callbackFunc;
		}

		public void ForEach() {
			ForEach__(Path);
		}

		public void ForEach(ForFilesCallback callbackFunc) {
			CallbackFunc = callbackFunc;
			ForEach();
		}

		private void ForEach__(string path) {
			Regex reg = new Regex(Mask, RegexOptions.IgnoreCase);
			foreach (string name in Directory.GetFiles(path)) {
				if (reg.IsMatch(System.IO.Path.GetFileName(name))) {
					CallbackFunc(false, name);
				}
			}
			foreach (string name in Directory.GetDirectories(path)) {
				if (SubDir) {
					ForEach__(name);
				}

				if (reg.IsMatch(System.IO.Path.GetFileName(name))) {
					CallbackFunc(true, name);
				}
			}
		}
	}
}
