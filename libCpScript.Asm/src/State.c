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
        if(r->Items[targetId] != NULL)
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

char* ReadRegister(State* state, int id)
{
    int targetId = id - 1;
    List* r = Stack_PeekList(state->_Registers);
    if(targetId < r->Count)
        return List_StringAtIndex(r, targetId);
    return NULL;
};

void AllocateMemoryBlockSet(State* state, char* Id, int Count)
{
    MemoryBlockSetHeader* h = MemoryBlockSetHeader_New(Id, Count);
    int i = 0;
    for(i = 0; i < Count; i++)
    {
        short found = 0;
        int c  = 0;
        for(c = 0; c < state->_Memory->Count; c++)
        {
            MemoryBlock* b = List_MemoryBlockAtIndex(state->_Memory, c);
            if(b->Used == 0)
            {
                MemoryBlockSetHeader_SetOffset(h, i, c);
                found = 1;
            }
        }

        if(found == 0)
        {
            MemoryBlock* mb = MemoryBlock_New();
            MemoryBlock_SetValue(mb, "");
            List_Add(state->_Memory, mb);
            MemoryBlockSetHeader_SetOffset(h, i, state->_Memory->Count - 1);
        }
    }

    List* Headers = Stack_Peek(state->_BlockHeaders);
    List_Add(Headers, h);
};

void AllocateMemoryBlock(State* state, char* Id)
{
    MemoryBlockHeader* h = NULL;
    short found = 0;
    int c = 0;
    for(c = 0; c < state->_Memory->Count; c++)
    {
        MemoryBlock* b = List_MemoryBlockAtIndex(state->_Memory, c);
        if(b->Used == 0)
        {
            h = MemoryBlockHeader_New(Id, c);
            MemoryBlock_SetValue(b, "");
            found = 1;
        }
    }

    if(!found)
    {
        MemoryBlock* b = MemoryBlock_New();
        MemoryBlock_SetValue(b, "");
        List_Add(state->_Memory, b);
        h = MemoryBlockHeader_New(Id, state->_Memory->Count - 1);
    }

    List* Headers = Stack_PeekList(state->_Headers);
    List_Add(Headers, h);
};

void ReferenceMemoryBlockSet(State* state, char* OrigId, char* NewId)
{
    MemoryBlockSetHeader* hed = NULL;
    MemoryBlockSetHeader* hed2 = NULL;
    List* BlockHeaders = Stack_Peek(state->_BlockHeaders);
    int c = 0;
    for(c = 0; c < BlockHeaders->Count; c++)
    {
        MemoryBlockSetHeader* h = List_MemoryBlockSetHeaderAtIndex(BlockHeaders, c);
        if(strcmp(h->Name, NewId) == 0)
            hed = h;
        if(strcmp(h->Name, OrigId) == 0)
            hed2 = h;
    }

    if(hed2 != NULL)
    {
        short shouldAdd = 1;
        if(hed == NULL)
            hed = MemoryBlockSetHeader_New(NewId, hed2->IndexOffset->Count);
        else
            shouldAdd = 0;
        int i = 0;
        for(i = 0; i < hed2->IndexOffset->Count; i++)
            MemoryBlockSetHeader_SetOffset(hed, i, MemoryBlockSetHeader_GetOffset(hed2, i));

        if(shouldAdd == 1)
        {
            List* Headers = Stack_Peek(state->_BlockHeaders);
            List_Add(Headers, hed);
        }
    }
};

void ReferenceMemoryBlock(State* state, char* OrigId, char* NewId)
{
    MemoryBlockHeader* hed = NULL;
    int c = 0;
    List* Headers = Stack_PeekList(state->_Headers);
    int targetOffset = -1;
    for(c = 0; c < Headers->Count; c++)
    {
        MemoryBlockHeader* h = List_MemoryBlockHeaderAtIndex(Headers, c);
        if(strcmp(h->Name, NewId) == 0)
            hed = h;
        if(strcmp(h->Name, OrigId) == 0)
            targetOffset = h->Offset;
    }

    if(hed == NULL)
    {
        hed = MemoryBlockHeader_New(NewId, targetOffset);
        List_Add(Headers, hed);
    }
    else
        hed->Offset = targetOffset;
};

void SetMemoryBlockSetIndex(State* state, char* Id, char* Value)
{
    List* Parts = SplitAndKeep(Id, ":");
    char* Name = List_StringAtIndex(Parts, 0);
    int Index = atoi(List_StringAtIndex(Parts, 2));
    List* BlockHeaders = Stack_PeekList(state->_BlockHeaders);
    int c = 0;
    for(c = 0; c < BlockHeaders->Count; c++)
    {
        MemoryBlockSetHeader* h = List_MemoryBlockSetHeaderAtIndex(BlockHeaders, c);
        if(strcmp(h->Name, Name) == 0)
        {
            int mbOffset = MemoryBlockSetHeader_GetOffset(h, Index - 1);
            MemoryBlock* b = List_MemoryBlockAtIndex(state->_Memory, mbOffset);
            MemoryBlock_SetValue(b, Value);
        }
    }
    for(c = 0; c < Parts->Count; c++)
        free(List_StringAtIndex(Parts, c));
    List_Delete(Parts);
};

void SetMemoryBlock(State* state, char* Id, char* Value)
{
    List* Headers = Stack_PeekList(state->_Headers);
    int c = 0;
    for(c = 0; c < Headers->Count; c++)
    {
        MemoryBlockHeader* h = List_MemoryBlockHeaderAtIndex(Headers, c);
        if(strcmp(h->Name, Id) == 0)
        {
            MemoryBlock* b = List_MemoryBlockAtIndex(state->_Memory, h->Offset);
            MemoryBlock_SetValue(b, Value);
        }
    }
};

char* ReadMemoryBlockSetIndex(State* state, char* Id)
{
    char* rValue = NULL;
    List* Parts = SplitAndKeep(Id, ":");
    char* Name = List_StringAtIndex(Parts, 0);
    int Index = atoi(List_StringAtIndex(Parts, 2));
    List* BlockHeaders = Stack_PeekList(state->_BlockHeaders);
    int c = 0;
    for(c = 0; c < BlockHeaders->Count; c++)
    {
        MemoryBlockSetHeader* h = List_MemoryBlockSetHeaderAtIndex(BlockHeaders, c);
        if(strcmp(h->Name, Name) == 0)
        {
            int mbOffset = MemoryBlockSetHeader_GetOffset(h, Index - 1);
            MemoryBlock* b = List_MemoryBlockAtIndex(state->_Memory, mbOffset);
            rValue = b->Value;
        }
    }

    for(c = 0; c < Parts->Count; c++)
        free(List_StringAtIndex(Parts, c));
    List_Delete(Parts);

    return rValue;
};

char* ReadMemoryBlock(State* state, char* Id)
{
    char* rValue = NULL;
    List* Headers = Stack_PeekList(state->_Headers);
    int c = 0;
    for(c = 0; c < Headers->Count; c++)
    {
        MemoryBlockHeader* h = List_MemoryBlockHeaderAtIndex(Headers, c);
        if(strcmp(h->Name, Id) == 0)
        {
            MemoryBlock* mb = List_MemoryBlockAtIndex(state->_Memory, h->Offset);
            rValue = mb->Value;
        }
    }
    return rValue;
};

void FreeMemoryBlockSet(State* state, char* Id)
{
    MemoryBlockSetHeader* hed = NULL;
    List* BlockHeaders = Stack_PeekList(state->_BlockHeaders);
    int c = 0;
    for(c = 0; c < BlockHeaders->Count; c++)
    {
        MemoryBlockSetHeader* h = List_MemoryBlockSetHeaderAtIndex(BlockHeaders, c);
        if(strcmp(h->Name, Id) == 0)
            hed = h;
    }

    if(hed != NULL)
    {
        for(c = 0; c < hed->IndexOffset->Count; c++)
        {
            MemoryBlock* mb = List_MemoryBlockAtIndex(state->_Memory, MemoryBlockSetHeader_GetOffset(hed, c));
            MemoryBlock_Free(mb);
        }
    }
};

void FreeMemoryBlock(State* state, char* Id)
{
    List* Headers = Stack_PeekList(state->_Headers);
    int c = 0;
    for(c = 0; c < Headers->Count; c++)
    {
        MemoryBlockHeader* h = List_MemoryBlockHeaderAtIndex(Headers, c);
        if(strcmp(h->Name, Id) == 0)
        {
            MemoryBlock* mb = List_MemoryBlockAtIndex(state->_Memory, h->Offset);
            MemoryBlock_Free(mb);
        }
    }
};

void State_DoInit(State* state)
{
    List* _NewToks = List_New();
    int i = 0;
    for(i = 0; i < state->_Tokens->Count; i++)
    {
        AssemblyToken* aTok = List_AssemblyTokenAtIndex(state->_Tokens, i);
        AssemblyToken* nTok = NULL;
        if((i + 1) < state->_Tokens->Count)
            nTok = List_AssemblyTokenAtIndex(state->_Tokens, i + 1);
        if(aTok->Tok == tLib && nTok != NULL && nTok->Tok == tQuotedLiteral)
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

AssemblyToken* CurrentTok(State* state)
{
    return List_AssemblyTokenAtIndex(state->_Tokens, state->_Offset);
}

int GetRegisterIndex(char* Src)
{
    char* tmp = SubStr(Src, 1, strlen(Src) - 1);
    int RegId = atoi(tmp);
    free(tmp);
    return RegId;
};

char* GetActualVal(State* state, char* Src)
{
    char* rVal = NULL;
    if(ptRegister(Src) == 1)
    {
        int RegId = GetRegisterIndex(Src);
        rVal = ReadRegister(state, RegId);
    }
    else if(ptMemoryVar(Src) == 1)
        rVal = ReadMemoryBlock(state, Src);
    else if(ptMemoryBlockVar(Src) == 1)
        rVal = ReadMemoryBlockSetIndex(state, Src);
    else
        rVal = Src;
    return rVal;
};

short State_Iterate(void* S)
{
    State* state = (State*)S;
    if(state->_Offset >= state->_Tokens->Count)
        state->_Offset = -1;

    if(state->_Offset > -1)
    {
        //printf("**********DBG: '%s'\n", CurrentTok(state)->Val);
        switch(CurrentTok(state)->Tok)
        {
            case tLabel: state->_Offset += 2; break;

            case tPushB:
            {
                state->_Offset++;
                if(ptMemoryBlockVar(CurrentTok(state)->Val) == 0)
                {
                    printf("ERROR\n");
                }
                List* BlockHeaders = Stack_PeekList(state->_BlockHeaders);
                int c = 0;
                for(c = 0; c < BlockHeaders->Count; c++)
                {
                    MemoryBlockSetHeader* h = List_MemoryBlockSetHeaderAtIndex(BlockHeaders, c);
                    if(strcmp(h->Name, CurrentTok(state)->Val) == 0)
                    {
                        int i = 0;
                        for(i = 0; i < h->IndexOffset->Count; i++)
                            State_PushInt(state, MemoryBlockSetHeader_GetOffset(h, i));
                        State_PushInt(state, h->IndexOffset->Count);
                    }
                }
                state->_Offset++;
            } break;

            case tPushA:
            {
                state->_Offset++;
                if(ptMemoryVar(CurrentTok(state)->Val) == 0)
                {
                    printf("ERROR\n");
                }
                int targetOffset = -1;
                List* Headers = Stack_PeekList(state->_Headers);
                int c = 0;
                for(c = 0; c < Headers->Count; c++)
                {
                    MemoryBlockHeader* h = List_MemoryBlockHeaderAtIndex(Headers, c);
                    if(strcmp(h->Name, CurrentTok(state)->Val) == 0)
                        targetOffset = h->Offset;
                }
                if(targetOffset == -1)
                {
                    printf("ERROR\n");
                }
                State_PushInt(state, targetOffset);
                state->_Offset++;
            } break;

            case tPopB:
            {
                state->_Offset++;
                if(ptMemoryBlockVar(CurrentTok(state)->Val) == 0)
                {
                    printf("ERROR\n");
                }
                MemoryBlockSetHeader* hed = NULL;
                List* BlockHeaders = Stack_PeekList(state->_BlockHeaders);
                int c = 0;
                for(c = 0; c < BlockHeaders->Count; c++)
                {
                    MemoryBlockSetHeader* h = List_MemoryBlockSetHeaderAtIndex(BlockHeaders, c);
                    if(strcmp(h->Name, CurrentTok(state)->Val) == 0)
                        hed = h;
                }
                short shouldAdd = 1;
                if(hed == NULL)
                    hed = MemoryBlockSetHeader_New(CurrentTok(state)->Val, 0);
                else
                {
                    List_Clear(hed->IndexOffset);
                    shouldAdd = 0;
                }

                int Count = State_PopInt(state);
                int i = 0;
                for(i = 0; i < Count; i++)
                    List_AddInt(hed->IndexOffset, -1);
                for(i = Count - 1; i >= 0; i--)
                    hed->IndexOffset->Items[i] = (void*)State_PopInt(state);

                if(shouldAdd)
                    List_Add(BlockHeaders, hed);
                state->_Offset++;
            } break;

            case tPopA:
            {
                state->_Offset++;
                if(ptMemoryVar(CurrentTok(state)->Val) == 0)
                {
                    printf("ERROR\n");
                }
                short found = 0;
                List* Headers = Stack_PeekList(state->_Headers);
                int c = 0;
                for(c = 0; c < Headers->Count; c++)
                {
                    MemoryBlockHeader* h = List_MemoryBlockHeaderAtIndex(Headers, c);
                    if(strcmp(h->Name, CurrentTok(state)->Val) == 0)
                    {
                        h->Offset = State_PopInt(state);
                        found = 1;
                    }
                }

                if(!found)
                {
                    MemoryBlockHeader* h = MemoryBlockHeader_New(CurrentTok(state)->Val, Stack_PopInt(state->_Stack));
                    List_Add(Headers, h);
                }

                state->_Offset++;
            } break;

            case tMove:
            {
                state->_Offset++;
                AssemblyToken* destTok = CurrentTok(state);
                state->_Offset++;
                AssemblyToken* commaTok = CurrentTok(state);
                state->_Offset++;
                AssemblyToken* srcTok = CurrentTok(state);

                if(pt4(state, destTok->Val) == 0)
                {
                    printf("ERROR\n");
                }
                if(commaTok->Tok != tComma)
                {
                    printf("ERROR\n");
                }
                if(pt8(state, srcTok->Val) == 0)
                {
                    printf("ERROR\n");
                }

                char* sVal = GetActualVal(state, srcTok->Val);
                if(ptRegister(destTok->Val) == 1)
                    SetRegister(state, GetRegisterIndex(destTok->Val), sVal);
                else if(ptMemoryBlockVarIndex(destTok->Val) == 1)
                    SetMemoryBlockSetIndex(state, destTok->Val, sVal);
                else
                    SetMemoryBlock(state, destTok->Val, sVal);

                state->_Offset++;
            } break;

            case tPush:
            {
                state->_Offset++;
                if(pt8(state, CurrentTok(state)->Val) == 0)
                {
                    printf("ERROR\n");
                }
                char* rVal = GetActualVal(state, CurrentTok(state)->Val);
                State_PushString(state, rVal);

                state->_Offset++;
            } break;

            case tPop:
            {
                state->_Offset++;
                if(pt4(state, CurrentTok(state)->Val) == 0)
                {
                    printf("ERROR\n");
                }

                char* Val = State_PopString(state);
                if(ptRegister(CurrentTok(state)->Val) == 1)
                    SetRegister(state, GetRegisterIndex(CurrentTok(state)->Val), Val);
                else if(ptMemoryBlockVarIndex(CurrentTok(state)->Val) == 1)
                    SetMemoryBlockSetIndex(state, CurrentTok(state)->Val, Val);
                else
                    SetMemoryBlock(state, CurrentTok(state)->Val, Val);

                state->_Offset++;
                free(Val);
            } break;

            case tAdd: case tSub: case tMul: case tDiv: case tMod:
            {
                char* s1 = State_PopString(state);
                char* s2 = State_PopString(state);

                double v2 = 0;
                double v1 = 0;

                if(CanConvertToDouble(s1) == 0 || CanConvertToDouble(s2) == 0)
                {
                    printf("ERROR\n");
                }

                v1 = (double)atof(s1);
                v2 = (double)atof(s2);

                double v3 = 0;

                switch(CurrentTok(state)->Tok)
                {
                    case tAdd: v3 = v2 + v1; break;
                    case tSub: v3 = v2 - v1; break;
                    case tMul: v3 = v2 * v1; break;
                    case tDiv: v3 = v2 / v1; break;
                    case tMod: v3 = v2 - (v1 * (v2 / v1)); break;
                }
                State_PushDouble(state, v3);

                free(s1);
                free(s2);

                state->_Offset++;
            } break;

            case tCat:
            {
                char* v2 = State_PopString(state);
                char* v1 = State_PopString(state);

                char* newChar = (char*)malloc(sizeof(char) * (strlen(v2) + strlen(v1) + 1));
                strcpy(newChar, v1);
                strcat(newChar, v2);

                State_PushString(state, newChar);
                free(v2);
                free(v1);
                free(newChar);

                state->_Offset++;
            } break;

            case tNeg:
            {
                char* s1 = State_PopString(state);
                double v1 = 0;
                if(CanConvertToDouble(s1) == 0)
                {
                    printf("ERROR\n");
                }
                v1 = (double)atof(s1);
                v1 = v1 * -1;
                State_PushDouble(state, v1);
                free(s1);
                state->_Offset++;
            } break;

            case tJumpOffset:
            {
                Stack_Push(state->_Registers, List_New());
                Stack_Push(state->_Headers, List_New());
                Stack_Push(state->_BlockHeaders, List_New());
                state->_Offset++;
                char* val = CurrentTok(state)->Val;
                state->_Offset++;
                Stack_Push(state->_CursorStack, (void*)state->_Offset);
                int newOffset = -1;
                char* rVal = GetActualVal(state, val);
                if(CanConvertToInt(rVal) == 0)
                {
                    printf("ERROR\n");
                }
                newOffset = atoi(rVal);
                state->_Offset = newOffset;
            } break;

            case tJump:
            {
                Stack_Push(state->_Registers, List_New());
                Stack_Push(state->_Headers, List_New());
                Stack_Push(state->_BlockHeaders, List_New());
                state->_Offset++;
                char* labelTitle = CurrentTok(state)->Val;
                state->_Offset++;
                Stack_Push(state->_CursorStack, (void*)state->_Offset);
                LabelDef* l = NULL;
                int c = 0;
                for(c = 0; c < state->_Labels->Count; c++)
                {
                    LabelDef* d = List_LabelDefAtIndex(state->_Labels, c);
                    if(strcmp(d->Label, labelTitle) == 0)
                        l = d;
                }
                if(l == NULL)
                {
                    printf("ERROR\n");
                }
                if(l->UserFunction != NULL)
                {
                    l->UserFunction(state);
                    Stack_PopList(state->_CursorStack);
                    List_Delete(Stack_PopList(state->_BlockHeaders));
                    List_Delete(Stack_PopList(state->_Registers));
                    List_Delete(Stack_PopList(state->_Headers));
                }
                else
                    state->_Offset = l->Offset;
            } break;

            case tReturn:
            {
                List* OldHeaders = Stack_PopList(state->_Headers);
                if(state->_Headers->Count < 1)
                    state->_Offset = -1;
                else
                {
                    List* OldBlockHeaders = Stack_PopList(state->_BlockHeaders);
                    List* OldRegisters = Stack_PopList(state->_Registers);
                    state->_Offset = Stack_PopInt(state->_CursorStack);
                    int c = 0;
                    for(c = 0; c < OldHeaders->Count; c++)
                        MemoryBlockHeader_Delete(List_MemoryBlockHeaderAtIndex(OldHeaders, c));
                    List_Delete(OldHeaders);
                    for(c = 0; c < OldBlockHeaders->Count; c++)
                        MemoryBlockSetHeader_Delete(List_MemoryBlockSetHeaderAtIndex(OldBlockHeaders, c));
                    List_Delete(OldBlockHeaders);
                    for(c = 0; c < OldRegisters->Count; c++)
                        free(List_StringAtIndex(OldRegisters, c));
                    List_Delete(OldRegisters);
                }
            } break;

            case tLongJumpOffset:
            {
                state->_Offset++;
                char* val = CurrentTok(state)->Val;
                state->_Offset++;
                int newOffset = -1;
                char* rVal = GetActualVal(state, val);
                if(CanConvertToInt(rVal) == 0)
                {
                    printf("ERROR\n");
                }
                newOffset = atoi(rVal);
                state->_Offset = newOffset;
            } break;

            case tLongJump:
            {
                state->_Offset++;
                char* labelTitle = CurrentTok(state)->Val;
                state->_Offset++;

                LabelDef* l = NULL;
                int c = 0;
                for(c = 0; c < state->_Labels->Count; c++)
                {
                    LabelDef* d = List_LabelDefAtIndex(state->_Labels, c);
                    if(strcmp(d->Label, labelTitle) == 0)
                        l = d;
                }
                if(l == NULL)
                {
                    printf("ERROR\n");
                }
                if(l->UserFunction != NULL)
                    l->UserFunction(state);
                else
                    state->_Offset = l->Offset;
            } break;

            case tCmpI:
            {
                char* v = State_PopString(state);
                if(CanConvertToBool(v) == 0)
                {
                    printf("ERROR\n");
                }
                short b = (short)atoi(v);
                State_PushBool(state, !b);
                free(v);
            } break;

            case tCmpA: case tCmpE: case tCmpG: case tCmpGE: case tCmpL: case tCmpLE: case tCmpN: case tCmpO:
            {
                char* v2 = State_PopString(state);
                char* v1 = State_PopString(state);

                switch(CurrentTok(state)->Tok)
                {
                    case tCmpA: case tCmpO:
                    {
                        short b1 = 0;
                        short b2 = 0;

                        if(CanConvertToBool(v1) == 0 || CanConvertToBool(v2) == 0)
                        {
                            printf("ERROR\n");
                        }
                        b1 = (short)atoi(v1);
                        b2 = (short)atoi(v2);
                        if(CurrentTok(state)->Tok == tCmpA)
                            State_PushBool(state, b1 && b2);
                        else
                            State_PushBool(state, b1 || b2);
                    } break;
                    case tCmpE : case tCmpN:
                    {
                        if(CurrentTok(state)->Tok == tCmpE)
                            State_PushBool(state, strcmp(v1, v2) == 0);
                        else
                            State_PushBool(state, strcmp(v1, v2) != 0);
                    }
                    default:
                    {
                        double d1 = 0;
                        double d2 = 0;
                        if(CanConvertToDouble(v1) == 0 || CanConvertToDouble(v2) == 0)
                        {
                            printf("ERROR\n");
                        }
                        d1 = (double)atof(v1);
                        d2 = (double)atof(v2);

                        switch(CurrentTok(state)->Tok)
                        {
                            case tCmpG: State_PushBool(state, d1 > d2); break;
                            case tCmpGE: State_PushBool(state, d1 >= d2); break;
                            case tCmpL: State_PushBool(state, d1 < d2); break;
                            case tCmpLE: State_PushBool(state, d1 <= d2); break;
                        }
                    } break;
                }
                free(v2);
                free(v1);
                state->_Offset++;
            } break;

            case tJumpE: case tJumpN: case tJumpG: case tJumpGE: case tJumpL: case tJumpLE:
            {
                short cmpTok = CurrentTok(state)->Tok;
                state->_Offset++;
                AssemblyToken* p1 = List_AssemblyTokenAtIndex(state->_Tokens, state->_Offset);
                state->_Offset++;
                AssemblyToken* c1 = List_AssemblyTokenAtIndex(state->_Tokens, state->_Offset);
                state->_Offset++;
                AssemblyToken* p2 = List_AssemblyTokenAtIndex(state->_Tokens, state->_Offset);
                state->_Offset++;
                AssemblyToken* c2 = List_AssemblyTokenAtIndex(state->_Tokens, state->_Offset);
                state->_Offset++;
                AssemblyToken* p3 = List_AssemblyTokenAtIndex(state->_Tokens, state->_Offset);

                if(c1->Tok != tComma || c2->Tok != tComma)
                {
                    printf("ERROR\n");
                }

                if(pt8(state, p1->Val) != 1 || pt8(state, p2->Val) != 1 || pt3(state, p3->Val) != 1)
                {
                    printf("ERROR\n");
                }

                char* val1 = GetActualVal(state, p1->Val);
                char* val2 = GetActualVal(state, p2->Val);
                int newOffset = -1;

                int c = 0;
                for(c = 0; c < state->_Labels->Count; c++)
                {
                    LabelDef* l = List_LabelDefAtIndex(state->_Labels, c);
                    if(strcmp(l->Label, p3->Val) == 0)
                        newOffset = l->Offset;
                }

                if(newOffset == -1)
                {
                    printf("ERROR\n");
                }

                short didEval = 0;
                short validDouble = 0;
                double d1 = 0;
                double d2 = 0;

                if(CanConvertToDouble(val1) == 1 && CanConvertToDouble(val2) == 1)
                {
                    d1 = (double)atof(val1);
                    d2 = (double)atof(val2);
                    validDouble = 1;
                }

                //printf("****DBG val1='%s' val2='%s' d1=%f d2=%f\n", val1, val2, d1, d2);

                switch(cmpTok)
                {
                    case tJumpE: if(strcmp(val1, val2) == 0) didEval = 1; break;
                    case tJumpN: if(strcmp(val1, val2) != 0) didEval = 1; break;
                    case tJumpG: if(validDouble == 1 && d1 > d2) didEval = 1; break;
                    case tJumpGE: if(validDouble == 1 && d1 >= d2) didEval = 1; break;
                    case tJumpL: if(validDouble == 1 && d1 < d2) didEval = 1; break;
                    case tJumpLE: if(validDouble == 1 && d1 <= d2) didEval = 1; break;
                }

                if(didEval == 1)
                    state->_Offset = newOffset;
                else
                    state->_Offset++;
            } break;

            case tAlloc:
            {
                state->_Offset++;
                AllocateMemoryBlock(state, CurrentTok(state)->Val);
                state->_Offset++;
            } break;

            case tAllocBlockSet:
            {
                state->_Offset++;
                char* BlockName = (List_AssemblyTokenAtIndex(state->_Tokens, state->_Offset))->Val;
                state->_Offset++;
                state->_Offset++;
                int Count = atoi(CurrentTok(state)->Val);
                AllocateMemoryBlockSet(state, BlockName, Count);
                state->_Offset++;
            } break;

            case tRef:
            {
                state->_Offset++;
                AssemblyToken* src = CurrentTok(state);
                state->_Offset += 2;
                AssemblyToken* dest = CurrentTok(state);
                ReferenceMemoryBlock(state, src->Val, dest->Val);
                state->_Offset++;
            } break;

            case tRefBlockSet:
            {
                state->_Offset++;
                AssemblyToken* src = CurrentTok(state);
                state->_Offset += 2;
                AssemblyToken* dest = CurrentTok(state);
                ReferenceMemoryBlockSet(state, src->Val, dest->Val);
                state->_Offset++;
            } break;

            case tDealloc:
            {
                state->_Offset++;
                FreeMemoryBlock(state, CurrentTok(state)->Val);
                state->_Offset++;
            } break;

            case tDeallocBlockSet:
            {
                state->_Offset++;
                FreeMemoryBlockSet(state, CurrentTok(state)->Val);
                state->_Offset++;
            } break;

            case tEndOfExec:
            {
                state->_Offset = -1;
            } break;
        }
    }

    if(state->_Offset > -1)
        return 1;
    return 0;
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
    for(i = 0; i < state->_Labels->Count && state->_Offset == -1; i++)
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
    if(ShouldMakeDoubletToInt(v) == 1)
        State_PushInt(S, (int)v);
    else
    {
        char* p = (char*)malloc(sizeof(char) * 3000);
        sprintf(p, "%f", v);
        State_PushString(S, p);
        free(p);
    }
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
