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
using System.Threading;

namespace libCpScript.Net.Utils
{
    public static class Extensions
    {
        public static IEnumerable<string> SplitAndKeep(this string s, char[] delims)
        {
            int start = 0;
            int index = 0;

            while ((index = s.IndexOfAny(delims, start)) != -1)
            {
                index++;
                index = Interlocked.Exchange(ref start, index);

                yield return s.Substring(index, start - index - 1);
                yield return s.Substring(start - 1, 1);
            }

            if (start < s.Length)
            {
                yield return s.Substring(start);
            }
        }
    }
}
