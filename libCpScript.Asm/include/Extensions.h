#ifndef __EXTENSIONS_H__
#define __EXTENSIONS_H__

#include <List.h>

char* StrToLowTrim(char* SrcStr);
char* StrLeft(char* SrcStr);
char* StrRight(char* SrcStr);
char* StrTrim(char* SrcStr);
char* SubStr(char* Src, int Start, int End);
List* SplitAndKeep(char* SrcStr, char* DelimChars);
char* StrToLower(char* SrcStr);

short CanConvertToInt(char* Src);
short CanConvertToBool(char* Src);
short CanConvertToDouble(char* Src);

short ShouldMakeDoubletToInt(double v);

char* IntToStr(int v);

#endif
