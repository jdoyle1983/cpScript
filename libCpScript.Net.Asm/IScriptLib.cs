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

/*
    This interface is used for extending cpScript
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace libCpScript.Net.Asm
{
    public interface IScriptLib
    {
        string Author();            //Should Report the Author of the Library
        string Version();           //Should Report the Current Version of the Library
        void Init(State Script);    //All initialization should happen here (registering static functions for use)
    }
}
