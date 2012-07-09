#include <Extensions.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>
#include <ctype.h>

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
        if(SrcStr[i] != ' ' &&
           SrcStr[i] != '\n' &&
           SrcStr[i] != '\r' &&
           SrcStr[i] != '\t')
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
        if(SrcStr[i] != ' ' &&
           SrcStr[i] != '\n' &&
           SrcStr[i] != '\r' &&
           SrcStr[i] != '\t')
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
    char* r = (char*)malloc(sizeof(char) * (strlen(SrcStr) + 1));
    strcpy(r, SrcStr);
    int i = 0;
    for(i = 0; i < strlen(r); i++)
        r[i] = tolower(r[i]);
    return r;
};

char* SubStr(char* Src, int Start, int End)
{
	int Len = End - Start + 2;
	char* r = (char*)malloc(sizeof(char) * Len);
	int i = 0;
	for(i = Start; i <= End; i++)
		r[i - Start] = Src[i];
	r[Len - 1] = '\0';
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
