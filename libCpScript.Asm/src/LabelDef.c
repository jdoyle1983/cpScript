#include <LabelDef.h>
#include <malloc.h>
#include <string.h>


LabelDef* LabelDef_NewWithOffset(char* Label, int Offset)
{
    LabelDef* d = (LabelDef*)malloc(sizeof(LabelDef));
    d->Label = NULL;
    d->Offset = -1;
    d->UserFunction = NULL;
    d->IsStub = 0;

    if(Label != NULL)
    {
        d->Label = (char*)malloc(sizeof(char) * (strlen(Label) + 1));
        strcpy(d->Label, Label);
    }

    d->Offset = Offset;
    return d;
};

LabelDef* LabelDef_NewWithFunction(char* Label, void (*UserFunction)(void*), short IsStub)
{
    LabelDef* d = (LabelDef*)malloc(sizeof(LabelDef));
    d->Label = NULL;
    d->Offset = -1;
    d->UserFunction = NULL;
    d->IsStub = 0;

    if(Label != NULL)
    {
        d->Label = (char*)malloc(sizeof(char) * (strlen(Label) + 1));
        strcpy(d->Label, Label);
    }

    d->UserFunction = UserFunction;
    d->IsStub = IsStub;
    return d;
};

void LabelDef_Delete(LabelDef* Def)
{
    if(Def->Label != NULL)
        free(Def->Label);
    free(Def);
};
