using System;
using System.Runtime.InteropServices;

namespace libCpScript.Net.Asm
{
	public delegate void UserFunction(System.IntPtr StatePtr);

	internal partial class NativeState 
	{
		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_New")]
		internal static extern  System.IntPtr State_New(System.IntPtr ScriptText) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_Delete")]
		internal static extern  void State_Delete(System.IntPtr StatePtr) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_RegisterFunction")]
		internal static extern  void State_RegisterFunction(System.IntPtr StatePtr, System.IntPtr Name, UserFunction UserFunction, short StubFunction) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_Iterate")]
		internal static extern  short State_Iterate(System.IntPtr StatePtr) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_RunFromMethod")]
		internal static extern  void State_RunFromMethod(System.IntPtr StatePtr, System.IntPtr Name) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_LoadMethod")]
		internal static extern  void State_LoadMethod(System.IntPtr StatePtr, System.IntPtr Name) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_PopInt")]
		internal static extern  int State_PopInt(System.IntPtr StatePtr) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_PopDouble")]
		internal static extern  double State_PopDouble(System.IntPtr StatePtr) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_PopBool")]
		internal static extern  short State_PopBool(System.IntPtr StatePtr) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_PopString")]
		internal static extern  System.IntPtr State_PopString(System.IntPtr StatePtr) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_PushInt")]
		internal static extern  void State_PushInt(System.IntPtr StatePtr, int v) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_PushDouble")]
		internal static extern  void State_PushDouble(System.IntPtr StatePtr, double v) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_PushBool")]
		internal static extern  void State_PushBool(System.IntPtr StatePtr, short v) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="State_PushString")]
		internal static extern  void State_PushString(System.IntPtr StatePtr, System.IntPtr v) ;

		[System.Runtime.InteropServices.DllImportAttribute("libCpScript.Asm.Shared.so", EntryPoint="InteropFreeString")]
		internal static extern  void InteropFreeString(System.IntPtr str) ;
	}
}