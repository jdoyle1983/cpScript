#include <Extensions.h>
#include <AssemblyToken.h>
#include <Parser.h>
#include <stdio.h>
#include <malloc.h>

int main(int argc, char* argv[])
{
	const char* SrcStr = "tok1=tok2,tok3*tok4/tok5 tok6";
	const char* Delim = "=,*/ ";

	List* toks = SplitAndKeep((char*)SrcStr, (char*)Delim);
	int i = 0;
	for(i = 0; i < toks->Count; i++)
		printf("Tok: '%s'\n", List_StringAtIndex(toks, i));

	for(i = 0; i < toks->Count; i++)
		free(toks->Items[i]);
	List_Delete(toks);

	char* Str = "   SDFD   ";
	char* tStr = StrToLowTrim(Str);
	printf("Pre Trim: '%s'\n", Str);
	printf("Trimmed: '%s'\n", tStr);

	free(tStr);

	char* TestScript = "LBL Main\nPUSH \" \"\nJMP _Console_WriteLine\nPUSH \"Enter Fibonacci Sequence Iteration Count: \"\nJMP _Console_Write\nAB %fibMax\nJMP _Console_ReadLine\nPOP %fibMax\nAB %f1\nPUSH 0\nPOP %f1\nAB %f2\nPUSH 0\nPOP %f2\nAB %f3\nPUSH 0\nPOP %f3\nAB %i\nPUSH 0\nPOP %i\nMOV %i, 1\nLBL _ForStart1\nPUSH %i\nPUSH 1\nCE\nPOP @1\nJN @1, True, _Skip3\nPUSH 1\nPOP %f1\nPUSH \"Seq \"\nPUSH %i\nCAT\nPUSH \": \"\nCAT\nPUSH %f1\nCAT\nJMP _Console_WriteLine\nLJMP _EndIf2\nLBL _Skip3\nPUSH %i\nPUSH 2\nCE\nPOP @1\nJN @1, True, _Skip4\nPUSH 1\nPOP %f2\nPUSH \"Seq \"\nPUSH %i\nCAT\nPUSH \": \"\nCAT\nPUSH %f2\nCAT\nJMP _Console_WriteLine\nLJMP _EndIf2\nLBL _Skip4\nPUSH %f1\nPUSH %f2\nADD\nPOP %f3\nPUSH \"Seq \"\nPUSH %i\nCAT\nPUSH \": \"\nCAT\nPUSH %f3\nCAT\nJMP _Console_WriteLine\nPUSH %f2\nPOP %f1\nPUSH %f3\nPOP %f2\nLBL _EndIf2\nPUSH 1\nPUSH %i\nADD\nPOP %i\nJLE %i, %fibMax, _ForStart1\nRTN";
    List* Tokens = Parse(TestScript);
    for(i = 0; i < Tokens->Count; i++)
    {
        AssemblyToken* tok = List_AssemblyTokenAtIndex(Tokens, i);
        printf("Tok: '%s'\n", tok->Val);
        AssemblyToken_Delete(tok);
    }
    List_Delete(Tokens);

	return 0;
};
