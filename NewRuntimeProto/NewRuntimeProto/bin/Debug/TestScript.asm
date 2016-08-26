; Data Segment allows the creation of variables
[DATA]
ArrAddress: 0
ArrCounter: 0
ArrSize: 10000
Arr: [10000]

[EXTERNAL]
_console_readline
_console_writeline


[CODE]
JMPL _Start

_Start:
	MOV &Arr, ArrAddress
_Count1_Start:
	PUSH " Counter: "
	PUSH ArrCounter
	PUSH 1
	ADD
	CON
	POP %ArrAddress + ArrCounter
	INC ArrCounter, 1
	PUSH ArrCounter
	PUSH ArrSize
	CMPL
	JMPLT _Count1_Start

	MOV 0, ArrCounter
	
_Count2_Start:
	PUSH %ArrAddress + ArrCounter
	JMPL _console_writeline
	INC ArrCounter, 1
	PUSH ArrCounter
	PUSH ArrSize
	CMPL
	JMPLT _Count2_Start
	
	