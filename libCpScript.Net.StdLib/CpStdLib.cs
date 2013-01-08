/*

	libCpScript.Net.StdLib
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

namespace libCpScript.Net.StdLib
{
	public class CpStdLib
	{
		[System.Runtime.InteropServices.DllImportAttribute("libCpStdLib.Shared.so", EntryPoint="CpStdLib_InstallConsoleIO")]
		public static extern void InstallConsoleIO(System.IntPtr State) ;
		
		[System.Runtime.InteropServices.DllImportAttribute("libCpStdLib.Shared.so", EntryPoint="CpStdLib_InstallFileIO")]
		public static extern void InstallFileIO(System.IntPtr State) ;
		
		[System.Runtime.InteropServices.DllImportAttribute("libCpStdLib.Shared.so", EntryPoint="CpStdLib_InstallMath")]
		public static extern void InstallMath(System.IntPtr State) ;
		
		[System.Runtime.InteropServices.DllImportAttribute("libCpStdLib.Shared.so", EntryPoint="CpStdLib_InstallUtilities")]
		public static extern void InstallUtilities(System.IntPtr State) ;
	}
}

