/*

    libCpScript.Asm,
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
	
	tResBlockSet,
	
	tDbgLine,
	tDbgColumn,
	tDbgFile,
};

#endif
