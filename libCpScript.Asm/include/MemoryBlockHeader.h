#ifndef __MEMORYBLOCKHEADER_H__
#define __MEMORYBLOCKHEADER_H__

typedef struct
{
    char* Name;
    int Offset;
} MemoryBlockHeader;

MemoryBlockHeader* MemoryBlockHeader_New(char* Name, int Offset);
void MemoryBlockHeader_Delete(MemoryBlockHeader* Header);

#endif
