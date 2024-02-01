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

#include <ObjBas_Token.h>
#include <Extensions.h>
#include <malloc.h>
#include <string.h>

Token* Token_New(int type)
{
	Token* t = (Token*)malloc(sizeof(Token));
	t->Type = type;
	t->Value = (char*)malloc(sizeof(char) * 1);
	t->Value[0] = '\0';
	
	return t;
};

Token* Token_NewWithValue(int type, char* value)
{
	Token* t = (Token*)malloc(sizeof(Token));
	t->Type = type;
	t->Value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(t->Value, value);
	return t;
};

Token* Token_Copy(Token* src)
{
	Token* t = (Token*)malloc(sizeof(Token));
	t->Type = src->Type;
	t->Value = (char*)malloc(sizeof(char) * (strlen(src->Value) + 1));
	strcpy(t->Value, src->Value);
	return t;
};

void Token_SetValue(Token* token, char* value)
{
	if(token->Value != NULL)
		free(token->Value);
	token->Value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
	strcpy(token->Value, value);
};

void Token_Delete(Token* token)
{
	if(token->Value != NULL)
		free(token->Value);
	free(token);
};





Token* TokenBeforeLast(List* Toks)
{
	if(List_Count(Toks) >= 2)
		return List_TokenAtIndex(Toks, List_Count(Toks) - 2);
	return Token_New(ttINVALID);
};

Token* LastToken(List* Toks)
{
	if(List_Count(Toks) >= 1)
		return List_TokenAtIndex(Toks, List_Count(Toks) - 1);
	return Token_New(ttINVALID);
};

void ReplaceLastToken(List* Toks, Token* Tok)
{
	int oldIndex = List_Count(Toks) - 1;
	Token* oldToken = List_TokenAtIndex(Toks, oldIndex);
	List_RemoveAt(Toks, oldIndex);
	List_Add(Toks, Tok);
	Token_Delete(oldToken);	
};

void ReplaceLastTwoTokens(List* Toks, Token* Tok)
{
	int oldIndex = List_Count(Toks) - 1;
	Token* oldToken = List_TokenAtIndex(Toks, oldIndex);
	List_RemoveAt(Toks, oldIndex);
	Token_Delete(oldToken);
	ReplaceLastToken(Toks, Tok);
};

List* Tokenizer_Tokenize(char* SrcScript)
{
	List* cToks = SplitAndKeep(SrcScript, "+-*/%(){}[]><,.!=\"\'&| \n\r\t:;");
	List* Toks = List_New();
	
	short InQuotes = 0;
	char* QuoteString = (char*)malloc(sizeof(char) * 1);
	QuoteString[0] = '\0';
	char QuoteChar = '\"';
	
	size_t i = 0;
	for(i = 0; i < List_Count(cToks); i++)
	{
		char* t = List_StringAtIndex(cToks, i);
		
		if(InQuotes == 0)
		{
			if(strcmp(t, "\n") == 0) List_Add(Toks, Token_NewWithValue(Eol, t));
			else if(strcmp(t, "+") == 0) List_Add(Toks, Token_NewWithValue(OpAdd, t));
			else if(strcmp(t, "-") == 0) List_Add(Toks, Token_NewWithValue(OpSub, t));
			else if(strcmp(t, "*") == 0) List_Add(Toks, Token_NewWithValue(OpMul, t));
			else if(strcmp(t, "/") == 0) List_Add(Toks, Token_NewWithValue(OpDiv, t));
			else if(strcmp(t, "%") == 0) List_Add(Toks, Token_NewWithValue(OpMod, t));
			else if(strcmp(t, "(") == 0) List_Add(Toks, Token_NewWithValue(OpenParen, t));
			else if(strcmp(t, ")") == 0) List_Add(Toks, Token_NewWithValue(CloseParen, t));
			else if(strcmp(t, "{") == 0) List_Add(Toks, Token_NewWithValue(OpenBrace, t));
			else if(strcmp(t, "}") == 0) List_Add(Toks, Token_NewWithValue(CloseBrace, t));
			else if(strcmp(t, "[") == 0) List_Add(Toks, Token_NewWithValue(OpenBracket, t));
			else if(strcmp(t, "]") == 0) List_Add(Toks, Token_NewWithValue(CloseBracket, t));
			else if(strcmp(t, ">") == 0) List_Add(Toks, Token_NewWithValue(OpGreaterThan, t));
			else if(strcmp(t, "<") == 0) List_Add(Toks, Token_NewWithValue(OpLessThan, t));
			else if(strcmp(t, ",") == 0) List_Add(Toks, Token_NewWithValue(Comma, t));
			else if(strcmp(t, "!") == 0) List_Add(Toks, Token_NewWithValue(OpNot, t));
			else if(strcmp(t, ":") == 0) List_Add(Toks, Token_NewWithValue(Colon, t));
			else if(strcmp(t, ";") == 0) List_Add(Toks, Token_NewWithValue(SemiColon, t));
			else if(strcmp(t, ".") == 0)
			{
				if(LastToken(Toks)->Type == Period)
					ReplaceLastToken(Toks, Token_NewWithValue(OpCat, ".."));
				else
					List_Add(Toks, Token_NewWithValue(Period, t));
			}
			else if(strcmp(t, "=") == 0)
			{
				Token* lTok = LastToken(Toks);
				if(lTok->Type == OpGreaterThan)
					ReplaceLastToken(Toks, Token_NewWithValue(OpGreaterThanOrEqualTo, ">="));
				else if(lTok->Type == OpLessThan)
					ReplaceLastToken(Toks, Token_NewWithValue(OpLessThanOrEqualTo, "<="));
				else if(lTok->Type == OpNot)
					ReplaceLastToken(Toks, Token_NewWithValue(OpNotEqualTo, "!="));
				else if(lTok->Type == Assignment)
					ReplaceLastToken(Toks, Token_NewWithValue(OpEqualTo, "=="));
				else
					List_Add(Toks, Token_NewWithValue(Assignment, t));
			}
			else if(strcmp(t, "&") == 0)
			{
				if(LastToken(Toks)->Type == And)
					ReplaceLastToken(Toks, Token_NewWithValue(OpAnd, "&&"));
				else
					List_Add(Toks, Token_NewWithValue(And, "&"));
			}
			else if(strcmp(t, "|") == 0)
			{
				if(LastToken(Toks)->Type == Or)
					ReplaceLastToken(Toks, Token_NewWithValue(OpOr, "||"));
				else
					List_Add(Toks, Token_NewWithValue(Or, "|"));
			}
			else if(strcmp(t, "\"") == 0)
			{
				InQuotes = 1;
				QuoteChar = '\"';
			}
			else if(strcmp(t, "\'") == 0)
			{
				InQuotes = 1;
				QuoteChar = '\'';
			}
			else
			{
				short AddLit = 1;
				char* trimmed = StrTrim(t);
				if(strcmp(trimmed, "") != 0)
				{
					Token* lastToken = LastToken(Toks);
					Token* tokenBeforeLast = TokenBeforeLast(Toks);
					
					if(lastToken->Type == Period && tokenBeforeLast->Type == Literal)
					{
						short CanConvert1 = CanConvertToInt(tokenBeforeLast->Value);
						short CanConvert2 = CanConvertToInt(t);
						
						if(CanConvert1 == 1 &&  CanConvert2 == 1)
						{
							AddLit = 0;
							char* trimValue1 = StrTrim(tokenBeforeLast->Value);
							char* trimValue2 = StrTrim(t);
							
							char* newValue = (char*)malloc(sizeof(char) * (strlen(trimValue1) + strlen(trimValue2) + 2));
							strcpy(newValue, trimValue1);
							strcat(newValue, ".");
							strcat(newValue, trimValue2);
							
							ReplaceLastTwoTokens(Toks, Token_NewWithValue(Literal, newValue));
							
							free(newValue);
							free(trimValue2);
							free(trimValue1);
						}					
					}
					
					if(lastToken->Type == ttINVALID)
						Token_Delete(lastToken);
					if(tokenBeforeLast->Type == ttINVALID)
						Token_Delete(tokenBeforeLast);
				}
				else
					AddLit = 0;
					
				free(trimmed);
				if(AddLit)
					List_Add(Toks, Token_NewWithValue(Literal, t));
			}
		}
		else
		{
			if(t[0] == QuoteChar)
			{
				InQuotes = 0;
				List_Add(Toks, Token_NewWithValue(QuotedLiteral, QuoteString));
				free(QuoteString);
				QuoteString = (char*)malloc(sizeof(char) * 1);
				QuoteString[0] = '\0';
			}
			else
			{
				QuoteString = (char*)realloc(QuoteString, sizeof(char) * (strlen(QuoteString) + strlen(t) + 1));
				strcat(QuoteString, t);
			}
		}
	}
	
	
	for(i = 0; i < List_Count(cToks); i++)
		free(List_StringAtIndex(cToks, i));
	List_Delete(cToks);
	
	return Toks;
};

List* Tokenizer_ParseExtended(List* Input)
{
	List* Toks = List_New();
	size_t count = 0;
	for(count = 0; count < List_Count(Input); count++)
	{
		Token* t = List_TokenAtIndex(Input, count);
		if(t->Type == Period && count > 0 && ((count + 1) < List_Count(Input)) && (List_TokenAtIndex(Input, count - 1))->Type == Literal && (List_TokenAtIndex(Input, count + 1))->Type == Literal)
		{
			char* p1 = (List_TokenAtIndex(Input, count - 1))->Value;
			char* p2 = (List_TokenAtIndex(Input, count + 1))->Value;
			char* newValue = (char*)malloc(sizeof(char) * (strlen(p1) + strlen(p2) + 2));
			strcpy(newValue, p1);
			strcat(newValue, ".");
			strcat(newValue, p2);
			ReplaceLastToken(Toks, Token_NewWithValue(ExClassAction, newValue));
			free(newValue);
			count++;
		}
		else if(t->Type == Literal)
		{
			char* tvl = StrToLowTrim(t->Value);
			
			if(strcmp(tvl, "end") == 0) List_Add(Toks, Token_NewWithValue(ExEnd, "END"));
			else if(strcmp(tvl, "else") == 0) List_Add(Toks, Token_NewWithValue(ExElse, "ELSE"));
			else if(strcmp(tvl, "then") == 0) List_Add(Toks, Token_NewWithValue(ExThen, "THEN"));
			else if(strcmp(tvl, "while") == 0) List_Add(Toks, Token_NewWithValue(ExWhile, "WHILE"));
			else if(strcmp(tvl, "loop") == 0) List_Add(Toks, Token_NewWithValue(ExLoop, "LOOP"));
			else if(strcmp(tvl, "for") == 0) List_Add(Toks, Token_NewWithValue(ExFor, "FOR"));
			else if(strcmp(tvl, "to") == 0) List_Add(Toks, Token_NewWithValue(ExTo, "TO"));
			else if(strcmp(tvl, "step") == 0) List_Add(Toks, Token_NewWithValue(ExStep, "STEP"));
			else if(strcmp(tvl, "next") == 0) List_Add(Toks, Token_NewWithValue(ExNext, "NEXT"));
			else if(strcmp(tvl, "var") == 0) List_Add(Toks, Token_NewWithValue(ExVar, "VAR"));
			else if(strcmp(tvl, "return") == 0) List_Add(Toks, Token_NewWithValue(ExReturn, "RETURN"));
			else if(strcmp(tvl, "prop") == 0) List_Add(Toks, Token_NewWithValue(ExProperty, "PROP"));
			else if(strcmp(tvl, "extends") == 0) List_Add(Toks, Token_NewWithValue(ExExtends, "EXTENDS"));
			else if(strcmp(tvl, "override") == 0) List_Add(Toks, Token_NewWithValue(ExOverride, "OVERRIDE"));
			else if(strcmp(tvl, "static") == 0) List_Add(Toks, Token_NewWithValue(ExStatic, "STATIC"));
			else if(strcmp(tvl, "asmpush") == 0) List_Add(Toks, Token_NewWithValue(ExAsmPush, "ASMPUSH"));
			else if(strcmp(tvl, "asmpop") == 0) List_Add(Toks, Token_NewWithValue(ExAsmPop, "ASMPOP"));
			else if(strcmp(tvl, "if") == 0)
			{
				Token* lastToken = LastToken(Toks);
				if(lastToken->Type == ExEnd)
					ReplaceLastToken(Toks, Token_NewWithValue(ExEndIf, "END IF"));
				else if(lastToken->Type == ExElse)
					ReplaceLastToken(Toks, Token_NewWithValue(ExElseIf, "ELSE IF"));
				else
					List_Add(Toks, Token_NewWithValue(ExIf, "IF"));
			}
			else if(strcmp(tvl, "function") == 0)
			{
				if(LastToken(Toks)->Type == ExEnd)
					ReplaceLastToken(Toks, Token_NewWithValue(ExEndFunction, "END FUNCTION"));
				else
					List_Add(Toks, Token_NewWithValue(ExFunction, "FUNCTION"));
			}
			else if(strcmp(tvl, "asmblock") == 0)
			{
				if(LastToken(Toks)->Type == ExEnd)
					ReplaceLastToken(Toks, Token_NewWithValue(ExEndAsmBlock, "END ASMBLOCK"));
				else
					List_Add(Toks, Token_NewWithValue(ExAsmBlock, "ASMBLOCK"));
			}
			else if(strcmp(tvl, "class") == 0)
			{
				if(LastToken(Toks)->Type == ExEnd)
					ReplaceLastToken(Toks, Token_NewWithValue(ExEndClass, "END CLASS"));
				else
					List_Add(Toks, Token_NewWithValue(ExClass, "CLASS"));
			}
			else if(strcmp(tvl, "method") == 0)
			{
				if(LastToken(Toks)->Type == ExEnd)
					ReplaceLastToken(Toks, Token_NewWithValue(ExEndMethod, "END METHOD"));
				else
					List_Add(Toks, Token_NewWithValue(ExMethod, "METHOD"));
			}
			else
			{
				if(LastToken(Toks)->Type == Colon)
					ReplaceLastToken(Toks, Token_NewWithValue(ExTypeHint, t->Value));
				else
					List_Add(Toks, Token_Copy(t));
			}
					
			free(tvl);
		}
		else
		{
			List_Add(Toks, Token_Copy(t));
		}
	}
	return Toks;
};