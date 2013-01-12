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

#include <stdio.h>
#include <malloc.h>
#include <List.h>

List* List_New(void)
{
	List* r = (List*)malloc(sizeof(List));
	r->Count = 0;
	r->Items = malloc(0);
	return r;
};

void List_Delete(List* list)
{
	if(list->Items != NULL)
		free(list->Items);
	free(list);
};

int List_Count(List* list)
{
	return list->Count;
};

void List_Add(List* list, void* item)
{
	list->Count++;
	list->Items = (void**)realloc(list->Items, sizeof(void*) * list->Count);
	list->Items[list->Count - 1]  = item;
};

void* List_AtIndex(List* list, int idx)
{
	return list->Items[idx];
};

void List_Remove(List* list, void* item)
{
	int idx = -1;
	int i = 0;
	for(i = 0; i < list->Count && idx == -1; i++)
		if(list->Items[i] == item)
			idx = i;
	List_RemoveAt(list, idx);
};

void List_RemoveAt(List* list, int idx)
{
	if(idx >= 0 && idx < list->Count)
	{
		int e = 0;
		for(e = idx; e < list->Count - 1; e++)
			list->Items[e] = list->Items[e + 1];
		list->Count--;
		list->Items = (void**)realloc(list->Items, sizeof(void*) * list->Count);
	}
};

void List_Clear(List* list)
{
    list->Count = 0;
    free(list->Items);
    list->Items = (void**)malloc(0);
};
