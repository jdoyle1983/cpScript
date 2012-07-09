#include <IScriptLib.h>
#include <State.h>
#include <string.h>
#include <malloc.h>

#ifdef LINUX
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#endif

#ifdef WINDOWS
#endif

Library* Library_Load(const char* Path)
{
    Library* l = (Library*)malloc(sizeof(Library));

    #ifdef LINUX
    l->LibHandle = dlopen(Path, RTLD_NOW);
    if(!l->LibHandle)
    {
        free(l);
        return NULL;
    }

    l->Author = dlsym(l->LibHandle, "Author");
    if(dlerror() != NULL)
    {
        dlclose(l->LibHandle);
        free(l);
        return NULL;
    }

    l->Version = dlsym(l->LibHandle, "Version");
    if(dlerror() != NULL)
    {
        dlclose(l->LibHandle);
        free(l);
        return NULL;
    }

    l->Init = dlsym(l->LibHandle, "Init");
    if(dlerror() != NULL)
    {
        dlclose(l->LibHandle);
        free(l);
        return NULL;
    }
    #endif

    return l;
};

void Library_Free(Library* Lib)
{
    if(Lib != NULL)
    {
        #ifdef LINUX
        dlclose(Lib->LibHandle);
        free(Lib);
        #endif
    }
};
