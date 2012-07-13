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
                Console.WriteLine("No File(s) Specified.");
            }
            else
            {
				//bool shouldLoad = true;
				State State = null;
				string ScriptText = System.IO.File.ReadAllText(args[0]);
				State = new State(ScriptText);
				CpStdLib.InstallConsoleIO(State.StatePtr);
				CpStdLib.InstallFileIO(State.StatePtr);
				CpStdLib.InstallMath(State.StatePtr);
				CpStdLib.InstallUtilities(State.StatePtr);
				State.RunFromMethod("Main");
				State.Delete();
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
