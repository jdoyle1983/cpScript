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

