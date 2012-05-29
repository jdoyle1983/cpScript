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
