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

#ifndef __STACK_H__
#define __STACK_H__

#include <List.h>

#define Stack List
#define Stack_New List_New
#define Stack_Delete List_Delete

void* Stack_Peek(Stack* stack);
void* Stack_Pop(Stack* stack);
void Stack_Push(Stack* stack, void* item);

#define Stack_PushInt(s, i) Stack_Push(s, (void*)i)

#define Stack_PeekInt(s) 	(int)Stack_Peek(s)
#define Stack_PeekString(s) (char*)Stack_Peek(s)
#define Stack_PeekList(s) 	(List*)Stack_Peek(s)

#define Stack_PopInt(s) 	(int)Stack_Pop(s)
#define Stack_PopString(s) 	(char*)Stack_Pop(s)
#define Stack_PopList(s) 	(List*)Stack_Pop(s)

#endif