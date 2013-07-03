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

namespace libCpScript.Net.ObjectBasic
{
	internal partial class NativeScript
	{
		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.ObjectBasic.Shared.so", EntryPoint="ObjScript_New")]
		internal static extern  System.IntPtr ObjScript_New() ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.ObjectBasic.Shared.so", EntryPoint="RegisterFunction")]
		internal static extern  void RegisterFunction(System.IntPtr ObjScript, System.IntPtr Name) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.ObjectBasic.Shared.so", EntryPoint="ObjScript_Load")]
		internal static extern  void ObjScript_Load(System.IntPtr ObjScript, System.IntPtr Script) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.ObjectBasic.Shared.so", EntryPoint="ObjScript_GetAsm")]
		internal static extern  System.IntPtr ObjScript_GetAsm(System.IntPtr ObjScript) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.ObjectBasic.Shared.so", EntryPoint="ObjScript_Delete")]
		internal static extern  void ObjScript_Delete(System.IntPtr ObjScript) ;
	}
}

