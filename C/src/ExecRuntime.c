#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libCpScript.Asm.h>
#include <CpStdLib.h>

int main(int argc, char* argv[])
{
	FILE* f = fopen(argv[0], "rb");
	fseek(f, sizeof(long) * -1, SEEK_END);
	long RealSize = 0;
	fread(&RealSize, sizeof(long), 1, f);
	fseek(f, (RealSize + sizeof(long)) * -1, SEEK_END);
	void* b = malloc(RealSize);
	fread(b, 1, RealSize, f);
	fclose(f);
	
	void* State = State_NewFromCompiled(b, RealSize);

	CpStdLib_InstallConsoleIO(State);
	CpStdLib_InstallFileIO(State);
	CpStdLib_InstallMath(State);
	CpStdLib_InstallUtilities(State);

	State_RunFromMethod(State, "Main");
	State_Delete(State);
	
	return 0;
};