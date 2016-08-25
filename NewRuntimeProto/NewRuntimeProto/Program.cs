using System;
using System.Collections.Generic;
using System.Text;

namespace NewRuntimeProto
{
    class Program
    {
        static void Main(string[] args)
        {
            State tState = new State();
            tState.Load(System.IO.File.ReadAllText(AppDomain.CurrentDomain.BaseDirectory + "TestScript.asm"));
        }
    }
}
