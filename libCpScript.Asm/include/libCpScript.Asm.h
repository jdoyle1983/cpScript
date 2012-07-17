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
EXPORT void* State_NewFromCompiled(void* Script, long Len);
EXPORT void* State_Compile(void* state, long* len);
EXPORT void State_Delete(void* State);
EXPORT void State_RegisterFunction(void* State, char* Name, void (*UserFunction)(void*), short StubFunction);
EXPORT short State_Iterate(void* State);
EXPORT void State_RunFromMethod(void* State, char* Name);
EXPORT void State_LoadMethod(void* State, char* Name);

EXPORT int State_PopInt(void* State);
EXPORT double State_PopDouble(void* State);
EXPORT short State_PopBool(void* State);
EXPORT char* State_PopString(void* State);

EXPORT void State_PushInt(void* State, int v);
EXPORT void State_PushDouble(void* State, double v);
EXPORT void State_PushBool(void* State, short v);
EXPORT void State_PushString(void* State, char* v);

EXPORT void InteropFreePtr(void* Ptr);
EXPORT void* InteropAllocLongPtr();
EXPORT long InteropLongPtrToLong(void* Ptr);

#ifdef __cplusplus
}
#endif

#endif
