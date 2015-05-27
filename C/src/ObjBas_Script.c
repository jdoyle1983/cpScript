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
#include <stdlib.h>

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
	
	List* Messages;
	
} ObjectBasicScript;

ParseMessage* ParseMessage_New(int msgType, char* msgFile, int msgLine, int msgColumn, int msgErrNum, char* msgErrMessage)
{
	ParseMessage* pMsg = (ParseMessage*)malloc(sizeof(ParseMessage));

	pMsg->File = NULL;
	pMsg->ErrMessage = NULL;
	
	pMsg->Type = msgType;
	pMsg->Line = msgLine;
	pMsg->Column = msgColumn;
	pMsg->ErrNum = msgErrNum;

	if(msgFile != NULL)
	{
		pMsg->File = (char*)malloc(sizeof(char) * (strlen(msgFile) + 1));
		strcpy(pMsg->File, msgFile);
	}
	
	if(msgErrMessage != NULL)
	{
		pMsg->ErrMessage = (char*)malloc(sizeof(char) * (strlen(msgErrMessage) + 1));
		strcpy(pMsg->ErrMessage, msgErrMessage);
	}
	
	return pMsg;
};

void ParseMessage_Delete(ParseMessage* msg)
{
	if(msg->File != NULL)
		free(msg->File);
		
	if(msg->ErrMessage != NULL)
		free(msg->ErrMessage);
		
	free(msg);
};

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
	
	obj->Messages = List_New();
	return obj;
};

void AppendAsm(ObjectBasicScript* obj, char* Value)
{
	obj->AsmResult = (char*)realloc(obj->AsmResult, sizeof(char) * (strlen(obj->AsmResult) + strlen(Value) + 1));
	strcat(obj->AsmResult, Value);
};

void AppendAsmLine(ObjectBasicScript* obj, char* NewLine)
{
	AppendAsm(obj, NewLine);
	obj->AsmResult = (char*)realloc(obj->AsmResult, sizeof(char) * (strlen(obj->AsmResult) + 2));
	strcat(obj->AsmResult, "\n");
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
	for(i = 0; i < List_Count(obj->Messages); i++)
		ParseMessage_Delete(List_ParseMessageAtIndex(obj->Messages, i));
	List_Delete(obj->Messages);
	
	free(obj);
};

void AddParseMessage(ObjectBasicScript* obj, int msgType, char* msgFile, int msgLine, int msgColumn, int msgErrNum, char* msgErrMessage)
{
	List_Add(obj->Messages, ParseMessage_New(msgType, msgFile, msgLine, msgColumn, msgErrNum, msgErrMessage));
};

#define AddInfoMessage(a, b, c, d, e, f)		AddParseMessage(a, PM_INFO, b, c, d, e, f)
#define AddWarnMessage(a, b, c, d, e, f)		AddParseMessage(a, PM_WARN, b, c, d, e, f)
#define AddErrMessage(a, b, c, d, e, f)			AddParseMessage(a, PM_ERR, b, c, d, e, f)

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
			ClassConversion_Delete(List_ClassConversionAtIndex(StaticMethods, e));
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
	int a = 0;
	for(i = 0; i < List_Count(_SrcLines); i++)
	{
		char* s = List_StringAtIndex(_SrcLines, i);
		char* t = StrTrim(s);
		if(strlen(t) > 0)
		{
			if(t[0] == '#')
				List_Add(_PreProcLines, StrCopy(t));
			else if(t[0] == '/' && t[1] == '/')
			{
			}
			else
				List_Add(_OutLines, StrCopy(t));
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
			
			for(a = 0; a < List_Count(_ThisSrcLines); a++)
			{
				char* s = List_StringAtIndex(_ThisSrcLines, a);
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
			for(a = 0; a < List_Count(_ThisSrcLines); a++)
				free(List_StringAtIndex(_ThisSrcLines, a));
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
			RegisterFunction(obj, funcName);
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
		rValue = StrCat(rValue, thisItem);
		rValue = StrCat(rValue, "\n");
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
	List* toEval = List_New();
	int i;
	for(i = bStart; i < List_Count(List_CodeBlockAtIndex(obj->CurrentFunction->Blocks, obj->CurrentBlock)->Tokens); i++)
		List_Add(toEval, List_TokenAtIndex(List_CodeBlockAtIndex(obj->CurrentFunction->Blocks, obj->CurrentBlock)->Tokens, i));
	List* availFunctions = FunctionList(obj);
	List* evaled = Evaluator_Parse(toEval, availFunctions);
	
	for(i = 0; i < List_Count(evaled); i++)
	{
		Token* t = List_TokenAtIndex(evaled, i);
		switch(t->Type)
		{
			case ExClassAction:
			{
				int e = 0;
				for(e = 0; e < List_Count(obj->CurrentClassProperties); e++)
				{
					ClassConversion* con = List_ClassConversionAtIndex(obj->CurrentClassProperties, e);
					if(strcmp(t->Value, con->Input) == 0)
					{
						AppendAsm(obj, "PUSH ");
						AppendAsmLine(obj, con->Output);
					}
				}
				for(e = 0; e < List_Count(obj->CurrentClassMethods); e++)
				{
					ClassConversion* con = List_ClassConversionAtIndex(obj->CurrentClassMethods, e);
					if(strcmp(con->Input, t->Value) == 0)
					{
						if(con->IsStatic == 0)
						{
							List* splitList = Split(con->Input, ".");
							AppendAsm(obj, "PUSHB $");
							AppendAsmLine(obj, List_StringAtIndex(splitList, 0));
							int a = 0;
							for(a = 0; a < List_Count(splitList); a++)
								free(List_StringAtIndex(splitList, a));
							List_Delete(splitList);
						}
						AppendAsm(obj, "JMP ");
						AppendAsmLine(obj, con->Output);
					}
				}
				for(e = 0; e < List_Count(obj->Classes); e++)
				{
					ClassDef* cd = List_ClassDefAtIndex(obj->Classes, e);
					List* StaticMethods = ClassDef_GetStaticMethods(cd);
					int a = 0;
					for(a = 0; a < List_Count(StaticMethods); a++)
					{
						ClassConversion* con = List_ClassConversionAtIndex(StaticMethods, a);
						if(strcmp(con->Input, t->Value) == 0)
						{
							AppendAsm(obj, "JMP ");
							AppendAsmLine(obj, con->Output);
						}
						
						ClassConversion_Delete(con);
					}
					List_Delete(StaticMethods);
				}
			} break;
			case QuotedLiteral:
			{
				AppendAsm(obj, "PUSH \"");
				AppendAsm(obj, t->Value);
				AppendAsmLine(obj, "\"");
			} break;
			case Literal:
			{
				short found = 0;
				int e = 0;
				for(e = 0; e < List_Count(obj->Functions); e++)
				{
					Function* f = List_FunctionAtIndex(obj->Functions, e);
					if(strcmp(f->Name, t->Value) == 0)
					{
						found = 1;
						AppendAsm(obj, "JMP ");
						AppendAsmLine(obj, t->Value);
					}
				}
				
				if(found == 0)
				{
					for(e = 0; e < List_Count(obj->CurrentVars); e++)
					{
						char* v = List_StringAtIndex(obj->CurrentVars, e);
						if(strcmp(v, t->Value) == 0)
						{
							found = 1;
							AppendAsm(obj, "PUSH %");
							AppendAsmLine(obj, t->Value);
						}
					}
				}
				
				if(found == 0)
				{
					for(e = 0; e < List_Count(obj->CurrentFunction->Parameters); e++)
					{
						FunctionParam* fp = List_FunctionParamAtIndex(obj->CurrentFunction->Parameters, e);
						if(strcmp(fp->VarName, t->Value) == 0)
						{
							found = 1;
							if(fp->IsClassVar == 1)
							{
								AppendAsm(obj, "PUSHB $");
								AppendAsmLine(obj, fp->VarName);
							}
							else
							{
								char* thisResult = (char*)malloc(sizeof(char) * 5000);
								sprintf(thisResult, "PUSH @%d", e + 1);
								AppendAsmLine(obj, thisResult);
								free(thisResult);
							}
						}
					}
				}
				
				if(found == 0 && obj->CurrentClass != NULL)
				{
					for(e = 0; e < List_Count(obj->CurrentClass->Properties); e++)
					{
						char* prop = List_StringAtIndex(obj->CurrentClass->Properties, e);
						if(strcmp(prop, t->Value) == 0)
						{
							found = 1;
							char* thisResult = (char*)malloc(sizeof(char) * 5000);
							sprintf(thisResult, "PUSH $this:%d", e + 1);
							AppendAsmLine(obj, thisResult);
							free(thisResult);
						}
					}
				}
				
				if(found == 0)
				{
					for(e = 0; e < List_Count(obj->CurrentClassVars); e++)
					{
						char* cv = List_StringAtIndex(obj->CurrentClassVars, e);
						if(strcmp(cv, t->Value) == 0)
						{
							found = 1;
							AppendAsm(obj, "PUSHB $");
							AppendAsmLine(obj, cv);
						}
					}
				}
				
				if(found == 0)
				{
					AppendAsm(obj, "PUSH ");
					AppendAsmLine(obj, t->Value);
				}
			} break;
			case OpAdd: AppendAsmLine(obj, "ADD"); break;
			case OpSub: AppendAsmLine(obj, "SUB"); break;
			case OpMul: AppendAsmLine(obj, "MUL"); break;
			case OpDiv: AppendAsmLine(obj, "DIV"); break;
			case OpMod: AppendAsmLine(obj, "MOD"); break;
			case OpCat: AppendAsmLine(obj, "CAT"); break;
			case OpNeg: AppendAsmLine(obj, "NEG"); break;
			case OpOr: AppendAsmLine(obj, "CO"); break;
			case OpAnd: AppendAsmLine(obj, "CA"); break;
			case OpGreaterThan: AppendAsmLine(obj, "CG"); break;
			case OpGreaterThanOrEqualTo: AppendAsmLine(obj, "CGE"); break;
			case OpLessThan: AppendAsmLine(obj, "CL"); break;
			case OpLessThanOrEqualTo: AppendAsmLine(obj, "CLE"); break;
			case OpEqualTo: AppendAsmLine(obj, "CE"); break;
			case OpNotEqualTo: AppendAsmLine(obj, "CN"); break;
			case OpNot: AppendAsmLine(obj, "CI"); break;
		}
	}
	for(i = 0; i < List_Count(toEval); i++)
		Token_Delete(List_TokenAtIndex(toEval, i));
	for(i = 0; i < List_Count(availFunctions); i++)
		free(List_StringAtIndex(availFunctions, i));
	List_Delete(availFunctions);
	List_Delete(toEval);
	List_Delete(evaled);
};

void ParseBlock(ObjectBasicScript* obj);

void ParseVarBlock(ObjectBasicScript* obj)
{
	List* CurrentBlockTokens = List_CodeBlockAtIndex(obj->CurrentFunction->Blocks, obj->CurrentBlock)->Tokens;
	char* VarName = List_TokenAtIndex(CurrentBlockTokens, 1)->Value;
	
	AppendAsm(obj, "AB %");
	AppendAsmLine(obj, VarName);
	if(List_Count(CurrentBlockTokens) >= 3)
	{
		EvaluateExpression(obj, 3);
		AppendAsm(obj, "POP %");
		AppendAsmLine(obj, VarName);
	}
	char* newVarName = (char*)malloc(sizeof(char) * (strlen(VarName) + 1));
	strcpy(newVarName, VarName);
	List_Add(obj->CurrentVars, newVarName);
};

CodeBlock* getCurrentBlock(ObjectBasicScript* obj)
{
	CodeBlock* r = List_CodeBlockAtIndex(obj->CurrentFunction->Blocks, obj->CurrentBlock);
	return r;
}

void ParseAsmBlock(ObjectBasicScript* obj)
{
	obj->CurrentBlock++;
	int CurrentTokenType = List_TokenAtIndex(getCurrentBlock(obj)->Tokens, 0)->Type;
	
	char* NewLine = (char*)malloc(sizeof(char) * 5000);
	
	while(CurrentTokenType != ExEndAsmBlock)
	{
		char* AsmLine = (char*)malloc(sizeof(char) * 5000);
		strcpy(AsmLine, "");
		
		int i = 0;
		
		for(i = 0; i < List_Count(getCurrentBlock(obj)->Tokens); i++)
		{
			strcat(AsmLine, List_TokenAtIndex(getCurrentBlock(obj)->Tokens, i)->Value);
			strcat(AsmLine, "");
		}
		
		AppendAsmLine(obj, AsmLine);
		free(AsmLine);
			
		obj->CurrentBlock++;
		CurrentTokenType = List_TokenAtIndex(getCurrentBlock(obj)->Tokens, 0)->Type;
	}
	
	free(NewLine);
}

void ParseIfBlock(ObjectBasicScript* obj)
{
	char* NewLine = (char*)malloc(sizeof(char) * 5000);
	
	long EndIfId = NextLabelId(obj);
	long CmpReg = obj->RegisterCount + 1;
	obj->RegisterCount++;
	EvaluateExpression(obj, 1);
	sprintf(NewLine, "POP @%ld", CmpReg);
	AppendAsmLine(obj, NewLine);
	long SkipId = NextLabelId(obj);
	sprintf(NewLine, "JN @%ld, 1, _Skip%ld", CmpReg, SkipId);
	AppendAsmLine(obj, NewLine);
	obj->CurrentBlock++;
	int CurrentTokenType = List_TokenAtIndex(getCurrentBlock(obj)->Tokens, 0)->Type;
	while(CurrentTokenType != ExEndIf)
	{
		if(CurrentTokenType == ExElseIf)
		{
			sprintf(NewLine, "LJMP _EndIf%ld", EndIfId);
			AppendAsmLine(obj, NewLine);
			sprintf(NewLine, "LBL _Skip%ld", SkipId);
			AppendAsmLine(obj, NewLine);
			EvaluateExpression(obj, 1);
			sprintf(NewLine, "POP @%ld", CmpReg);
			AppendAsmLine(obj, NewLine);
			SkipId = NextLabelId(obj);
			sprintf(NewLine, "JN @%ld, 1, _Skip%ld", CmpReg, SkipId);
			AppendAsmLine(obj, NewLine);
			obj->CurrentBlock++;
		}
		else if(CurrentTokenType == ExElse)
		{
			sprintf(NewLine, "LJMP _EndIf%ld", EndIfId);
			AppendAsmLine(obj, NewLine);
			sprintf(NewLine, "LBL _Skip%ld", SkipId);
			AppendAsmLine(obj, NewLine);
			obj->CurrentBlock++;
		}
		else
			ParseBlock(obj);
		CurrentTokenType = List_TokenAtIndex(getCurrentBlock(obj)->Tokens, 0)->Type;
	}
	sprintf(NewLine, "LBL _EndIf%ld", EndIfId);
	AppendAsmLine(obj, NewLine);
	free(NewLine);
};


char* getVarOrLit(ObjectBasicScript* obj, char* input)
{
	char* r = (char*)malloc(sizeof(char) * 5000);
	int i = 0;
	for(i = 0; i < List_Count(obj->CurrentFunction->Parameters); i++)
	{
		FunctionParam* thisParam = List_FunctionParamAtIndex(obj->CurrentFunction->Parameters, i);
		if(strcmp(thisParam->VarName, input) == 0)
		{
			if(thisParam->IsClassVar == 1)
			{
				sprintf(r, "$%s", thisParam->ClassName);
				return r;
			}
			else
			{
				sprintf(r, "@%d", (i + 1));
				return r;
			}
		}
	}
	
	if(obj->CurrentClass != NULL)
	{
		for(i = 0; i < List_Count(obj->CurrentClass->Properties); i++)
		{
			char* thisProperty = List_StringAtIndex(obj->CurrentClass->Properties, i);
			if(strcmp(thisProperty, input) == 0)
			{
				sprintf(r, "$this:%d", (i + 1));
				return r;
			}
		}
	}
	
	for(i = 0; i < List_Count(obj->CurrentVars); i++)
	{
		if(strcmp(List_StringAtIndex(obj->CurrentVars, i), input) == 0)
		{
			sprintf(r, "%%%s", List_StringAtIndex(obj->CurrentVars, i));
			return r;
		}
	}
	
	strcpy(r, input);
	return r;
};

void ParseForBlock(ObjectBasicScript* obj)
{
	long forId = NextLabelId(obj);
	CodeBlock* Block = getCurrentBlock(obj);
	
	//Token* forToken = List_TokenAtIndex(Block->Tokens, 0);
	Token* varToken = List_TokenAtIndex(Block->Tokens, 1);
	//Token* eqToken = List_TokenAtIndex(Block->Tokens, 2);
	Token* initToken = List_TokenAtIndex(Block->Tokens, 3);
	//Token* toToken = List_TokenAtIndex(Block->Tokens, 4);
	Token* endValue = List_TokenAtIndex(Block->Tokens, 5);
	
	char* stepVal = (char*)malloc(sizeof(char) * 5000);
	sprintf(stepVal, "1");
	char* initialVal = getVarOrLit(obj, initToken->Value);
	int op = OpAdd;
	char* opVar = getVarOrLit(obj, varToken->Value);
	char* endVar = getVarOrLit(obj, endValue->Value);
	
	if(StrStartsWith(opVar, "@") == 1 && StrStartsWith(opVar, "%") == 0)
	{
		printf("***Exception: Variable Not Found\n");
		exit(0);
	}
	
	//Token* stepToken = NULL;
	Token* opToken = NULL;
	Token* valToken = NULL;
	
	if(List_Count(Block->Tokens) == 9)
	{
		//stepToken = List_TokenAtIndex(Block->Tokens, 6);
		opToken = List_TokenAtIndex(Block->Tokens, 7);
		valToken = List_TokenAtIndex(Block->Tokens, 8);
		op = opToken->Type;
		free(stepVal);
		stepVal = getVarOrLit(obj, valToken->Value);
	}
	
	char* writeVal = (char*)malloc(sizeof(char) * 5000);
	sprintf(writeVal, "MOV %s, %s", opVar, initialVal);
	AppendAsmLine(obj, writeVal);
	sprintf(writeVal, "LBL _ForStart%ld", forId);
	AppendAsmLine(obj, writeVal);
	obj->CurrentBlock++;
	
	while(List_TokenAtIndex(List_CodeBlockAtIndex(obj->CurrentFunction->Blocks, obj->CurrentBlock)->Tokens, 0)->Type != ExNext)
		ParseBlock(obj);
	
	sprintf(writeVal, "PUSH %s", opVar);
	AppendAsmLine(obj, writeVal);
	sprintf(writeVal, "PUSH %s", stepVal);
	AppendAsmLine(obj, writeVal);
	if(op == OpSub)
	{
		AppendAsmLine(obj, "SUB");
		sprintf(writeVal, "POP %s", opVar);
		AppendAsmLine(obj, writeVal);
		sprintf(writeVal, "JGE %s, %s, _ForStart%ld", opVar, endVar, forId);
		AppendAsmLine(obj, writeVal);
	}
	else
	{
		AppendAsmLine(obj, "ADD");
		sprintf(writeVal, "POP %s", opVar);
		AppendAsmLine(obj, writeVal);
		sprintf(writeVal, "JLE %s, %s, _ForStart%ld", opVar, endVar, forId);
		AppendAsmLine(obj, writeVal);
	}
	
	free(writeVal);
	free(stepVal);
	free(endVar);
	free(opVar);
	free(initialVal);
};

void ParseWhileBlock(ObjectBasicScript* obj)
{
	long whileId = NextLabelId(obj);
	long CmpReg = obj->RegisterCount + 1;
	obj->RegisterCount++;
	
	char* writeVal = (char*)malloc(sizeof(char) * 5000);
	sprintf(writeVal, "LBL _WhileTestStart%ld", whileId);
	AppendAsmLine(obj, writeVal);
	EvaluateExpression(obj, 1);
	sprintf(writeVal, "POP @%ld", CmpReg);
	AppendAsmLine(obj, writeVal);
	sprintf(writeVal, "JN @%ld, 1, _WhileEnd%ld", CmpReg, whileId);
	AppendAsmLine(obj, writeVal);
	obj->CurrentBlock++;
	while(List_TokenAtIndex(List_CodeBlockAtIndex(obj->CurrentFunction->Blocks, obj->CurrentBlock)->Tokens, 0)->Type != ExLoop)
		ParseBlock(obj);
	sprintf(writeVal, "LJMP _WhileTestStart%ld", whileId);
	AppendAsmLine(obj, writeVal);
	sprintf(writeVal, "LBL _WhileEnd%ld", whileId);
	AppendAsmLine(obj, writeVal);
	
	free(writeVal);
};

void RecurseInit(ObjectBasicScript* obj, char* varName, ClassDef* cls)
{
	if(cls->Extends != NULL)
		RecurseInit(obj, varName, cls->Extends);
	char* writeVal = (char*)malloc(sizeof(char) * 5000);
	sprintf(writeVal, "PUSHB $%s", varName);
	AppendAsmLine(obj, writeVal);
	sprintf(writeVal, "JMP _%s_Init", cls->Name);
	AppendAsmLine(obj, writeVal);
};

void ParseBlock(ObjectBasicScript* obj)
{
	int i = 0;
	int e = 0;
	char* thisResult = (char*)malloc(sizeof(char) * 5000);
	CodeBlock* thisBlock = List_CodeBlockAtIndex(obj->CurrentFunction->Blocks, obj->CurrentBlock);
	Token* thisToken = List_TokenAtIndex(thisBlock->Tokens, 0);
	switch(thisToken->Type)
	{
		case ExIf: ParseIfBlock(obj); break;
		case ExWhile: ParseWhileBlock(obj); break;
		case ExVar: ParseVarBlock(obj); break;
		case ExFor: ParseForBlock(obj); break;
		case ExAsmBlock: ParseAsmBlock(obj); break;
		case ExAsmPush:
		{
			int wasFound = 0;
			for(e = 0; e < List_Count(obj->CurrentClassVars); e++)
			{
				char* cv = List_StringAtIndex(obj->CurrentClassVars, e);
				if(strcmp(cv, List_TokenAtIndex(thisBlock->Tokens, 1)->Value) == 0)
				{
					wasFound = 1;
					AppendAsm(obj, "PUSHB $");
					AppendAsmLine(obj, cv);
				}
				//free(cv);
			}
			if(wasFound == 0)
			{
				char* Var = getVarOrLit(obj, List_TokenAtIndex(thisBlock->Tokens, 1)->Value);
				AppendAsm(obj, "PUSH ");
				AppendAsmLine(obj, Var);
				free(Var);
			}
		} break;
		case ExAsmPop:
		{
			int wasFound = 0;
			for(e = 0; e < List_Count(obj->CurrentClassVars); e++)
			{
				char* cv = List_StringAtIndex(obj->CurrentClassVars, e);
				if(strcmp(cv, List_TokenAtIndex(thisBlock->Tokens, 1)->Value) == 0)
				{
					wasFound = 1;
					AppendAsm(obj, "POPB $");
					AppendAsmLine(obj, cv);
				}
				//free(cv);
			}
			if(wasFound == 0)
			{
				char* Var = getVarOrLit(obj, List_TokenAtIndex(thisBlock->Tokens, 1)->Value);
				AppendAsm(obj, "POP ");
				AppendAsmLine(obj, Var);
				free(Var);
			}
		} break;
		default:
		{
			if(List_Count(thisBlock->Tokens) >= 2 && List_TokenAtIndex(thisBlock->Tokens, 1)->Type == Assignment)
			{
				Token* t = List_TokenAtIndex(thisBlock->Tokens, 0);
				EvaluateExpression(obj, 2);
				short found = 0;
				for(i = 0; i < List_Count(obj->CurrentVars); i++)
				{
					char* v = List_StringAtIndex(obj->CurrentVars, i);
					if(strcmp(v, t->Value) == 0)
					{
						found = 1;
						sprintf(thisResult, "POP %%%s", t->Value);
						AppendAsmLine(obj, thisResult);
					}
				}
				if(found == 0)
				{
					for(i = 0; i < List_Count(obj->CurrentFunction->Parameters); i++)
					{
						FunctionParam* fp = List_FunctionParamAtIndex(obj->CurrentFunction->Parameters, i);
						if(strcmp(fp->VarName, t->Value) == 0)
						{
							found = 1;
							if(fp->IsClassVar == 1)
							{
								sprintf(thisResult, "POPB $%s", fp->VarName);
								AppendAsmLine(obj, thisResult);
							}
							else
							{
								sprintf(thisResult, "POP @%d", (i + 1));
								AppendAsmLine(obj, thisResult);
							}
						}
					}
				}
				if(found == 0 && obj->CurrentClass != NULL)
				{
					for(i = 0; i < List_Count(obj->CurrentClass->Properties); i++)
					{
						char* thisProp = List_StringAtIndex(obj->CurrentClass->Properties, i);
						if(strcmp(thisProp, t->Value) == 0)
						{
							found = 1;
							sprintf(thisResult, "POP $this:%d", (i + 1));
							AppendAsmLine(obj, thisResult);
						}
					}
				}
			}
			else
			{
				short isClass = 0;
				for(i = 0; i < List_Count(obj->Classes); i++)
				{
					ClassDef* thisClass = List_ClassDefAtIndex(obj->Classes, i);
					if(strcmp(thisToken->Value, thisClass->Name) == 0)
					{
						isClass = 1;
						char* tclassVar = List_TokenAtIndex(thisBlock->Tokens, 1)->Value;
						char* classVar = (char*)malloc(sizeof(char) * (strlen(tclassVar) + 1));
						strcpy(classVar, tclassVar);
						sprintf(thisResult, "ABS $%s, %d", classVar, List_Count(thisClass->Properties));
						AppendAsmLine(obj, thisResult);
						List_Add(obj->CurrentClassVars, classVar);
						RecurseInit(obj, classVar, thisClass);
						int p = 0;
						for(p = 0; p < List_Count(thisClass->Properties); p++)
						{
							char* thisProp = List_StringAtIndex(thisClass->Properties, p);
							ClassConversion* con = ClassConversion_New();
							con->IsStatic = 0;
							con->Input = (char*)malloc(sizeof(char) * (strlen(classVar) + strlen(thisProp) + 2));
							sprintf(con->Input, "%s.%s", classVar, thisProp);
							con->Output = (char*)malloc(sizeof(char) * (strlen(classVar) + 100));
							sprintf(con->Output, "$%s:%d", classVar, (p + 1));
							List_Add(obj->CurrentClassProperties, con);
						}
						List* instanceMethods = ClassDef_GetInstanceMethods(thisClass);
						for(e = 0; e < List_Count(instanceMethods); e++)
						{
							ClassConversion* con = List_ClassConversionAtIndex(instanceMethods, e);
							ClassConversion* cc = ClassConversion_New();
							cc->IsStatic = 0;
							cc->Input = (char*)malloc(sizeof(char) * (strlen(classVar) + strlen(con->Input) + 2));
							sprintf(cc->Input, "%s.%s", classVar, con->Input);
							cc->Output = (char*)malloc(sizeof(char) * (strlen(con->Output) + 1));
							strcpy(cc->Output, con->Output);
							List_Add(obj->CurrentClassMethods, cc);
							ClassConversion_Delete(con);
						}
						List_Delete(instanceMethods);
						List* staticMethods = ClassDef_GetStaticMethods(thisClass);
						for(e = 0; e < List_Count(staticMethods); e++)
						{
							ClassConversion* con = List_ClassConversionAtIndex(staticMethods, e);
							ClassConversion* cc = ClassConversion_New();
							cc->IsStatic = 1;
							cc->Input = (char*)malloc(sizeof(char) * (strlen(thisClass->Name) + strlen(con->Input) + 2));
							sprintf(cc->Input, "%s.%s", thisClass->Name, con->Input);
							cc->Output = (char*)malloc(sizeof(char) * (strlen(con->Output) + 1));
							strcpy(cc->Output, con->Output);
							List_Add(obj->CurrentClassMethods, cc);
							ClassConversion_Delete(con);
						}
						List_Delete(staticMethods);
					}
				}
				if(isClass == 0)
					EvaluateExpression(obj, 0);
			}
		} break;
	}
	free(thisResult);
	obj->CurrentBlock++;
};

void resetCurrentVars(ObjectBasicScript* obj)
{
	int i = 0;
	for(i = 0; i < List_Count(obj->CurrentVars); i++)
		free(List_StringAtIndex(obj->CurrentVars, i));
	List_Delete(obj->CurrentVars);
	for(i = 0; i < List_Count(obj->CurrentClassVars); i++)
		free(List_StringAtIndex(obj->CurrentClassVars, i));
	List_Delete(obj->CurrentClassVars);
	
	obj->CurrentVars = List_New();
	obj->CurrentClassVars = List_New();
};

void ParseClasses(ObjectBasicScript* obj)
{
	char* thisResult = (char*)malloc(sizeof(char) * 5000);
	
	int i = 0;
	int e = 0;
	int a = 0;
	int b = 0;
	for(i = 0; i < List_Count(obj->Classes); i++)
	{
		ClassDef* def = List_ClassDefAtIndex(obj->Classes, i);
		for(e = 0; e < List_Count(def->Methods); e++)
		{
			Function* mdef = List_FunctionAtIndex(def->Methods, e);
			resetCurrentVars(obj);
			obj->RegisterCount = 0;
			AppendAsm(obj, "LBL _");
			AppendAsm(obj, def->Name);
			AppendAsm(obj, "_");
			AppendAsmLine(obj, mdef->Name);
			if(mdef->IsStatic == 0)
				AppendAsmLine(obj, "POPB $this");
			int total = List_Count(mdef->Parameters);
			int counter = 0;
			for(counter = 0; counter < total; counter++)
			{
				FunctionParam* thisParam = List_FunctionParamAtIndex(mdef->Parameters, counter);
				if(thisParam->IsClassVar == 1)
				{
					sprintf(thisResult, "POPB $%s", thisParam->VarName);
					AppendAsmLine(obj, thisResult);
					for(a = 0; a < List_Count(obj->Classes); a++)
					{
						ClassDef* cdef = List_ClassDefAtIndex(obj->Classes, a);
						if(strcmp(cdef->Name, thisParam->ClassName) == 0)
						{
							List* conv = ClassDef_GetInstanceMethods(cdef);
							for(b = 0; b < List_Count(conv); b++)
							{
								ClassConversion* cconv = List_ClassConversionAtIndex(conv, b);
								ClassConversion* cnv = ClassConversion_New();
								cnv->Input = (char*)malloc(sizeof(char) * (strlen(thisParam->VarName) + strlen(cconv->Input) + 2));
								sprintf(cnv->Input, "%s.%s", thisParam->VarName, cconv->Input);
								cnv->Output = (char*)malloc(sizeof(char) * (strlen(cconv->Output) + 1));
								strcpy(cnv->Output, cconv->Output);
								List_Add(obj->CurrentClassMethods, cnv);
							}
						}
					}
				}
				else
				{
					sprintf(thisResult, "POP @%d", counter + 1);
					AppendAsmLine(obj, thisResult);					
				}
				obj->RegisterCount++;
			}
			
			obj->CurrentBlock = 0;
			obj->CurrentClass = def;
			obj->CurrentFunction = mdef;
			
			while(obj->CurrentBlock < List_Count(mdef->Blocks))
				ParseBlock(obj);
				
			AppendAsmLine(obj, "RTN");
		}
	}
	
	free(thisResult);
};

void ParseFunctions(ObjectBasicScript* obj)
{
	int i = 0;
	int e = 0;
	for(i = 0; i < List_Count(obj->Functions); i++)
	{
		Function* func = List_FunctionAtIndex(obj->Functions, i);
		if(func->UserFunction == 0)
		{
			resetCurrentVars(obj);
			obj->RegisterCount = 0;
			AppendAsm(obj, "LBL ");
			AppendAsmLine(obj, func->Name);
			List* parms = List_New();
			
			List* headerToks = List_CodeBlockAtIndex(func->Blocks, 0)->Tokens;
			for(e = 3; e < List_Count(headerToks); e++)
			{
				Token* t = List_TokenAtIndex(headerToks, e);
				if(t->Type == ExTypeHint)
				{
					FunctionParam* thisParam = List_FunctionParamAtIndex(parms, List_Count(parms) - 1);
					thisParam->IsClassVar = 1;
					thisParam->ClassName = (char*)malloc(sizeof(char) * (strlen(t->Value) + 1));
					strcpy(thisParam->ClassName, t->Value);
				}
				else if(t->Type != OpenParen && t->Type != CloseParen && t->Type != Comma)
				{
					FunctionParam* p = FunctionParam_New();
					p->IsClassVar = 0;
					p->VarName = (char*)malloc(sizeof(char) * (strlen(t->Value) + 1));
					strcpy(p->VarName, t->Value);
					List_Add(parms, p);
				}
			}
			
			List_Reverse(parms);
			func->Parameters = parms;
			int total = List_Count(parms);
			
			char* thisResult = (char*)malloc(sizeof(char) * 5000);
			
			while(total > 0)
			{
				FunctionParam* p = List_FunctionParamAtIndex(parms, total - 1);
				if(p->IsClassVar == 1)
				{
					sprintf(thisResult, "POPB $%s", p->VarName);
					AppendAsmLine(obj, thisResult);
				}
				else
				{
					sprintf(thisResult, "POP @%d", total);
					AppendAsmLine(obj, thisResult);
				}
				total--;
				obj->RegisterCount++;
			}
			
			obj->CurrentBlock = 1;
			obj->CurrentFunction = func;
			
			while(obj->CurrentBlock < List_Count(func->Blocks))
				ParseBlock(obj);
				
			AppendAsmLine(obj, "RTN");
			
			free(thisResult);
		}
	}
};














/*EXPORT void LoadMethod(ObjectBasicScript* obj, char* MethodName)
{
	State_LoadMethod(obj->AsmState, MethodName);
};

EXPORT short Iterate(ObjectBasicScript* obj)
{
	State_Iterate(obj->AsmState);
};

EXPORT void RunScript(ObjectBasicScript* obj)
{
	while(Iterate(obj) == 1);
};*/

EXPORT void LoadScript(ObjectBasicScript* obj, char* Script)
{
	if(obj->ScriptLoaded == 0)
	{
		char* ProcScript = ParsePreProcessor(obj, Script);		
		obj->AsmResult = (char*)malloc(sizeof(char) * 1);
		obj->AsmResult[0] = '\0';
		int i = 0;
		
		List* Stage1 = Tokenizer_Tokenize(ProcScript);
		List* Stage2 = Tokenizer_ParseExtended(Stage1);
		
		for(i = 0; i < List_Count(Stage1); i++)
			Token_Delete(List_TokenAtIndex(Stage1, i));
		List_Delete(Stage1);
		
		obj->Blocks = BlockBuilder_ConvertToBlocks(Stage2);
		List_Delete(obj->Classes);
		List* funcs = Function_ParseFunctionDefs(obj->Blocks);
		for(i = 0; i < List_Count(funcs); i++)
			List_Add(obj->Functions, List_AtIndex(funcs, i));
		List_Delete(funcs);
		obj->Classes = ClassDef_ParseClassDefs(obj->Blocks);
		
		ParseClasses(obj);
		ParseFunctions(obj);
		obj->ScriptLoaded = 1;
	}
};

EXPORT void ObjScript_Load(void* ObjScript, char* Script)
{
	LoadScript((ObjectBasicScript*)ObjScript, Script);
};

EXPORT char* ObjScript_GetAsm(void* ObjScript)
{
	return ((ObjectBasicScript*)ObjScript)->AsmResult;
};

EXPORT void* ObjScript_New()
{
	return (void*)ObjectBasicScript_New();
};

EXPORT void ObjScript_Delete(void* obj)
{
	ObjectBasicScript_Delete((ObjectBasicScript*)obj);
};

EXPORT void RegisterFunction(void* ObjScript, char* Name)
{
	ObjectBasicScript* obj = (ObjectBasicScript*)ObjScript;
	
	Function* f = Function_New();
	f->Func = NULL;
	if(f->Name != NULL)
		free(f->Name);
	f->Name = StrCopy(Name);
	f->UserFunction = 1;
	List_Add(obj->Functions, f);
};
