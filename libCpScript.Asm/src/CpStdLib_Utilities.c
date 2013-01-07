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
	int i = 0;
    int Size = State_PopInt(State);
    CpArray* a = (CpArray*)malloc(sizeof(CpArray));
    a->Count = Size;
    a->Items = (char**)malloc(sizeof(char*) * Size);
    for(i = 0; i < Size; i++)
    {
        a->Items[i] = (char*)malloc(sizeof(char) * 2);
        strcpy(a->Items[i], "");
    }
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

void Utilities_Array_Resize(void* State)
{
    CpArray* a = (CpArray*)State_PopInt(State);
    int newSize = State_PopInt(State);

    int origSize = a->Count;
    int i = 0;

    if(newSize < origSize)
    {
        for(i = newSize; i < origSize; i++)
            free(a->Items[i]);
    }

    a->Items = (char**)realloc(a->Items, sizeof(char*) * newSize);

    if(newSize > origSize)
    {
        for(i = origSize; i < newSize; i++)
        {
            a->Items[i] = (char*)malloc(sizeof(char) * 2);
            strcpy(a->Items[i], "");
        }
    }
};

void Utilities_Array_SetItem(void* State)
{
    int ArrayIndex = State_PopInt(State);
    char* ArrayValue = State_PopString(State);
    CpArray* a = (CpArray*)State_PopInt(State);
    if(ArrayIndex >= 0 && ArrayIndex < a->Count)
    {
        free(a->Items[ArrayIndex]);
        a->Items[ArrayIndex] = (char*)malloc(sizeof(char) * (strlen(ArrayValue) + 1));
        strcpy(a->Items[ArrayIndex], ArrayValue);
    }
};

void Utilities_Array_GetItem(void* State)
{
    int ArrayIndex = State_PopInt(State);
    CpArray* a = (CpArray*)State_PopInt(State);
    if(ArrayIndex >= 0 && ArrayIndex < a->Count)
        State_PushString(State, a->Items[ArrayIndex]);
    else
        State_PushString(State, "");
};
