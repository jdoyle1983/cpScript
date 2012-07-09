#include <MemoryBlockHeader.h>
#include <string.h>
#include <malloc.h>

MemoryBlockHeader* MemoryBlockHeader_New(char* Name, int Offset)
{
    MemoryBlockHeader* header = (MemoryBlockHeader*)malloc(sizeof(MemoryBlockHeader));
    header->Name = NULL;
    header->Offset = Offset;
    if(Name != NULL)
    {
        header->Name = (char*)malloc(sizeof(char) * (strlen(Name) + 1));
        strcpy(header->Name, Name);
    }
    return header;
};

void MemoryBlockHeader_Delete(MemoryBlockHeader* Header)
{
    if(Header->Name != NULL)
        free(Header->Name);
    free(Header);
};
