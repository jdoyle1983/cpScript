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
LBL WriteStrings
POP @2
POP @1
PUSH "String 1: "
PUSH @2
CAT
JMP _Console_WriteLine
PUSH "String 2: "
PUSH @1
CAT
JMP _Console_WriteLine
RTN
LBL Main
AB %fibMax
PUSH 10
POP %fibMax
AB %f1
PUSH 0
POP %f1
AB %f2
PUSH 0
POP %f2
AB %f3
PUSH 0
POP %f3
AB %i
PUSH 0
POP %i
MOV %i, 1
LBL _ForStart1
PUSH %i
PUSH 1
CE
POP @1
JN @1, 1, _Skip3
PUSH 1
POP %f1
PUSH "Seq "
PUSH %i
CAT
PUSH ": "
CAT
PUSH %f1
CAT
JMP _Console_WriteLine
LJMP _EndIf2
LBL _Skip3
PUSH %i
PUSH 2
CE
POP @1
JN @1, 1, _Skip4
PUSH 1
POP %f2
PUSH "Seq "
PUSH %i
CAT
PUSH ": "
CAT
PUSH %f2
CAT
JMP _Console_WriteLine
LJMP _EndIf2
LBL _Skip4
PUSH %f1
PUSH %f2
ADD
POP %f3
PUSH "Seq "
PUSH %i
CAT
PUSH ": "
CAT
PUSH %f3
CAT
JMP _Console_WriteLine
PUSH %f2
POP %f1
PUSH %f3
POP %f2
LBL _EndIf2
PUSH 1
PUSH %i
ADD
POP %i
JLE %i, %fibMax, _ForStart1
RTN