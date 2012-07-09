#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <libCpScript.Asm.h>
#include <Stack.h>
#include <AssemblyToken.h>
#include <Extensions.h>
#include <IScriptLib.h>
#include <LabelDef.h>
#include <List.h>
#include <MemoryBlock.h>
#include <MemoryBlockHeader.h>
#include <MemoryBlockSetHeader.h>
#include <Parser.h>
#include <Token.h>

typedef struct
{
    Stack* _CursorStack;
    Stack* _Stack;
    List* _Memory;
    List* _Labels;
    Stack* _Registers;
    Stack* _Headers;
    Stack* _BlockHeaders;
    List* _LibTokens;
    List* _Tokens;
    List* _Libraries;
    int _Offset;

    char* OrigScript;
} State;

short ptMemoryBlockVar(char* Val)
{
    if(Val[0] == '$')
        return 1;
    return 0;
};

short ptMemoryBlockVarIndex(char* Val)
{
    if(Val[0] == '$')
    {
        int i = 0;
        for(i = 0; i < strlen(Val); i++)
            if(Val[i] == ':')
                return 1;
    }
    return 0;
};

short ptMemoryVar(char* Val)
{
    if(Val[0] == '%')
        return 1;
    return 0;
};

short ptRegister(char* Val)
{
    if(Val[0] == '@')
    {
        char* tStr = SubStr(Val, 1, strlen(Val) - 1);
        int r = 0;
        if(CanConvertToInt(tStr) == 1)
            r = 1;
        free(tStr);
        return r;
    }
    return 0;
};

short ptLabel(State* state, char* Val)
{
    int i = 0;
    for(i = 0; i < state->_Labels->Count; i++)
    {
        LabelDef* l = List_LabelDefAtIndex(state->_Labels, i);
        if(strcmp(l->Label, Val) == 0)
            return 1;
    }
    return 0;
};

short ptIntegerNonNegative(char* Val)
{
    if(CanConvertToInt(Val) == 0)
        return 0;
    int v = atoi(Val);
    if(v >= 0)
        return 1;
    return 0;
};

short ptInteger(char* Val)
{
    if(CanConvertToInt(Val) == 1)
        return 1;
    return 0;
};

short pt1(State* state, char* Val)
{
    return ptIntegerNonNegative(Val);
};

short pt2(State* state, char* Val)
{
    return ptInteger(Val);
};

short pt3(State* state, char* Val)
{
    return ptLabel(state, Val);
};

short pt4(State* state, char* Val)
{
    if(ptRegister(Val) == 1 || ptMemoryVar(Val) == 1 || ptMemoryBlockVarIndex(Val) == 1)
        return 1;
    return 0;
};

short pt5(State* state, char* Val)
{
    if(ptLabel(state, Val) == 0 && ptRegister(Val) == 0 && ptMemoryVar(Val) == 0)
        return 1;
    return 0;
};

short pt8(State* state, char* Val)
{
    if(ptLabel(state, Val) == 0)
        return 1;
    return 0;
};

short pt9(State* state, char* Val)
{
    return ptMemoryVar(Val);
};

void SetRegister(State* state, int id, char* Value)
{
    int targetId = id - 1;
    List* r = Stack_PeekList(state->_Registers);
    char* n = (char*)malloc(sizeof(char) * (strlen(Value) + 1));
    strcpy(n, Value);
    if(targetId < r->Count)
    {
        free((char*)r->Items[targetId]);
        r->Items[targetId] = n;
    }
    else
    {
        while(targetId != r->Count)
        {
            char* t = (char*)malloc(sizeof(char));
            t[0] = '\n';
            List_Add(r, t);
        }
        List_Add(r, n);
    }
};

void State_DoInit(State* state)
{
    List* _NewToks = List_New();
    int i = 0;
    for(i = 0; i < state->_Tokens->Count; i++)
    {
        AssemblyToken* aTok = List_AssemblyTokenAtIndex(state->_Tokens, i);
        AssemblyToken* nTok = List_AssemblyTokenAtIndex(state->_Tokens, i + 1);
        if(aTok->Tok == tLib && nTok->Tok == tQuotedLiteral)
        {
            List_Add(state->_LibTokens, aTok);
            List_Add(state->_LibTokens, nTok);
            i++;
            Library* lib = Library_Load(nTok->Val);
            if(lib != NULL)
            {
                List_Add(state->_Libraries, lib);
                lib->Init(state);
            }
        }
        else
            List_Add(_NewToks, aTok);
    }

    List_Delete(state->_Tokens);
    state->_Tokens = _NewToks;

    for(i = 0; i < state->_Tokens->Count; i++)
    {
        AssemblyToken* aTok = List_AssemblyTokenAtIndex(state->_Tokens, i);
        if(aTok->Tok == tLabel && (i + 1) < state->_Tokens->Count)
        {
            AssemblyToken* nTok = List_AssemblyTokenAtIndex(state->_Tokens, i + 1);
            if(nTok->Tok == tLiteral)
            {
                LabelDef* d = LabelDef_NewWithOffset(nTok->Val, i + 2);
                i+=2;
                List_Add(state->_Labels, d);
            }
        }
    }

    Stack_Push(state->_Registers, List_New());
    Stack_Push(state->_Headers, List_New());
    Stack_Push(state->_BlockHeaders, List_New());
};

void* State_New(char* ScriptText)
{
    State* state = (State*)malloc(sizeof(State));
    state->_CursorStack = Stack_New();
    state->_Stack = Stack_New();
    state->_Memory = List_New();
    state->_Labels = List_New();
    state->_Registers = Stack_New();
    state->_Headers = Stack_New();
    state->_BlockHeaders = Stack_New();
    state->_LibTokens = List_New();
    state->_Libraries = List_New();
    state->_Offset = -1;
    state->OrigScript = (char*)malloc(sizeof(char) * (strlen(ScriptText) + 1));
    strcpy(state->OrigScript, ScriptText);

    state->_Tokens = Parse(ScriptText);
    State_DoInit(state);
    return state;
};

void State_Delete(void* S)
{
    State* state = (State*)S;
    int i = 0;
    int e = 0;

    for(i = 0; i < state->_LibTokens->Count; i++)
        AssemblyToken_Delete(List_AssemblyTokenAtIndex(state->_LibTokens, i));
    List_Delete(state->_LibTokens);
    for(i = 0; i < state->_Tokens->Count; i++)
        AssemblyToken_Delete(List_AssemblyTokenAtIndex(state->_Tokens, i));
    List_Delete(state->_Tokens);
    for(i = 0; i < state->_Libraries->Count; i++)
        Library_Free(List_LibraryAtIndex(state->_Libraries, i));
    List_Delete(state->_Libraries);
    for(i = 0; i < state->_Stack->Count; i++)
        free(List_StringAtIndex(state->_Stack, i));
    List_Delete(state->_Stack);
    for(i = 0; i < state->_Memory->Count; i++)
        MemoryBlock_Delete(List_MemoryBlockAtIndex(state->_Memory, i));
    List_Delete(state->_Memory);
    for(i = 0; i < state->_Labels->Count; i++)
        LabelDef_Delete(List_LabelDefAtIndex(state->_Labels, i));
    List_Delete(state->_Labels);
    for(i = 0; i < state->_Registers->Count; i++)
    {
        List* rList = List_ListAtIndex(state->_Registers, i);
        for(e = 0; e < rList->Count; e++)
            free(List_StringAtIndex(rList, e));
        List_Delete(rList);
    }
    List_Delete(state->_Registers);
    for(i = 0; i < state->_Headers->Count; i++)
    {
        List* hList = List_ListAtIndex(state->_Headers, i);
        for(e = 0; e < hList->Count; e++)
            MemoryBlockHeader_Delete(List_MemoryBlockHeaderAtIndex(hList, e));
        List_Delete(hList);
    }
    List_Delete(state->_Headers);
    for(i = 0; i < state->_BlockHeaders->Count; i++)
    {
        List* hList = List_ListAtIndex(state->_BlockHeaders, i);
        for(e = 0; e < hList->Count; e++)
            MemoryBlockSetHeader_Delete(List_MemoryBlockSetHeaderAtIndex(hList, e));
        List_Delete(hList);
    }
    List_Delete(state->_BlockHeaders);
    List_Delete(state->_CursorStack);

    free(state);
};

void State_RegisterFunction(void* S, char* Name, void (*UserFunction)(void*), short StubFunction)
{
    State* state = (State*)S;
    short wasFound = 0;
    int i = 0;
    for(i = 0; i < state->_Labels->Count; i++)
    {
        LabelDef* l = List_LabelDefAtIndex(state->_Labels, i);
        if(strcmp(l->Label, Name) == 0)
        {
            wasFound = 1;
            if(l->UserFunction != NULL)
            {
                if(l->IsStub == 1 && StubFunction == 0)
                    l->UserFunction = UserFunction;
            }
        }
    }
    if(wasFound == 0)
    {
        LabelDef* l = LabelDef_NewWithFunction(Name, UserFunction, StubFunction);
        List_Add(state->_Labels, l);
    }
};

short State_Iterate(void* State)
{
    return 1;
};

void State_RunFromMethod(void* S, char* Name)
{
    State_LoadMethod(S, Name);
    while(State_Iterate(S) == 1);
};

void State_LoadMethod(void* S, char* Name)
{
    State* state = (State*)S;
    state->_Offset = -1;
    int i = 0;
    for(i = 0; i < state->_Labels->Count; i++)
    {
        LabelDef* l = List_LabelDefAtIndex(state->_Labels, i);
        if(strcmp(l->Label, Name) == 0 && l->Offset >= 0)
            state->_Offset = l->Offset;
    }
};

void State_ResetState(void* S)
{
    State* state = (State*)S;
    char* oScript = (char*)malloc(sizeof(char) * (strlen(state->OrigScript) + 1));
    strcpy(oScript, state->OrigScript);
    State_Delete(state);
    state = State_New(oScript);
    free(oScript);
};

int State_PopInt(void* S)
{
    char* v = State_PopString(S);
    int r = atoi(v);
    free(v);
    return r;
};

double State_PopDouble(void* S)
{
    char* v = State_PopString(S);
    double r = (double)atof(v);
    free(v);
    return r;
};

short State_PopBool(void* S)
{
    char* v = State_PopString(S);
    short r = (short)atoi(v);
    free(v);
    return r;
};

char* State_PopString(void* S)
{
    State* state = (State*)S;
    char* r = Stack_PopString(state->_Stack);
    return r;
};

void State_PushInt(void* S, int v)
{
    char* p = (char*)malloc(sizeof(char) * 3000);
    sprintf(p, "%d", v);
    State_PushString(S, p);
    free(p);
};

void State_PushDouble(void* S, double v)
{
    char* p = (char*)malloc(sizeof(char) * 3000);
    sprintf(p, "%f", v);
    State_PushString(S, p);
    free(p);
};

void State_PushBool(void* S, short v)
{
    char* p = (char*)malloc(sizeof(char) * 100);
    sprintf(p, "%d", v);
    State_PushString(S, p);
    free(p);
};

void State_PushString(void* S, char* v)
{
    State* state = (State*)S;
    char* toPush = (char*)malloc(sizeof(char) * (strlen(v) + 1));
    strcpy(toPush, v);
    Stack_Push(state->_Stack, toPush);
};
