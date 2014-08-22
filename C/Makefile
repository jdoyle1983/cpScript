VERMAJ = 1
VERMIN = 0
VERREL = 0
VERSION = $(VERMAJ).$(VERMIN).$(VERREL)
MINORVERSION = $(VERMAJ).$(VERMIN)
MAJORVERSION = $(VERMAJ)

CC = gcc
CP = gcc
AR = ar

INC = -Iinclude
LIB = -L../Build

STATIC_OBJDIR = obj/libCpScript.Asm/static
STATIC_OBJ = $(STATIC_OBJDIR)/State.o $(STATIC_OBJDIR)/Stack.o $(STATIC_OBJDIR)/Parser.o $(STATIC_OBJDIR)/MemoryBlockSetHeader.o $(STATIC_OBJDIR)/MemoryBlockHeader.o $(STATIC_OBJDIR)/MemoryBlock.o $(STATIC_OBJDIR)/List.o $(STATIC_OBJDIR)/LabelDef.o $(STATIC_OBJDIR)/Extensions.o $(STATIC_OBJDIR)/AssemblyToken.o 
STATIC_CFLAGS = -Wall -O2 -ggdb
STATIC_BIN = ../Build/libCpScript.Asm.a

SHARED_OBJDIR = obj/libCpScript.Asm/shared
SHARED_OBJ = $(SHARED_OBJDIR)/State.o $(SHARED_OBJDIR)/Stack.o $(SHARED_OBJDIR)/Parser.o $(SHARED_OBJDIR)/MemoryBlockSetHeader.o $(SHARED_OBJDIR)/MemoryBlockHeader.o $(SHARED_OBJDIR)/MemoryBlock.o $(SHARED_OBJDIR)/List.o $(SHARED_OBJDIR)/LabelDef.o $(SHARED_OBJDIR)/Extensions.o $(SHARED_OBJDIR)/AssemblyToken.o 
SHARED_CFLAGS = -Wall -fPIC -DBUILDINGDLL -ggdb
SHARED_BIN = ../Build/libCpScript.Asm.Shared.so.$(VERSION)
SHARED_BIN_MIN = ../Build/libCpScript.Asm.Shared.so.$(MINORVERSION)
SHARED_BIN_MAJ = ../Build/libCpScript.Asm.Shared.so.$(MAJORVERSION)
SHARED_BIN_CORE = ../Build/libCpScript.Asm.Shared.so

OBJSTATIC_OBJDIR = obj/libCpScript.ObjectBasic/static
OBJSTATIC_OBJ = $(OBJSTATIC_OBJDIR)/ObjBas_BlockBuilder.o $(OBJSTATIC_OBJDIR)/ObjBas_ClassDef.o $(OBJSTATIC_OBJDIR)/ObjBas_Evaluator.o $(OBJSTATIC_OBJDIR)/ObjBas_Function.o $(OBJSTATIC_OBJDIR)/ObjBas_Script.o $(OBJSTATIC_OBJDIR)/ObjBas_Token.o $(OBJSTATIC_OBJDIR)/Stack.o $(OBJSTATIC_OBJDIR)/List.o $(OBJSTATIC_OBJDIR)/Extensions.o
OBJSTATIC_CFLAGS = -Wall -O2 -ggdb
OBJSTATIC_BIN = ../Build/libCpScript.ObjectBasic.a

OBJSHARED_OBJDIR = obj/libCpScript.ObjectBasic/shared
OBJSHARED_OBJ = $(OBJSHARED_OBJDIR)/ObjBas_BlockBuilder.o $(OBJSHARED_OBJDIR)/ObjBas_ClassDef.o $(OBJSHARED_OBJDIR)/ObjBas_Evaluator.o $(OBJSHARED_OBJDIR)/ObjBas_Function.o $(OBJSHARED_OBJDIR)/ObjBas_Script.o $(OBJSHARED_OBJDIR)/ObjBas_Token.o $(OBJSHARED_OBJDIR)/Stack.o $(OBJSHARED_OBJDIR)/List.o $(OBJSHARED_OBJDIR)/Extensions.o
OBJSHARED_CFLAGS = -Wall -fPIC -DBUILDINGDLL -ggdb
OBJSHARED_BIN = ../Build/libCpScript.ObjectBasic.Shared.so.$(VERSION)
OBJSHARED_BIN_MIN = ../Build/libCpScript.ObjectBasic.Shared.so.$(MINORVERSION)
OBJSHARED_BIN_MAJ = ../Build/libCpScript.ObjectBasic.Shared.so.$(MAJORVERSION)
OBJSHARED_BIN_CORE = ../Build/libCpScript.ObjectBasic.Shared.so

STDSTATIC_OBJDIR = obj/libCpStdLib/static
STDSTATIC_OBJ = $(STDSTATIC_OBJDIR)/CpStdLib.o $(STDSTATIC_OBJDIR)/CpStdLib_ConsoleIO.o $(STDSTATIC_OBJDIR)/CpStdLib_FileIO.o $(STDSTATIC_OBJDIR)/CpStdLib_Math.o $(STDSTATIC_OBJDIR)/CpStdLib_Utilities.o
STDSTATIC_CFLAGS = -Wall -O2 -ggdb
STDSTATIC_BIN = ../Build/libCpStdLib.a

STDSHARED_OBJDIR = obj/libCpStdLib/shared
STDSHARED_OBJ = $(STDSHARED_OBJDIR)/CpStdLib.o $(STDSHARED_OBJDIR)/CpStdLib_ConsoleIO.o $(STDSHARED_OBJDIR)/CpStdLib_FileIO.o $(STDSHARED_OBJDIR)/CpStdLib_Math.o $(STDSHARED_OBJDIR)/CpStdLib_Utilities.o
STDSHARED_CFLAGS = -Wall -fPIC -DBUILDINGDLL -ggdb
STDSHARED_CLIBS = -lCpScript.Asm.Shared
STDSHARED_BIN = ../Build/libCpStdLib.Shared.so.$(VERSION)
STDSHARED_BIN_MIN = ../Build/libCpStdLib.Shared.so.$(MINORVERSION)
STDSHARED_BIN_MAJ = ../Build/libCpStdLib.Shared.so.$(MAJORVERSION)
STDSHARED_BIN_CORE = ../Build/libCpStdLib.Shared.so

CPINT_OBJDIR = obj/cpInt
CPINT_OBJ = $(CPINT_OBJDIR)/CpInt.o
CPINT_CFLAGS = -Wall -O2 -ggdb
CPINT_CLIBS = $(STATIC_BIN) $(STDSTATIC_BIN) -lm
CPINT_BIN = ../Build/cpInt

CPOBJCMP_OBJDIR = obj/cpObjCmp
CPOBJCMP_OBJ = $(CPOBJCMP_OBJDIR)/CpObjCmp.o
CPOBJCMP_CFLAGS = -Wall -O2 -ggdb
CPOBJCMP_CLIBS = $(OBJSTATIC_BIN) -lm
CPOBJCMP_BIN = ../Build/cpObjCmp

CPCONSOLE_OBJDIR = obj/cpConsole
CPCONSOLE_OBJ = $(CPCONSOLE_OBJDIR)/CpConsole.o
CPCONSOLE_CFLAGS = -Wall -O2 -ggdb
CPCONSOLE_CLIBS = $(STATIC_BIN) $(STDSTATIC_BIN) $(OBJSTATIC_BIN) -lm
CPCONSOLE_BIN = ../Build/cpConsole

ER_OBJDIR = obj/makeExec
ER_OBJ = $(ER_OBJDIR)/ExecRuntime.o
ER_CFLAGS = -Wall -O2 -ggdb
ER_CLIBS = $(STATIC_BIN) $(STDSTATIC_BIN) -lm
ER_BIN = ../Build/execRuntime

ME_OBJ = $(ER_OBJDIR)/MakeExec.o
ME_CFLAGS = -Wall -O2 -ggdb
ME_CLIBS = $(STATIC_BIN) $(STDSTATIC_BIN) $(OBJSTATIC_BIN) -lm
ME_BIN = ../Build/makeExec

all: static shared cpint cpobjcmp cpconsole er me objheaders

static: $(STATIC_BIN) $(STDSTATIC_BIN) $(OBJSTATIC_BIN)

shared: $(SHARED_BIN) $(STDSHARED_BIN) $(OBJSHARED_BIN) $(SHARED_BIN_MIN) $(STDSHARED_BIN_MIN) $(OBJSHARED_BIN_MIN) $(SHARED_BIN_MAJ) $(STDSHARED_BIN_MAJ) $(OBJSHARED_BIN_MAJ) $(SHARED_BIN_CORE) $(STDSHARED_BIN_CORE) $(OBJSHARED_BIN_CORE)

cpint: $(CPINT_BIN)

cpobjcmp: $(CPOBJCMP_BIN)

cpconsole: $(CPCONSOLE_BIN)

er: $(ER_BIN)

me: $(ME_BIN)

libdir:
	mkdir ../Build/Lib

objheaders: libdir
	cp ObjectBasicHeaders/*.obh ../Build/Lib
	
lightcheaders: libdir
	cp LightCHeaders/*.lch ../Build/Lib
	cp LightCHeaders/*.lcl ../Build/Lib

clean:
	rm -f $(STATIC_BIN)
	rm -f $(SHARED_BIN)
	rm -f $(SHARED_BIN_MIN)
	rm -f $(SHARED_BIN_MAJ)
	rm -f $(SHARED_BIN_CORE)
	rm -f $(STDSTATIC_BIN)
	rm -f $(STDSHARED_BIN)
	rm -f $(STDSHARED_BIN_MIN)
	rm -f $(STDSHARED_BIN_MAJ)
	rm -f $(STDSHARED_BIN_CORE)
	rm -f $(CPINT_BIN)
	rm -f $(OBJSTATIC_BIN)
	rm -f $(OBJSHARED_BIN)
	rm -f $(OBJSHARED_BIN_MIN)
	rm -f $(OBJSHARED_BIN_MAJ)
	rm -f $(OBJSHARED_BIN_CORE)
	rm -f $(CPOBJCMP_BIN)
	rm -f  $(CPCONSOLE_BIN)
	rm -f  $(ER_BIN)
	rm -f  $(ME_BIN)
	rm -f $(STATIC_OBJDIR)/*.o
	rm -f $(SHARED_OBJDIR)/*.o
	rm -f $(STDSTATIC_OBJDIR)/*.o
	rm -f $(STDSHARED_OBJDIR)/*.o
	rm -f $(CPINT_OBJDIR)/*.o
	rm -f $(OBJSTATIC_OBJDIR)/*.o
	rm -f $(OBJSHARED_OBJDIR)/*.o
	rm -f $(CPOBJCMP_OBJDIR)/*.o
	rm -f $(CPCONSOLE_OBJDIR)/*.o
	rm -f $(ER_OBJDIR)/*.o
	rm -f ../Build/Lib/*.obh
	rm -f ../Build/Lib/*.lch
	rm -f ../Build/Lib/*.lcl
	rmdir ../Build/Lib
	
$(CPINT_BIN): $(CPINT_OBJ)
	$(CP) $(CPINT_CFLAGS) $(CPINT_OBJ) $(CPINT_CLIBS) -o $(CPINT_BIN)
	
$(CPOBJCMP_BIN): $(CPOBJCMP_OBJ)
	$(CP) $(CPOBJCMP_CFLAGS) $(CPOBJCMP_OBJ) $(CPOBJCMP_CLIBS) -o $(CPOBJCMP_BIN)

$(CPCONSOLE_BIN): $(CPCONSOLE_OBJ)
	$(CP) $(CPCONSOLE_CFLAGS) $(CPCONSOLE_OBJ) $(CPCONSOLE_CLIBS) -o $(CPCONSOLE_BIN)
	
$(ER_BIN): $(ER_OBJ)
	$(CP) $(ER_CFLAGS) $(ER_OBJ) $(ER_CLIBS) -o $(ER_BIN)
	
$(ME_BIN): $(ME_OBJ)
	$(CP) $(ME_CFLAGS) $(ME_OBJ) $(ME_CLIBS) -o $(ME_BIN)

$(STATIC_BIN): $(STATIC_OBJ)
	$(AR) rcs $(STATIC_BIN) $(STATIC_OBJ)
	
$(SHARED_BIN): $(SHARED_OBJ)
	$(CP) -shared $(SHARED_OBJ) -o $(SHARED_BIN)
	
$(SHARED_BIN_MIN): $(SHARED_BIN)
	ln -s $(SHARED_BIN) $(SHARED_BIN_MIN)
	
$(SHARED_BIN_MAJ): $(SHARED_BIN)
	ln -s $(SHARED_BIN) $(SHARED_BIN_MAJ)

$(SHARED_BIN_CORE): $(SHARED_BIN)
	ln -s $(SHARED_BIN) $(SHARED_BIN_CORE)
	
$(OBJSTATIC_BIN): $(OBJSTATIC_OBJ)
	$(AR) rcs $(OBJSTATIC_BIN) $(OBJSTATIC_OBJ)
	
$(OBJSHARED_BIN): $(OBJSHARED_OBJ)
	$(CP) -shared $(OBJSHARED_OBJ) -o $(OBJSHARED_BIN)
	
$(OBJSHARED_BIN_MIN): $(OBJSHARED_BIN)
	ln -s $(OBJSHARED_BIN) $(OBJSHARED_BIN_MIN)
	
$(OBJSHARED_BIN_MAJ): $(OBJSHARED_BIN)
	ln -s $(OBJSHARED_BIN) $(OBJSHARED_BIN_MAJ)

$(OBJSHARED_BIN_CORE): $(OBJSHARED_BIN)
	ln -s $(OBJSHARED_BIN) $(OBJSHARED_BIN_CORE)
	
$(STDSTATIC_BIN): $(STDSTATIC_OBJ)
	$(AR) rcs $(STDSTATIC_BIN) $(STDSTATIC_OBJ)
	
$(STDSHARED_BIN): $(STDSHARED_OBJ)
	$(CP) $(SHARED_BIN) -shared $(STDSHARED_OBJ) -o $(STDSHARED_BIN)
	
$(STDSHARED_BIN_MIN): $(STDSHARED_BIN)
	ln -s $(STDSHARED_BIN) $(STDSHARED_BIN_MIN)
	
$(STDSHARED_BIN_MAJ): $(STDSHARED_BIN)
	ln -s $(STDSHARED_BIN) $(STDSHARED_BIN_MAJ)

$(STDSHARED_BIN_CORE): $(STDSHARED_BIN)
	ln -s $(STDSHARED_BIN) $(STDSHARED_BIN_CORE)

$(STATIC_OBJDIR)/State.o: src/State.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/State.c -o $(STATIC_OBJDIR)/State.o

$(STATIC_OBJDIR)/Stack.o: src/Stack.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/Stack.c -o $(STATIC_OBJDIR)/Stack.o

$(STATIC_OBJDIR)/Parser.o: src/Parser.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/Parser.c -o $(STATIC_OBJDIR)/Parser.o

$(STATIC_OBJDIR)/MemoryBlockSetHeader.o: src/MemoryBlockSetHeader.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/MemoryBlockSetHeader.c -o $(STATIC_OBJDIR)/MemoryBlockSetHeader.o

$(STATIC_OBJDIR)/MemoryBlockHeader.o: src/MemoryBlockHeader.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/MemoryBlockHeader.c -o $(STATIC_OBJDIR)/MemoryBlockHeader.o

$(STATIC_OBJDIR)/MemoryBlock.o: src/MemoryBlock.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/MemoryBlock.c -o $(STATIC_OBJDIR)/MemoryBlock.o

$(STATIC_OBJDIR)/List.o: src/List.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/List.c -o $(STATIC_OBJDIR)/List.o

$(STATIC_OBJDIR)/LabelDef.o: src/LabelDef.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/LabelDef.c -o $(STATIC_OBJDIR)/LabelDef.o

$(STATIC_OBJDIR)/Extensions.o: src/Extensions.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/Extensions.c -o $(STATIC_OBJDIR)/Extensions.o

$(STATIC_OBJDIR)/AssemblyToken.o: src/AssemblyToken.c
	$(CC) $(STATIC_CFLAGS) $(INC) -c src/AssemblyToken.c -o $(STATIC_OBJDIR)/AssemblyToken.o
	
$(SHARED_OBJDIR)/State.o: src/State.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/State.c -o $(SHARED_OBJDIR)/State.o

$(SHARED_OBJDIR)/Stack.o: src/Stack.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/Stack.c -o $(SHARED_OBJDIR)/Stack.o

$(SHARED_OBJDIR)/Parser.o: src/Parser.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/Parser.c -o $(SHARED_OBJDIR)/Parser.o

$(SHARED_OBJDIR)/MemoryBlockSetHeader.o: src/MemoryBlockSetHeader.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/MemoryBlockSetHeader.c -o $(SHARED_OBJDIR)/MemoryBlockSetHeader.o

$(SHARED_OBJDIR)/MemoryBlockHeader.o: src/MemoryBlockHeader.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/MemoryBlockHeader.c -o $(SHARED_OBJDIR)/MemoryBlockHeader.o

$(SHARED_OBJDIR)/MemoryBlock.o: src/MemoryBlock.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/MemoryBlock.c -o $(SHARED_OBJDIR)/MemoryBlock.o

$(SHARED_OBJDIR)/List.o: src/List.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/List.c -o $(SHARED_OBJDIR)/List.o

$(SHARED_OBJDIR)/LabelDef.o: src/LabelDef.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/LabelDef.c -o $(SHARED_OBJDIR)/LabelDef.o

$(SHARED_OBJDIR)/Extensions.o: src/Extensions.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/Extensions.c -o $(SHARED_OBJDIR)/Extensions.o

$(SHARED_OBJDIR)/AssemblyToken.o: src/AssemblyToken.c
	$(CC) $(SHARED_CFLAGS) $(INC) -c src/AssemblyToken.c -o $(SHARED_OBJDIR)/AssemblyToken.o
	
$(STDSTATIC_OBJDIR)/CpStdLib.o: src/CpStdLib.c
	$(CC) $(STDSTATIC_CFLAGS) $(INC) -c src/CpStdLib.c -o $(STDSTATIC_OBJDIR)/CpStdLib.o
	
$(STDSTATIC_OBJDIR)/CpStdLib_ConsoleIO.o: src/CpStdLib_ConsoleIO.c
	$(CC) $(STDSTATIC_CFLAGS) $(INC) -c src/CpStdLib_ConsoleIO.c -o $(STDSTATIC_OBJDIR)/CpStdLib_ConsoleIO.o
	
$(STDSTATIC_OBJDIR)/CpStdLib_FileIO.o: src/CpStdLib_FileIO.c
	$(CC) $(STDSTATIC_CFLAGS) $(INC) -c src/CpStdLib_FileIO.c -o $(STDSTATIC_OBJDIR)/CpStdLib_FileIO.o
	
$(STDSTATIC_OBJDIR)/CpStdLib_Math.o: src/CpStdLib_Math.c
	$(CC) $(STDSTATIC_CFLAGS) $(INC) -c src/CpStdLib_Math.c -o $(STDSTATIC_OBJDIR)/CpStdLib_Math.o
	
$(STDSTATIC_OBJDIR)/CpStdLib_Utilities.o: src/CpStdLib_Utilities.c
	$(CC) $(STDSTATIC_CFLAGS) $(INC) -c src/CpStdLib_Utilities.c -o $(STDSTATIC_OBJDIR)/CpStdLib_Utilities.o
	
$(STDSHARED_OBJDIR)/CpStdLib.o: src/CpStdLib.c
	$(CC) $(STDSHARED_CFLAGS) $(INC) -c src/CpStdLib.c -o $(STDSHARED_OBJDIR)/CpStdLib.o
	
$(STDSHARED_OBJDIR)/CpStdLib_ConsoleIO.o: src/CpStdLib_ConsoleIO.c
	$(CC) $(STDSHARED_CFLAGS) $(INC) -c src/CpStdLib_ConsoleIO.c -o $(STDSHARED_OBJDIR)/CpStdLib_ConsoleIO.o
	
$(STDSHARED_OBJDIR)/CpStdLib_FileIO.o: src/CpStdLib_FileIO.c
	$(CC) $(STDSHARED_CFLAGS) $(INC) -c src/CpStdLib_FileIO.c -o $(STDSHARED_OBJDIR)/CpStdLib_FileIO.o
	
$(STDSHARED_OBJDIR)/CpStdLib_Math.o: src/CpStdLib_Math.c
	$(CC) $(STDSHARED_CFLAGS) $(INC) -c src/CpStdLib_Math.c -o $(STDSHARED_OBJDIR)/CpStdLib_Math.o
	
$(STDSHARED_OBJDIR)/CpStdLib_Utilities.o: src/CpStdLib_Utilities.c
	$(CC) $(STDSHARED_CFLAGS) $(INC) -c src/CpStdLib_Utilities.c -o $(STDSHARED_OBJDIR)/CpStdLib_Utilities.o
	
$(CPINT_OBJDIR)/CpInt.o: src/CpInt.c
	$(CC) $(CPINT_CFLAGS) $(INC) -c src/CpInt.c -o $(CPINT_OBJDIR)/CpInt.o
	
$(OBJSTATIC_OBJDIR)/ObjBas_BlockBuilder.o: src/ObjBas_BlockBuilder.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/ObjBas_BlockBuilder.c -o $(OBJSTATIC_OBJDIR)/ObjBas_BlockBuilder.o

$(OBJSTATIC_OBJDIR)/ObjBas_ClassDef.o: src/ObjBas_ClassDef.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/ObjBas_ClassDef.c -o $(OBJSTATIC_OBJDIR)/ObjBas_ClassDef.o

$(OBJSTATIC_OBJDIR)/ObjBas_Evaluator.o: src/ObjBas_Evaluator.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/ObjBas_Evaluator.c -o $(OBJSTATIC_OBJDIR)/ObjBas_Evaluator.o

$(OBJSTATIC_OBJDIR)/ObjBas_Function.o: src/ObjBas_Function.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/ObjBas_Function.c -o $(OBJSTATIC_OBJDIR)/ObjBas_Function.o

$(OBJSTATIC_OBJDIR)/ObjBas_Script.o: src/ObjBas_Script.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/ObjBas_Script.c -o $(OBJSTATIC_OBJDIR)/ObjBas_Script.o

$(OBJSTATIC_OBJDIR)/ObjBas_Token.o: src/ObjBas_Token.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/ObjBas_Token.c -o $(OBJSTATIC_OBJDIR)/ObjBas_Token.o

$(OBJSTATIC_OBJDIR)/List.o: src/List.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/List.c -o $(OBJSTATIC_OBJDIR)/List.o
	
$(OBJSTATIC_OBJDIR)/Stack.o: src/Stack.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/Stack.c -o $(OBJSTATIC_OBJDIR)/Stack.o

$(OBJSTATIC_OBJDIR)/Extensions.o: src/Extensions.c
	$(CC) $(OBJSTATIC_CFLAGS) $(INC) -c src/Extensions.c -o $(OBJSTATIC_OBJDIR)/Extensions.o
	
$(OBJSHARED_OBJDIR)/ObjBas_BlockBuilder.o: src/ObjBas_BlockBuilder.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/ObjBas_BlockBuilder.c -o $(OBJSHARED_OBJDIR)/ObjBas_BlockBuilder.o

$(OBJSHARED_OBJDIR)/ObjBas_ClassDef.o: src/ObjBas_ClassDef.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/ObjBas_ClassDef.c -o $(OBJSHARED_OBJDIR)/ObjBas_ClassDef.o

$(OBJSHARED_OBJDIR)/ObjBas_Evaluator.o: src/ObjBas_Evaluator.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/ObjBas_Evaluator.c -o $(OBJSHARED_OBJDIR)/ObjBas_Evaluator.o

$(OBJSHARED_OBJDIR)/ObjBas_Function.o: src/ObjBas_Function.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/ObjBas_Function.c -o $(OBJSHARED_OBJDIR)/ObjBas_Function.o

$(OBJSHARED_OBJDIR)/ObjBas_Script.o: src/ObjBas_Script.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/ObjBas_Script.c -o $(OBJSHARED_OBJDIR)/ObjBas_Script.o

$(OBJSHARED_OBJDIR)/ObjBas_Token.o: src/ObjBas_Token.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/ObjBas_Token.c -o $(OBJSHARED_OBJDIR)/ObjBas_Token.o

$(OBJSHARED_OBJDIR)/List.o: src/List.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/List.c -o $(OBJSHARED_OBJDIR)/List.o

$(OBJSHARED_OBJDIR)/Stack.o: src/Stack.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/Stack.c -o $(OBJSHARED_OBJDIR)/Stack.o

$(OBJSHARED_OBJDIR)/Extensions.o: src/Extensions.c
	$(CC) $(OBJSHARED_CFLAGS) $(INC) -c src/Extensions.c -o $(OBJSHARED_OBJDIR)/Extensions.o
	
$(CPOBJCMP_OBJDIR)/CpObjCmp.o: src/CpObjCmp.c
	$(CC) $(CPOBJCMP_CFLAGS) $(INC) -c src/CpObjCmp.c -o $(CPOBJCMP_OBJDIR)/CpObjCmp.o

$(CPCONSOLE_OBJDIR)/CpConsole.o: src/CpConsole.c
	$(CC) $(CPCONSOLE_CFLAGS) $(INC) -c src/CpConsole.c -o $(CPCONSOLE_OBJDIR)/CpConsole.o
	
$(ER_OBJDIR)/ExecRuntime.o: src/ExecRuntime.c
	$(CC) $(ER_CFLAGS) $(INC) -c src/ExecRuntime.c -o $(ER_OBJDIR)/ExecRuntime.o
	
$(ER_OBJDIR)/MakeExec.o: src/MakeExec.c
	$(CC) $(ME_CFLAGS) $(INC) -c src/MakeExec.c -o $(ER_OBJDIR)/MakeExec.o
