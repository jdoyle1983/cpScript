/*

    libCpScript.Asm,
    Copyright (C) 2013 Jason Doyle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact Information:

    Original Author: Jason Doyle (jdoyle1983@gmail.com)
*/

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
