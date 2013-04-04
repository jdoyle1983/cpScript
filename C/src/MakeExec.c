#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libCpScript.Asm.h>
#include <libCpScript.ObjectBasic.h>

void ShowHelp()
{
	printf("MakeExec [SrcScript] [OutExec]\n");
};

int main(int argc, char* argv[])
{
	if(argc != 3)
		ShowHelp();
	else
	{
		char* SrcScript = argv[1];
		char* DestExec = argv[2];
		
		void* Script = ObjScript_New();
		FILE* srcFile = fopen(SrcScript, "rb");
		fseek(srcFile, 0, SEEK_END);
		long srcSize = ftell(srcFile);
		fseek(srcFile, 0, SEEK_SET);
		char* srcText = (char*)malloc(sizeof(char) * (srcSize + 1));
		fread(srcText, sizeof(char), srcSize, srcFile);
		srcText[srcSize] = '\0';
		fclose(srcFile);
		
		ObjScript_Load(Script, srcText);
		void* State = State_New(ObjScript_GetAsm(Script));
		void* binData = NULL;
		long binSize = 0;
		
		printf("Compiling Script...\n");
		
		binData = State_Compile(State, &binSize);
		State_Delete(State);		
		ObjScript_Delete(Script);
		free(srcText);
		
		printf("Generating Binary...\n");
		
		FILE* runtime = fopen("ExecRuntime.exe", "rb");
		if(!runtime)
			runtime = fopen("ExecRuntime", "rb");
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