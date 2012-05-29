using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using libCpScript.Net.Asm;

namespace libCpScript.Net.StdLib
{
    public class ConsoleIO : IScriptLib
    {
        #region IScriptLib Members

        public string Author()
        {
            return "Jason Doyle";
        }

        public string Version()
        {
            return "1.0.0.0";
        }

        public void Init(State Script)
        {
            Script.RegisterFunction("__StdLib__ConsoleIO__Write", ConsoleIO.Write, false);
            Script.RegisterFunction("__StdLib__ConsoleIO__WriteLine", ConsoleIO.WriteLine, false);
            Script.RegisterFunction("__StdLib__ConsoleIO__ReadLine", ConsoleIO.ReadLine, false);
        }

        #endregion

        public static void Write(State state)
        {
            Console.Write(state.PopString());
        }

        public static void WriteLine(State state)
        {
            Console.WriteLine(state.PopString());
        }

        public static void ReadLine(State state)
        {
            state.PushString(Console.ReadLine());
        }
    }
}
