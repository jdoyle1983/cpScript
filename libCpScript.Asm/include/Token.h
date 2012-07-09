#ifndef __TOKEN_H__
#define __TOKEN_H__

enum Token //Token values
{
	tInvalid = 0,
	tPush,
	tPop,
	tMove,
	tAdd,
	tMul,
	tDiv,
	tSub,
	tMod,
	tCat,
	tNeg,
	tLabel,
	tJump,
	tLongJump,
	tJumpOffset,
	tLongJumpOffset,
	tReturn,
	tJumpE,
	tJumpN,
	tJumpG,
	tJumpGE,
	tJumpL,
	tJumpLE,
	tCmpE,
	tCmpN,
	tCmpG,
	tCmpGE,
	tCmpL,
	tCmpLE,
	tCmpO,
	tCmpA,
	tCmpI,
	tAlloc,
	tRef,
	tDealloc,
	tPushA,
	tPopA,
	tAllocBlockSet,
	tRefBlockSet,
	tDeallocBlockSet,
	tPushB,
	tPopB,


	tComma,
	tLiteral,
	tQuotedLiteral,

	tRegister,
	tMemoryVar,

	tEndOfExec,

	tLib,
};

#endif
