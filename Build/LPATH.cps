#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

CLASS Storable
	METHOD Init()
	END METHOD
	
	METHOD ToDataString()
	END METHOD
	
	METHOD FromDataString(DataString)
	END METHOD
END CLASS

CLASS Route EXTENDS Storable
	PROP Dest
	PROP Cost
	
	METHOD Init()
	END METHOD
	
	METHOD OVERRIDE ToDataString()
		RETURN Dest .. "," .. Cost
	END METHOD
	
	METHOD OVERRIDE FromDataString(DataString)
		Array Results
		String.Split(DataString, ",", Results)
		Dest = Results.GetItem(0)
		Cost = Results.GetItem(1)
		Results.Free()
	END METHOD
END CLASS

CLASS Node EXTENDS Storable
	PROP Neighbors
	
	METHOD Init()
		Array NeighborsArray
		Neighbors = NeighborsArray.GetPrimitiveType()
	END METHOD
	
	METHOD OVERRIDE ToDataString()
		RETURN Neighbors
	END METHOD
	
	METHOD OVERRIDE FromDataString(DataString)
		Neighbors = DataString
	END METHOD
	
	METHOD GetNeighborsArrayPrimitive()
		RETURN Neighbors
	END METHOD
END CLASS

CLASS LongestPathFinder
	METHOD Init()
	END METHOD
	
	METHOD TestData()
	END METHOD
	
	METHOD ReadPlaces(FilePath)
		Console.WriteLine("RP 1")
		Array Lines
		Console.WriteLine("RP 2")
		File readFile
		Console.WriteLine("RP 3")
		readFile.OpenRead(FilePath)
		Console.WriteLine("RP 4")
		VAR ArrPrim = readFile.ReadAllLines()
		Console.WriteLine("RP 5")
		readFile.Close()
		Console.WriteLine("RP 6")
		Array readResults
		Console.WriteLine("RP 7")
		readResults.FromPrimitiveType(ArrPrim)
		Console.WriteLine("RP 8")
		VAR NumNodes = readResults.GetItem(0)
		Console.WriteLine("RP 9")
		NumNodes = NumNodes - 1
		Console.WriteLine("RP 10")
		Array Nodes
		Console.WriteLine("RP 11")
		Nodes.Resize(NumNodes)
		Console.WriteLine("RP 12")
		Array Routes
		Console.WriteLine("RP 13")
		Routes.Resize(NumNodes)
		Console.WriteLine("RP 14")
		VAR i = 0
		Console.WriteLine("RP 15")
		FOR i = 0 TO NumNodes
			Node newNode
			Console.WriteLine("RP 16")
			Nodes.SetItem(i, newNode.ToDataString())
			Console.WriteLine("RP 17")
			Array routesArray
			Console.WriteLine("RP 18")
			Routes.SetItem(i, routesArray.GetPrimitiveType())
			Console.WriteLine("RP 19")
		NEXT i
		Console.WriteLine("RP 20")
		VAR LineCount = Lines.Size()
		Console.WriteLine("RP 21")
		FOR i = 1 to LineCount - 1
			Console.WriteLine("RP 22")
			Array Nums
			Console.WriteLine("RP 23")
			String.Split(Lines.GetItem(i), " ", Nums)
			Console.WriteLine("RP 24 - '" .. Nums.Size() .. "'")
			IF Nums.Size() >= 3 THEN
				Console.WriteLine("RP 25")
				VAR tNode = Nums.GetItem(0)
				Console.WriteLine("RP 26")
				VAR tNeighbor = Nums.GetItem(1)
				Console.WriteLine("RP 27")
				VAR tCost = Nums.GetItem(2)
				Console.WriteLine("RP 28")
				VAR RouteArrPrim = Routes.GetItem(tNode)
				Console.WriteLine("RP 29")
				Array RouteArray
				Console.WriteLine("RP 30")
				RouteArray.FromPrimitiveType(RouteArrPrim)
				Console.WriteLine("RP 31")
				Route tRoute
				Console.WriteLine("RP 32")
				tRoute.Dest = tNeighbor
				Console.WriteLine("RP 33")
				tRoute.Cost = tCost
				Console.WriteLine("RP 34")
				RouteArray.Resize(RouteArray.Size() + 1)
				Console.WriteLine("RP 35")
				RouteArray.SetItem(RouteArray.Size() - 1, tRoute.ToDataString())
				Console.WriteLine("RP 36")
			END IF
			Console.WriteLine("RP 37")
		NEXT i
		Console.WriteLine("RP 38")
		VAR RouteCount = Routes.Size() - 1
		Console.WriteLine("RP 39")
		FOR i = 0 TO RouteCount
			Console.WriteLine("RP 40")
			Node tNode
			Console.WriteLine("RP 41")
			tNode.FromDataString(Nodes.GetItem(i))
			Console.WriteLine("RP 42")
			VAR NArrPrim = tNode.GetNeighborsArrayPrimitive()
			Console.WriteLine("RP 43")
			Array NArr
			Console.WriteLine("RP 44")
			NArr.FromPrimitiveType(NArrPrim)
			Console.WriteLine("RP 45")
			VAR RouteArrPrim = Routes.GetItem(i)
			Console.WriteLine("RP 46")
			Array RouteArr
			Console.WriteLine("RP 47")
			RouteArr.FromPrimitiveType(RouteArrPrim)
			Console.WriteLine("RP 48")
			NArr.Resize(RouteArr.Size())
			Console.WriteLine("RP 49")
			VAR ArCounter = 0
			Console.WriteLine("RP 50")
			VAR ArSize = RouteArr.Size() - 1
			Console.WriteLine("RP 51")
			FOR ArCounter = 0 TO ArSize
				Console.WriteLine("RP 52")
				NArr.SetItem(ArCounter, RouteArr.GetItem(ArCounter))
				Console.WriteLine("RP 53")
			NEXT ArCounter
			Console.WriteLine("RP 54")
		NEXT i
		Console.WriteLine("RP 55")
		RETURN Nodes.GetPrimitiveType()
	END METHOD
	
	METHOD GetLongestPath(NodesArrPrim, NodeIndex, VisitedPrim)
	
		TestData()
	
		VAR Max = 0
		
		Array VArr
		VArr.FromPrimitiveType(VisitiedPrim)
		VArr.SetItem(NodeIndex, 1)
		
		Array NArr
		NArr.FromPrimitiveType(NodesArrPrim)
		Node TNode
		TNode.FromDataString(NArr.GetItem(NodeIndex))
		
		Array NRoute
		NRoute.FromPrimitiveType(TNode.GetNeighborsArrayPrimitive())
		
		VAR i = 0
		VAR NeighborsLen = NRoute.Size() - 1
		
		FOR i = 0 TO NeighborsLen
			Route ThisRoute
			ThisRoute.FromDataString(NRoute.GetItem(i))
			
			IF VArr.GetItem(ThisRoute.Dest) != 1 THEN
				VAR Dist = ThisRoute.Cost + GetLongestPath(NodesArrPrim, ThisRoute.Dest, VisitedPrim)
				IF Dist > Max THEN
					Max = Dist
				END IF
			END IF
		NEXT i
		
		VArr.SetItem(NodeIndex, 0)
		
		RETURN Max
	END METHOD
END CLASS


FUNCTION Main()

	Console.WriteLine("Here 1")
	LongestPathFinder PathFinder
	Console.WriteLine("Here 2")
	VAR NodePrim = PathFinder.ReadPlaces("./agraph")
	Console.WriteLine("Here 3")
	Array Nodes
	Console.WriteLine("Here 4")
	Nodes.FromPrimitiveType(Nodes)
	Console.WriteLine("Here 5")
	Array Visited
	Console.WriteLine("Here 6")
	Visited.Resize(Nodes.Size())
	Console.WriteLine("Here 7")
	VAR i = 0
	Console.WriteLine("Here 8")
	VAR c = Nodes.Size() - 1
	Console.WriteLine("Here 9")
	FOR i = 0 to c
		Visited.SetItem(i, 0)
	NEXT i
	Console.WriteLine("Here 10")
	VAR StartTime = Time.GetTimeOfDay()
	Console.WriteLine("Here 11")
	VAR Result = PathFinder.GetLongestPath(NodePrim, 0, Visited.GetPrimitiveType())
	Console.WriteLine("Here 12")
	VAR EndTime = Time.GetTimeOfDay()
	Console.WriteLine("Here 13")
	VAR TotalTime = Time.GetDiff(EndTime, StartTime)
	Console.WriteLine("Here 14")
	VAR TotalMs = Time.GetMilliSeconds(TotalTime)
	Console.WriteLine("Here 15")
	Time.FreeTimeOfDay(StartTime)
	Console.WriteLine("Here 16")
	Time.FreeTimeOfDay(EndTime)
	Console.WriteLine("Here 17")
	Time.FreeTimeOfDay(TotalTime)
	Console.WriteLine("Here 18")
	Console.WriteLine(Result .. " LANGUAGE ObjectBasic / AsmScript " .. TotalMs .. " ms")
	Console.WriteLine("Here 19")
	Nodes.Free()
	Console.WriteLine("Here 20")
	Visited.Free()
	Console.WriteLine("Here 21")
END FUNCTION
