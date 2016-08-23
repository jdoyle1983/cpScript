using System;
using System.Collections.Generic;
using System.Text;

namespace NewRuntimeProto
{
    class Program
    {
        static string TestProgram = @"\
; Nothing Here
JMP _Start

_TestLabel:
    POP @1
    MOV &2, @2
    JMP @1

_Start:
    PUSH @1
    PUSH \""Your Stuff\""
    PUSH 'More Stuff'
    PUSHR
    PUSH %CLOC + 2
    JMP _TestLabel
    MOV %Data, @2
    POPR
";
        static void Main(string[] args)
        {
            State tState = new State();
            tState.Load(TestProgram);
        }
    }
}
