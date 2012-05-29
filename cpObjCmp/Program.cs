using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace cpObjCmp
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 3)
            {
                Console.WriteLine("No Input File(s) / Output File Specified");
                Console.WriteLine("cpObjCmp [input] [input] -o [output]");
            }
            else
            {
                List<string> inputFiles = new List<string>();
                string outputFile = "";
                for (int i = 0; i < args.Length; i++)
                {
                    if (args[i] == "-o")
                    {
                        i++;
                        outputFile = args[i];
                    }
                    else
                        inputFiles.Add(args[i]);
                }

                if (outputFile == "")
                    Console.WriteLine("No Output File Specified.");
                string fullScript = "";
                foreach (string s in inputFiles)
                    fullScript += System.IO.File.ReadAllText(s) + "\n";
                libCpScript.Net.ObjectBasic.ObjectBasicScript obj = new libCpScript.Net.ObjectBasic.ObjectBasicScript();
                obj.LoadScript(fullScript);
                System.IO.File.WriteAllText(outputFile, obj.AsmScript);
            }
        }
    }
}
