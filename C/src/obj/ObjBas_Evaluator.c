/*

    libCpScript.ObjectBasic
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

#include <ObjBas_Evaluator.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <Stack.h>
#include <Extensions.h>
#include <stdlib.h>

int OpPreced(Token* c)
{
	int t = c->Type;
	
	if(	t == OpNot || t == OpNeg ) return 5;
	
	if(	t == OpEqualTo 	|| t == OpNotEqualTo ||
		t == OpGreaterThan || t == OpGreaterThanOrEqualTo ||
		t == OpLessThan || t == OpLessThanOrEqualTo ||
		t == OpMul || t == OpDiv || t == OpMod) return 4;
	
	if(	t == OpAnd || t == OpOr ) return 3;
	
	if(	t == OpAdd || t == OpSub || t == OpCat ) return 2;
	
	if( t == Assignment ) return 1;
	
	return 0;
};

short OpLeftAssoc(Token* c)
{
	int t = c->Type;
	
	if(	t == OpMul || t == OpDiv || t == OpMod || t == OpAdd ||
		t == OpSub || t == OpAnd || t == OpOr || t == OpCat ||
		t == OpEqualTo || t == OpNotEqualTo || t == OpGreaterThan ||
		t == OpGreaterThanOrEqualTo || t == OpLessThan || 
		t == OpLessThanOrEqualTo ) return 1;
	
	if(	t == Assignment || t == OpNot || t == OpNeg ) return 0;
	
	return 0;
};

int OpArgCount(Token* c)
{
	int t = c->Type;
	
	if(	t == OpMul || t == OpDiv || t == OpMod || t == OpAdd ||
		t == OpSub || t == OpAnd || t == OpOr || t == OpCat ||
		t == OpEqualTo || t == OpNotEqualTo ||
		t == OpGreaterThan || t == OpGreaterThanOrEqualTo ||
		t == OpLessThan || t == OpLessThanOrEqualTo ||
		t == Assignment ) return 2;
		
	if(	t == OpNot || t == OpNeg ) return 1;
	
	return 0;
};

short IsOperator(Token* c)
{
	int t = c->Type;
	
	if(	t == OpMul || t == OpDiv || t == OpMod || t == OpAdd ||
		t == OpSub || t == OpAnd || t == OpOr || t == OpCat ||
		t == OpNotEqualTo || t == OpEqualTo ||
		t == OpGreaterThan || t == OpGreaterThanOrEqualTo ||
		t == OpLessThan || t == OpLessThanOrEqualTo ||
		t == Assignment || t == OpNot || t == OpNeg ) return 1;
		
	return 0;
};

short IsFunction(Token* c, List* ValidFunctions)
{
	int t = c->Type;
	
	if(t == Literal || t == ExClassAction)
	{
		size_t i = 0;
		for(i = 0; i < List_Count(ValidFunctions); i++)
			if(strcmp(List_StringAtIndex(ValidFunctions, i), c->Value) == 0)
				return 1;
	}
	
	return 0;
};

short IsSpecial(Token* c)
{
	int t = c->Type;
	
	if(	t == OpenParen || t == Comma || t == CloseParen) return 1;
	
	return 0;
};

short IsIdent(Token* c, List* ValidFunctions)
{
	if(IsOperator(c) == 0 && IsFunction(c, ValidFunctions) == 0 && IsSpecial(c) == 0)
		return 1;
		
	return 0;
};

List* Evaluator_Parse(List* Input, List* ValidFunctions)
{
	//Pre parse negative
	List* PreParsedTokens = List_New();
	int lastTokenType = ttINVALID;
	size_t i = 0;
	for(i = 0; i < List_Count(Input); i++)
	{
		Token* t = List_TokenAtIndex(Input, i);
		if(t->Type == OpSub)
		{
			if(	lastTokenType == OpMul || lastTokenType == OpDiv ||
				lastTokenType == OpMod || lastTokenType == OpAdd ||
				lastTokenType == OpSub || lastTokenType == OpCat ||
				lastTokenType == OpenParen || lastTokenType == ttINVALID) t->Type = OpNeg;
		}
		
		List_Add(PreParsedTokens, t);
		lastTokenType = t->Type;
	}
	
	Stack* stack = Stack_New();
	Stack* output = Stack_New();
	
	Token* sc;
	
	short lpe = 0;
	short rpe = 0;
	
	for(i = 0; i < List_Count(PreParsedTokens); i++)
	{
		Token* c = List_TokenAtIndex(PreParsedTokens, i);
		int t = c->Type;
		char* trimmedValue = StrTrim(c->Value);
		
		if( (t != Literal && t != QuotedLiteral) || (strcmp(trimmedValue, "") != 0 || t == QuotedLiteral) )
		{
			if(IsIdent(c, ValidFunctions) == 1)
				Stack_Push(output, c);
			else if(IsFunction(c, ValidFunctions) == 1)
				Stack_Push(stack, c);
			else if(t == Comma)
			{
				lpe = 0;
				while(Stack_Count(stack) > 0)
				{
					sc = Stack_PeekToken(stack);
					if(sc->Type == OpenParen)
					{
						lpe = 1;
						break;
					}
					else
						Stack_Push(output, Stack_Pop(stack));
				}
				
				if(!lpe)
				{
					printf("***EXCEPTION: Separator or Parentheses Mismatched.");
					exit(0);
				}
			}
			else if(IsOperator(c) == 1)
			{
				while(Stack_Count(stack) > 0)
				{
					sc = Stack_PeekToken(stack);
					if(IsOperator(sc) == 1 && ((OpLeftAssoc(c) == 1 && (OpPreced(c) <= OpPreced(sc))) || (OpLeftAssoc(c) == 1 && (OpPreced(c) < OpPreced(sc)))))
						Stack_Push(output, Stack_Pop(stack));
					else
						break;
				}
				Stack_Push(stack, c);
			}
			else if(t == OpenParen)
				Stack_Push(stack, c);
			else if(t == CloseParen)
			{
				rpe = 0;
				while(Stack_Count(stack) > 0)
				{
					sc = Stack_PeekToken(stack);
					if(sc->Type == OpenParen)
					{
						rpe = 1;
						break;
					}
					else
						Stack_Push(output, Stack_Pop(stack));
				}
				
				if(rpe == 0)
				{
					printf("***EXCEPTIONI: Parentheses Mismatch");
					exit(0);
				}
				Stack_Pop(stack);
				if(Stack_Count(stack) > 0)
				{
					sc = Stack_PeekToken(stack);
					if(IsFunction(sc, ValidFunctions) == 1)
						Stack_Push(output, Stack_Pop(stack));
				}
			}
			else
			{
				printf("***EXCEPTION: Unknown Token '%s'\n", c->Value);
				exit(0);
			}
		}
		
		free(trimmedValue);
	}
	
	while(Stack_Count(stack) > 0)
	{
		sc = Stack_PopToken(stack);
		if(sc->Type == OpenParen || sc->Type == CloseParen)
		{
			printf("***EXCEPTION: Parentheses Mismatched");
			exit(0);
		}
		Stack_Push(output, sc);
	}
	
	List* rValue = List_New();
	while(Stack_Count(output) > 0)
		List_AddInFront(rValue, Stack_Pop(output));
		
	Stack_Delete(output);
	Stack_Delete(stack);
	List_Delete(PreParsedTokens);
	
	return rValue;
};