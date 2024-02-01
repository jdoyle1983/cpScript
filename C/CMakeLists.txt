cmake_minimum_required(VERSION 3.10)
project(cpScript)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../Build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../Build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/../Build)

add_library(StaticAsmLibrary STATIC
        src/asm/State.c
        src/shared/Stack.c
        src/asm/Parser.c
        src/asm/MemoryBlockSetHeader.c
        src/asm/MemoryBlockHeader.c
        src/asm/MemoryBlock.c
        src/shared/List.c
        src/asm/LabelDef.c
        src/shared/Extensions.c
        src/asm/AssemblyToken.c
        src/asm/miniz.c
)

set_target_properties(StaticAsmLibrary PROPERTIES PREFIX "lib")
set_target_properties(StaticAsmLibrary PROPERTIES OUTPUT_NAME "cpScript.Asm")
set_target_properties(StaticAsmLibrary PROPERTIES SUFFIX ".a")

target_include_directories(StaticAsmLibrary PUBLIC include include/asm include/shared)

add_library(SharedAsmLibrary SHARED
        src/asm/State.c
        src/shared/Stack.c
        src/asm/Parser.c
        src/asm/MemoryBlockSetHeader.c
        src/asm/MemoryBlockHeader.c
        src/asm/MemoryBlock.c
        src/shared/List.c
        src/asm/LabelDef.c
        src/shared/Extensions.c
        src/asm/AssemblyToken.c
        src/asm/miniz.c
)

set_target_properties(SharedAsmLibrary PROPERTIES PREFIX "lib")
set_target_properties(SharedAsmLibrary PROPERTIES OUTPUT_NAME "cpScript.Asm.Shared")
set_target_properties(SharedAsmLibrary PROPERTIES SUFFIX ".so")

target_include_directories(SharedAsmLibrary PUBLIC include include/asm include/shared)

add_library(StaticObjLibrary STATIC
        src/obj/ObjBas_BlockBuilder.c
        src/obj/ObjBas_ClassDef.c
        src/obj/ObjBas_Evaluator.c
        src/obj/ObjBas_Function.c
        src/obj/ObjBas_Script.c
        src/obj/ObjBas_Token.c
        src/shared/Stack.c
        src/shared/List.c
        src/shared/Extensions.c
)

set_target_properties(StaticObjLibrary PROPERTIES PREFIX "lib")
set_target_properties(StaticObjLibrary PROPERTIES OUTPUT_NAME "cpScript.ObjectBasic")
set_target_properties(StaticObjLibrary PROPERTIES SUFFIX ".a")

target_include_directories(StaticObjLibrary PUBLIC include include/obj include/shared)

add_library(SharedObjLibrary SHARED
        src/obj/ObjBas_BlockBuilder.c
        src/obj/ObjBas_ClassDef.c
        src/obj/ObjBas_Evaluator.c
        src/obj/ObjBas_Function.c
        src/obj/ObjBas_Script.c
        src/obj/ObjBas_Token.c
        src/shared/Stack.c
        src/shared/List.c
        src/shared/Extensions.c
)

set_target_properties(SharedObjLibrary PROPERTIES PREFIX "lib")
set_target_properties(SharedObjLibrary PROPERTIES OUTPUT_NAME "cpScript.ObjectBasic.Shared")
set_target_properties(SharedObjLibrary PROPERTIES SUFFIX ".so")

target_include_directories(SharedObjLibrary PUBLIC include include/obj include/shared)

add_library(StaticStandardLibrary STATIC
        src/std/CpStdLib.c
        src/std/CpStdLib_ConsoleIO.c
        src/std/CpStdLib_FileIO.c
        src/std/CpStdLib_Math.c
        src/std/CpStdLib_Utilities.c
)

set_target_properties(StaticStandardLibrary PROPERTIES PREFIX "lib")
set_target_properties(StaticStandardLibrary PROPERTIES OUTPUT_NAME "CpStdLib")
set_target_properties(StaticStandardLibrary PROPERTIES SUFFIX ".a")

target_include_directories(StaticStandardLibrary PUBLIC include include/std include/shared)

add_library(SharedStandardLibrary SHARED
        src/std/CpStdLib.c
        src/std/CpStdLib_ConsoleIO.c
        src/std/CpStdLib_FileIO.c
        src/std/CpStdLib_Math.c
        src/std/CpStdLib_Utilities.c
)

target_link_libraries(SharedStandardLibrary SharedAsmLibrary)

set_target_properties(SharedStandardLibrary PROPERTIES PREFIX "lib")
set_target_properties(SharedStandardLibrary PROPERTIES OUTPUT_NAME "CpStdLib.Shared")
set_target_properties(SharedStandardLibrary PROPERTIES SUFFIX ".so")

target_include_directories(SharedStandardLibrary PUBLIC include include/std include/shared)

add_executable(cpInt
        src/cpint/CpInt.c
)

target_link_libraries(cpInt StaticAsmLibrary StaticStandardLibrary "m")
target_include_directories(cpInt PUBLIC include)

add_executable(cpObjCmp
        src/cpobjcmp/CpObjCmp.c
)

target_link_libraries(cpObjCmp StaticObjLibrary "m")
target_include_directories(cpObjCmp PUBLIC include)

add_executable(cpConsole
        src/cpconsole/CpConsole.c
)

target_link_libraries(cpConsole StaticAsmLibrary StaticObjLibrary StaticStandardLibrary "m")
target_include_directories(cpConsole PUBLIC include)

add_executable(execRuntime
        src/execruntime/ExecRuntime.c
)

target_link_libraries(execRuntime StaticAsmLibrary StaticStandardLibrary "m")
target_include_directories(execRuntime PUBLIC include)

add_executable(makeExec
        src/makeexec/MakeExec.c
)

target_link_libraries(makeExec StaticAsmLibrary StaticObjLibrary StaticStandardLibrary "m")
target_include_directories(makeExec PUBLIC include)