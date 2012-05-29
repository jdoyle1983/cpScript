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

namespace libCpScript.Net.Asm
{
	public enum Token
	{
		Invalid = 0,
		Push,
		Pop,
		Move,
		Add,
		Mul,
		Div,
		Sub,
		Mod,
		Cat,
        Neg,
		Label,
		Jump,
		LongJump,
		JumpOffset,
		LongJumpOffset,
		Return,
		JumpE,
		JumpN,
		JumpG,
		JumpGE,
		JumpL,
		JumpLE,
		CmpE,
		CmpN,
		CmpG,
		CmpGE,
		CmpL,
		CmpLE,
		CmpO,
		CmpA,
		CmpI,
		Alloc,
		Ref,
		Dealloc,
		PushA,
		PopA,
		AllocBlockSet,
		RefBlockSet,
		DeallocBlockSet,
		PushB,
		PopB,
		
		
		Comma,
		Literal,
		QuotedLiteral,
		
		Register,
		MemoryVar,
		
		EndOfExec,

        Lib,
	}
	
	public class AssemblyToken
	{
		public Token Tok = Token.Invalid;
		public string Val = "";
		
		public AssemblyToken(Token _Tok, string _Val)
		{
			Tok = _Tok;
			Val = _Val;
		}
	}
}

