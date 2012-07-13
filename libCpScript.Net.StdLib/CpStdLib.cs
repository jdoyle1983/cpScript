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

