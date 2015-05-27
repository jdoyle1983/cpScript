#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

CLASS Node
	PROP ArrayPrimitive
	
	METHOD Init()
		Array Tmp
		ArrayPrimitive = Tmp.GetPrimitiveType()
	END METHOD
	
END CLASS

CLASS Route
	PROP Dest
	PROP Cost
	
	METHOD Init()
		Dest = 0
		Cost = 0
	END METHOD
	
	METHOD Initialize(dest, cost)
		Dest = dest
		Cost = cost
	END METHOD
END CLASS

CLASS LongestPathFinder
	METHOD Init()
	END METHOD
	
	METHOD ReadPlaces(FilePath)
		Array Lines
		
	END METHOD
END CLASS


FUNCTION Main()
	VAR StartTime = Time.GetTimeOfDay()
	
	
	VAR Counter = 0
	FOR Counter = 1 TO 1000
		Console.WriteLine(Counter)
	NEXT Counter
	
	
	VAR BaseString = "1 2 3 4 5"
	VAR DelimString = " "
	
	Array Res

	String.Split(BaseString, DelimString, Res)
	
	
	Console.WriteLine(Res.Size())
	Counter = 0
	VAR TotalSize = Res.Size() - 1
	FOR Counter = 0 TO TotalSize
		Console.WriteLine(Res.GetItem(Counter))
	NEXT Counter
	
	File readFile
	readFile.OpenRead("./agraph")
	VAR PrimVal = readFile.ReadAllLines()
	
	Array readArray
	readArray.FromPrimitiveType(PrimVal)
	
	Counter = 0
	TotalSize = readArray.Size() - 1
	FOR Counter = 0 to TotalSize
		Console.WriteLine(readArray.GetItem(Counter))
	NEXT COUNTER
	
	VAR EndTime = Time.GetTimeOfDay()
	VAR TotalTime = Time.GetDiff(EndTime, StartTime)
	VAR TotalMs = Time.GetMilliSeconds(TotalTime)
	
	Time.FreeTimeOfDay(StartTime)
	Time.FreeTimeOfDay(EndTime)
	Time.FreeTimeOfDay(TotalTime)
	
	Console.Write(TotalMs)
	Console.WriteLine("ms Execution")
	
	Res.Free()
	
END FUNCTION
