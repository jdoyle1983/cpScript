/*
This file is part of libCpScript.

    libCpScript is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libCpScript is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libCpScript.  If not, see <http://www.gnu.org/licenses/>.
	
	
	libCpScript
	Original Author: Jason Doyle (jdoyle1983@gmail.com)
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using libCpScript.Net.Asm;
using libCpScript.Net.StdLib;

namespace cpInt
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length < 1)
            {
                Console.WriteLine("No File(s) Specified. (use -o file.cmp to just compile asm file)");
            }
            else
            {
				bool shouldRun = true;
				string outFile = "";
				if(args.Length > 1)
				{
					for(int i = 1; i < args.Length; i++)
					{
						if(args[i] == "-o")
						{ 
							i++;
							shouldRun = false;
							outFile = args[i];
						}
					}
				}
				byte[] inBytes = System.IO.File.ReadAllBytes(args[0]);
				if( inBytes[0] != 'C' || inBytes[1] != 'P' || inBytes[2] != 'A' || inBytes[3] != 'S' || inBytes[4] != 'M' || inBytes[5] != 'C' || inBytes[6] != 'M' || inBytes[7] != 'P')
				{
					State s = new State(System.IO.File.ReadAllText(args[0]));
					inBytes = s.Compile();
					s.Delete();
				}

				State state = new State(inBytes);
				if(shouldRun)
				{
					CpStdLib.InstallConsoleIO(state.StatePtr);
					CpStdLib.InstallFileIO(state.StatePtr);
					CpStdLib.InstallMath(state.StatePtr);
					CpStdLib.InstallUtilities(state.StatePtr);
					state.RunFromMethod("Main");
				}
				else
					System.IO.File.WriteAllBytes(outFile, state.Compile());
				state.Delete();
				/*
				if(args.Length == 1) //Compiled scripts will be compiled into a single file
				{
					try
					{
						State = new libCpScript.Net.Asm.State(System.IO.File.ReadAllBytes(args[0]));
					}
					catch{}
					if(State != null && State.ScriptLoaded) //If we loaded binary, don't attempt to load it a script
						shouldLoad = false;
				}
				
				if(shouldLoad) //If this flag is set, it means it wasn't binary and / or there are multiple input files
				{
	                string fullScript = "";
	                foreach (string s in args)
	                    fullScript += System.IO.File.ReadAllText(s) + "\n";
					try
					{
	                	State = new libCpScript.Net.Asm.State(fullScript);
					} catch{}
				}
				
				if(State != null && State.ScriptLoaded)
                	State.RunFromMethod("Main");*/
            }
        }
    }
}
