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

#ifndef __LIST_H__
#define __LIST_H__

/*
	Items stored in list must be allocated / cleaned up outside of the list
*/

#include <stddef.h>

typedef struct
{
	size_t Count;
	void** Items;
	size_t ActualCount;
} List;

List* List_New(void);
void List_Delete(List* list);

size_t List_Count(List* list);

void List_Add(List* list, void* item);
void List_AddInFront(List* list, void* item);
void* List_AtIndex(List* list, size_t idx);

void List_Remove(List* list, void* item);
void List_RemoveAt(List* list, size_t idx);

void List_Reverse(List* list);

void List_Clear(List* list);

int List_IntAtIndex(List* list, size_t idx);
void List_AddInt(List* list, int item);

/*
	Shared
*/
#define List_StringAtIndex(l, i) 				((char*)List_AtIndex(l, i))
#define List_ListAtIndex(l, i)                  ((List*)List_AtIndex(l, i))

/*
	For ASM
*/
#define List_MemoryBlockAtIndex(l, i) 			((MemoryBlock*)List_AtIndex(l, i))
#define List_LabelDefAtIndex(l, i) 				((LabelDef*)List_AtIndex(l, i))
#define List_MemoryBlockHeaderAtIndex(l, i) 	((MemoryBlockHeader*)List_AtIndex(l, i))
#define List_MemoryBlockSetHeaderAtIndex(l, i) 	((MemoryBlockSetHeader*)List_AtIndex(l, i))
#define List_AssemblyTokenAtIndex(l, i) 		((AssemblyToken*)List_AtIndex(l, i))

/*
	For Object Basic
*/
#define List_TokenAtIndex(l, i)					((Token*)List_AtIndex(l, i))
#define List_FunctionAtIndex(l, i)				((Function*)List_AtIndex(l, i))
#define List_FunctionParamAtIndex(l, i)			((FunctionParam*)List_AtIndex(l, i))
#define List_ClassDefAtIndex(l, i)				((ClassDef*)List_AtIndex(l, i))
#define List_CodeBlockAtIndex(l, i)				((CodeBlock*)List_AtIndex(l, i))
#define List_ClassConversionAtIndex(l, i)		((ClassConversion*)List_AtIndex(l, i))
#define List_ParseMessageAtIndex(l, i)			((ParseMessage*)List_AtIndex(l, i))

#endif
