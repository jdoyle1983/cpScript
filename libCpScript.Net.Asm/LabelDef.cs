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
namespace libCpScript.Net.Asm
{
	public class LabelDef
	{
		public string Label = "";
		public int Offset = -1;
		public UserFunction Function = null;
        public bool IsStub = false;
		
		public LabelDef (string _Label, int _Offset)
		{
			Label = _Label;
			Offset = _Offset;
		}
		
		public LabelDef(string _Label, UserFunction _Function, bool StubFunc)
		{
			Label = _Label;
			Function = _Function;
            IsStub = StubFunc;
		}
	}
}

