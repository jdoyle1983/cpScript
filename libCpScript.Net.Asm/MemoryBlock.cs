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
    //An array of memory (continous block of memory)
	public class MemoryBlockSetHeader
	{
		public string Name;             //Title of the memory
		public List<int> IndexOffset;   //Index of memory block
		
        //Create new memory block set
		public MemoryBlockSetHeader(string _Name, int _Count)
		{
			Name = _Name;
            //Set inital memory offset to an invalid value
			IndexOffset = new List<int>();
			for(int i = 0; i < _Count; i++)
				IndexOffset.Add(-1);
		}
	}
	
    //A single block of memory
	public class MemoryBlockHeader
	{
		public string Name;             //Title of the memory
		public int Offset;              //Offset of memory
		
        //Create new memory block 
		public MemoryBlockHeader(string _Name, int _Offset)
		{
			Name = _Name;
			Offset = _Offset;
		}
	}
	
    //The physical memory block
	public class MemoryBlock
	{
		public bool Used = false;       //Is the block used
		public string Value = "";       //Value of the block
		
        //Set the value of the block
		public void SetValue(string Val)
		{
			Used = true;
			Value = Val;
		}
		
        //Free the block
		public void Free()
		{
			Used = false;
			Value = "";
		}
	}
}

