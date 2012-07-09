#ifndef __ISCRIPTLIB_H__
#define __ISCRIPTLIB_H__

typedef struct
{
    void* LibHandle;
    char* (*Author)();
    char* (*Version)();
    void (*Init)(void*);
} Library;

Library* Library_Load(const char* Path);
void Library_Free(Library* Lib);

#endif
