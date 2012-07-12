LBL _BaseObject_Init
POPB $this
PUSH 0
POP $this:1
RTN
LBL _String_Init
POPB $this
PUSH 1
POP $this:1
RTN
LBL _StringBuilder_Init
POPB $this
PUSH 2
POP $this:1
RTN
LBL _StringBuilder_Append
POPB $this
POP @1
PUSH $this:2
PUSH @1
CAT
POP $this:2
RTN
LBL _StringBuilder_AppendLine
POPB $this
POP @1
PUSH $this:2
PUSH @1
CAT
PUSH "\n"
CAT
POP $this:2
RTN
LBL _TestClass1_Init
POPB $this
RTN
LBL _TestClass1_ShouldDoSomething
POPB $this
POP @1
PUSH @1
JMP _Console_WriteLine
RTN
LBL _TestClass2_Init
POPB $this
RTN
LBL _TestClass2_DoSomething
POPB $this
POPB $myitem
PUSH "Test Line"
PUSHB $myitem
JMP _TestClass1_ShouldDoSomething
RTN
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
LBL _File_Init
POPB $this
RTN
LBL _File_OpenRead
POPB $this
POP @1
PUSH "r"
PUSH @1
JMP __StdLib__FileIO__Open
POP $this:1
RTN
LBL _File_OpenWrite
POPB $this
POP @1
PUSH "w"
PUSH @1
JMP __StdLib__FileIO__Open
POP $this:1
RTN
LBL _File_OpenBinaryRead
POPB $this
POP @1
PUSH "rb"
PUSH @1
JMP __StdLib__FileIO__Open
POP $this:1
RTN
LBL _File_OpenBinaryWrite
POPB $this
POP @1
PUSH "wb"
PUSH @1
JMP __StdLib__FileIO__Open
POP $this:1
RTN
LBL _File_WriteLine
POPB $this
POP @1
PUSH $this:1
PUSH @1
JMP __StdLib__FileIO__WriteLine
RTN
LBL _File_ReadLine
POPB $this
PUSH $this:1
JMP __StdLib__FileIO__ReadLine
RTN
LBL _File_Close
POPB $this
PUSH $this:1
JMP __StdLib__FileIO__Close
RTN
LBL _File_Eof
POPB $this
PUSH $this:1
JMP __StdLib__FileIO__Eof
RTN
LBL _File_WriteInt
POPB $this
POP @1
PUSH $this:1
PUSH @1
JMP __StdLib__FileIO__WriteInt
RTN
LBL _File_ReadInt
POPB $this
PUSH $this:1
JMP __StdLib__FileIO__ReadInt
RTN
LBL _File_WriteDouble
POPB $this
POP @1
PUSH $this:1
PUSH @1
JMP __StdLib__FileIO__WriteDouble
RTN
LBL _File_ReadDouble
POPB $this
PUSH $this:1
JMP __StdLib__FileIO__ReadDouble
RTN
LBL _File_WriteBool
POPB $this
POP @1
PUSH $this:1
PUSH @1
JMP __StdLib__FileIO__WriteBool
RTN
LBL _File_ReadBool
POPB $this
PUSH $this:1
JMP __StdLib__FileIO__ReadBool
RTN
LBL _File_WriteString
POPB $this
POP @1
PUSH $this:1
PUSH @1
JMP __StdLib__FileIO__WriteString
RTN
LBL _File_ReadString
POPB $this
PUSH $this:1
JMP __StdLib__FileIO__ReadString
RTN
LBL _Math_ABS
POP @1
PUSH @1
JMP __StdLib__Math__Abs
RTN
LBL _Math_FLOOR
POP @1
PUSH @1
JMP __StdLib__Math__Floor
RTN
LBL _Math_CEILING
POP @1
PUSH @1
JMP __StdLib__Math__Ceiling
RTN
LBL _Array_Init
POPB $this
PUSH 1
NEG
POP $this:1
RTN
LBL _Array_Alloc
POPB $this
POP @1
PUSH @1
JMP __StdLib__Utilities__Array__New
POP $this:1
RTN
LBL _Array_Free
POPB $this
PUSH $this:1
JMP __StdLib__Utilities__Array__Free
RTN
LBL _Array_Size
POPB $this
PUSH $this:1
JMP __StdLib__Utilities__Array__Count
RTN
LBL _Array_SetItem
POPB $this
POP @2
POP @1
PUSH $this:1
PUSH @2
PUSH @1
JMP __StdLib__Utilities__Array__SetItem
RTN
LBL _Array_GetItem
POPB $this
POP @1
PUSH $this:1
PUSH @1
JMP __StdLib__Utilities__Array__GetItem
RTN
LBL Main
ABS $ts1, 0
PUSHB $ts1
JMP _TestClass1_Init
ABS $ts2, 0
PUSHB $ts2
JMP _TestClass2_Init
PUSHB $ts1
PUSHB $ts2
JMP _TestClass2_DoSomething
ABS $f1, 1
PUSHB $f1
JMP _File_Init
PUSH "output.txt"
PUSHB $f1
JMP _File_OpenWrite
PUSH "Line 1"
PUSHB $f1
JMP _File_WriteLine
PUSH "Line 2"
PUSHB $f1
JMP _File_WriteLine
PUSHB $f1
JMP _File_Close
ABS $f2, 1
PUSHB $f2
JMP _File_Init
PUSH "output.txt"
PUSHB $f2
JMP _File_OpenRead
PUSHB $f2
JMP _File_ReadLine
JMP _Console_WriteLine
PUSHB $f2
JMP _File_ReadLine
JMP _Console_WriteLine
PUSHB $f2
JMP _File_Close
PUSH "Function Main() Start"
JMP _Console_WriteLine
ABS $builder, 2
PUSHB $builder
JMP _BaseObject_Init
PUSHB $builder
JMP _String_Init
PUSHB $builder
JMP _StringBuilder_Init
PUSH "Line 1"
PUSHB $builder
JMP _StringBuilder_AppendLine
PUSH "Line 2"
PUSHB $builder
JMP _StringBuilder_AppendLine
PUSH "Line 3"
PUSHB $builder
JMP _StringBuilder_AppendLine
PUSH "Live 4"
PUSHB $builder
JMP _StringBuilder_AppendLine
PUSH $builder:2
JMP _Console_WriteLine
PUSH "Enter a String: "
JMP _Console_WriteLine
AB %Input
JMP _Console_ReadLine
POP %Input
PUSH "You Entered: "
JMP _Console_Write
PUSH %Input
JMP _Console_WriteLine
PUSH "Goodbye."
JMP _Console_WriteLine
PUSH "Function Main() End"
JMP _Console_WriteLine
AB %myVar
PUSH 189
PUSH 232
PUSH 34
DIV
PUSH 83
ADD
PUSH 34
SUB
MUL
PUSH 45
MUL
POP %myVar
PUSH %myVar
JMP _Console_WriteLine
AB %myVar2
PUSH 8
PUSH 3
MOD
POP %myVar2
PUSH %myVar2
JMP _Console_WriteLine
PUSH 23
NEG
JMP _Math_ABS
JMP _Console_WriteLine
AB %myVar3
PUSH 125
PUSH 4
PUSH 3
ADD
PUSH 28
PUSH 4
SUB
NEG
PUSH 5
MUL
MUL
PUSH 88
ADD
MUL
POP %myVar3
PUSH %myVar3
JMP _Console_WriteLine
ABS $ar1, 1
PUSHB $ar1
JMP _Array_Init
PUSH 5
PUSHB $ar1
JMP _Array_Alloc
PUSH 0
PUSH "Idx 0"
PUSHB $ar1
JMP _Array_SetItem
PUSH 1
PUSH "Idx 1"
PUSHB $ar1
JMP _Array_SetItem
PUSH 2
PUSH "Idx 2"
PUSHB $ar1
JMP _Array_SetItem
PUSH 3
PUSH "Idx 3"
PUSHB $ar1
JMP _Array_SetItem
PUSH 4
PUSH "Idx 4"
PUSHB $ar1
JMP _Array_SetItem
AB %c
PUSH 0
POP %c
AB %s
PUSHB $ar1
JMP _Array_Size
PUSH 1
SUB
POP %s
MOV %c, 0
LBL _ForStart1
PUSH %c
PUSH " - "
CAT
PUSH %c
PUSHB $ar1
JMP _Array_GetItem
CAT
JMP _Console_WriteLine
PUSH 1
PUSH %c
ADD
POP %c
JLE %c, %s, _ForStart1
PUSHB $ar1
JMP _Array_Free
RTN
