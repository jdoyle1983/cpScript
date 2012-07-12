#include <CpStdLib_Math.h>
#include <libCpScript.Asm.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

void Math_Abs(void* State)
{
    State_PushInt(State, State_PopInt(State));
};

void Math_Floor(void* State)
{
    State_PushDouble(State, State_PopDouble(State));
};

void Math_Ceiling(void* State)
{
    State_PushDouble(State, State_PopDouble(State));
};
