using System;
using System.Collections;
using System.Collections.Generic;
using libCpScript.Net.ObjectBasic;
using libCpScript.Net.Asm;
using libCpScript.Net.StdLib;
using System.Text;

namespace cpConsole
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Console.WriteLine ("Enter Script Below, Must Include Function Main().");
			Console.WriteLine ("Type .complete on it's own line to execute script.");
			Console.WriteLine ("Type .exit on it's own line to quit.");

			StringBuilder sb = new StringBuilder();
			sb.AppendLine("#include Lib/ConsoleIO.obh");
			sb.AppendLine("#include Lib/FileIO.obh");
			sb.AppendLine("#include Lib/Math.obh");
			sb.AppendLine("#include Lib/Utilities.obh");
			bool ExitFlag = false;
			while (!ExitFlag) {
				string thisLine = Console.ReadLine();
				if(thisLine.Trim().ToLower() == ".complete")
				{
					Script obs = new Script();
					obs.LoadScript(sb.ToString());
					State thisState = obs.CreateAsmState();

					CpStdLib.InstallConsoleIO(thisState.StatePtr);
					CpStdLib.InstallFileIO(thisState.StatePtr);
					CpStdLib.InstallMath(thisState.StatePtr);
					CpStdLib.InstallUtilities(thisState.StatePtr);

					thisState.RunFromMethod("Main");

					sb = new StringBuilder();
					sb.AppendLine("#include Lib/ConsoleIO.obh");
					sb.AppendLine("#include Lib/FileIO.obh");
					sb.AppendLine("#include Lib/Math.obh");
					sb.AppendLine("#include Lib/Utilities.obh");
				}
				else if(thisLine.Trim().ToLower() == ".exit")
				{
					ExitFlag = true;
				}
				else
				{
					sb.AppendLine(thisLine);
				}
			}
		}
	}
}
