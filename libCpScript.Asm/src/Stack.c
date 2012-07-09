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
