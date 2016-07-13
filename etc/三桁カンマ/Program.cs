using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace test2 {
    class Program {
        static void Main(string[] args) {
            Console.WriteLine(System.Text.RegularExpressions.Regex.Replace("1234", @"(\d)(?=(\d{3})+$)", "$1,"));
            Console.WriteLine(System.Text.RegularExpressions.Regex.Replace("1234567890", @"(\d)(?=(\d{3})+$)", "$1,"));
        }
    }
}
