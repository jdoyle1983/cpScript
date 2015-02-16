#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

FUNCTION Main()
	VAR StartTime = Time.GetTimeOfDay()
	
	VAR i = 0
	FOR i = 1 TO 10000
	NEXT 
	
	VAR EndTime = Time.GetTimeOfDay()
	VAR TotalTime = Time.GetDiff(EndTime, StartTime)
	
	VAR TotalMs = Time.GetMilliSeconds(TotalTime)
	
	Time.FreeTimeOfDay(StartTime)
	Time.FreeTimeOfDay(EndTime)
	Time.FreeTimeOfDay(TotalTime)
	
	Console.Write(TotalMs)
	Console.WriteLine("ms Execution")
	
END FUNCTION