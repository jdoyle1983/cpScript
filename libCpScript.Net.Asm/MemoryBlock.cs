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


namespace libCpScript.Net.Asm
{
	public class MemoryBlockSetHeader
	{
		public string Name;
		public List<int> IndexOffset;
		
		public MemoryBlockSetHeader(string _Name, int _Count)
		{
			Name = _Name;
			IndexOffset = new List<int>();
			for(int i = 0; i < _Count; i++)
				IndexOffset.Add(-1);
		}
	}
	
	public class MemoryBlockHeader
	{
		public string Name;
		public int Offset;
		
		public MemoryBlockHeader(string _Name, int _Offset)
		{
			Name = _Name;
			Offset = _Offset;
		}
	}
	
	public class MemoryBlock
	{
		public bool Used = false;
		public string Value = "";
		
		public void SetValue(string Val)
		{
			Used = true;
			Value = Val;
		}
		
		public void Free()
		{
			Used = false;
			Value = "";
		}
	}
}

