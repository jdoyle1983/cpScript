#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libCpScript.ObjectBasic.h>

#define VERSION "1.0.0"

void DisplayVersion()
{
	printf("Object Basic Script Compiler\n");
	printf("v%s\n\n", VERSION);
};

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		DisplayVersion();
		
		printf("No Input File(s) / Output File Specified\n");
		printf("cpObjCmp [input] [input] -o [output]\n");
		printf("If no -o is specified, it will write the\n");
		printf("resulting script to standard output.\n\n");
	}
	else
	{
		char** inputFiles = (char**)malloc(sizeof(char*) * argc - 1);
		char* outputFile = NULL;
		int i = 0;
		int inCount = 0;
		for(i = 1; i < argc; i++)
		{
			if(strcmp(argv[i], "-o") == 0)
			{
				i++;
				outputFile = argv[i];
			}
			else
			{
				inputFiles[inCount] = argv[i];
				inCount++;
			}
		}
		
		inputFiles = (char**)realloc(inputFiles, sizeof(char*) * inCount);
		
		char* fullScript = (char*)malloc(sizeof(char) * 1);
		fullScript[0] = '\0';
		
		for(i = 0; i < inCount; i++)
		{
			char* thisBuf = 0;
			long thisLength = 0;
			FILE* thisFile = fopen(inputFiles[i], "rb");
			if(thisFile)
			{
				fseek(thisFile, 0, SEEK_END);
				thisLength = ftell(thisFile);
				fseek(thisFile, 0, SEEK_SET);
				thisBuf = (char*)malloc(sizeof(char) * (thisLength + 1));
				if(thisBuf)
				{
					fread(thisBuf, 1, thisLength, thisFile);
					fclose(thisFile);
					thisBuf[thisLength] = '\0';
					
					fullScript = (char*)realloc(fullScript, sizeof(char) * (strlen(fullScript) + strlen(thisBuf) + 2));
					strcat(fullScript, "\n");
					strcat(fullScript, thisBuf);
					
					free(thisBuf);
				}
				else
				{
					printf("Failed to alloc buffer for file '%s'\n", inputFiles[i]);
					fclose(thisFile);
					exit(0);
				}
			}
			else
			{
				printf("Error Reading File '%s'\n", inputFiles[i]);
				exit(0);
			}
		}
		
		void* objScript = ObjScript_New();
		ObjScript_Load(objScript, fullScript);
		char* AsmScript = ObjScript_GetAsm(objScript);
		if(outputFile != NULL)
		{
			FILE* outFile = fopen(outputFile, "wb");
			if(!outFile)
				printf("Error Writing File '%s'\n", outputFile);
			else
			{
				fwrite(AsmScript, 1, strlen(AsmScript), outFile);
				fclose(outFile);
			}
		}
		else
			printf("%s\n", AsmScript);
		
		ObjScript_Delete(objScript);
		free(fullScript);
		free(inputFiles);
	}
	
	return 0;
};