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
