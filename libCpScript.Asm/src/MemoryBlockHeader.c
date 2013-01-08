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

#include <MemoryBlockHeader.h>
#include <string.h>
#include <malloc.h>

MemoryBlockHeader* MemoryBlockHeader_New(char* Name, int Offset)
{
    MemoryBlockHeader* header = (MemoryBlockHeader*)malloc(sizeof(MemoryBlockHeader));
    header->Name = NULL;
    header->Offset = Offset;
    if(Name != NULL)
    {
        header->Name = (char*)malloc(sizeof(char) * (strlen(Name) + 1));
        strcpy(header->Name, Name);
    }
    return header;
};

void MemoryBlockHeader_Delete(MemoryBlockHeader* Header)
{
    if(Header->Name != NULL)
        free(Header->Name);
    free(Header);
};
