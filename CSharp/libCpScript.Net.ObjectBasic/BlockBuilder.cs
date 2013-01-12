/*

	libCpScript.Net.ObjectBasic
    Copyright (C) 2013 Jason Doyle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Contact Information:

    Original Author: Jason Doyle (jdoyle1983@gmail.com)
*/

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

