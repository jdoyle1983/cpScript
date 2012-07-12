#include <CpStdLib_ConsoleIO.h>
#include <libCpScript.Asm.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

char * Internal_GetLine()
{
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            char * linen = realloc(linep, lenmax *= 2);
            len = lenmax;

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
};

void Console_Write(void* State)
{
    char* toPrint = State_PopString(State);
    printf("%s", toPrint);
    free(toPrint);
};

void Console_WriteLine(void* State)
{
    Console_Write(State);
    printf("\n");
};

void Console_ReadLine(void* State)
{
    char* v = Internal_GetLine();
    if(v == NULL)
        State_PushString(State, "");
    else
    {
        State_PushString(State, v);
        free(v);
    }
};
