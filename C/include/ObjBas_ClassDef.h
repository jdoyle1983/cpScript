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

#ifndef __OBJBAS_CLASSDEF_H__
#define __OBJBAS_CLASSDEF_H__

#include <List.h>

typedef struct
{
	short IsStatic;
	char* Input;
	char* Output;
} ClassConversion;

typedef struct ClassDef ClassDef;
struct ClassDef
{
	char* Name;
	ClassDef* Extends;
	List* Properties;
	List* Methods;
};

ClassConversion* ClassConversion_New(void);
ClassConversion* ClassConversion_Copy(ClassConversion* src);
void ClassConversion_Delete(ClassConversion* cc);

ClassDef* ClassDef_New(void);
void ClassDef_Delete(ClassDef* def);
char* ClassDef_GetFirstMethodLabel(ClassDef* def, char* Name);
List* ClassDef_GetStaticMethods(ClassDef* def);
List* ClassDef_GetInstanceMethods(ClassDef* def);

List* ClassDef_ParseClassDefs(List* Blocks);


#endif
