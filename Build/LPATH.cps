#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

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


FUNCTION Main()
	VAR StartTime = Time.GetTimeOfDay()
	
	
	VAR Counter = 0
	FOR Counter = 1 TO 1000
		Console.WriteLine(Counter)
	NEXT Counter
	
	
	VAR BaseString = "1 2 3 4 5"
	VAR DelimString = " "
	
	Array Res
	Res.Alloc(0)
	
	String.Split(BaseString, DelimString, Res)
	
	
	Console.WriteLine(Res.Size())
	Counter = 0
	VAR TotalSize = Res.Size() - 1
	FOR Counter = 0 TO TotalSize
		Console.WriteLine(Res.GetItem(Counter))
	NEXT Counter
	
	VAR EndTime = Time.GetTimeOfDay()
	VAR TotalTime = Time.GetDiff(EndTime, StartTime)
	VAR TotalMs = Time.GetMilliSeconds(TotalTime)
	
	Time.FreeTimeOfDay(StartTime)
	Time.FreeTimeOfDay(EndTime)
	Time.FreeTimeOfDay(TotalTime)
	
	Console.Write(TotalMs)
	Console.WriteLine("ms Execution")
	
END FUNCTION
