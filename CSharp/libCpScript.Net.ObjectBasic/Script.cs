/*

	libCpScript.Net.ObjectBasic
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
using System.Runtime.InteropServices;
using libCpScript.Net.Asm;
using System.Collections;
using System.Collections.Generic;

namespace libCpScript.Net.ObjectBasic
{
	internal class tmpRegFunc
	{
		public string Name { get; set; }
		public UserFunction Function { get; set; }
		public bool StubFunction { get; set; }

		public tmpRegFunc(string name, UserFunction function, bool stubFunction)
		{
			Name = name;
			Function = function;
			StubFunction = stubFunction;
		}
	}

	public class Script
	{
		internal IntPtr _Script;
		internal List<tmpRegFunc> _Functions = new List<tmpRegFunc>();

		public IntPtr ScriptPtr { get { return _Script; } }



		public string AsmScript
		{
			get
			{
				IntPtr AnsiAsm = NativeScript.ObjScript_GetAsm(_Script);
				string r = Marshal.PtrToStringAnsi(AnsiAsm);
				return r;
			}
		}

		public Script()
		{
			_Script = NativeScript.ObjScript_New();
		}

		public void LoadScript(string ScriptText)
		{
			IntPtr AnsiScriptText = Marshal.StringToHGlobalAnsi(ScriptText);
			NativeScript.ObjScript_Load(_Script, AnsiScriptText);
			Marshal.FreeHGlobal(AnsiScriptText);
		}

		public void RegisterFunction(string Name, UserFunction Function, bool StubFunction)
		{
			_Functions.Add(new tmpRegFunc(Name, Function, StubFunction));

			IntPtr AnsiName = Marshal.StringToHGlobalAnsi(Name);
			short MStubFunction = (short)(StubFunction ? 1 : 0);
			NativeScript.RegisterFunction(_Script, AnsiName);
			Marshal.FreeHGlobal(AnsiName);
		}

		public void DeleteScript()
		{
			NativeScript.ObjScript_Delete(_Script);
		}

		public State CreateAsmState()
		{
			State s = new State(AsmScript);
			foreach(tmpRegFunc f in _Functions)
				s.RegisterFunction(f.Name, f.Function, f.StubFunction);
			return s;
		}
	}
}

