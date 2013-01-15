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

#ifndef __OBJBAS_BLOCKBUILDER_H__
#define __OBJBAS_BLOCKBUILDER_H__

#include <ObjBas_Token.h>
#include <List.h>

typedef struct
{
	List* Tokens;
} CodeBlock;

CodeBlock* CodeBlock_New(List* Toks);
void CodeBlock_Delete(CodeBlock* cb);
List* BlockBuilder_ConvertToBlocks(List* Toks);

#endif