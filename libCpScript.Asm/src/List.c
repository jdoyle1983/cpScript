#include <malloc.h>
#include <List.h>

List* List_New()
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
