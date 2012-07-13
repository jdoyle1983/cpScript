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
    Header->IndexOffset->Items[Idx] = (void*)Offset;
};

int MemoryBlockSetHeader_GetOffset(MemoryBlockSetHeader* Header, int Idx)
{
    return (int)Header->IndexOffset->Items[Idx];
};

void MemoryBlockSetHeader_Delete(MemoryBlockSetHeader* Header)
{
    List_Delete(Header->IndexOffset);
    if(Header->Name != NULL)
        free(Header->Name);
    free(Header);
};
