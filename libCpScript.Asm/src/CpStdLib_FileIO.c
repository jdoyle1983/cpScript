/*

    libCpStdLib
    Copyright (C) 2013 Jason Doyle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact Information:

    Original Author: Jason Doyle (jdoyle1983@gmail.com)
*/

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
    char* filePath = State_PopString(State);
    char* readMode = State_PopString(State);
    FILE* f = fopen(filePath, readMode);
    State_PushInt(State, (int)f);
    free(filePath);
    free(readMode);
};

void File_WriteLine(void* State)
{
    char* toWrite = State_PopString(State);
    FILE* f = (FILE*)State_PopInt(State);
    fprintf(f, "%s\n", toWrite);
    free(toWrite);
};

void File_ReadLine(void* State)
{
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
    FILE* f = (FILE*)State_PopInt(State);
    fclose(f);
};

void File_Eof(void* State)
{
    FILE* f = (FILE*)State_PopInt(State);
    if(feof(f))
        State_PushBool(State, 1);
    else
        State_PushBool(State, 0);
};

void File_ReadBool(void* State)
{
    FILE* f = (FILE*)State_PopInt(State);
    short b = -1;
    fread(&b, sizeof(short), 1, f);
    State_PushBool(State, b);
};

void File_ReadDouble(void* State)
{
    FILE* f = (FILE*)State_PopInt(State);
    double d = -1;
    fread(&d, sizeof(double), 1, f);
    State_PushDouble(State, d);
};

void File_ReadInt(void* State)
{
    FILE* f = (FILE*)State_PopInt(State);
    int i = -1;
    fread(&i, sizeof(int), 1, f);
    State_PushInt(State, i);
};

void File_ReadString(void* State)
{
	char* strVal = NULL;
    FILE* f = (FILE*)State_PopInt(State);
    int strSize = 0;
    fread(&strSize, sizeof(int), 1, f);
    strVal = (char*)malloc(sizeof(char) * (strSize + 1));
    fread(strVal, sizeof(char), strSize, f);
    strVal[strSize] = '\0';
    State_PushString(State, strVal);
    free(strVal);
};

void File_WriteBool(void* State)
{
    short toWrite = State_PopBool(State);
    FILE* f = (FILE*)State_PopInt(State);
    fwrite(&toWrite, sizeof(short), 1, f);
};

void File_WriteDouble(void* State)
{
    double toWrite = State_PopDouble(State);
    FILE* f = (FILE*)State_PopInt(State);
    fwrite(&toWrite, sizeof(double), 1, f);
};

void File_WriteInt(void* State)
{
    int toWrite = State_PopInt(State);
    FILE* f = (FILE*)State_PopInt(State);
    fwrite(&toWrite, sizeof(int), 1, f);
};

void File_WriteString(void* State)
{
    char* toWrite = State_PopString(State);
    FILE* f = (FILE*)State_PopInt(State);
    int strSize = strlen(toWrite);
    fwrite(&strSize, sizeof(int), 1, f);
    fwrite(toWrite, sizeof(char), strSize, f);
    free(toWrite);
};
