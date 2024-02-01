/*

    libCpScript.Asm,
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

#include <MemoryBlockSetHeader.h>
#include <string.h>
#include <malloc.h>

MemoryBlockSetHeader* MemoryBlockSetHeader_New(char* Name, int Count)
{
	int i = 0;

    MemoryBlockSetHeader* header = (MemoryBlockSetHeader*)malloc(sizeof(MemoryBlockSetHeader));
    header->IndexOffset = List_New();
    header->Name = NULL;
    if(Name != NULL)
    {
        header->Name = (char*)malloc(sizeof(char) * (strlen(Name) + 1));
        strcpy(header->Name, Name);
    }

    for(i = 0; i < Count; i++)
        List_AddInt(header->IndexOffset, -1);
    return header;
};

void MemoryBlockSetHeader_SetOffset(MemoryBlockSetHeader* Header, int Idx, int Offset)
{
    *((int*)Header->IndexOffset->Items[Idx]) = Offset;
};

int MemoryBlockSetHeader_GetOffset(MemoryBlockSetHeader* Header, int Idx)
{
    return *((int*)Header->IndexOffset->Items[Idx]);
};

void MemoryBlockSetHeader_Delete(MemoryBlockSetHeader* Header)
{
	int i = 0;
	for(i = 0; i < Header->IndexOffset->Count; i++)
		free(Header->IndexOffset->Items[i]);
    List_Delete(Header->IndexOffset);
    if(Header->Name != NULL)
        free(Header->Name);
    free(Header);
};
