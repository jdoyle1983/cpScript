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
            if (args.Length < 1)
            {
                Console.WriteLine("No Input File(s) / Output File Specified");
				Console.WriteLine("cpObjCmp [input] [input] -o [output]");
				Console.WriteLine("If no -o is specified, it will write the");
				Console.WriteLine("resulting script to standard output.");
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

                string fullScript = "";
                foreach (string s in inputFiles)
                    fullScript += System.IO.File.ReadAllText(s) + "\n";
                libCpScript.Net.ObjectBasic.ObjectBasicScript obj = new libCpScript.Net.ObjectBasic.ObjectBasicScript();
                obj.LoadScript(fullScript);
				if(outputFile != "")
                	System.IO.File.WriteAllText(outputFile, obj.AsmScript);
				else
					Console.Write(obj.AsmScript);
            }
        }
    }
}
