using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CalnTime {
    class AlermSet {
        static public List<string> ReadAlermList(string fname) {
            List<string> result = new List<string>();

            try {
                System.IO.FileStream fs = System.IO.File.OpenRead(fname);
            } catch {
            }

            return result;
        }

        static public void UpsertAlerm(string fname, string alermName, string alermTime) {
        }
    }
}
