#include <MemoryBlock.h>
#include <string.h>
#include <malloc.h>

MemoryBlock* MemoryBlock_New()
{
    MemoryBlock* b = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    b->Used = 0;
    b->Value = NULL;
    return b;
};

void MemoryBlock_Delete(MemoryBlock* block)
{
    if(block->Value != NULL)
        free(block->Value);
    free(block);
};

void MemoryBlock_SetValue(MemoryBlock* block, char* Val)
{
    if(block->Value != NULL)
        free(block->Value);
    block->Used = 1;
    block->Value = (char*)malloc(sizeof(char) * (strlen(Val) + 1));
    strcpy(block->Value, Val);
};

void MemoryBlock_Free(MemoryBlock* block)
{
    if(block->Value != NULL)
        free(block->Value);
    block->Used = 0;
};
