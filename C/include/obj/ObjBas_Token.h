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

#ifndef __OBJBAS_TOKEN_H__
#define __OBJBAS_TOKEN_H__

#include <List.h>

enum TokenType
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
	
	ExAsmBlock,
	ExEndAsmBlock,
	ExAsmPush,
	ExAsmPop,
};


typedef struct
{
	int Type;
	char* Value;
} Token;

Token* Token_New(int type);
Token* Token_NewWithValue(int type, char* value);
Token* Token_Copy(Token* src);
void Token_SetValue(Token* token, char* value);
void Token_Delete(Token* token);

List* Tokenizer_Tokenize(char* SrcScript);
List* Tokenizer_ParseExtended(List* Input);


#endif