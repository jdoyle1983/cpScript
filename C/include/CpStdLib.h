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

#ifndef __CPSTDLIB_H__
#define __CPSTDLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WINDLL
#define EXPORT __declspec( dllexport )
#else
#define EXPORT
#endif

EXPORT void CpStdLib_InstallConsoleIO(void* State);
EXPORT void CpStdLib_InstallFileIO(void* State);
EXPORT void CpStdLib_InstallMath(void* State);
EXPORT void CpStdLib_InstallUtilities(void* State);

#ifdef __cplusplus
}
#endif

#endif
