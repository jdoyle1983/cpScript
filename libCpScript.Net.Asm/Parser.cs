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

namespace libCpScript.Net.Asm
{
	public class Parser
	{
		public static AssemblyToken[] Parse(string Input)
		{
			List<AssemblyToken> Toks = new List<AssemblyToken>();
			bool inQuotes = false;
			string quoteChar = "";
			string quotedString = "";
			foreach(string s in Input.SplitAndKeep(new char[] { ' ', ',', '\n', '\r', '\'', '"' }))
			{
				string tVal = s.ToLower().Trim();
				
				if(!inQuotes)
				{
					if(tVal == "" && Toks.Count > 0 && Toks[Toks.Count - 1].Tok == Token.Literal && s != "\n" && s != "\r")
						Toks[Toks.Count - 1].Val += s;
					else if(tVal == "")
					{
					}
					else if(tVal == "'")
					{
						inQuotes = true;
						quotedString = "";
						quoteChar = "'";
					}
					else if(tVal == "\"")
					{
						inQuotes = true;
						quotedString = "";
						quoteChar = "\"";
					}
					else
					{
						Token tok = Token.Literal;
						switch(tVal)
						{
						case "push": tok = Token.Push; break;
						case "pop": tok = Token.Pop; break;
						case "mov": tok = Token.Move; break;
						case "add": tok = Token.Add; break;
						case "mul": tok = Token.Mul; break;
						case "div": tok = Token.Div; break;
						case "sub": tok = Token.Sub; break;
						case "mod": tok = Token.Mod; break;
						case "cat": tok = Token.Cat; break;
                        case "neg": tok = Token.Neg; break;
						case "lbl": tok = Token.Label; break;
						case "jmp": tok = Token.Jump; break;
						case "ljmp": tok = Token.LongJump; break;
						case "jmpo": tok = Token.JumpOffset; break;
						case "ljmpo": tok = Token.LongJumpOffset; break;
						case "rtn": tok = Token.Return; break;
						case "je": tok = Token.JumpE; break;
						case "jn": tok = Token.JumpN; break;
						case "jg": tok = Token.JumpG; break;
						case "jge": tok = Token.JumpGE; break;
						case "jl": tok = Token.JumpL; break;
						case "jle": tok = Token.JumpLE; break;
						case "ce": tok = Token.CmpE; break;
						case "cn": tok = Token.CmpN; break;
						case "cg": tok = Token.CmpG; break;
						case "cge": tok = Token.CmpGE; break;
						case "cl": tok = Token.CmpL; break;
						case "cle": tok = Token.CmpLE; break;
						case "co": tok = Token.CmpO; break;
						case "ca": tok = Token.CmpA; break;
						case "ci": tok = Token.CmpI; break;
						case "ab": tok = Token.Alloc; break;
						case "rb": tok = Token.Ref; break;
						case "fb": tok = Token.Dealloc; break;
						case "pusha": tok = Token.PushA; break;
						case "popa": tok = Token.PopA; break;
						case "abs": tok = Token.AllocBlockSet; break;
						case "rbs": tok = Token.RefBlockSet; break;
						case "fbs": tok = Token.DeallocBlockSet; break;
						case "pushb": tok = Token.PushB; break;
						case "popb": tok = Token.PopB; break;
						case ",": tok = Token.Comma; break;
						case "eoe": tok = Token.EndOfExec; break;
                        case "lib": tok = Token.Lib; break;
						default:
						{
							if(tVal.StartsWith("@"))
								tok = Token.Register;
							else if(tVal.StartsWith("%"))
								tok = Token.MemoryVar;
							else
								tok = Token.Literal;
							}break;
						}
						Toks.Add(new AssemblyToken(tok, s));
					}
				}
				else
				{
					if(tVal == quoteChar)
					{
						inQuotes = false;
						quoteChar = "";
						Toks.Add(new AssemblyToken(Token.QuotedLiteral, quotedString));
						quotedString = "";
					}
					else
						quotedString += s;
				}
			}
			return Toks.ToArray();
		}
	}
}

