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

#include <libCpScript.ObjectBasic.h>
#include <ObjBas_BlockBuilder.h>
#include <ObjBas_ClassDef.h>
#include <ObjBas_Evaluator.h>
#include <ObjBas_Function.h>
#include <ObjBas_Token.h>
#include <List.h>
#include <Stack.h>
#include <Extensions.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct
{
	List* Functions;
	List* Classes;
	char* AsmResult;
	List* Blocks;
	long NextLabel;
	short ScriptLoaded;
	long RegisterCount;
	Function* CurrentFunction;
	ClassDef* CurrentClass;
	long CurrentBlock;
	List* CurrentVars;
	List* CurrentClassVars;
	List* CurrentClassProperties;
	List* CurrentClassMethods;
	
} ObjectBasicScript;

ObjectBasicScript* ObjectBasicScript_New()
{
	ObjectBasicScript* obj = (ObjectBasicScript*)malloc(sizeof(ObjectBasicScript));
	obj->Functions = List_New();
	obj->Classes = List_New();
	obj->AsmResult = (char*)malloc(sizeof(char) * 1);
	obj->AsmResult[0] = '\0';
	obj->Blocks = List_New();
	obj->NextLabel = 1;
	obj->ScriptLoaded = 0;
	obj->RegisterCount = 0;
	obj->CurrentFunction = NULL;
	obj->CurrentClass = NULL;
	obj->CurrentBlock = -1;
	obj->CurrentVars = List_New();
	obj->CurrentClassVars = List_New();
	obj->CurrentClassProperties = List_New();
	obj->CurrentClassMethods = List_New();
	return obj;
};

void MethodStub(void* State)
{
};

void ObjectBasicScript_Delete(ObjectBasicScript* obj)
{
	int i = 0;
	for(i = 0; i < List_Count(obj->Functions); i++)
		Function_Delete(List_FunctionAtIndex(obj->Functions, i));
	List_Delete(obj->Functions);
	for(i = 0; i < List_Count(obj->Classes); i++)
		ClassDef_Delete(List_ClassDefAtIndex(obj->Classes, i));
	List_Delete(obj->Classes);
	free(obj->AsmResult);
	for(i = 0; i < List_Count(obj->Blocks); i++)
		CodeBlock_Delete(List_CodeBlockAtIndex(obj->Blocks, i));
	List_Delete(obj->Blocks);
	for(i = 0; i < List_Count(obj->CurrentVars); i++)
		free(List_StringAtIndex(obj->CurrentVars, i));
	List_Delete(obj->CurrentVars);
	for(i = 0; i < List_Count(obj->CurrentClassVars); i++)
		free(List_StringAtIndex(obj->CurrentClassVars, i));
	List_Delete(obj->CurrentClassVars);
	for(i = 0; i < List_Count(obj->CurrentClassProperties); i++)
		ClassConversion_Delete(List_ClassConversionAtIndex(obj->CurrentClassProperties, i));
	List_Delete(obj->CurrentClassProperties);
	for(i = 0; i < List_Count(obj->CurrentClassMethods); i++)
		ClassConversion_Delete(List_ClassConversionAtIndex(obj->CurrentClassMethods, i));
	List_Delete(obj->CurrentClassMethods);
	free(obj);
};

long NextLabelId(ObjectBasicScript* obj)
{
	long rValue = obj->NextLabel;
	obj->NextLabel++;
	return rValue;
};

List* FunctionList(ObjectBasicScript* obj)
{
	List* Names = List_New();
	
	int i = 0;
	for(i = 0; i < List_Count(obj->Functions); i++)
	{
		Function* thisFunc = List_FunctionAtIndex(obj->Functions, i);
		char* thisValue = (char*)malloc(sizeof(char) * (strlen(thisFunc->Name) + 1));
		strcpy(thisValue, thisFunc->Name);
		List_Add(Names, thisValue);
	}
	for(i = 0; i < List_Count(obj->CurrentClassMethods); i++)
	{
		ClassConversion* cnv = List_ClassConversionAtIndex(obj->CurrentClassMethods, i);
		char* thisValue = (char*)malloc(sizeof(char) * (strlen(cnv->Input) + 1));
		strcpy(thisValue, cnv->Input);
		List_Add(Names, thisValue);
	}
	for(i = 0; i < List_Count(obj->Classes); i++)
	{
		ClassDef* classDef = List_ClassDefAtIndex(obj->Classes, i);
		List* StaticMethods = ClassDef_GetStaticMethods(classDef);
		int e = 0;
		for(e = 0; e < List_Count(StaticMethods); e++)
		{
			ClassConversion* cnv = List_ClassConversionAtIndex(StaticMethods, e);
			char* thisValue = (char*)malloc(sizeof(char) * (strlen(cnv->Input) + 1));
			strcpy(thisValue, cnv->Input);
			List_Add(Names, thisValue);
		}
		for(e = 0; e < List_Count(StaticMethods); e++)
			ClassConversion_Delete(List_ClassConversionAtIndex(StaticMethods, i));
		List_Delete(StaticMethods);
	}
	
	return Names;
};

char* ParsePreProcessor(ObjectBasicScript* obj, char* Script)
{
	List* _OutLines = List_New();
	List* _PreProcLines = List_New();
	List* _SrcLines = Split(Script, "\n");
	int i = 0;
	for(i = 0; i < List_Count(_SrcLines); i++)
	{
		char* s = List_StringAtIndex(_SrcLines, i);
		char* t = StrTrim(s);
		if(strlen(t) > 0)
		{
			if(t[0] == '#')
				List_Add(_PreProcLines, StrCopy(s));
			else if(s[0] == '/' && s[1] == '/')
			{
			}
			else
				List_Add(_OutLines, StrCopy(s));
		}
		free(t);
	}
	
	List* IncludedFiles = List_New();
	List* ToIncludeFiles = List_New();
	
	for(i = 0; i < List_Count(_PreProcLines); i++)
	{
		char* ppLine = List_StringAtIndex(_PreProcLines, i);
		char* ppLower = StrToLower(ppLine);
		if(StrStartsWith(ppLower, "#include ") == 1)
		{
			char* ppFileName = SubStr(ppLine, 9, strlen(ppLine) - 1);
			List_Add(ToIncludeFiles, ppFileName);
		}
		free(ppLower);
	}
	
	for(i = 0; i < List_Count(ToIncludeFiles); i++)
	{
		char* filePath = List_StringAtIndex(ToIncludeFiles, i);
		short found = 0;
		int e = 0;
		for(e = 0; e < List_Count(IncludedFiles); e++)
			if(strcmp(List_StringAtIndex(IncludedFiles, e), filePath) == 0)
				found = 1;
		if(found == 0)
		{
			char* includeContent = ReadFileContents(filePath);
			List* _ThisSrcLines = Split(includeContent, "\n");
			free(includeContent);
			
			for(i = 0; i < List_Count(_ThisSrcLines); i++)
			{
				char* s = List_StringAtIndex(_ThisSrcLines, i);
				char* t = StrTrim(s);
				char* l = StrToLower(t);
				if(strlen(t) > 0)
				{
					if(t[0] == '#')
					{
						if(StrStartsWith(l, "#include ") == 1)
						{
							char* ppFileName = SubStr(t, 9, strlen(t) - 1);
							List_Add(ToIncludeFiles, ppFileName);
						
						}
						else
							List_Add(_PreProcLines, StrCopy(s));
					}
					else
						List_Add(_OutLines, StrCopy(s));
				}
				free(t);
				free(l);
			}
			for(i = 0; i < List_Count(_ThisSrcLines); i++)
				free(List_StringAtIndex(_ThisSrcLines, i));
			List_Delete(_ThisSrcLines);
		}
	}
	
	for(i = 0; i < List_Count(_PreProcLines); i++)
	{
		char* preProc = List_StringAtIndex(_PreProcLines, i);
		char* preProcTrim = StrTrim(preProc);
		char* preProcLower = StrToLower(preProcTrim);
		if(StrStartsWith(preProcLower, "#hostfunction ") == 1)
		{
			char* funcName = SubStr(preProcTrim, 14, strlen(preProcTrim) - 1);
			RegisterFunction(obj, funcName, &MethodStub, 1);
			free(funcName);
		}
		
		free(preProcLower);
		free(preProcTrim);
	}
	
	char* rValue = (char*)malloc(sizeof(char) * 1);
	rValue[0] = '\0';
	for(i = 0; i < List_Count(_OutLines); i++)
	{
		char* thisItem = List_StringAtIndex(_OutLines, i);
		StrCat(rValue, thisItem);
		StrCat(rValue, "\n");
	}
	
	for(i = 0; i < List_Count(ToIncludeFiles); i++)
		free(List_StringAtIndex(ToIncludeFiles, i));
	List_Delete(ToIncludeFiles);
	
	for(i = 0; i < List_Count(IncludedFiles); i++)
		free(List_StringAtIndex(IncludedFiles, i));
	List_Delete(IncludedFiles);
	
	for(i = 0; i < List_Count(_SrcLines); i++)
		free(List_StringAtIndex(_SrcLines, i));
	List_Delete(_SrcLines);
	
	for(i = 0; i < List_Count(_PreProcLines); i++)
		free(List_StringAtIndex(_PreProcLines, i));
	List_Delete(_PreProcLines);
	
	for(i = 0; i < List_Count(_OutLines); i++)
		free(List_StringAtIndex(_OutLines, i));
	List_Delete(_OutLines);
	
	return rValue;
};

void EvaluateExpression(ObjectBasicScript* obj, int bStart)
{
	
};

void ParseVarBlock(ObjectBasicScript* obj)
{
};

void ParseIfBlock(ObjectBasicScript* obj)
{
};


char* getVarOrLit(ObjectBasicScript* obj, char* input)
{
	return NULL;
};

void ParseForBlock(ObjectBasicScript* obj)
{
};

void ParseWhileBlock(ObjectBasicScript* obj)
{
};

void ParseBlock(ObjectBasicScript* obj)
{
};

void RecurseInit(ObjectBasicScript* obj, char* varName, ClassDef* cls)
{
};

void ParseClasses(ObjectBasicScript* obj)
{
};

void ParseFunctions(ObjectBasicScript* obj)
{
};














EXPORT void LoadMethod(ObjectBasicScript* obj, char* MethodName)
{
};

EXPORT void RunScript(ObjectBasicScript* obj)
{
};

EXPORT short Iterate(ObjectBasicScript* obj)
{
	return 0;
};

EXPORT void LoadScript(ObjectBasicScript* obj, char* Script)
{
};

EXPORT void* ObjScript_New()
{
	return (void*)ObjectBasicScript_New();
};

EXPORT void ObjScript_Delete(void* obj)
{
	ObjectBasicScript_Delete((ObjectBasicScript*)obj);
};

EXPORT void RegisterFunction(void* ObjScript, char* Name, void (*Func)(void* State), short StubMethod)
{
	ObjectBasicScript* obj = (ObjectBasicScript*)ObjScript;
	
	Function* f = Function_New();
	f->Func = Func;
	if(f->Name != NULL)
		free(f->Name);
	f->Name = StrCopy(Name);
	f->UserFunction = 1;
	List_Add(obj->Functions, f);
};
