/*

    libCpScript.Asm, libCpScript.ObjectBasic
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
