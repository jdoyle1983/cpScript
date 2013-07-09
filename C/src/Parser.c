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

#include <Parser.h>
#include <Extensions.h>
#include <AssemblyToken.h>
#include <Token.h>
#include <string.h>
#include <malloc.h>

List* Parse(char* Input)
{
    List* Toks = List_New();

    short inQuotes = 0;
    char quoteChar = 0;
    char* quotedString = NULL;

    List* pToks = SplitAndKeep(Input, " ,\n\r\'\"");
    int a = 0;

    for(a = 0; a < pToks->Count; a++)
    {
        char* s = List_StringAtIndex(pToks, a);
        char* tVal = StrToLowTrim(s);

        if(inQuotes == 0)
        {
            if(strcmp(tVal, "") == 0 && Toks->Count > 0 && (List_AssemblyTokenAtIndex(Toks, Toks->Count - 1))->Tok == tLiteral && strcmp(s, "\n") != 0 && strcmp(s, "\r") != 0)
            {
                char* tStr = (List_AssemblyTokenAtIndex(Toks, Toks->Count - 1))->Val;
                char* NewStr = (char*)malloc(sizeof(char) * (strlen(tStr) + strlen(s) + 1));
                strcpy(NewStr, tStr);
                strcat(NewStr, s);
                free(tStr);
                (List_AssemblyTokenAtIndex(Toks, Toks->Count - 1))->Val = NewStr;
            }
            else if(strcmp(tVal, "") == 0)
            {

            }
            else if(strcmp(tVal, "'") == 0)
            {
                inQuotes = 1;
                quotedString = (char*)malloc(sizeof(char));
                quotedString[0] = '\0';
                quoteChar = '\'';
            }
            else if(strcmp(tVal, "\"") == 0)
            {
                inQuotes = 1;
                quotedString = (char*)malloc(sizeof(char));
                quotedString[0] = '\0';
                quoteChar = '"';
            }
            else
            {
                short tok = tLiteral;
                if(strcmp(tVal, "push") == 0) tok = tPush;
                else if(strcmp(tVal, "pop") == 0) tok = tPop;
                else if(strcmp(tVal, "mov") == 0) tok = tMove;
                else if(strcmp(tVal, "add") == 0) tok = tAdd;
                else if(strcmp(tVal, "mul") == 0) tok = tMul;
                else if(strcmp(tVal, "div") == 0) tok = tDiv;
                else if(strcmp(tVal, "sub") == 0) tok = tSub;
                else if(strcmp(tVal, "mod") == 0) tok = tMod;
                else if(strcmp(tVal, "cat") == 0) tok = tCat;
                else if(strcmp(tVal, "neg") == 0) tok = tNeg;
                else if(strcmp(tVal, "lbl") == 0) tok = tLabel;
                else if(strcmp(tVal, "jmp") == 0) tok = tJump;
                else if(strcmp(tVal, "ljmp") == 0) tok = tLongJump;
                else if(strcmp(tVal, "jmpo") == 0) tok = tJumpOffset;
                else if(strcmp(tVal, "ljmpo") == 0) tok = tLongJumpOffset;
                else if(strcmp(tVal, "rtn") == 0) tok = tReturn;
                else if(strcmp(tVal, "je") == 0) tok = tJumpE;
                else if(strcmp(tVal, "jn") == 0) tok = tJumpN;
                else if(strcmp(tVal, "jg") == 0) tok = tJumpG;
                else if(strcmp(tVal, "jge") == 0) tok = tJumpGE;
                else if(strcmp(tVal, "jl") == 0) tok = tJumpL;
                else if(strcmp(tVal, "jle") == 0) tok = tJumpLE;
                else if(strcmp(tVal, "ce") == 0) tok = tCmpE;
                else if(strcmp(tVal, "cn") == 0) tok = tCmpN;
                else if(strcmp(tVal, "cg") == 0) tok = tCmpG;
                else if(strcmp(tVal, "cge") == 0) tok = tCmpGE;
                else if(strcmp(tVal, "cl") == 0) tok = tCmpL;
                else if(strcmp(tVal, "cle") == 0) tok = tCmpLE;
                else if(strcmp(tVal, "co") == 0) tok = tCmpO;
                else if(strcmp(tVal, "ca") == 0) tok = tCmpA;
                else if(strcmp(tVal, "ci") == 0) tok = tCmpI;
                else if(strcmp(tVal, "ab") == 0) tok = tAlloc;
                else if(strcmp(tVal, "rb") == 0) tok = tRef;
                else if(strcmp(tVal, "fb") == 0) tok = tDealloc;
                else if(strcmp(tVal, "pusha") == 0) tok = tPushA;
                else if(strcmp(tVal, "popa") == 0) tok = tPopA;
                else if(strcmp(tVal, "abs") == 0) tok = tAllocBlockSet;
                else if(strcmp(tVal, "rbs") == 0) tok = tRefBlockSet;
				else if(strcmp(tVal, "rebs") == 0) tok = tResBlockSet;
                else if(strcmp(tVal, "fbs") == 0) tok = tDeallocBlockSet;
                else if(strcmp(tVal, "pushb") == 0) tok = tPushB;
                else if(strcmp(tVal, "popb") == 0) tok = tPopB;
                else if(strcmp(tVal, ",") == 0) tok = tComma;
                else if(strcmp(tVal, "eoe") == 0) tok = tEndOfExec;
                else if(strcmp(tVal, "lib") == 0) tok = tLib;
                else
                {
                    if(tVal[0] == '@')
                        tok = tRegister;
                    else if(tVal[0] == '%')
                        tok = tMemoryVar;
                    else
                        tok = tLiteral;
                }
                List_Add(Toks, AssemblyToken_New(tok, s));
            }
        }
        else
        {
            if(strlen(tVal) == 1 && tVal[0] == quoteChar)
            {
                inQuotes = 0;
                quoteChar = 0;
                List_Add(Toks, AssemblyToken_New(tQuotedLiteral, quotedString));
                free(quotedString);
            }
            else
            {
                char* NewString = (char*)malloc(sizeof(char) * (strlen(quotedString) + strlen(s) + 1));
                strcpy(NewString, quotedString);
                strcat(NewString, s);
                free(quotedString);
                quotedString = NewString;
            }
        }

        free(tVal);
        free(s);
    }
    List_Delete(pToks);
    return Toks;
};
