#include <List.h>
#include <Stack.h>
#include <Extensions.h>

#include <utarray.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

enum AsmCommandCode
{
	NOOP = 0,
	LBL, 		PUSH,		POP,
	PUSHR,		POPR,		PUSHA,
	MOV,		MOVA,		ALLOC,
	FREE,		ADD,		SUB,
	MUL,		DIV,		MOD,
	CON,		NEG,		INC,
	DEC,		CMPA,		CMPO,
	CMPE,		CMPN,		CMPG,
	CMPGE,		CMPL,		CMPLE,
	JMPL,		JMPO,		JMPF,
	JMPLT,		JMPOT,		JMPFT
	
};

typedef struct
{
	short Used;
	short Fixed;
	char* Value;
} MemoryBlock;

void UT_MemoryBlock_Copy(void* destObj, void* srcObj)
{
	MemoryBlock* destMb = (MemoryBlock*)destObj;
	MemoryBlock* srcMb = (MemoryBlock*)srcObj;
	
	destMb->Used = srcMb->Used;
	destMb->Fixed = srcMb->Fixed;
	destMb->Value = (char*)malloc(sizeof(char) * (strlen(srcMb->Value) + 1));
	strcpy(destMb->Value, srcMb->Value);
};

void UT_MemoryBlock_Delete(void* obj)
{
	MemoryBlock* mb = (MemoryBlock*)obj;
	if(mb->Value != NULL)
		free(mb->Value);
};

void MemoryBlock_ReAlloc(MemoryBlock* memoryBlock, char* newValue)
{
	memoryBlock->Used = 1;
	if(memoryBlock->Value != NULL)
		free(memoryBlock->Value);
	memoryBlock->Value = (char*)malloc(sizeof(char) * (strlen(newValue) + 1));
	strcpy(memoryBlock->Value, newValue);
};

short MemoryBlock_FreeValue(MemoryBlock* memoryBlock)
{
	if(memoryBlock->Fixed)
		return 0;
	if(memoryBlock->Value != NULL)
		free(memoryBlock->Value);
	memoryBlock->Used = 0;
	return 1;
};

UT_icd MemoryBlock_icd = {sizeof(MemoryBlock), NULL, UT_MemoryBlock_Copy, UT_MemoryBlock_Delete};



/*
typedef struct
{
	long NumericValue;
	void* MemoryValue;
} Tuple;

Tuple* AsmCommand_New(short command, List* codeParts)
{
	int i = 0;
	Tuple* asmCommand = (Tuple*)malloc(sizeof(Tuple));
	asmCommand->NumericValue = command;
	List* parts = List_New();
	for(i = 0; i < List_Count(codeParts); i++)
	{
		char* srcText = List_StringAtIndex(codeParts, i);
		char* destText = (char*)malloc(sizeof(char) * (strlen(srcText) + 1));
		strcpy(destText, srcText);
		List_Add(parts, destText);
	}
	asmCommand->MemoryValue = parts;
	return asmCommand;
};

void AsmCommand_Delete(Tuple* tuple)
{
	int i = 0;
	List* codeParts = (List*)tuple->MemoryValue;
	for(i = 0; i < List_Count(codeParts); i++)
		free(List_StringAtIndex(codeParts, i));
	List_Delete((List*)tuple->MemoryValue);
	free(tuple);
};

#define AsmCommand_GetCommand(t)		t->NumericValue
#define	AsmCommand_GetCodeParts(t)		((List*)t->MemoryValue

typedef struct
{
	char* FileName;
	long SourceLine;
} CodeLocation;

Tuple* CodeLocation_New(char* fileName, long sourceLine)
{
	Tuple* codeLocation = (Tuple*)malloc(sizeof(Tuple));
	codeLocation->NumericValue = sourceLine;
	codeLocation->MemoryValue = malloc(sizeof(char) * (strlen(fileName) + 1));
	strcpy((char*)codeLocation->MemoryValue, fileName);
	return codeLocation;
};

void CodeLocation_Delete(Tuple* tuple)
{
	free((char*)tuple->MemoryValue);
	free(tuple);
};

#define CodeLocation_GetCodeLine(t)				t->NumericValue
#define	CodeLocation_GetCodeFileName(t)			((char*)t->MemoryValue)

#define LabelOffset_New(label, offset)			CodeLocation_New(label, offset)
#define LabelOffset_Delete(label)				CodeLocation_Delete(label)
#define	LabelOffset_GetOffset(t)				t->NumericValue
#define LabelOffset_GetLabel(t)					((char*)t->MemoryValue)

#define DataVariable_New(varname, memoffset)	CodeLocation_New(varname, memoffset)
#define	DataVariable_Delete(datavariable)		CodeLocation_Delete(datavariable)
#define	DataVariable_GetVariableName(datavar)	((char*)datavar->MemoryValue)
#define	DataVariable_GetVariableOffset(datavar)	datavar->NumericValue

#define List_TupleAtIndex(l, i)					((Tuple*)List_AtIndex(l, i))
#define	List_CodeLocationAtIndex(l, i)			List_TupleAtIndex(l, i)
#define	List_LabelOffsetAtIndex(l, i)			List_TupleAtIndex(l, i)
#define	List_DataVariableAtIndex(l, i)			List_TupleAtIndex(l, i)
#define List_MemoryBlockAtIndex(l, i)			((MemoryBlock*)List_AtIndex(l, i))
*/

typedef struct
{
	UT_array*	Memory;
	/*
	List*	CodeParts;
	List*	OriginalLocations;
	List*	Externals;
	List* 	DataVarDeclarations;
	
	List*	LabelOffsets;
	
	
	Stack*	Stack;
	
	long	CurrentScriptOffset;
	
	List*	ExternalFunctions;
	
	void (*ExecutionException) (char* sourceFile, long sourceLine, char* exceptionDetails);
	*/
} State2;

/*
typedef struct
{
	char* Label;
	void (*Function) (State2*);
} ExternalFunction;

ExternalFunction* ExternalFunction_New(char* label, void (*function)(State2*))
{
	ExternalFunction* externalFunction = (ExternalFunction*)malloc(sizeof(ExternalFunction));
	externalFunction->Label = (char*)malloc(sizeof(char) * (strlen(label) + 1));
	strcpy(externalFunction->Label, label);
	externalFunction->Function = function;
	return externalFunction;
};

void ExternalFunction_Delete(ExternalFunction* externalFunction)
{
	free(externalFunction->Label);
	free(externalFunction);
};

#define List_ExternalFunctionAtIndex(l, i)			((ExternalFunction*)List_AtIndex(l, i))
*/

State2* State2_New()
{
	State2* state = (State2*)malloc(sizeof(State2));
	utarray_new(state->Memory, &MemoryBlock_icd);
	
	/*state->CodeParts = List_New();
	state->OriginalLocations = List_New();
	state->Externals = List_New();
	state->DataVarDeclarations = List_New();
	state->LabelOffsets = List_New();
	state->Stack = Stack_New();
	state->CurrentScriptOffset = -1;
	state->ExternalFunctions = List_New();
	state->ExecutionException = NULL;*/
};

void State2_Delete(State2* state)
{
	/*long i = 0;
	for(i = 0; i < List_Count(state->ExternalFunctions); i++)
		ExternalFunction_Delete(List_ExternalFunctionAtIndex(state->ExternalFunctions, i));
	List_Delete(state->ExternalFunctions);
	for(i = 0; i < List_Count(state->Stack); i++)
		free(List_StringAtIndex(state->Stack, i));
	Stack_Delete(state->Stack);
	for(i = 0; i < List_Count(state->LabelOffsets); i++)
		LabelOffset_Delete(List_LabelOffsetAtIndex(state->LabelOffsets, i));
	List_Delete(state->LabelOffsets);
	for(i = 0; i < List_Count(state->DataVarDeclarations); i++)
		DataVariable_Delete(List_DataVariableAtIndex(state->DataVarDeclarations, i));
	List_Delete(state->DataVarDeclarations);
	for(i = 0; i < List_Count(state->Externals); i++)
		free(List_StringAtIndex(state->Externals, i));
	List_Delete(state->Externals);
	for(i = 0; i < List_Count(state->OriginalLocations); i++)
		CodeLocation_Delete(List_CodeLocationAtIndex(state->OriginalLocations, i));
	List_Delete(state->OriginalLocations);
	for(i = 0; i < List_Count(state->CodeParts); i++)
		AsmCommand_Delete(List_TupleAtIndex(state->CodeParts, i));
	List_Delete(state->CodeParts);*/
	free(state);
};

void State2_RegisterFunction(char* label, void (*function)(State2*))
{
};

int main(int argc, char** argv)
{
	printf("Test\n");
	
	State2* newState = State2_New();
	State2_Delete(newState);
	
	return 0;
};