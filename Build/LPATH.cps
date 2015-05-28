#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

CLASS RouteList
	PROP DestArray
	PROP CostArray
	
	METHOD Init()
		Array DestArr
		Array CostArr
		DestArray = DestArr.GetPrimitiveType()
		CostArray = CostArr.GetPrimitiveType()
	END METHOD
	
	METHOD AddRoute(Dest, Cost)
		Array DestArray
		Array CostArray
		DestArray.FromPrimitiveType(DestArr)
		CostArray.FromPrimitiveType(CostArr)
		DestArray.Resize(DestArray.Size() + 1)
		DestArray.SetItem(DestArray.Size() - 1, Dest)
		CostArray.Resize(CostArray.Size() + 1)
		CostArray.SetItem(CostArray.Size() - 1, Cost)
	END METHOD
	
	METHOD GetRouteDest(Index)
		Array DestArray
		DestArray.FromPrimitiveType(DestArr)
		RETURN DestArray.GetItem(Index)
	END METHOD
	
	METHOD GetRouteCost(Index)
		Array CostArray
		CostArray.FromPrimitiveType(CostArr)
		RETURN CostArray.GetItem(Index)
	END METHOD
END CLASS

CLASS LongestPathFinder
	METHOD Init()
	END METHOD
	
	METHOD ReadPlaces(FilePath)
		Array Lines
		File readFile
		readFile.OpenRead(FilePath)
		VAR ArrPrim = readFile.ReadAllLines()
		readFile.Close()
		Array readResults
		readResults.FromPrimitiveType(ArrPrim)
		VAR NumNodes = readResults.GetItem(0)
		Array Nodes
		Nodes.Resize(NumNodes)
		Array Routes
		VAR i = 0
		FOR i = 0 TO NumNodes - 1
			
		NEXT i
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
		Console.WriteLine("Read Value: '" .. readArray.GetItem(Counter) .. "'")
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
