using System;
using System.Collections.Generic;

namespace libCpScript.Net.ObjectBasic
{
	public class CodeBlock
	{
		public Token[] Tokens;
		
		public CodeBlock(Token[] Toks)
		{
			Tokens = Toks;
			foreach(Token t in Tokens)
				if(t.Type != TokenType.QuotedLiteral)
					t.Value = t.Value.Trim();
		}
	}
	
	public class BlockBuilder
	{
		public static CodeBlock[] ConvertToBlocks(Token[] Toks)
		{
			List<CodeBlock> Blocks = new List<CodeBlock>();
			List<Token> Current = new List<Token>();
			for(int i = 0; i < Toks.Length; i++)
			{
				if(Toks[i].Type == TokenType.Eol)
				{
					if(Current.Count > 0)
					{
						Blocks.Add(new CodeBlock(Current.ToArray()));
						Current.Clear();
					}
				}
				else
					Current.Add(Toks[i]);
			}
			
			if(Current.Count > 0)
				Blocks.Add(new CodeBlock(Current.ToArray()));
			
			return Blocks.ToArray();
		}
	}
}

