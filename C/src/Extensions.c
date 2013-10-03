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

#include <Extensions.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/*
	Various helper methods used by Object Basic and Cp Asm
*/

char* StrToLowTrim(char* SrcStr)
{
    char* lStr = StrToLower(SrcStr);
    char* tStr = StrTrim(lStr);
    free(lStr);
    return tStr;
};

char* StrLeft(char* SrcStr)
{
    int Start = -1;
    int i = 0;
	
    for(i = 0; i < strlen(SrcStr) && Start == -1; i++)
        if(SrcStr[i] != ' ' && SrcStr[i] != '\n' && SrcStr[i] != '\r' && SrcStr[i] != '\t')
           Start = i;
		   
    if(Start == -1)
    {
        char* r = (char*)malloc(sizeof(char));
        r[0] = '\0';
        return r;
    }
    else
        return SubStr(SrcStr, Start, strlen(SrcStr) - 1);
};

char* StrRight(char* SrcStr)
{
    int End = -1;
    int i = 0;
    
	for(i = 0; i < strlen(SrcStr); i++)
        if(SrcStr[i] != ' ' && SrcStr[i] != '\n' && SrcStr[i] != '\r' && SrcStr[i] != '\t')
           End = i;
		   
    if(End == -1)
    {
        char* r = (char*)malloc(sizeof(char));
        r[0] = '\0';
        return r;
    }
    else
        return SubStr(SrcStr, 0, End);
};

char* StrTrim(char* SrcStr)
{
    char* lStr = StrLeft(SrcStr);
    char* rStr = StrRight(lStr);
    free(lStr);
    return rStr;
};

char* StrToLower(char* SrcStr)
{
	int i = 0;
    char* r = (char*)malloc(sizeof(char) * (strlen(SrcStr) + 1));
    strcpy(r, SrcStr);
    for(i = 0; i < strlen(r); i++)
        r[i] = tolower(r[i]);
    return r;
};

short StrStartsWith(char* SrcStr, char* TestStr)
{
	short r = 0;
	
	int len = strlen(TestStr);
	if(strlen(SrcStr) >= len)
	{
		short allMatch = 1;
		int i = 0;
		for(i = 0; i < strlen(TestStr) && allMatch == 1; i++)
		{
			if(TestStr[i] != SrcStr[i])
				allMatch = 0;
		}
		
		if(allMatch)
			r = 1;
	}
	
	return r;
};

char* StrCat(char* SrcStr, char* CatStr)
{
	SrcStr = (char*)realloc(SrcStr, sizeof(char) * (strlen(SrcStr) + strlen(CatStr) + 1));
	strcat(SrcStr, CatStr);
	return SrcStr;
};

char* StrCopy(char* SrcStr)
{
	char* r = (char*)malloc(sizeof(char) * (strlen(SrcStr) + 1));
	strcpy(r, SrcStr);
	return r;
};

char* SubStr(char* Src, int Start, int End)
{
	char* r = (char*)malloc(sizeof(char) * (strlen(Src) + 1));
	int c = 0;
	int i = 0;
	for(i = Start; i <= End && i < strlen(Src); i++)
	{
		r[c] = Src[i];
		c++;
	}
	r[c] = '\0';
	
	return r;
};

List* Split(char* SrcStr, char* DelimChars)
{
	List* r = List_New();

	int start = 0;

	int s = 0;
	int d = 0;
	for(s = 0; s < strlen(SrcStr); s++)
	{
		for(d = 0; d < strlen(DelimChars); d++)
		{
			if(SrcStr[s] == DelimChars[d])
			{
				List_Add(r, SubStr(SrcStr, start, s - 1));
				start = s + 1;
			}
		}
	}

	if(start < strlen(SrcStr) - 1)
		List_Add(r, SubStr(SrcStr, start, strlen(SrcStr) - 1));
	return r;
};

List* SplitAndKeep(char* SrcStr, char* DelimChars)
{
	List* r = List_New();

	int start = 0;

	int s = 0;
	int d = 0;
	for(s = 0; s < strlen(SrcStr); s++)
	{
		for(d = 0; d < strlen(DelimChars); d++)
		{
			if(SrcStr[s] == DelimChars[d])
			{
				List_Add(r, SubStr(SrcStr, start, s - 1));
				List_Add(r, SubStr(SrcStr, s, s));
				start = s + 1;
			}
		}
	}

	if(start < strlen(SrcStr) - 1)
		List_Add(r, SubStr(SrcStr, start, strlen(SrcStr) - 1));
	return r;
};

char* ReadFileContents(const char* FilePath)
{
	FILE* srcFile;
    char* srcBuffer;
    unsigned long srcLen;

    srcFile = fopen(FilePath, "rb");
	
    fseek(srcFile, 0, SEEK_END);
    srcLen = ftell(srcFile);
    fseek(srcFile, 0, SEEK_SET);

    srcBuffer = (char*)malloc(srcLen + 1);

    if(!srcBuffer)
    {
		printf("***EXCEPTION: Memory Error Reading File.");
		exit(0);
    }

    fread(srcBuffer, srcLen, 1, srcFile);
    fclose(srcFile);
	
	return srcBuffer;
};

short CanConvertToInt(char* Src)
{
    short rValue = 1;
    char* t = StrTrim(Src);

    int i = 0;
    for(i = 0; i < strlen(t) && rValue == 1; i++)
    {
        if( t[i] == '-' && i != 0)
            rValue = 0;
        else if( t[i] != '0' && t[i] != '1' && t[i] != '2' && t[i] != '3' && t[i] != '4' && t[i] != '5' && t[i] != '6' && t[i] != '7' && t[i] != '8' && t[i] != '9')
            rValue = 0;
    }

    free(t);
    return rValue;
};

short CanConvertToBool(char* Src)
{
    short rValue = 1;
    char* t = StrTrim(Src);

    int i = 0;
    for(i = 0; i < strlen(t) && rValue == 1; i++)
    {
        if( t[i] != '0' && t[i] != '1')
            rValue = 0;
    }

    free(t);
    return rValue;
};

short CanConvertToDouble(char* Src)
{
    short rValue = 1;
    char* t = StrTrim(Src);

    int i = 0;
    int d = 0;
    for(i = 0; i < strlen(t) && rValue == 1; i++)
    {
        if( t[i] == '-')
        {
            if(i != 0)
                rValue = 0;
        }
        else if(t[i] == '.')
        {
            if(d == 0)
                d++;
            else
                rValue = 0;
        }
        else if( t[i] != '0' && t[i] != '1' && t[i] != '2' && t[i] != '3' && t[i] != '4' && t[i] != '5' && t[i] != '6' && t[i] != '7' && t[i] != '8' && t[i] != '9')
            rValue = 0;
    }
    //printf("\n");
    free(t);
    return rValue;
};

short ShouldMakeDoubletToInt(double v)
{
	short rValue = 1;
	int i = 0;
    int d = -1;

    char* Src = (char*)malloc(sizeof(char) * 3000);
    sprintf(Src, "%f", v);

    if(CanConvertToDouble(Src) == 0)
        rValue = 0;

    for(i = 0; i < strlen(Src) && d == -1; i++)
        if(Src[i] == '.')
            d = i;
    if(d != -1)
    {
        char* s1 = SubStr(Src, 0, d - 1);
        char* s2 = SubStr(Src, d + 1, strlen(Src) - 1);
        if(atoi(s2) > 0)
            rValue = 0;
        free(s1);
        free(s2);
    }
    free(Src);
    return rValue;
}

char* IntToStr(int v)
{
    char* Str = (char*)malloc(sizeof(char) * 3000);
	char* r;
    sprintf(Str, "%d", v);
    r = (char*)malloc(sizeof(char) * (strlen(Str) + 1));
    strcpy(r, Str);
    free(Str);
    return r;
};
