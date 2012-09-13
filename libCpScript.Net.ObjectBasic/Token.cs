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
using libCpScript.Net.Utils;

/*
 Add parameter hints FUNCTION myFunction(firstparam:File, secondparam:MyCustomClass) 
*/

namespace libCpScript.Net.ObjectBasic
{
	public enum TokenType
	{
		ttINVALID = 0,
		
		OpAdd,
		OpSub,
		OpMul,
		OpDiv,
		OpMod,
		OpCat,
		OpNot,
		OpAnd,
		OpOr,
        OpNeg,
		OpEqualTo,
		OpNotEqualTo,
		OpGreaterThan,
		OpGreaterThanOrEqualTo,
		OpLessThan,
		OpLessThanOrEqualTo,
		OpenParen,
		CloseParen,
		OpenBrace,
		CloseBrace,
		OpenBracket,
		CloseBracket,
		Comma,
		Period,
		Assignment,
		Literal,
		QuotedLiteral,
		Or,
		And,
		Colon,
		SemiColon,
		Eol,
		
		
		//Extended Types
		
		ExFunction,
		ExEndFunction,
		
		ExReturn,
		
		ExVar,
		
		ExIf,
		ExElseIf,
		ExElse,
		ExThen,
		ExEndIf,
		
		
		ExFor,
		ExTo,
		ExStep,
		ExNext,
		
		ExWhile,
		ExLoop,
		
		ExEnd,
		
		ExClassAction,
		
		ExClass,
		ExEndClass,
		ExMethod,
		ExEndMethod,
		ExProperty,
		ExExtends,
		ExOverride,
        ExStatic,

        ExTypeHint,
	}
	
	public class Token
	{
		public TokenType Type { get;set; }
		public string Value { get;set; }
		
		public Token(TokenType _type, string _value)
		{
			Type = _type;
			Value = _value;
		}
	}
	
	public class Tokenizer
	{
		private static Token TokenBeforeLast 
		{
			get { if(Toks.Count > 1) return Toks[Toks.Count - 2]; return new Token(TokenType.ttINVALID, ""); }
		}
			
		private static Token LastToken
		{
			get { if(Toks.Count > 0) return Toks[Toks.Count - 1]; return new Token(TokenType.ttINVALID, ""); }
		}
		
		private static void ReplaceLastTwoTokens (Token t)
		{
			Toks.RemoveAt(Toks.Count - 1);
			ReplaceLastToken(t);
		}
		
		private static void ReplaceLastToken(Token t)
		{
			Toks[Toks.Count - 1] = t;
		}
		
		private static List<Token> Toks;
		
		public static Token[] Tokenize(string Input)
		{
			char[] cToks = { '+','-','*','/','%','(',')','{','}','[',']','>','<',',','.','!','=','\"','\'','&','|',' ','\n','\r','\t',':',';' };
			
			
			Toks = new List<Token>();
			
			bool inQuotes = false;
			string quoteString = "";
			string quoteChar = "\"";
			
			foreach(string t in Input.SplitAndKeep(cToks))
			{
				if(!inQuotes)
				{
					switch(t)
					{
						case "\n": Toks.Add(new Token(TokenType.Eol, "\n")); break;
						case "+": Toks.Add(new Token(TokenType.OpAdd, t)); break;
						case "-": Toks.Add(new Token(TokenType.OpSub, t)); break;
						case "*": Toks.Add(new Token(TokenType.OpMul, t)); break;
						case "/": Toks.Add(new Token(TokenType.OpDiv, t)); break;
						case "%": Toks.Add(new Token(TokenType.OpMod, t)); break;
						case "(": Toks.Add(new Token(TokenType.OpenParen, t)); break;
						case ")": Toks.Add(new Token(TokenType.CloseParen, t)); break;
						case "{": Toks.Add(new Token(TokenType.OpenBrace, t)); break;
						case "}": Toks.Add(new Token(TokenType.CloseBrace, t)); break;
						case "[": Toks.Add(new Token(TokenType.OpenBracket, t)); break;
						case "]": Toks.Add(new Token(TokenType.CloseBracket, t)); break;
						case ">": Toks.Add(new Token(TokenType.OpGreaterThan, t)); break;
						case "<": Toks.Add(new Token(TokenType.OpLessThan, t)); break;
						case ",": Toks.Add(new Token(TokenType.Comma, t)); break;
						case "!": Toks.Add(new Token(TokenType.OpNot, t)); break;
						case ":": Toks.Add(new Token(TokenType.Colon, t)); break;
						case ";": Toks.Add(new Token(TokenType.SemiColon, t)); break;
						case ".": 
						{
							if(LastToken.Type == TokenType.Period)
								ReplaceLastToken(new Token(TokenType.OpCat, ".."));
							else
								Toks.Add(new Token(TokenType.Period, t));
						} break;
						case "=":
						{
							if(LastToken.Type == TokenType.OpGreaterThan)
								ReplaceLastToken(new Token(TokenType.OpGreaterThanOrEqualTo, ">="));
							else if(LastToken.Type == TokenType.OpLessThan)
								ReplaceLastToken(new Token(TokenType.OpLessThanOrEqualTo, "<="));
							else if(LastToken.Type == TokenType.OpNot)
								ReplaceLastToken(new Token(TokenType.OpNotEqualTo, "!="));
							else if(LastToken.Type == TokenType.Assignment)
								ReplaceLastToken(new Token(TokenType.OpEqualTo, "=="));
							else
								Toks.Add(new Token(TokenType.Assignment, t));
						} break;
						case "&":
						{
							if(LastToken.Type == TokenType.And)
								ReplaceLastToken(new Token(TokenType.OpAnd, "&&"));
							else
								Toks.Add(new Token(TokenType.And, "&"));
						} break;
						case "|":
						{
							if(LastToken.Type == TokenType.Or)
								ReplaceLastToken(new Token(TokenType.OpOr, "||"));
							else
								Toks.Add(new Token(TokenType.Or, "|"));
						} break;
						case "\"": inQuotes = true; quoteString = ""; quoteChar = "\""; break;
						case "\'": inQuotes = true; quoteString = ""; quoteChar = "\'"; break;
						default:
						{
							bool AddLit = true;
							if(t.Trim() != "")
							{
								if(LastToken.Type == TokenType.Period && TokenBeforeLast.Type == TokenType.Literal)
								{
									bool CanConvert1 = false;
									bool CanConvert2 = false;
									int ti = 0;
									try
									{
										ti = Convert.ToInt32(TokenBeforeLast.Value);
										CanConvert1 = true;
									} catch{}
									try
									{
										ti = Convert.ToInt32(t);
										CanConvert2 = true;
									} catch{}
									
									if(CanConvert1 && CanConvert2)
									{
										AddLit = false;
										ReplaceLastTwoTokens(new Token(TokenType.Literal, TokenBeforeLast.Value.Trim() + "." + t.Trim()));
									}
								}
							}
							else
								AddLit = false;
							if(AddLit)
								Toks.Add(new Token(TokenType.Literal, t));
						}break;
					}
				}
				else
				{
					if(t == quoteChar)
					{
						inQuotes = false;
						Toks.Add(new Token(TokenType.QuotedLiteral, quoteString));
					}
					else
						quoteString += t;
				}
			}
			
			return Toks.ToArray();
		}
		
		public static Token[] ParseExtended(Token[] Input)
		{
			Toks = new List<Token>();
			for(int count = 0; count < Input.Length; count++)
			{
				Token t = Input[count];
				if(	t.Type == TokenType.Period && 
				   	(count > 0) && 
				   	((count + 1) < Input.Length) && 
				   	Input[count - 1].Type == TokenType.Literal && 
				   	Input[count + 1].Type == TokenType.Literal)
				{
					string p1 = Input[count - 1].Value;
					string p2 = Input[count + 1].Value;
					Toks.RemoveAt(Toks.Count - 1);
					Toks.Add(new Token(TokenType.ExClassAction, p1 + "." + p2));
					count++;
				}
				else if(t.Type == TokenType.Literal)
				{
					switch(t.Value.ToLower().Trim())
					{
					case "function":
					{
						if(LastToken.Type == TokenType.ExEnd)
							ReplaceLastToken(new Token(TokenType.ExEndFunction, "END FUNCTION"));
						else
							Toks.Add(new Token(TokenType.ExFunction, "FUNCTION"));
						} break;
					case "end":
					{
						Toks.Add(new Token(TokenType.ExEnd, "END"));
						} break;
					case "if":
					{
						if(LastToken.Type == TokenType.ExEnd)
							ReplaceLastToken(new Token(TokenType.ExEndIf, "END IF"));
						else if(LastToken.Type == TokenType.ExElse)
							ReplaceLastToken(new Token(TokenType.ExElseIf, "ELSE IF"));
						else
							Toks.Add(new Token(TokenType.ExIf, "IF"));
						} break;
					case "else": Toks.Add(new Token(TokenType.ExElse, "ELSE")); break;
					case "then": Toks.Add(new Token(TokenType.ExThen, "THEN")); break;
					case "while": Toks.Add(new Token(TokenType.ExWhile, "WHILE")); break;
					case "loop": Toks.Add(new Token(TokenType.ExLoop, "LOOP")); break;
					case "for": Toks.Add(new Token(TokenType.ExFor, "FOR")); break;
					case "to": Toks.Add(new Token(TokenType.ExTo, "TO")); break;
					case "step": Toks.Add(new Token(TokenType.ExStep, "STEP")); break;
					case "next": Toks.Add(new Token(TokenType.ExNext, "NEXT")); break;
					case "var": Toks.Add(new Token(TokenType.ExVar, "VAR")); break;
					case "return": Toks.Add(new Token(TokenType.ExReturn, "RETURN")); break;
					case "class":
					{
						if(LastToken.Type == TokenType.ExEnd)
							ReplaceLastToken(new Token(TokenType.ExEndClass, "END CLASS"));
						else
							Toks.Add(new Token(TokenType.ExClass, "CLASS")); 
					}break;
					case "prop": Toks.Add(new Token(TokenType.ExProperty, "PROP")); break;
					case "method": 
					{
						if(LastToken.Type == TokenType.ExEnd)
							ReplaceLastToken(new Token(TokenType.ExEndMethod, "END METHOD"));
						else
							Toks.Add(new Token(TokenType.ExMethod, "METHOD"));
					}break;
					case "extends": Toks.Add(new Token(TokenType.ExExtends, "EXTENDS")); break;
					case "override": Toks.Add(new Token(TokenType.ExOverride, "OVERRIDE")); break;
                    case "static": Toks.Add(new Token(TokenType.ExStatic, "STATIC")); break;
                    default:
                        {
                            if (LastToken.Type == TokenType.Colon)
                                ReplaceLastToken(new Token(TokenType.ExTypeHint, t.Value));
                            else
                                Toks.Add(t);
                        } break;
					}
				}
				else
					Toks.Add(t);
			}
			
			return Toks.ToArray();
		}
	}
}

