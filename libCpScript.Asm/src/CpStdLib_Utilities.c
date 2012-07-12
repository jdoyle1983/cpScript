#include <CpStdLib_Utilities.h>
#include <libCpScript.Asm.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

typedef struct
{
    char** Items;
    int Count;
} CpArray;

void Utilities_Array_New(void* State)
{
    CpArray* a = (CpArray*)malloc(sizeof(CpArray));
    a->Items = malloc(0);
    a->Count = 0;
    State_PushInt(State, (int)a);
};

void Utilities_Array_Free(void* State)
{
    CpArray* a = (CpArray*)State_PopInt(State);
    int i = 0;
    for(i = 0; i < a->Count; i++)
        free(a->Items[i]);
    free(a->Items);
    free(a);
};

void Utilities_Array_Count(void* State)
{
    CpArray* a = (CpArray*)State_PopInt(State);
    State_PushInt(State, a->Count);
};

void Utilities_Array_SetItem(void* State)
{
    char* toSet = State_PopString(State);
    CpArray* a = (CpArray*)State_PopInt(State);
    a->Count++;
    a->Items = (char**)realloc(a->Items, sizeof(char*) * a->Count);
    a->Items[a->Count - 1] = toSet;
};

void Utilities_Array_GetItem(void* State)
{
    int idx = State_PopInt(State);
    CpArray* a = (CpArray*)State_PopInt(State);
    State_PushString(State, a->Items[idx]);
};
