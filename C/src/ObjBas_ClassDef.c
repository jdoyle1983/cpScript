/*

    libCpScript.ObjectBasic
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

#include <ObjBas_ClassDef.h>
#include <ObjBas_Function.h>
#include <ObjBas_BlockBuilder.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

ClassConversion* ClassConversion_New(void)
{
	ClassConversion* cc = (ClassConversion*)malloc(sizeof(ClassConversion));
	cc->IsStatic = 0;
	cc->Input = NULL;
	cc->Output = NULL;
	return cc;
};

ClassConversion* ClassConversion_Copy(ClassConversion* src)
{
	ClassConversion* cc = ClassConversion_New();
	cc->IsStatic = src->IsStatic;
	cc->Input = (char*)malloc(sizeof(char) * (strlen(src->Input) + 1));
	cc->Output = (char*)malloc(sizeof(char) * (strlen(src->Output) + 1));
	strcpy(cc->Input, src->Input);
	strcpy(cc->Output, src->Output);
	return cc;
};

void ClassConversion_Delete(ClassConversion* cc)
{
	if(cc->Input != NULL)
		free(cc->Input);
	if(cc->Output != NULL)
		free(cc->Output);
	free(cc);
};

ClassDef* ClassDef_New(void)
{
	ClassDef* def = (ClassDef*)malloc(sizeof(ClassDef));
	def->Name = NULL;
	def->Extends = NULL;
	def->Properties = List_New();
	def->Methods = List_New();
	return def;
};

void ClassDef_Delete(ClassDef* def)
{
	if(def->Name == NULL)
		free(def->Name);
	int i = 0;
	for(i = 0; i < List_Count(def->Properties); i++)
		free(List_StringAtIndex(def->Properties, i));
	List_Delete(def->Properties);
	for(i = 0; i < List_Count(def->Methods); i++)
		Function_Delete(List_FunctionAtIndex(def->Methods, i));
	List_Delete(def->Methods);
	free(def);
};

char* ClassDef_GetFirstMethodLabel(ClassDef* def, char* Name)
{
	int i = 0;
	for(i = 0; i < List_Count(def->Methods); i++)
	{
		Function* meth = List_FunctionAtIndex(def->Methods, i);
		if(strcmp(Name, meth->Name) == 0)
		{
			char* newName = (char*)malloc(sizeof(char) * (strlen(Name) + strlen(meth->Name) + 3));
			strcpy(newName, "_");
			strcat(newName, Name);
			strcat(newName, "_");
			strcat(newName, meth->Name);
			return newName;
		}
	}
	
	if(def->Extends != NULL)
		return ClassDef_GetFirstMethodLabel(def->Extends, Name);
	
	char* t = (char*)malloc(sizeof(char) * 1);
	t[0] = '\0';
	return t;
};

List* ClassDef_GetStaticMethods(ClassDef* def)
{
	List* Results = List_New();
	int i = 0;
	for(i = 0; i < List_Count(def->Methods); i++)
	{
		Function* f = List_FunctionAtIndex(def->Methods, i);
		if(f->IsStatic == 1)
		{
			ClassConversion* cnv = ClassConversion_New();
			
			char* inputValue = (char*)malloc(sizeof(char) * (strlen(def->Name) + strlen(f->Name) + 2));
			strcpy(inputValue, def->Name);
			strcat(inputValue, ".");
			strcat(inputValue, f->Name);
			
			char* outputValue = (char*)malloc(sizeof(char) * (strlen(def->Name) + strlen(f->Name) + 3));
			strcpy(outputValue, "_");
			strcat(outputValue, def->Name);
			strcat(outputValue, "_");
			strcat(outputValue, f->Name);
			
			cnv->Input = inputValue;
			cnv->Output = outputValue;
			
			List_Add(Results, cnv);
		}
	}
	return Results;
};

List* ClassDef_GetInstanceMethods(ClassDef* def)
{
	List* Results = List_New();
	int i = 0;
	for(i = 0; i < List_Count(def->Methods); i++)
	{
		Function* f = List_FunctionAtIndex(def->Methods, i);
		if(f->IsStatic == 0)
		{
			ClassConversion* cnv = ClassConversion_New();
			
			char* inputValue = (char*)malloc(sizeof(char) * (strlen(f->Name) + 1));
			strcpy(inputValue, f->Name);
			
			char* outputValue = (char*)malloc(sizeof(char) * (strlen(def->Name) + strlen(f->Name) + 3));
			strcpy(outputValue, "_");
			strcat(outputValue, def->Name);
			strcat(outputValue, "_");
			strcat(outputValue, f->Name);
			
			cnv->Input = inputValue;
			cnv->Output = outputValue;
			
			List_Add(Results, cnv);
		}
	}
	
	if(def->Extends != NULL)
	{
		List* parentResults = ClassDef_GetInstanceMethods(def->Extends);
		int i = 0;
		for(i = 0; i < List_Count(parentResults); i++)
		{
			short found = 0;
			ClassConversion* cnv = List_ClassConversionAtIndex(parentResults, i);
			int e = 0;
			for(e = 0; e < List_Count(Results); e++)
			{
				ClassConversion* cnv2 = List_ClassConversionAtIndex(Results, e);
				if(strcmp(cnv->Input, cnv2->Input) == 0)
					found = 1;
			}
		
			if(found == 0)
				List_Add(Results, ClassConversion_Copy(cnv));
		}
		
		for(i = 0; i < List_Count(parentResults); i++)
			ClassConversion_Delete(List_ClassConversionAtIndex(parentResults, i));
		List_Delete(parentResults);
	}
	
	return Results;
};

void RecurseGetProps(ClassDef* def, ClassDef* exDef)
{
	if(exDef->Extends != NULL)
		RecurseGetProps(def, exDef->Extends);
	int i = 0;
	for(i = 0; i < List_Count(exDef->Properties); i++)
	{
		char* thisProp = List_StringAtIndex(exDef->Properties, i);
		short found = 0;
		int e = 0;
		for(e = 0; e < List_Count(def->Properties); e++)
			if(strcmp(thisProp, List_StringAtIndex(def->Properties, e)) == 0)
				found = 1;
		if(found == 0)
		{
			char* newProp = (char*)malloc(sizeof(char) * (strlen(thisProp) + 1));
			strcpy(newProp, thisProp);
			List_Add(def->Properties, newProp);
		}
	}
};

List* ClassDef_ParseClassDefs(List* Blocks)
{
	List* defs = List_New();
	int i = 0;
	for(i = 0; i < List_Count(Blocks); i++)
	{
		CodeBlock* thisBlock = List_CodeBlockAtIndex(Blocks, i);
		if(List_TokenAtIndex(thisBlock->Tokens, 0)->Type == ExClass)
		{
			ClassDef* c = ClassDef_New();
			if(List_Count(thisBlock->Tokens) == 4)
			{
				char* exClassName = List_TokenAtIndex(thisBlock->Tokens, 3)->Value;
				ClassDef* exClass = NULL;
				int r = 0;
				for(r = 0; r < List_Count(defs); r++)
					if(strcmp(List_ClassDefAtIndex(defs, r)->Name, exClassName) == 0)
						exClass = List_ClassDefAtIndex(defs, r);
				if(exClass == NULL)
				{
					printf("***EXCEPTION: Base Class Not Found.");
					exit(0);
				}
				RecurseGetProps(c, exClass);
				c->Extends = exClass;
			}
			
			c->Name = (char*)malloc(sizeof(char) * (strlen(List_TokenAtIndex(thisBlock->Tokens, 1)->Value) + 1));
			strcpy(c->Name, List_TokenAtIndex(thisBlock->Tokens, 1)->Value);
			while(List_TokenAtIndex(List_CodeBlockAtIndex(Blocks, i)->Tokens, 0)->Type != ExEndClass)
			{
				thisBlock = List_CodeBlockAtIndex(Blocks, i);
				if(List_TokenAtIndex(thisBlock->Tokens, 0)->Type == ExProperty)
				{
					char* thisProp = (char*)malloc(sizeof(char) * (strlen(List_TokenAtIndex(thisBlock->Tokens, 1)->Value) + 1));
					strcpy(thisProp, List_TokenAtIndex(thisBlock->Tokens, 1)->Value);
					List_Add(c->Properties, thisProp);
				}
				else if(List_TokenAtIndex(thisBlock->Tokens, 0)->Type == ExMethod)
				{
					int tIdx = 1;
					Function* mdef = Function_New();
					if(List_TokenAtIndex(thisBlock->Tokens, tIdx)->Type == ExOverride)
					{
						mdef->IsOverride = 1;
						tIdx++;
					}
					else if(List_TokenAtIndex(thisBlock->Tokens, tIdx)->Type == ExStatic)
					{
						mdef->IsStatic = 1;
						tIdx++;
					}
					
					mdef->Name = (char*)malloc(sizeof(char) * (strlen(List_TokenAtIndex(thisBlock->Tokens, tIdx)->Value) + 1));
					strcpy(mdef->Name, List_TokenAtIndex(thisBlock->Tokens, tIdx)->Value);
					tIdx++;
					while(List_TokenAtIndex(thisBlock->Tokens, tIdx)->Type != CloseParen)
					{
						Token* thisTok = List_TokenAtIndex(thisBlock->Tokens, tIdx);
						if(thisTok->Type == ExTypeHint)
						{
						}
						else if(thisTok->Type != OpenParen && thisTok->Type != Comma)
						{
							FunctionParam* p = FunctionParam_New();
							p->IsClassVar = 0;
							FunctionParam_SetVarName(p, thisTok->Value);
							if(mdef->Parameters == NULL)
								mdef->Parameters = List_New();
							List_Add(mdef->Parameters, p);
						}
						tIdx++;
					}
					if(mdef->Parameters == NULL)
						mdef->Parameters = List_New();
					List_Reverse(mdef->Parameters);
					
					i++;
					thisBlock = List_CodeBlockAtIndex(Blocks, i);
					if(mdef->Blocks == NULL)
						mdef->Blocks = List_New();
					while(List_TokenAtIndex(thisBlock->Tokens, 0)->Type != ExEndMethod)
					{
						List_Add(mdef->Blocks, thisBlock);
						i++;
						thisBlock = List_CodeBlockAtIndex(Blocks, i);
					}
					List_Add(c->Methods, mdef);
				}
				i++;
			}
			List_Add(defs, c);
		}
	}
	
	return defs;
};