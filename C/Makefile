CC = gcc
CP = gcc
AR = ar

INC = -Iinclude
LIB = -L../Build

STATIC_OBJDIR = obj/libCpScript.Asm/static
STATIC_OBJ = $(STATIC_OBJDIR)/State.o $(STATIC_OBJDIR)/Stack.o $(STATIC_OBJDIR)/Parser.o $(STATIC_OBJDIR)/MemoryBlockSetHeader.o $(STATIC_OBJDIR)/MemoryBlockHeader.o $(STATIC_OBJDIR)/MemoryBlock.o $(STATIC_OBJDIR)/List.o $(STATIC_OBJDIR)/LabelDef.o $(STATIC_OBJDIR)/Extensions.o $(STATIC_OBJDIR)/AssemblyToken.o 
STATIC_CFLAGS = -Wall -O2
STATIC_BIN = ../Build/libCpScript.Asm.a

SHARED_OBJDIR = obj/libCpScript.Asm/shared
SHARED_OBJ = $(SHARED_OBJDIR)/State.o $(SHARED_OBJDIR)/Stack.o $(SHARED_OBJDIR)/Parser.o $(SHARED_OBJDIR)/MemoryBlockSetHeader.o $(SHARED_OBJDIR)/MemoryBlockHeader.o $(SHARED_OBJDIR)/MemoryBlock.o $(SHARED_OBJDIR)/List.o $(SHARED_OBJDIR)/LabelDef.o $(SHARED_OBJDIR)/Extensions.o $(SHARED_OBJDIR)/AssemblyToken.o 
SHARED_CFLAGS = -Wall -fPIC -DBUILDINGDLL
SHARED_BIN = ../Build/libCpScript.Asm.Shared.so

STDSTATIC_OBJDIR = obj/libCpStdLib/static
STDSTATIC_OBJ = $(STDSTATIC_OBJDIR)/CpStdLib.o $(STDSTATIC_OBJDIR)/CpStdLib_ConsoleIO.o $(STDSTATIC_OBJDIR)/CpStdLib_FileIO.o $(STDSTATIC_OBJDIR)/CpStdLib_Math.o $(STDSTATIC_OBJDIR)/CpStdLib_Utilities.o
STDSTATIC_CFLAGS = -Wall -O2
STDSTATIC_BIN = ../Build/libCpStdLib.a

STDSHARED_OBJDIR = obj/libCpStdLib/shared
STDSHARED_OBJ = $(STDSHARED_OBJDIR)/CpStdLib.o $(STDSHARED_OBJDIR)/CpStdLib_ConsoleIO.o $(STDSHARED_OBJDIR)/CpStdLib_FileIO.o $(STDSHARED_OBJDIR)/CpStdLib_Math.o $(STDSHARED_OBJDIR)/CpStdLib_Utilities.o
STDSHARED_CFLAGS = -Wall -fPIC -DBUILDINGDLL
STDSHARED_CLIBS = -lCpScript.Asm.Shared
STDSHARED_BIN = ../Build/libCpStdLib.Shared.so

CPINT_OBJDIR = obj/cpInt
CPINT_OBJ = $(CPINT_OBJDIR)/CpInt.o
CPINT_CFLAGS = -Wall -O2
CPINT_CLIBS = $(STATIC_BIN) $(STDSTATIC_BIN)
CPINT_BIN = ../Build/cpInt

all: static shared cpint

static: $(STATIC_BIN) $(STDSTATIC_BIN)

shared: $(SHARED_BIN) $(STDSHARED_BIN)

cpint: $(CPINT_BIN)

clean:
	rm -f $(STATIC_BIN)
	rm -f $(SHARED_BIN)
	rm -f $(STDSTATIC_BIN)
	rm -f $(STDSHARED_BIN)
	rm -f $(CPINT_BIN)
	rm -f $(STATIC_OBJDIR)/*.o
	rm -f $(SHARED_OBJDIR)/*.o
	rm -f $(STDSTATIC_OBJDIR)/*.o
	rm -f $(STDSHARED_OBJDIR)/*.o
	rm -f $(CPINT_OBJDIR)/*.o
	
$(CPINT_BIN): $(CPINT_OBJ)
	$(CP) $(CPINT_CFLAGS) $(CPINT_OBJ) $(CPINT_CLIBS) -o $(CPINT_BIN)

$(STATIC_BIN): $(STATIC_OBJ)
	$(AR) rcs $(STATIC_BIN) $(STATIC_OBJ)
	
$(SHARED_BIN): $(SHARED_OBJ)
	$(CP) -shared $(SHARED_OBJ) -o $(SHARED_BIN)
	
$(STDSTATIC_BIN): $(STDSTATIC_OBJ)
	$(AR) rcs $(STDSTATIC_BIN) $(STDSTATIC_OBJ)
	
$(STDSHARED_BIN): $(STDSHARED_OBJ)
	$(CP) $(SHARED_BIN) -shared $(STDSHARED_OBJ) -o $(STDSHARED_BIN)

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