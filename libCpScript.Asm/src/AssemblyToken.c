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
