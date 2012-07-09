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
void MemoryBlockSetHeader_Delete(MemoryBlockSetHeader* Header);

#endif
