/*

    libCpStdLib
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

#ifndef __CPSTDLIB_FILEIO_H__
#define __CPSTDLIB_FILEIO_H__

void File_Open(void* State);
void File_WriteLine(void* State);
void File_ReadLine(void* State);
void File_Close(void* State);
void File_Eof(void* State);
void File_ReadBool(void* State);
void File_ReadDouble(void* State);
void File_ReadInt(void* State);
void File_ReadString(void* State);
void File_WriteBool(void* State);
void File_WriteDouble(void* State);
void File_WriteInt(void* State);
void File_WriteString(void* State);

#endif
