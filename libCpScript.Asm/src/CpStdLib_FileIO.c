#include <CpStdLib_FileIO.h>
#include <libCpScript.Asm.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

char * Internal_FileGetLine(FILE* f)
{
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(f);
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
    if(linep[strlen(linep) - 1] == '\n')
        linep[strlen(linep) - 1] = '\0';
    return linep;
};

void File_Open(void* State)
{
    //printf("File_Open 1\n");
    char* filePath = State_PopString(State);
    //printf("File_Open 2 (Path: '%s'\n", filePath);
    char* readMode = State_PopString(State);

    //printf("File_Open 3 (Mode: '%s')\n", readMode);
    FILE* f = fopen(filePath, readMode);
    //printf("File_Open 4\n");
    State_PushInt(State, (int)f);
    //printf("File_Open 5 (%d)\n", (int)f);
    free(filePath);
    //printf("File_Open 6\n");
    free(readMode);
    //printf("File_Open 7\n");
};

void File_WriteLine(void* State)
{
    //printf("File_WriteLine\n");
    char* toWrite = State_PopString(State);
    FILE* f = (FILE*)State_PopInt(State);
    fprintf(f, "%s\n", toWrite);
    free(toWrite);
};

void File_ReadLine(void* State)
{
    //printf("File_ReadLine\n");
    FILE* f = (FILE*)State_PopInt(State);
    char* toRead = Internal_FileGetLine(f);
    if(toRead == NULL)
        State_PushString(State, "");
    else
    {
        State_PushString(State, toRead);
        free(toRead);
    }
};

void File_Close(void* State)
{
    //printf("File_Close\n");
    FILE* f = (FILE*)State_PopInt(State);
    fclose(f);
};

void File_Eof(void* State)
{
    //printf("File_Eof\n");
    FILE* f = (FILE*)State_PopInt(State);
    if(feof(f))
        State_PushBool(State, 1);
    else
        State_PushBool(State, 0);
};

void File_ReadBool(void* State)
{
    //printf("File_ReadBool\n");
    FILE* f = (FILE*)State_PopInt(State);
    short b = -1;
    fread(&b, sizeof(short), 1, f);
    State_PushBool(State, b);
};

void File_ReadDouble(void* State)
{
    //printf("File_ReadDouble\n");
    FILE* f = (FILE*)State_PopInt(State);
    double d = -1;
    fread(&d, sizeof(double), 1, f);
    State_PushDouble(State, d);
};

void File_ReadInt(void* State)
{
    //printf("File_ReadInt\n");
    FILE* f = (FILE*)State_PopInt(State);
    int i = -1;
    fread(&i, sizeof(int), 1, f);
    State_PushInt(State, i);
};

void File_ReadString(void* State)
{
    //printf("File_ReadString\n");
    FILE* f = (FILE*)State_PopInt(State);
    int strSize = 0;
    fread(&strSize, sizeof(int), 1, f);
    char* strVal = (char*)malloc(sizeof(char) * (strSize + 1));
    fread(strVal, sizeof(char), strSize, f);
    strVal[strSize] = '\0';
    State_PushString(State, strVal);
    free(strVal);
};

void File_WriteBool(void* State)
{
    //printf("File_WriteBool\n");
    short toWrite = State_PopBool(State);
    FILE* f = (FILE*)State_PopInt(State);
    fwrite(&toWrite, sizeof(short), 1, f);
};

void File_WriteDouble(void* State)
{
    //printf("File_WriteDouble\n");
    double toWrite = State_PopDouble(State);
    FILE* f = (FILE*)State_PopInt(State);
    fwrite(&toWrite, sizeof(double), 1, f);
};

void File_WriteInt(void* State)
{
    //printf("File_WriteInt\n");
    int toWrite = State_PopInt(State);
    FILE* f = (FILE*)State_PopInt(State);
    fwrite(&toWrite, sizeof(int), 1, f);
};

void File_WriteString(void* State)
{
    //printf("File_WriteString\n");
    char* toWrite = State_PopString(State);
    FILE* f = (FILE*)State_PopInt(State);
    int strSize = strlen(toWrite);
    fwrite(&strSize, sizeof(int), 1, f);
    fwrite(toWrite, sizeof(char), strSize, f);
    free(toWrite);
};
