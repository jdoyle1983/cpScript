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
