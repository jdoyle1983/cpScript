/*

	cpObjCmp
    Copyright (C) 2013 Jason Doyle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact Information:

    Original Author: Jason Doyle (jdoyle1983@gmail.com)
*/

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
                /*Console.WriteLine("cpObjCmp [input] [input] -c -o [output]");
				Console.WriteLine("If the -c option is passed, a compiled asm script will be output");*/
            }
            else
            {
				//bool shouldCompile = false;
                List<string> inputFiles = new List<string>();
                string outputFile = "";
                for (int i = 0; i < args.Length; i++)
                {
                    if (args[i] == "-o")
                    {
                        i++;
                        outputFile = args[i];
                    }
					/*else if(args[i] == "-c")
						shouldCompile = true;*/
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
				//if(!shouldCompile)
                	System.IO.File.WriteAllText(outputFile, obj.AsmScript);
				/*else
					System.IO.File.WriteAllBytes(outputFile, obj.Compile());*/
            }
        }
    }
}
