#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libCpScript.Asm.h>
#include <CpStdLib.h>

int main(int argc, char* argv[])
{
	printf("Reading Self...\n");
	FILE* f = fopen(argv[0], "rb");
	fseek(f, sizeof(long), SEEK_END);
	long RealSize = 0;
	fread(&RealSize, sizeof(long), 1, f);
	printf("Real Size: %ld\n", RealSize);
	fseek(f, RealSize + sizeof(long), SEEK_END);
	void* b = malloc(RealSize);
	fread(b, 1, RealSize, f);
	fclose(f);
	
	printf("Generating State... (%ld)\n", RealSize);
	void* State = State_NewFromCompiled(b, RealSize);
	printf("Loading Std Lib...\n");
	CpStdLib_InstallConsoleIO(State);
	CpStdLib_InstallFileIO(State);
	CpStdLib_InstallMath(State);
	CpStdLib_InstallUtilities(State);
	printf("Running Script...\n");
	State_RunFromMethod(State, "Main");
	State_Delete(State);
	
	return 0;
};