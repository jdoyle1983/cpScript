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
#include <stdint.h>

List* List_New(void)
{
	List* r = (List*)malloc(sizeof(List));
	r->Count = 0;
	r->Items = malloc(0);
	r->ActualCount = 0;
	return r;
};

void List_Delete(List* list)
{
	if(list->Items != NULL)
		free(list->Items);
	free(list);
};

size_t List_Count(List* list)
{
	return list->Count;
};

void List_Add(List* list, void* item)
{
	list->Count++;
	if(list->Count > list->ActualCount)
	{
		list->ActualCount = list->ActualCount + 10;
		list->Items = (void**)realloc(list->Items, sizeof(void*) * list->ActualCount);
	}
	list->Items[list->Count - 1]  = item;
};

void List_AddInFront(List* list, void* item)
{
	list->Count++;
	if(list->Count > list->ActualCount)
	{
		list->ActualCount = list->ActualCount + 10;
		list->Items = (void**)realloc(list->Items, sizeof(void*) * list->ActualCount);
	}
	size_t i = 0;
	for(i = list->Count - 1; i > 0; i--)
		list->Items[i] = list->Items[i - 1];
	list->Items[0] = item;
};

void* List_AtIndex(List* list, size_t idx)
{
	return list->Items[idx];
};

void List_Remove(List* list, void* item)
{
	intmax_t idx = -1;
	size_t i = 0;
	for(i = 0; i < list->Count && idx == -1; i++)
		if(list->Items[i] == item)
			idx = i;
	List_RemoveAt(list, idx);
};

void List_RemoveAt(List* list, size_t idx)
{
	if(idx < list->Count)
	{
		size_t e = 0;
		for(e = idx; e < list->Count - 1; e++)
			list->Items[e] = list->Items[e + 1];
		list->Count--;
		if((list->ActualCount - list->Count) >= 10)
		{
			list->ActualCount = list->ActualCount - 10;
			list->Items = (void**)realloc(list->Items, sizeof(void*) * list->ActualCount);
		}
	}
};

void List_Reverse(List* list)
{
	void** revList = (void**)malloc(sizeof(void*) * list->Count);
	size_t i = 0;
	for(i = 0; i < list->Count; i++)
		revList[i] = list->Items[list->Count - (i + 1)];
	free(list->Items);
	list->Items = revList;
};

void List_Clear(List* list)
{
    list->Count = 0;
	list->ActualCount = 0;
    free(list->Items);
    list->Items = (void**)malloc(0);
};

int List_IntAtIndex(List* list, size_t idx)
{
	return *((int*)List_AtIndex(list, idx));
};

void List_AddInt(List* list, int item)
{
	int* tInt = (int*)malloc(sizeof(int));
	*tInt = item;
	List_Add(list, tInt);
};
