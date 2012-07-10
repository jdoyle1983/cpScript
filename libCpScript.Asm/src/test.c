#include <libCpScript.Asm.h>
#include <stdio.h>
#include <malloc.h>

void ConsoleWriteLine(void* State)
{
    char* toPrint = State_PopString(State);
    printf("%s\n", toPrint);
    free(toPrint);
};

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
    State_RegisterFunction(State, "WriteLine", &ConsoleWriteLine, 0);
    State_RunFromMethod(State, "Main");
    State_Delete(State);

    State = State_New(result);
    State_RegisterFunction(State, "WriteLine", &ConsoleWriteLine, 0);
    State_PushString(State, "String 2");
    State_PushString(State, "String 1");
    State_RunFromMethod(State, "WriteStrings");
    State_Delete(State);

    free(result);
	return 0;
};
