using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace cpInt
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine("No File(s) Specified.");
            }
            else
            {
                string fullScript = "";
                foreach (string s in args)
                    fullScript += System.IO.File.ReadAllText(s) + "\n";
                libCpScript.Net.Asm.State State = new libCpScript.Net.Asm.State(fullScript);
                State.RunFromMethod("Main");
            }
        }
    }
}
