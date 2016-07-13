using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace md5sample {
	class Program {

		static void Main(string[] args) {


			var targetPath = @"\\TS00\share\@開発\document\LDL(液晶台ランプ)\95_出荷モジュール\02_FT出荷モジュール\20140708VerUp_v1.000p02_rev2\GnaviV1000p02\GnaviSV\scripts\gnavi.zip";

			//targetPath = @"C:\Users\kojima\Downloads\xampp-win32-1.8.0-VC9.zip";

			int n = Kjm.MD5Helper.CheckMD5(targetPath);

			string md5str_1 = Kjm.MD5Helper.CalcMD5(targetPath);

			string md5sum_2 = Kjm.MD5Helper.FindMD5(targetPath);
		}
	}
}
