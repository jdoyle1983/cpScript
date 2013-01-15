LBL _Console_Write
POP @1
PUSH @1
JMP __StdLib__ConsoleIO__Write
RTN
LBL _Console_WriteLine
POP @1
PUSH @1
JMP __StdLib__ConsoleIO__WriteLine
RTN
LBL _Console_ReadLine
JMP __StdLib__ConsoleIO__ReadLine
RTN
LBL Main
PUSH ""
JMP _Console_WriteLine
PUSH ""
JMP _Console_WriteLine
PUSH "Enter Start Value: "
JMP _Console_Write
AB %startValue
JMP _Console_ReadLine
POP %startValue
PUSH "Enter End Value: "
JMP _Console_Write
AB %endValue
JMP _Console_ReadLine
POP %endValue
PUSH ""
JMP _Console_WriteLine
PUSH %startValue
PUSH %endValue
CE
POP @1
JN @1, 1, _Skip2
PUSH "Values Match, No Counting Required."
JMP _Console_WriteLine
LJMP _EndIf1
LBL _Skip2
PUSH %startValue
PUSH %endValue
CL
POP @1
JN @1, 1, _Skip3
AB %lc
PUSH 0
POP %lc
MOV %lc, %startValue
LBL _ForStart4
PUSH "Value: "
PUSH %lc
CAT
JMP _Console_WriteLine
PUSH %lc
PUSH 1
ADD
POP %lc
JLE %lc, %endValue, _ForStart4
LJMP _EndIf1
LBL _Skip3
AB %gc
PUSH 0
POP %gc
MOV %gc, %startValue
LBL _ForStart5
PUSH "Value: "
PUSH %gc
CAT
JMP _Console_WriteLine
PUSH %gc
PUSH 1
SUB
POP %gc
JGE %gc, %endValue, _ForStart5
LBL _EndIf1
RTN
