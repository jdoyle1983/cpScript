/*

    libCpStdLib
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

#include <CpStdLib_Utilities.h>
#include <libCpScript.Asm.h>
#include <Extensions.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <sys/time.h>

typedef struct
{
    char** Items;
    int Count;
} CpArray;

typedef struct
{
	char** Items;
	int XCount;
	int YCount;
} CpArray2D;

void Utilities_ListToArray(List* SrcList, CpArray* DestArray)
{
	int i = 0;
	for(i = 0; i < DestArray->Count; i++)
		free(DestArray->Items[i]);
	free(DestArray->Items);
	DestArray->Count = List_Count(SrcList);
	DestArray->Items = (char**)malloc(sizeof(char*) * DestArray->Count);
	for(i = 0; i < DestArray->Count; i++)
		DestArray->Items[i] = (char*)List_AtIndex(SrcList, i);
};

void Utilities_Array_New(void* State)
{
	int i = 0;
    int Size = State_PopInt(State);
    CpArray* a = (CpArray*)malloc(sizeof(CpArray));
    a->Count = Size;
    a->Items = (char**)malloc(sizeof(char*) * Size);
    for(i = 0; i < Size; i++)
    {
        a->Items[i] = (char*)malloc(sizeof(char) * 2);
        strcpy(a->Items[i], "");
    }
    State_Push(State, a);
};

void Utilities_Array_Free(void* State)
{
    CpArray* a = (CpArray*)State_Pop(State);
    int i = 0;
    for(i = 0; i < a->Count; i++)
        free(a->Items[i]);
    free(a->Items);
    free(a);
};

void Utilities_Array_Count(void* State)
{
    CpArray* a = (CpArray*)State_Pop(State);
    State_PushInt(State, a->Count);
};

void Utilities_Array_Resize(void* State)
{
	int newSize = State_PopInt(State);
    CpArray* a = (CpArray*)State_Pop(State);

    int origSize = a->Count;
    int i = 0;

    if(newSize < origSize)
    {
        for(i = newSize; i < origSize; i++)
            free(a->Items[i]);
    }

    a->Items = (char**)realloc(a->Items, sizeof(char*) * newSize);
	a->Count = newSize;

    if(newSize > origSize)
    {
        for(i = origSize; i < newSize; i++)
        {
            a->Items[i] = (char*)malloc(sizeof(char) * 2);
            strcpy(a->Items[i], "");
        }
    }
};

void Utilities_Array_SetItem(void* State)
{
	char* ArrayValue = State_PopString(State);
    int ArrayIndex = State_PopInt(State);
    CpArray* a = (CpArray*)State_Pop(State);
    if(ArrayIndex >= 0 && ArrayIndex < a->Count)
    {
        free(a->Items[ArrayIndex]);
        a->Items[ArrayIndex] = (char*)malloc(sizeof(char) * (strlen(ArrayValue) + 1));
        strcpy(a->Items[ArrayIndex], ArrayValue);
    }
};

void Utilities_Array_GetItem(void* State)
{
    int ArrayIndex = State_PopInt(State);
    CpArray* a = (CpArray*)State_Pop(State);
    if(ArrayIndex >= 0 && ArrayIndex < a->Count)
        State_PushString(State, a->Items[ArrayIndex]);
    else
        State_PushString(State, "");
};

int Get2DPos(int XSize, int YSize, int X, int Y)
{
	return (X * YSize) + Y;
};

int Get2DSize(int XSize, int YSize)
{
	return XSize * YSize;
};

void Utilities_Array2D_New(void* State)
{
	int i = 0;
	int YSize = State_PopInt(State);
    int XSize = State_PopInt(State);
    CpArray2D* a = (CpArray2D*)malloc(sizeof(CpArray2D));
	a->XCount = XSize;
	a->YCount = YSize;
    a->Items = (char**)malloc(sizeof(char*) * Get2DSize(a->XCount, a->YCount));
    for(i = 0; i < Get2DSize(a->XCount, a->YCount); i++)
    {
        a->Items[i] = (char*)malloc(sizeof(char) * 2);
        strcpy(a->Items[i], "");
    }
    State_Push(State, a);
};

void Utilities_Array2D_Free(void* State)
{
    CpArray2D* a = (CpArray2D*)State_Pop(State);
    int i = 0;
    for(i = 0; i < Get2DSize(a->XCount, a->YCount); i++)
        free(a->Items[i]);
    free(a->Items);
    free(a);
};

void Utilities_Array2D_XCount(void* State)
{
    CpArray2D* a = (CpArray2D*)State_Pop(State);
    State_PushInt(State, a->XCount);
};

void Utilities_Array2D_YCount(void* State)
{
    CpArray2D* a = (CpArray2D*)State_Pop(State);
    State_PushInt(State, a->YCount);
};

void ResizeArray2D(CpArray2D* a, int newXSize, int newYSize)
{
	int x = 0;
	int y = 0;
	char** tmpStorage = (char**)malloc(sizeof(char*) * Get2DSize(newXSize, newYSize));
	
	if(newXSize < a->XCount)
	{
		for(x = newXSize; x < a->XCount; x++)
			for(y = 0; y < a->YCount; y++)
				free(a->Items[Get2DPos(a->XCount, a->YCount, x, y)]);
	}

	if(newYSize < a->YCount)
	{
		for(x = 0; x < a->XCount; x++)
			for(y = newYSize; y < a->YCount; y++)
				free(a->Items[Get2DPos(a->XCount, a->YCount, x, y)]);
	}

	for(x = 0; x < newXSize; x++)
	{
		for(y = 0; y < newYSize; y++)
		{
			if(x < a->XCount && y < a->YCount)
			{
				int pd = Get2DPos(newXSize, newYSize, x, y);
				int ps = Get2DPos(a->XCount, a->YCount, x, y);
				tmpStorage[pd] = a->Items[ps];
			}
			else
			{
				tmpStorage[Get2DPos(newXSize, newYSize, x, y)] = (char*)malloc(sizeof(char) * 2);
				strcpy(tmpStorage[Get2DPos(newXSize, newYSize, x, y)], "");
			}
		}
	}

	free(a->Items);
	a->Items = tmpStorage;
	a->XCount = newXSize;
	a->YCount = newYSize;
};

void Utilities_Array2D_Resize(void* State)
{
	int newYSize = State_PopInt(State);
	int newXSize = State_PopInt(State);
    CpArray2D* a = (CpArray2D*)State_Pop(State);
	
	ResizeArray2D(a, newXSize, newYSize);
};

void Utilities_Array2D_ResizeX(void* State)
{
	int newSize = State_PopInt(State);
    CpArray2D* a = (CpArray2D*)State_Pop(State);
	
	ResizeArray2D(a, newSize, a->YCount);
};

void Utilities_Array2D_ResizeY(void* State)
{
	int newSize = State_PopInt(State);
    CpArray2D* a = (CpArray2D*)State_Pop(State);
	
	ResizeArray2D(a, a->XCount, newSize);
};

void Utilities_Array2D_SetItem(void* State)
{
	char* ArrayValue = State_PopString(State);
	int ArrayIndexY = State_PopInt(State);
    int ArrayIndexX = State_PopInt(State);
    CpArray2D* a = (CpArray2D*)State_Pop(State);

	int ArrayIndex = Get2DPos(a->XCount, a->YCount, ArrayIndexX, ArrayIndexY);

    if(ArrayIndex >= 0 && ArrayIndex < Get2DSize(a->XCount, a->YCount))
    {
        free(a->Items[ArrayIndex]);
        a->Items[ArrayIndex] = (char*)malloc(sizeof(char) * (strlen(ArrayValue) + 1));
        strcpy(a->Items[ArrayIndex], ArrayValue);
    }
};

void Utilities_Array2D_GetItem(void* State)
{
	int ArrayIndexY = State_PopInt(State);
    int ArrayIndexX = State_PopInt(State);
    CpArray2D* a = (CpArray2D*)State_Pop(State);
	int ArrayIndex = Get2DPos(a->XCount, a->YCount, ArrayIndexX, ArrayIndexY);
    if(ArrayIndex >= 0 && ArrayIndex < Get2DSize(a->XCount, a->YCount))
        State_PushString(State, a->Items[ArrayIndex]);
    else
        State_PushString(State, "");
};

void Utilities_Time_GetTimeOfDay(void* State)
{
	struct timeval* thisTime = malloc(sizeof(struct timeval));
	gettimeofday(thisTime, NULL);
	State_Push(State, thisTime);
};

void Utilities_Time_Diff(void* State)
{
	struct timeval* firstTime = (struct timeval*)State_Pop(State);
	struct timeval* secondTime = (struct timeval*)State_Pop(State);
	
	struct timeval* diffTime = malloc(sizeof(struct timeval));
	diffTime->tv_sec = secondTime->tv_sec - firstTime->tv_sec;
	
	diffTime->tv_usec = secondTime->tv_usec - firstTime->tv_usec;
	
	State_Push(State, diffTime);
};

void Utiltiess_Time_GetMilliseconds(void* State)
{
	struct timeval* thisTime = (struct timeval*)State_Pop(State);
	State_PushInt(State, (thisTime->tv_sec * 1000) + (thisTime->tv_usec / 1000));
};

void Utilities_Time_FreeTimeOfDay(void* State)
{
	struct timeval* thisTime = (struct timeval*)State_Pop(State);
	free(thisTime);
};

void Utilities_String_Trim(void* State)
{
	char* StrValue = State_PopString(State);
	char* TrmValue = StrTrim(StrValue);
	free(StrValue);
	State_PushString(State, TrmValue);
};

void Utilities_String_Split(void* State)
{
	CpArray* DestArray = (CpArray*)State_Pop(State);
	char* Delims = State_PopString(State);
	char* Target = State_PopString(State);
	List* Values = Split(Target, Delims);
	Utilities_ListToArray(Values, DestArray);
	List_Delete(Values);
};

void Utilities_String_SplitAndKeep(void* State)
{
	CpArray* DestArray = (CpArray*)State_Pop(State);
	char* Delims = State_PopString(State);
	char* Target = State_PopString(State);
	
	List* Values = SplitAndKeep(Target, Delims);
	
	Utilities_ListToArray(Values, DestArray);
	List_Delete(Values);
};
