#include <libCpScript.Asm.h>
#include <CpStdLib.h>
#include <stdio.h>
#include <malloc.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
        return -1;
    FILE* f = fopen(argv[1], "rb");
    if(f == NULL)
    {
        return -1;
    }
    fseek(f, 0, SEEK_END);
    int size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* result = (char*)malloc(size + 1);
    fread(result, sizeof(char), size, f);
    fclose(f);
    result[size] = '\0';
    void* State = State_New(result);
    void* CompiledScript = malloc(0);
    long CompiledLen = 0;
    State_Compile(State, &CompiledScript, &CompiledLen);
    State_Delete(State);

    f = fopen("out.cmp", "wb");
    fwrite(CompiledScript, sizeof(char), CompiledLen, f);
    fclose(f);

    free(CompiledScript);
    CompiledLen = 0;

    f = fopen("out.cmp", "rb");
    fseek(f, 0, SEEK_END);
    CompiledLen = ftell(f);
    fseek(f, 0, SEEK_SET);
    CompiledScript = malloc(CompiledLen);
    fread(CompiledScript, sizeof(char), CompiledLen, f);
    fclose(f);

    State = State_NewFromCompiled(CompiledScript, CompiledLen);
    CpStdLib_InstallConsoleIO(State);
    CpStdLib_InstallFileIO(State);
    CpStdLib_InstallMath(State);
    CpStdLib_InstallUtilities(State);
    State_RunFromMethod(State, "Main");
    State_Delete(State);

/*
    State = State_New(result);
    CpStdLib_InstallConsoleIO(State);
    CpStdLib_InstallFileIO(State);
    CpStdLib_InstallMath(State);
    CpStdLib_InstallUtilities(State);
    State_PushString(State, "String 2");
    State_PushString(State, "String 1");
    State_RunFromMethod(State, "WriteStrings");
    State_Delete(State);
*/
    free(result);
	return 0;
};
