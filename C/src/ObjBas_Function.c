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

#include <ObjBas_Function.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

FunctionParam* FunctionParam_New()
{
	FunctionParam* p = (FunctionParam*)malloc(sizeof(FunctionParam));
	p->IsClassVar = 0;
	p->VarName = (char*)malloc(sizeof(char) * 1);
	p->VarName[0] = '\0';
	p->ClassName = (char*)malloc(sizeof(char) * 1);
	p->ClassName[0] = '\0';
	return p;
};

void FunctionParam_SetVarName(FunctionParam* p, char* varName)
{
	if(p->VarName != NULL)
		free(p->VarName);
	p->VarName = (char*)malloc(sizeof(char) * (strlen(varName) + 1));
	strcpy(p->VarName, varName);
};

void FunctionParam_SetClassName(FunctionParam* p, char* className)
{
	if(p->ClassName != NULL)
		free(p->ClassName);
	p->ClassName = (char*)malloc(sizeof(char) * (strlen(className) + 1));
	strcpy(p->ClassName, className);
};

void FunctionParam_Delete(FunctionParam* p)
{
	if(p->VarName != NULL)
		free(p->VarName);
	if(p->ClassName != NULL)
		free(p->ClassName);
	free(p);
};


void Function_Delete(Function* func)
{
	if(func->Name != NULL)
		free(func->Name);
	List_Delete(func->Blocks);
	int i = 0;
	for(i = 0; i < List_Count(func->Parameters); i++)
		FunctionParam_Delete(List_FunctionParamAtIndex(func->Parameters, i));
	List_Delete(func->Parameters);
	free(func);
};

List* Function_ParseFunctionDefs(List* Blocks)
{
	return NULL;
};
