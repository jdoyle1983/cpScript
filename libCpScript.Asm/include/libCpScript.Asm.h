#ifndef __LIBCPSCRIPT_ASM_H__
#define __LIBCPSCRIPT_ASM_H__

void* State_New(char* ScriptText);
void State_Delete(void* State);
void State_RegisterFunction(void* State, char* Name, void (*UserFunction)(void*), short StubFunction);
short State_Iterate(void* State);
void State_RunFromMethod(void* State, char* Name);
void State_LoadMethod(void* State, char* Name);
void State_ResetState(void* State);

int State_PopInt(void* State);
double State_PopDouble(void* State);
short State_PopBool(void* State);
char* State_PopString(void* State);

void State_PushInt(void* State, int v);
void State_PushDouble(void* State, double v);
void State_PushBool(void* State, short v);
void State_PushString(void* State, char* v);

#endif
