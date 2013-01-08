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

#ifndef __MEMORYBLOCKSETHEADER_H__
#define __MEMORYBLOCKSETHEADER_H__

#include <List.h>

typedef struct
{
    char* Name;
    List* IndexOffset;
} MemoryBlockSetHeader;

MemoryBlockSetHeader* MemoryBlockSetHeader_New(char* Name, int Count);
void MemoryBlockSetHeader_SetOffset(MemoryBlockSetHeader* Header, int Idx, int Offset);
int MemoryBlockSetHeader_GetOffset(MemoryBlockSetHeader* Header, int Idx);
void MemoryBlockSetHeader_Delete(MemoryBlockSetHeader* Header);

#endif
