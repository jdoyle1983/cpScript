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

typedef struct
{
	int Count;
	void** Items;
} List;

List* List_New(void);
void List_Delete(List* list);

int List_Count(List* list);

void List_Add(List* list, void* item);
void* List_AtIndex(List* list, int idx);

void List_Remove(List* list, void* item);
void List_RemoveAt(List* list, int idx);

void List_Clear(List* list);

#define List_StringAtIndex(l, i) 				(char*)List_AtIndex(l, i)
#define List_MemoryBlockAtIndex(l, i) 			(MemoryBlock*)List_AtIndex(l, i)
#define List_LabelDefAtIndex(l, i) 				(LabelDef*)List_AtIndex(l, i)
#define List_MemoryBlockHeaderAtIndex(l, i) 	(MemoryBlockHeader*)List_AtIndex(l, i)
#define List_MemoryBlockSetHeaderAtIndex(l, i) 	(MemoryBlockSetHeader*)List_AtIndex(l, i)
#define List_AssemblyTokenAtIndex(l, i) 		(AssemblyToken*)List_AtIndex(l, i)
#define List_ListAtIndex(l, i)                  (List*)List_AtIndex(l, i)
#define List_IntAtIndex(l, i)                   (int)List_AtIndex(l, i)

#define List_AddInt(l, i)                       List_Add(l, (void*)i)
#endif
