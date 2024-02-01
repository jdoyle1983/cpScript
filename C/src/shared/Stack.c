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

#include <malloc.h>
#include <Stack.h>

void* Stack_Peek(Stack* stack)
{
	return stack->Items[stack->Count - 1];
};

void* Stack_Pop(Stack* stack)
{
	void* r = Stack_Peek(stack);
	List_RemoveAt(stack, stack->Count - 1);
	return r;
};

void Stack_Push(Stack* stack, void* item)
{
	List_Add(stack, item);
};

void Stack_PushInt(Stack* stack, int val)
{
	List_AddInt(stack, val);
};

int Stack_PeekInt(Stack* stack)
{
	return List_IntAtIndex(stack, stack->Count - 1);
};

int Stack_PopInt(Stack* stack)
{
	int rVal = Stack_PeekInt(stack);
	int* tVal = Stack_Peek(stack);
	List_RemoveAt(stack, stack->Count - 1);
	free(tVal);
	return rVal;
};
