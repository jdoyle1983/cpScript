#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libCpScript.Asm.h>
#include <libCpScript.ObjectBasic.h>
#include <CpStdLib.h>
#include <Extensions.h>

//meh, does the trick, probably a better way...
char* DoAppend(char* src, char* toappend)
{
	char* r = (char*)malloc(sizeof(char) * (strlen(src) + strlen(toappend) + 2));
	strcpy(r, src);
	strcat(r, "\n");
	strcat(r, toappend);
	free(src);
	return r;
}

char* AddStdHeader(char* Src)
{
	Src = DoAppend(Src, "#include Lib/ConsoleIO.obh");
	Src = DoAppend(Src, "#include Lib/FileIO.obh");
	Src = DoAppend(Src, "#include Lib/Math.obh");
	Src = DoAppend(Src, "#include Lib/Utilities.obh");
	return Src;
}

int main(int argc, char* argv[])
{
	printf("Enter Script Below, Must Include Function Main().\n");
	printf("Type .complete on its own line to execute the script.\n");
	printf("Type .exit on its own line to quit.\n");
	
	char* srcText = (char*)malloc(sizeof(char) * 1);
	srcText[0] = '\0';
	srcText = AddStdHeader(srcText);
	short ExitFlag = 0;
	while(ExitFlag == 0)
	{
		char* tmpIn = (char*)malloc(sizeof(char) * 5000);
		tmpIn = fgets(tmpIn, 5000, stdin);
		char* lowTrim = StrToLowTrim(tmpIn);
		if(strcmp(lowTrim, ".complete") == 0)  //User is ready to execute script, compile
		{
			void* Script = ObjScript_New();
			ObjScript_Load(Script, srcText);
			char* AsmScript = ObjScript_GetAsm(Script);
			void* State = State_New(AsmScript);
			
			CpStdLib_InstallConsoleIO(State);
			CpStdLib_InstallFileIO(State);
			CpStdLib_InstallMath(State);
			CpStdLib_InstallUtilities(State);
			
			State_RunFromMethod(State, "Main");
			
			State_Delete(State);
			ObjScript_Delete(Script);
			free(srcText);
			srcText = (char*)malloc(sizeof(char) * 1);
			srcText = AddStdHeader(srcText);
		}
		else if(strcmp(lowTrim, ".exit") == 0) //User wants to close the program
		{
			ExitFlag = 1;
		}
		else //Add to the script
		{
			srcText = DoAppend(srcText, tmpIn);
		}
		free(tmpIn);
		free(lowTrim);
	}
	
	free(srcText);
	
	return 0;
};