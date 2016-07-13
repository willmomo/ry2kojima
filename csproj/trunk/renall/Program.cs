using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Text.RegularExpressions;

namespace renall {
	class Program {

		static void Main(string[] args) {
			//RenameAll(Environment.CurrentDirectory, args[0], args[1]);

			string path = @"C:\Users\kojima\SkyDrive\tools\ElectronicObserver\KCAPI\resources\swf\ships";
			string mask = @"fsgwccpesgfh";
			string dest = @"初霜 改二";
			Regex reg = new Regex(mask, RegexOptions.IgnoreCase);

			Kjm.ForFiles ff = new Kjm.ForFiles(path, mask, true);
			ff.ForEach((isdir, fullName) => {
				string newName = Path.Combine(Path.GetDirectoryName(fullName), reg.Replace(Path.GetFileName(fullName), dest));
				if (isdir) {
					// Directory.Move(fullName, newName);
				} else {
					// File.Move(fullName, newName);
				}
				Console.WriteLine(fullName);
				Console.WriteLine(newName);
			});
		}
	}
}
