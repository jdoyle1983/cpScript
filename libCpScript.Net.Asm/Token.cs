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

