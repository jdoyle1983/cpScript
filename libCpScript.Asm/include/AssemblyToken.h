#ifndef __ASSEMBLYTOKEN_H__
#define __ASSEMBLYTOKEN_H__

typedef struct
{
	short Tok;
	char* Val;
} AssemblyToken;

AssemblyToken* AssemblyToken_New(short Tok, const char* Val);
void AssemblyToken_Delete(AssemblyToken* Tok);
void AssemblyToken_SetValue(AssemblyToken* Tok, const char* NewVal);

#endif
