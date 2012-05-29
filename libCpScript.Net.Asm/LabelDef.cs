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

