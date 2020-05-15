/*

    libCpScript.Asm,
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

#ifndef __LIBCPSCRIPT_ASM_H__
#define __LIBCPSCRIPT_ASM_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDLL
#define EXPORT __declspec( dllexport )
#else
#define EXPORT
#endif

EXPORT void* State_New(char* ScriptText);
#ifdef USECOMPRESSION
EXPORT void* State_NewFromCompiled(void* Script, long Len);
EXPORT void* State_Compile(void* state, long* len);
#endif
EXPORT void State_Delete(void* State);
EXPORT void State_RegisterFunction(void* State, char* Name, void (*UserFunction)(void*), short StubFunction);
EXPORT short State_Iterate(void* State);
EXPORT void State_RunFromMethod(void* State, char* Name);
EXPORT void State_LoadMethod(void* State, char* Name);

EXPORT void* State_Pop(void* State);
EXPORT long State_PopInt(void* State);
EXPORT double State_PopDouble(void* State);
EXPORT short State_PopBool(void* State);
EXPORT char* State_PopString(void* State);
EXPORT void* State_Pop(void* State);

EXPORT void State_Push(void* State, void* v);
EXPORT void State_PushInt(void* State, long v);
EXPORT void State_PushDouble(void* State, double v);
EXPORT void State_PushBool(void* State, short v);
EXPORT void State_PushString(void* State, char* v);
EXPORT void State_Push(void* State, void* v);

EXPORT long State_GetIntVariableInScope(void* S, char* n);
EXPORT double State_GetDoubleVariableInScope(void* S, char* n);
EXPORT short State_GetBoolVariableInScope(void* S, char* n);
EXPORT char* State_GetStringVariableInScope(void* S, char* n);

EXPORT void State_SetIntVariableInScope(void* S, char* n, long v);
EXPORT void State_SetDoubleVariableInScope(void* S, char* n, double v);
EXPORT void State_SetBoolVariableInScope(void* S, char* n, short v);
EXPORT void State_SetStringVariableInScope(void* S, char* n, char* v);

EXPORT void InteropFreePtr(void* Ptr);
EXPORT void* InteropAllocLongPtr();
EXPORT long InteropLongPtrToLong(void* Ptr);

#ifdef __cplusplus
}
#endif

#endif
