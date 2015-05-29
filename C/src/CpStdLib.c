/*

    libCpStdLib
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

#include <CpStdLib.h>
#include <libCpScript.Asm.h>
#include <CpStdLib_ConsoleIO.h>
#include <CpStdLib_FileIO.h>
#include <CpStdLib_Math.h>
#include <CpStdLib_Utilities.h>

EXPORT void CpStdLib_InstallConsoleIO(void* State)
{
    State_RegisterFunction(State, "__StdLib__ConsoleIO__Write", &Console_Write, 0);
    State_RegisterFunction(State, "__StdLib__ConsoleIO__WriteLine", &Console_WriteLine, 0);
    State_RegisterFunction(State, "__StdLib__ConsoleIO__ReadLine", &Console_ReadLine, 0);
};

EXPORT void CpStdLib_InstallFileIO(void* State)
{
    State_RegisterFunction(State, "__StdLib__FileIO__Open", &File_Open, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__WriteLine", &File_WriteLine, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__ReadLine", &File_ReadLine, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__Close", &File_Close, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__Eof", &File_Eof, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__ReadBool", &File_ReadBool, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__ReadDouble", &File_ReadDouble, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__ReadInt", &File_ReadInt, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__ReadString", &File_ReadString, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__WriteBool", &File_WriteBool, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__WriteDouble", &File_WriteDouble, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__WriteInt", &File_WriteInt, 0);
    State_RegisterFunction(State, "__StdLib__FileIO__WriteString", &File_WriteString, 0);
};

EXPORT void CpStdLib_InstallMath(void* State)
{
    State_RegisterFunction(State, "__StdLib__Math__Abs", &Math_Abs, 0);
    State_RegisterFunction(State, "__StdLib__Math__Floor", &Math_Floor, 0);
    State_RegisterFunction(State, "__StdLib__Math__Ceiling", &Math_Ceiling, 0);
};

EXPORT void CpStdLib_InstallUtilities(void* State)
{
    State_RegisterFunction(State, "__StdLib__Utilities__Array__New", &Utilities_Array_New, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array__Free", &Utilities_Array_Free, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array__Count", &Utilities_Array_Count, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array__Resize", &Utilities_Array_Resize, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array__SetItem", &Utilities_Array_SetItem, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array__GetItem", &Utilities_Array_GetItem, 0);

	State_RegisterFunction(State, "__StdLib__Utilities__Array2D__New", &Utilities_Array2D_New, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array2D__Free", &Utilities_Array2D_Free, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array2D__XCount", &Utilities_Array2D_XCount, 0);
	State_RegisterFunction(State, "__StdLib__Utilities__Array2D__YCount", &Utilities_Array2D_YCount, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array2D__ResizeX", &Utilities_Array2D_ResizeX, 0);
	State_RegisterFunction(State, "__StdLib__Utilities__Array2D__ResizeY", &Utilities_Array2D_ResizeY, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array2D__SetItem", &Utilities_Array2D_SetItem, 0);
    State_RegisterFunction(State, "__StdLib__Utilities__Array2D__GetItem", &Utilities_Array2D_GetItem, 0);
	
	State_RegisterFunction(State, "__StdLib__Utilities__Time__GetTimeOfDay", &Utilities_Time_GetTimeOfDay, 0);
	State_RegisterFunction(State, "__StdLib__Utilities__Time__GetTimeDiff", &Utilities_Time_Diff, 0);
	State_RegisterFunction(State, "__StdLib__Utilities__Time__GetTimeMilliSeconds", &Utiltiess_Time_GetMilliseconds, 0);
	State_RegisterFunction(State, "__StdLib__Utilities__Time__FreeTimeOfDay", &Utilities_Time_FreeTimeOfDay, 0);
	
	State_RegisterFunction(State, "__StdLib__Utilities__String__Trim", &Utilities_String_Trim, 0);
	State_RegisterFunction(State, "__StdLib__Utilities__String__Split", &Utilities_String_Split, 0);
	State_RegisterFunction(State, "__StdLib__Utilities__String__SplitAndKeep", &Utilities_String_SplitAndKeep, 0);
};

