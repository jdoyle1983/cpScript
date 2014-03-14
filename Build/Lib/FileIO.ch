extern __StdLib__FileIO__Open
extern __StdLib__FileIO__WriteLine
extern __StdLib__FileIO__ReadLine
extern __StdLib__FileIO__Close
extern __StdLib__FileIO__Eof
extern __StdLib__FileIO__ReadBool
extern __StdLib__FileIO__ReadDouble
extern __StdLib__FileIO__ReadInt
extern __StdLib__FileIO__ReadString
extern __StdLib__FileIO__WriteBool
extern __StdLib__FileIO__WriteDouble
extern __StdLib__FileIO__WriteInt
extern __StdLib__FileIO__WriteString

class File
{
	var FileObj;
	
	File() {}
	~File() {}
	
	void OpenRead(var filepath)
	{
		FileObj = __StdLib__FileIO__Open("r", filepath);
	}
	
	void OpenWrite(var filepath)
	{
		FileObj = __StdLib__FileIO__Open("w", filepath);
	}
	
	void OpenBinaryRead(var filepath)
	{
		FileObj = __StdLib__FileIO__Open("rb", filepath);
	}
	
	void OpenBinaryWrite(var filepath)
	{
		FileObj = __StdLib__FileIO__Open("wb", filepath);
	}
	
	void WriteLine(var val)
	{
		__StdLib__FileIO__WriteLine(FileObj, val);
	}
	
	var ReadLine()
	{
		return __StdLib__FileIO__ReadLine(FileObj);
	}
	
	void Close()
	{
		__StdLib__FileIO__Close(FileObj);
	}
	
	var Eof()
	{
		return __StdLib__FileIO__Eof(FileObj);
	}
	
	void WriteInt(var val)
	{
		__StdLib__FileIO__WriteInt(FileObj, val);
	}
	
	var ReadInt()
	{
		return __StdLib__FileIO__ReadInt(FileObj);
	}
	
	void WriteDouble(var val)
	{
		__StdLib__FileIO__WriteDouble(FileObj, val);
	}
	
	var ReadDouble()
	{
		return __StdLib__FileIO__ReadDouble(FileObj);
	}
	
	void WriteBool(var val)
	{
		__StdLib__FileIO__WriteBool(FileObj, val);
	}
	
	var ReadBool()
	{
		return __StdLib__FileIO__ReadBool(FileObj);
	}
	
	void WriteString(var val)
	{
		__StdLib__FileIO__WriteString(FileObj, val);
	}
	
	var ReadString()
	{
		return __StdLib__FileIO__ReadString(FileObj);
	}
}
