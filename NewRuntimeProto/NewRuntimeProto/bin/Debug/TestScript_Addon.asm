; Data Segment allows the creation of variables
[DATA]
MyCounter: 0
JumpSegment: 0

[EXTERNAL]
_console_read
_console_write

[CODE]
	MOV 0, MyCounter
	MOVA JumpSegment
	PUSH MyCounter
	JMPL _console_writeline
	INC MyCounter, 1
	PUSH MyCounter
	PUSH 41
	CMPL
	JMPFT %JumpSegment