using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace blowfishcs {
	class Program {
		static int Main(string[] args) {
			if (args.Length != 2) {
				Console.WriteLine("usage: bfkjmcs.exe [key] [data]");
				return 0;
			}

			BlowFishCS.BlowFish bf = new BlowFishCS.BlowFish(System.Text.Encoding.ASCII.GetBytes(args[0]));

			if (args[1].Substring(0, 1) != "?") {
				Console.WriteLine("?{0}", bf.Encrypt_ECB(args[1]));
			} else {
				Console.WriteLine(bf.Decrypt_ECB(args[1].Substring(1)));
			}

			return 0;
		}
	}
}

