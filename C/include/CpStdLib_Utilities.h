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

#ifndef __CPSTDLIB_UTILITIES_H__
#define __CPSTDLIB_UTILITIES_H__

void Utilities_Array_New(void* State);
void Utilities_Array_Free(void* State);
void Utilities_Array_Count(void* State);
void Utilities_Array_Resize(void* State);
void Utilities_Array_SetItem(void* State);
void Utilities_Array_GetItem(void* State);

void Utilities_Array2D_New(void* State);
void Utilities_Array2D_Free(void* State);
void Utilities_Array2D_XCount(void* State);
void Utilities_Array2D_YCount(void* State);
void Utilities_Array2D_Resize(void* State);
void Utilities_Array2D_ResizeX(void* State);
void Utilities_Array2D_ResizeY(void* State);
void Utilities_Array2D_SetItem(void* State);
void Utilities_Array2D_GetItem(void* State);

void Utilities_Time_GetTimeOfDay(void* State);
void Utilities_Time_Diff(void* State);
void Utiltiess_Time_GetMilliseconds(void* State);
void Utilities_Time_FreeTimeOfDay(void* State);

void Utilities_String_Trim(void* State);
void Utilities_String_Split(void* State);
void Utilities_String_SplitAndKeep(void* State);

#endif
