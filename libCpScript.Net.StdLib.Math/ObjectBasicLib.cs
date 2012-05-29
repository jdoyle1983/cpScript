using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using libCpScript.Net.Asm;

namespace libCpScript.Net.StdLib
{
    public class MathLib : IScriptLib
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
            Script.RegisterFunction("__StdLib__Math__Abs", MathLib.Abs, false);
            Script.RegisterFunction("__StdLib__Math__Floor", MathLib.Floor, false);
            Script.RegisterFunction("__StdLib__Math__Ceiling", MathLib.Ceiling, false);
        }

        #endregion

        public static void Abs(State state)
        {
            double val = state.PopDouble();
            double r = Math.Abs(val);
            state.PushDouble(r);
        }

        public static void Floor(State state)
        {
            double val = state.PopDouble();
            double r = Math.Floor(val);
            state.PushDouble(r);
        }

        public static void Ceiling(State state)
        {
            double val = state.PopDouble();
            double r = Math.Ceiling(val);
            state.PushDouble(r);
        }
    }
}
