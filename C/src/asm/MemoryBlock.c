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

#include <MemoryBlock.h>
#include <string.h>
#include <malloc.h>

MemoryBlock* MemoryBlock_New()
{
    MemoryBlock* b = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    b->Used = 0;
    b->Value = NULL;
    return b;
};

void MemoryBlock_Delete(MemoryBlock* block)
{
    if(block->Value != NULL)
        free(block->Value);
    free(block);
};

void MemoryBlock_SetValue(MemoryBlock* block, char* Val)
{
    if(block->Value != NULL)
        free(block->Value);
    block->Used = 1;
    block->Value = (char*)malloc(sizeof(char) * (strlen(Val) + 1));
    strcpy(block->Value, Val);
};

void MemoryBlock_Free(MemoryBlock* block)
{
    if(block->Value != NULL)
        free(block->Value);
    block->Used = 0;
};
