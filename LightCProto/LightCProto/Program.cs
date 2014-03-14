using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LightCProto
{
    class Program
    {
        static void Main(string[] args)
        {
            string OutputFile = "Out.asm";
            List<string> SourceFiles = new List<string>();

            for(int i = 0; i < args.Length; i++)
            {
                if(args[i] == "-o")
                {
                    i++;
                    OutputFile = args[i];
                }
                else
                    SourceFiles.Add(args[i]);
            }
            

        }
    }
}
