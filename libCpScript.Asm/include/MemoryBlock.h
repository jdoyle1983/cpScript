#ifndef __MEMORYBLOCK_H__
#define __MEMORYBLOCK_H__

typedef struct
{
    short Used;
    char* Value;
} MemoryBlock;

MemoryBlock* MemoryBlock_New();
void MemoryBlock_Delete(MemoryBlock* block);
void MemoryBlock_SetValue(MemoryBlock* block, char* Val);
void MemoryBlock_Free(MemoryBlock* block);

#endif
