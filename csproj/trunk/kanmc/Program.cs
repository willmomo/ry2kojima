using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Net;
using System.Diagnostics;
using Codeplex.Data;

namespace kanmc {
	class Program {
		static void Main(string[] args) {
			Kjm.Log.Write("-----[ 艦娘コレクション start ]-----");

			KanMusuCollection kmc = new KanMusuCollection();
			if (kmc.LoadApiStart2()) {
				kmc.DiffApiStart2();
				kmc.PrintNewKanMusuList();
				kmc.GetSWF();
				kmc.SaveApiStart2();
			}

			Kjm.Log.Write("-----[ 艦娘コレクション end ]-----");
		}
	}
}
