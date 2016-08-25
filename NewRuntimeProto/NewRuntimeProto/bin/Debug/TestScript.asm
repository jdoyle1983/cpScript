[DATA]
Var1: "Var Data 1"
Var2: 11
Var3: false
Var4: 12.01
[EXTERNAL]
_console_readline
_console_read
_console_write
_console_writeline
[CODE]
; Nothing Here
JMP _Start

_TestLabel:
    POP @1
    MOV &2, @2
    JMP @1
	
_Start:
    PUSH @1
    PUSH "Your Stuff"
    PUSH 'More Stuff'
    PUSHR
    PUSH %CLOC + 2
    JMP _TestLabel
    MOV %Data, @2
    POPR