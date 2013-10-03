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

#include <CpStdLib_ConsoleIO.h>
#include <libCpScript.Asm.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

//This block was found on the net somewhere, author and credit to be added when I figure out where....
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
    if(linep[strlen(linep) - 1] == '\n')
        linep[strlen(linep) - 1] = '\0';
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
