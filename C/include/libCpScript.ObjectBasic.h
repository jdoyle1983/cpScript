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

#ifndef __LIBCPSCRIPT_OBJECTBASIC_H__
#define __LIBCPSCRIPT_OBJECTBASIC_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDLL
#define EXPORT __declspec( dllexport )
#else
#define EXPORT
#endif

enum ParseMessageLevel
{
	PM_INFO = 0,
	PM_WARN = 1,
	PM_ERR = 2,
};

typedef struct
{
	int Type;
	char* File;
	int Line;
	int Column;
	int ErrNum;
	char* ErrMessage;
} ParseMessage;

EXPORT void* ObjScript_New();
EXPORT void RegisterFunction(void* ObjScript, char* Name);
EXPORT void ObjScript_Load(void* ObjScript, char* Script);
EXPORT char* ObjScript_GetAsm(void* ObjScript);
EXPORT void ObjScript_Delete(void* ObjScript);

#ifdef __cplusplus
}
#endif

#endif
