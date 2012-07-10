CC=gcc
CFLAGS=-c -Iinclude
BIN=bin/libCpScriptAsmTest
BINOBJ=obj/test.o
LIB=lib/libCpScriptAsm.a
LIBOBJ=obj/AssemblyToken.o obj/Extensions.o obj/IScriptLib.o obj/LabelDef.o obj/List.o obj/MemoryBlock.o obj/MemoryBlockHeader.o obj/MemoryBlockSetHeader.o obj/Parser.o obj/Stack.o obj/State.o

all: $(LIB) $(BIN)

clean: 
	rm -f $(LIB) $(BIN) $(LIBOBJ) $(BINOBJ)

$(LIB): $(LIBOBJ)
	ar rcs $(LIB) $(LIBOBJ)

$(BIN): $(LIB) $(BINOBJ)
	$(CC) $(BINOBJ) -Llib -lCpScriptAsm -ldl -o $(BIN)

obj/AssemblyToken.o: src/AssemblyToken.c
	$(CC) $(CFLAGS) src/AssemblyToken.c -o obj/AssemblyToken.o

obj/Extensions.o: src/Extensions.c
	$(CC) $(CFLAGS) src/Extensions.c -o obj/Extensions.o

obj/IScriptLib.o: src/IScriptLib.c
	$(CC) $(CFLAGS) src/IScriptLib.c -o obj/IScriptLib.o

obj/LabelDef.o: src/LabelDef.c
	$(CC) $(CFLAGS) src/LabelDef.c -o obj/LabelDef.o

obj/List.o: src/List.c
	$(CC) $(CFLAGS) src/List.c -o obj/List.o

obj/MemoryBlock.o: src/MemoryBlock.c
	$(CC) $(CFLAGS) src/MemoryBlock.c -o obj/MemoryBlock.o

obj/MemoryBlockHeader.o: src/MemoryBlockHeader.c
	$(CC) $(CFLAGS) src/MemoryBlockHeader.c -o obj/MemoryBlockHeader.o

obj/MemoryBlockSetHeader.o: src/MemoryBlockSetHeader.c
	$(CC) $(CFLAGS) src/MemoryBlockSetHeader.c -o obj/MemoryBlockSetHeader.o

obj/Parser.o: src/Parser.c
	$(CC) $(CFLAGS) src/Parser.c -o obj/Parser.o

obj/Stack.o: src/Stack.c
	$(CC) $(CFLAGS) src/Stack.c -o obj/Stack.o

obj/State.o: src/State.c
	$(CC) $(CFLAGS) src/State.c -o obj/State.o

obj/test.o: src/test.c
	$(CC) $(CFLAGS) src/test.c -o obj/test.o





