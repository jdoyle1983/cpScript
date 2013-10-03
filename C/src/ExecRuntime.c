#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <libCpScript.Asm.h>
#include <CpStdLib.h>

/*
	Compiled script is laid out like this:
	| A | B | C |
	
	A: This compiled exe
	B: Compiled assembly script
	C: Size of compiled assembly script
	
	1) Reads script size from end of file.
	2) Reads script from end of file.
	3) Executes script from Main function.
*/

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