#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libCpScript.Asm.h>
#include <libCpScript.ObjectBasic.h>

void ShowHelp()
{
	printf("\n");
	printf("MakeExec -t[asm|obj] [SrcScript] [OutExec]\n\n");
	printf("Examples:\n");
	printf("    MakeExe -tasm MyScript.asm MyScriptExe.exe\n");
	printf("    MakeExe -tobj MyScript.cps MyObjScriptExe.exe\n");
	printf("\n\n");
};

int main(int argc, char* argv[])
{
	if(argc != 4)
		ShowHelp();
	else
	{
		short AsmScript = 0;
		
		if(strcmp(argv[1], "-tasm") == 0)
			AsmScript = 1;
		else if(strcmp(argv[1], "-tobj") == 0)
			AsmScript = 0;
		else
		{
			ShowHelp();
			exit(0);
		}
	
		char* SrcScript = argv[2];
		char* DestExec = argv[3];
		
		void* binData = NULL;
		long binSize = 0;
		
		FILE* srcFile = fopen(SrcScript, "rb");
		fseek(srcFile, 0, SEEK_END);
		long srcSize = ftell(srcFile);
		fseek(srcFile, 0, SEEK_SET);
		char* srcText = (char*)malloc(sizeof(char) * (srcSize + 1));
		fread(srcText, sizeof(char), srcSize, srcFile);
		srcText[srcSize] = '\0';
		fclose(srcFile);
		
		if(AsmScript == 0)
		{
			void* Script = ObjScript_New();	
			ObjScript_Load(Script, srcText);
			void* State = State_New(ObjScript_GetAsm(Script));
		
			printf("Compiling Script...\n");
		
			binData = State_Compile(State, &binSize);
			State_Delete(State);		
			ObjScript_Delete(Script);
		}
		else
		{
			void* State = State_New(srcText);
			
			printf("Compiling Script...\n");
			
			binData = State_Compile(State, &binSize);
			State_Delete(State);
		}
		
		free(srcText);
		
		printf("Generating Binary...\n");
		
		FILE* runtime = fopen("execRuntime.exe", "rb");
		if(!runtime)
			runtime = fopen("execRuntime", "rb");
		if(runtime)
		{
			fseek(runtime, 0, SEEK_END);
			long runtimeSize = ftell(runtime);
			fseek(runtime, 0, SEEK_SET);
			void* runtimeBin = malloc(runtimeSize);
			fread(runtimeBin, 1, runtimeSize, runtime);
			fclose(runtime);
			
			printf("Writing Binary...\n");
			
			FILE* exec = fopen(DestExec, "wb");
			fwrite(runtimeBin, 1, runtimeSize, exec);
			fwrite(binData, 1, binSize, exec);
			fwrite(&binSize, sizeof(long), 1, exec);
			fclose(exec);
			
			printf("Done!\n");
		}
	}
	
	return 0;
};