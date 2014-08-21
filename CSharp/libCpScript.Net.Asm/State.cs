/*

	libCpScript.Net.Asm
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

namespace libCpScript.Net.Asm
{
	public class State
	{

		IntPtr _State;
		
		public IntPtr StatePtr { get { return _State; } }

		public State(IntPtr StatePtr)
		{
			_State = StatePtr;
		}

		public State(string ScriptText)
		{
			IntPtr AnsiScriptText = Marshal.StringToHGlobalAnsi(ScriptText);
			_State = NativeState.State_New(AnsiScriptText);
			Marshal.FreeHGlobal(AnsiScriptText);
		}

		public State(byte[] CompiledScript)
		{
			IntPtr pnt = Marshal.AllocHGlobal(CompiledScript.Length);
			Marshal.Copy(CompiledScript, 0, pnt, CompiledScript.Length);
			_State = NativeState.State_NewFromCompiled(pnt, CompiledScript.Length);
			Marshal.FreeHGlobal(pnt);
		}

		public byte[] Compile()
		{
			IntPtr LengthPtr = NativeState.InteropAllocLongPtr();
			IntPtr Compiled = NativeState.State_Compile(_State, LengthPtr);
			int Length = NativeState.InteropLongPtrToLong(LengthPtr);
			byte[] rVal = new byte[Length];
			Marshal.Copy(Compiled, rVal, 0, Length);
			NativeState.InteropFreePtr(LengthPtr);
			NativeState.InteropFreePtr(Compiled);
			return rVal;
		}

		public void Delete ()
		{
			NativeState.State_Delete (_State);
		}

		public void RegisterFunction(string Name, UserFunction Function, bool StubFunction)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi(Name);
			short MStubFunction = (short)(StubFunction ? 1 : 0);
			NativeState.State_RegisterFunction(_State, AnsiName, Function, MStubFunction);
			Marshal.FreeHGlobal(AnsiName);
		}

		public bool Iterate ()
		{
			short r = NativeState.State_Iterate (_State);
			return r == 1 ? true : false;
		}

		public void RunFromMethod (string Name)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi(Name);
			NativeState.State_RunFromMethod(_State, AnsiName);
			Marshal.FreeHGlobal(AnsiName);
		}

		public void LoadMethod (string Name)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi (Name);
			NativeState.State_LoadMethod (_State, AnsiName);
			Marshal.FreeHGlobal (AnsiName);
		}

		public object Pop ()
		{
			GCHandle tmpHandle = GCHandle.FromIntPtr(NativeState.State_Pop(_State));
			object ReturnValue = tmpHandle.Target;
			tmpHandle.Free();
			return ReturnValue;
		}
		
		public int PopInt ()
		{
			return NativeState.State_PopInt (_State);
		}
		
		public int GetIntVariableInScope (string Name)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi(Name);
			int r = NativeState.State_GetIntVariableInScope(_State, AnsiName);
			Marshal.FreeHGlobal(AnsiName);
			return r;
		}

		public double PopDouble ()
		{
			return NativeState.State_PopDouble (_State);
		}
		
		public double GetDoubleVariableInScope (string Name)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi(Name);
			double r = NativeState.State_GetDoubleVariableInScope(_State, AnsiName);
			Marshal.FreeHGlobal(AnsiName);
			return r;
		}

		public bool PopBool ()
		{
			short r = NativeState.State_PopBool (_State);
			return r == 1 ? true : false;
		}
		
		public bool GetBoolVariableInScope (string Name)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi(Name);
			bool r = NativeState.State_GetBoolVariableInScope(_State, AnsiName)  == 1 ? true : false;
			Marshal.FreeHGlobal(AnsiName);
			return r;
		}

		public string PopString ()
		{
			IntPtr AnsiR = NativeState.State_PopString (_State);
			string rValue = Marshal.PtrToStringAnsi (AnsiR);
			NativeState.InteropFreePtr(AnsiR);
			return rValue;
		}
		
		public string GetStringVariableInScope (string Name)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi (Name);
			IntPtr AnsiR = NativeState.State_GetStringVariableInScope (_State, AnsiName);
			string r = Marshal.PtrToStringAnsi (AnsiR);
			NativeState.InteropFreePtr (AnsiR);
			Marshal.FreeHGlobal (AnsiName);
			return r;
		}

		public void Push (object Obj)
		{
			GCHandle tmpHandle = GCHandle.Alloc(Obj);
			NativeState.State_Push(_State, GCHandle.ToIntPtr(tmpHandle));
		}
		
		public void PushInt (int v)
		{
			NativeState.State_PushInt(_State, v);
		}
		
		public void SetIntVariableInScope (string Name, int v)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi (Name);
			NativeState.State_SetIntVariableInScope(_State, AnsiName, v);
			Marshal.FreeHGlobal (AnsiName);
		}

		public void PushDouble (double v)
		{
			NativeState.State_PushDouble(_State, v);
		}
		
		public void SetDoubleVariableInScope (string Name, double v)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi (Name);
			NativeState.State_SetDoubleVariableInScope(_State, AnsiName, v);
			Marshal.FreeHGlobal (AnsiName);
		}

		public void PushBool (bool v)
		{
			NativeState.State_PushBool(_State, (short)(v ? 1 : 0));
		}
		
		public void SetBoolVariableInScope (string Name, bool v)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi (Name);
			NativeState.State_SetBoolVariableInScope(_State, AnsiName, (short)(v ? 1 : 0));
			Marshal.FreeHGlobal (AnsiName);
		}

		public void PushString (string v)
		{
			IntPtr Ansiv = Marshal.StringToHGlobalAnsi(v);
			NativeState.State_PushString(_State, Ansiv);
			Marshal.FreeHGlobal(Ansiv);
		}
		
		public void SetStringVariableInScope (string Name, string v)
		{
			IntPtr AnsiName = Marshal.StringToHGlobalAnsi (Name);
			IntPtr Ansiv = Marshal.StringToHGlobalAnsi(v);
			NativeState.State_SetStringVariableInScope(_State, AnsiName, Ansiv);
			Marshal.FreeHGlobal(Ansiv);
			Marshal.FreeHGlobal(AnsiName);
		}
	}
}

