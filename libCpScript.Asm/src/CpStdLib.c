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
};

