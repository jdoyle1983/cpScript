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

#include <stdio.h>
#include <stdlib.h>
#include <Token.h>
#include <string.h>
#include <malloc.h>
#include <AssemblyToken.h>

AssemblyToken* AssemblyToken_New(short Tok, const char* Val)
{
	AssemblyToken* t = (AssemblyToken*)malloc(sizeof(AssemblyToken));
	short shouldAllocString = 1;

	t->Tok = Tok;
	t->Val = NULL;

	if(Tok == tRegister || Tok == tMemoryVar || Tok == tLiteral || Tok == tQuotedLiteral)
        shouldAllocString = 1;
	if(Val != NULL && shouldAllocString == 1)
	{
		t->Val = (char*)malloc(sizeof(char*) * (strlen(Val) + 1));
		strcpy(t->Val, Val);
	}
	return t;
};

void AssemblyToken_Delete(AssemblyToken* Tok)
{
	if(Tok->Val != NULL)
		free(Tok->Val);
	free(Tok);
};

void AssemblyToken_SetValue(AssemblyToken* Tok, const char* NewVal)
{
	if(Tok->Val != NULL)
		free(Tok->Val);
	Tok->Val = NULL;
	if(NewVal != NULL)
	{
		Tok->Val = (char*)malloc(sizeof(char) * (strlen(NewVal) + 1));
		strcpy(Tok->Val, NewVal);
	}
};
