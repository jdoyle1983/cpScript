#ifndef __LABELDEF_H__
#define __LABELDEF_H__

typedef struct
{
    char* Label;
    int Offset;
    void (*UserFunction)(void*);
    short IsStub;
} LabelDef;

LabelDef* LabelDef_NewWithOffset(char* Label, int Offset);
LabelDef* LabelDef_NewWithFunction(char* Label, void (*UserFunction)(void*), short IsStub);
void LabelDef_Delete(LabelDef* Def);


#endif
