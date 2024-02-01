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

#include <ObjBas_BlockBuilder.h>
#include <malloc.h>

CodeBlock* CodeBlock_New(List* Toks)
{
	CodeBlock* cb = (CodeBlock*)malloc(sizeof(CodeBlock));
	cb->Tokens = Toks;
	return cb;
};

void CodeBlock_Delete(CodeBlock* cb)
{
	List_Delete(cb->Tokens);
	free(cb);
};

List* BlockBuilder_ConvertToBlocks(List* Toks)
{
	List* Blocks = List_New();
	List* Current = List_New();

	size_t i = 0;
	for(i = 0; i < List_Count(Toks); i++)
	{
		if((List_TokenAtIndex(Toks, i))->Type == Eol)
		{
			if(List_Count(Current) > 0)
			{
				List_Add(Blocks, CodeBlock_New(Current));
				Current = List_New();
			}
		}
		else
			List_Add(Current, List_TokenAtIndex(Toks, i));
	}
	
	if(List_Count(Current) > 0)
		List_Add(Blocks, CodeBlock_New(Current));
	else
		List_Delete(Current);
		
	return Blocks;
};