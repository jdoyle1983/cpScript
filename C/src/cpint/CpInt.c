/*

    cpInt
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

#include <libCpScript.Asm.h>
#include <CpStdLib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("No File(s) Specified. (use -o file.cmp to just compile asm file)\n");
        return 0;
    }

    short shouldRun = 1;
    char* outFile = NULL;
    if(argc > 2)
    {
        int i = 0;
        for(i = 0; i < argc; i++)
        {
            if(strcmp(argv[i], "-o") == 0)
            {
                i++;
                shouldRun = 0;
                outFile = argv[i];
            }
        }
    }

    FILE* srcFile;
    char* srcBuffer;
    unsigned long srcLen;

    srcFile = fopen(argv[1], "rb");
    fseek(srcFile, 0, SEEK_END);
    srcLen = ftell(srcFile);
    fseek(srcFile, 0, SEEK_SET);

    srcBuffer = (char*)malloc(srcLen + 1);

    if(!srcBuffer)
    {
        printf("Memory Error Reading File!\n");
        fclose(srcFile);
        return 0;
    }

    fread(srcBuffer, srcLen, 1, srcFile);
	srcBuffer[srcLen] = '\0';
    fclose(srcFile);

    short didCompile = 0;
    void* cmpBuffer = NULL;
    long cmpLen = 0;

    if(srcBuffer[0] != 'C' || srcBuffer[1] != 'A' || srcBuffer[2] != 'C')
    {
        void* State = State_New(srcBuffer);
        cmpBuffer = (void*)State_Compile(State, &cmpLen);
        didCompile = 1;
        State_Delete(State);
    }

    if(!didCompile)
    {
        cmpBuffer = srcBuffer;
        cmpLen = srcLen;
    }

    void* State = (void*)State_NewFromCompiled(cmpBuffer, cmpLen);

    if(shouldRun == 1)
    {
        CpStdLib_InstallConsoleIO(State);
        CpStdLib_InstallFileIO(State);
        CpStdLib_InstallMath(State);
        CpStdLib_InstallUtilities(State);
    }
    else
    {
        FILE* writeFile = fopen(outFile, "wb");
        fwrite(cmpBuffer, cmpLen, 1, writeFile);
        fclose(writeFile);
    }

    free(cmpBuffer);
    if(didCompile == 1)
        free(srcBuffer);

    if(shouldRun == 1)
        State_RunFromMethod(State, "Main");

    State_Delete(State);

    return 0;
};

