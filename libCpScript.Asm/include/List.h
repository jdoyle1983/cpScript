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

List* List_New();
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
