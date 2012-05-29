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

namespace libCpScript.Net.ObjectBasic
{
	public class Evaluator
	{
		private string[] Functions;
		
		private int OpPreced(Token c)
		{
			switch(c.Type)
			{
            case TokenType.OpNot: case TokenType.OpNeg: return 5; 
			case TokenType.OpEqualTo: case TokenType.OpNotEqualTo: 
			case TokenType.OpGreaterThan: case TokenType.OpGreaterThanOrEqualTo: 
			case TokenType.OpLessThan: case TokenType.OpLessThanOrEqualTo:
			case TokenType.OpMul: case TokenType.OpDiv: case TokenType.OpMod: return 4;
			case TokenType.OpAnd: case TokenType.OpOr: return 3;
			case TokenType.OpAdd: case TokenType.OpSub: case TokenType.OpCat: return 2;
			case TokenType.Assignment: return 1;
			}
			return 0;
		}
		
		private bool OpLeftAssoc(Token c)
		{
			switch(c.Type)
			{
			case TokenType.OpMul: case TokenType.OpDiv: case TokenType.OpMod: case TokenType.OpAdd: 
			case TokenType.OpSub: case TokenType.OpAnd: case TokenType.OpOr: case TokenType.OpCat:
			case TokenType.OpEqualTo: case TokenType.OpNotEqualTo: 
			case TokenType.OpGreaterThan: case TokenType.OpGreaterThanOrEqualTo: 
			case TokenType.OpLessThan: case TokenType.OpLessThanOrEqualTo: return true;
            case TokenType.Assignment: case TokenType.OpNot: case TokenType.OpNeg: return false;
			}
			return false;
		}
		
		private int OpArgCount(Token c)
		{
			switch(c.Type)
			{
			case TokenType.OpMul: case TokenType.OpDiv: case TokenType.OpMod: case TokenType.OpAdd: 
			case TokenType.OpSub: case TokenType.OpAnd: case TokenType.OpOr: case TokenType.OpCat:
			case TokenType.OpEqualTo: case TokenType.OpNotEqualTo: 
			case TokenType.OpGreaterThan: case TokenType.OpGreaterThanOrEqualTo: 
			case TokenType.OpLessThan: case TokenType.OpLessThanOrEqualTo:
			case TokenType.Assignment: { return 2; }
            case TokenType.OpNot: case TokenType.OpNeg: return 1;
			}
			return 0;
		}
		
		private bool IsOperator(Token c)
		{
			switch(c.Type)
			{
			case TokenType.OpMul: case TokenType.OpDiv: case TokenType.OpMod: case TokenType.OpAdd: 
			case TokenType.OpSub: case TokenType.OpAnd: case TokenType.OpOr: case TokenType.OpCat:
			case TokenType.OpNotEqualTo: case TokenType.OpEqualTo:
			case TokenType.OpGreaterThan: case TokenType.OpGreaterThanOrEqualTo: 
			case TokenType.OpLessThan: case TokenType.OpLessThanOrEqualTo:
            case TokenType.Assignment: case TokenType.OpNot: case TokenType.OpNeg: return true;
			}
			
			return false;
		}
		
		private bool IsFunction(Token c)
		{
			if(c.Type == TokenType.Literal || c.Type == TokenType.ExClassAction)
				foreach(string f in Functions)
					if(f == c.Value)
						return true;
			return false;
		}
		
		private bool IsSpecial(Token c)
		{
			if(c.Type == TokenType.OpenParen || c.Type == TokenType.Comma || c.Type == TokenType.CloseParen)
				return true;
			return false;
		}
		
		private bool IsIdent(Token c)
		{
			if(!IsOperator(c) && !IsFunction(c) && !IsSpecial(c))
				return true;
			return false;
		}
		
		public Token[] Parse(Token[] Input, string[] ValidFunctions)
		{
            //Pre sort negative
            List<Token> PreParsedTokens = new List<Token>();
            TokenType lastTokenType = TokenType.ttINVALID;
            foreach (Token t in Input)
            {
                if (t.Type == TokenType.OpSub)
                {
                    switch (lastTokenType)
                    {
                        case TokenType.OpMul:
                        case TokenType.OpDiv:
                        case TokenType.OpMod:
                        case TokenType.OpAdd:
                        case TokenType.OpSub:
                        case TokenType.OpCat:
                        case TokenType.OpenParen:
                        case TokenType.ttINVALID:
                        {
                            t.Type = TokenType.OpNeg;
                        } break;
                    }
                }
                
                PreParsedTokens.Add(t);
                lastTokenType = t.Type;
            }

			Functions = ValidFunctions;
			
			Stack<Token> stack = new Stack<Token>();
			Stack<Token> output = new Stack<Token>();

			Token sc;
			
			bool lpe = false;
			bool rpe = false;
			
			foreach(Token c in PreParsedTokens)
			{
				if((c.Type != TokenType.Literal && c.Type != TokenType.QuotedLiteral) || (c.Value.Trim() != "" || c.Type == TokenType.QuotedLiteral))
				{
					//Console.WriteLine("'" + c.Value + "'");
					if(IsIdent(c))
						output.Push(c);
					else if(IsFunction(c))
						stack.Push(c);
					else if(c.Type == TokenType.Comma)
					{
						lpe = false;
						while(stack.Count > 0)
						{
							sc = stack.Peek();
							if(sc.Type == TokenType.OpenParen)
							{
								lpe = true;
								break;
							}
							else
								output.Push(stack.Pop());
						}
						
						if(!lpe)
							throw new Exception("Separator or Parentheses Mismatched.");
						
					}
					else if(IsOperator(c))
					{
						while(stack.Count >  0)
						{
							sc = stack.Peek();
							if(IsOperator(sc) && ((OpLeftAssoc(c) && (OpPreced(c) <= OpPreced(sc))) || (!OpLeftAssoc(c) && (OpPreced(c) < OpPreced(sc)))))
							{
								output.Push(stack.Pop());
							}
							else
								break;
						}
						stack.Push(c);
					}
					else if(c.Type == TokenType.OpenParen)
						stack.Push(c);
					else if(c.Type == TokenType.CloseParen)
					{
						rpe = false;
						while(stack.Count > 0)
						{
							sc = stack.Peek();
							if(sc.Type == TokenType.OpenParen)
							{
								rpe = true;
								break;
							}
							else
								output.Push(stack.Pop());
						}
						
						if(rpe == false)
						{
							throw new Exception("Parentheses Mismatch");
						}
						stack.Pop();
						if(stack.Count > 0)
						{
							sc = stack.Peek();
							if(IsFunction(sc))
								output.Push(stack.Pop());
						}
					}
					else
						throw new Exception("Unknown Token " + c);
				}
			}
			
			while(stack.Count > 0)
			{
				sc = stack.Pop();
				if(sc.Type == TokenType.OpenParen || sc.Type == TokenType.CloseParen)
					throw new Exception("Parentheses Mismatched");
				output.Push(sc);
			}
			
			List<Token> crValue = new List<Token>();
			while(output.Count > 0)
				crValue.Insert(0, output.Pop());
			
			return crValue.ToArray();
		}
	}
}

