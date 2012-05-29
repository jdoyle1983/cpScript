using System;
using System.Collections.Generic;

namespace libCpScript.Net.ObjectBasic
{
    public class FunctionParam
    {
        public bool isClassVar = false;
        public string VarName = "";
        public string ClassName = "";
    }

	public class Function
	{
		public string _Name { get;set; }
		public bool _UserFunction { get;set; }
		public libCpScript.Net.Asm.UserFunction _Func { get;set; }
		public CodeBlock[] _Blocks { get;set; }
		
		public FunctionParam[] _Parameters { get;set; }
		
		public bool _IsOverride { get;set; }
        public bool _IsStatic { get; set; }
        public bool _IsStub { get; set; }
		
		public static Function[] ParseFunctionDefs(CodeBlock[] Blocks)
		{
			
			List<Function> Functions = new List<Function>();
			for(int i = 0; i < Blocks.Length; i++)
			{
				CodeBlock b = Blocks[i];
				List<CodeBlock> blk = new List<CodeBlock>();
				if(b.Tokens[0].Type == TokenType.ExFunction)
				{	
					blk.Clear();
					blk.Add(b);
					if(b.Tokens[1].Type != TokenType.Literal)
						throw new Exception("Expected Function name not found.");
					
					Function d = new Function();
					d._Name = b.Tokens[1].Value;
					d._UserFunction = false;
					
					CodeBlock s = Blocks[i];
					while(s.Tokens[0].Type != TokenType.ExEndFunction)
					{
						i++;
						try
						{
							s = Blocks[i];
							blk.Add(s);
						}
						catch
						{
							throw new Exception("No End Function found.");
						}
					}
					d._Blocks = blk.ToArray();
					Functions.Add(d);
				}
			}
			return Functions.ToArray();
		}
		public Function ()
		{
		}
	}
}

