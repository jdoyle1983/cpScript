/*
This file is part of libCpScript.

    libCpScript is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libCpScript is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libCpScript.  If not, see <http://www.gnu.org/licenses/>.
	
	
	libCpScript
	Original Author: Jason Doyle (jdoyle1983@gmail.com)
*/

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
