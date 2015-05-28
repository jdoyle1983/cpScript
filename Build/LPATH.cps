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
		Array Lines
		File readFile
		readFile.OpenRead(FilePath)
		VAR ArrPrim = readFile.ReadAllLines()
		readFile.Close()
		Array readResults
		readResults.FromPrimitiveType(ArrPrim)
		VAR NumNodes = readResults.GetItem(0)
		NumNodes = NumNodes - 1
		Array Nodes
		Nodes.Resize(NumNodes)
		Array Routes
		Routes.Resize(NumNodes)
		VAR i = 0
		FOR i = 0 TO NumNodes
			Node newNode
			Nodes.SetItem(i, newNode.ToDataString())
			Array routesArray
			Routes.SetItem(i, routesArray.GetPrimitiveType())
		NEXT i
		
		VAR LineCount = Lines.Size()
		FOR i = 1 to LineCount - 1
			Array Nums
			String.Split(Lines.GetItem(i), " ", Nums)
			IF Nums.Size() >= 3 THEN
				VAR tNode = Nums.GetItem(0)
				VAR tNeighbor = Nums.GetItem(1)
				VAR tCost = Nums.GetItem(2)
				VAR RouteArrPrim = Routes.GetItem(tNode)
				Array RouteArray
				RouteArray.FromPrimitiveType(RouteArrPrim)
				Route tRoute
				tRoute.Dest = tNeighbor
				tRoute.Cost = tCost
				RouteArray.Resize(RouteArray.Size() + 1)
				RouteArray.SetItem(RouteArray.Size() - 1, tRoute.ToDataString())
			END IF
		NEXT i
		
		VAR RouteCount = Routes.Size() - 1
		FOR i = 0 TO RouteCount
			Node tNode
			tNode.FromDataString(Nodes.GetItem(i))
			VAR NArrPrim = tNode.GetNeighborsArrayPrimitive()
			Array NArr
			NArr.FromPrimitiveType(NArrPrim)
			
			VAR RouteArrPrim = Routes.GetItem(i)
			Array RouteArr
			RouteArr.FromPrimitiveType(RouteArrPrim)
			NArr.Resize(RouteArr.Size())
			VAR ArCounter = 0
			VAR ArSize = RouteArr.Size() - 1
			FOR ArCounter = 0 TO ArSize
				NArr.SetItem(ArCounter, RouteArr.GetItem(ArCounter))
			NEXT ArCounter
		NEXT i
		
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

	LongestPathFinder PathFinder
	
	VAR NodePrim = PathFinder.ReadPlaces("./agraph")
	Array Nodes
	Nodes.FromPrimitiveType(Nodes)
	
	Array Visited
	Visited.Resize(Nodes.Size())
	VAR i = 0
	VAR c = Nodes.Size() - 1
	FOR i = 0 to c
		Visited.SetItem(i, 0)
	NEXT i
	
	VAR StartTime = Time.GetTimeOfDay()
	
	VAR Result = PathFinder.GetLongestPath(NodePrim, 0, Visited.GetPrimitiveType())
	
	VAR EndTime = Time.GetTimeOfDay()
	VAR TotalTime = Time.GetDiff(EndTime, StartTime)
	VAR TotalMs = Time.GetMilliSeconds(TotalTime)
	
	Time.FreeTimeOfDay(StartTime)
	Time.FreeTimeOfDay(EndTime)
	Time.FreeTimeOfDay(TotalTime)
	
	Console.WriteLine(Result .. " LANGUAGE ObjectBasic / AsmScript " .. TotalMs .. " ms")
	
	Nodes.Free()
	Visited.Free()
	
END FUNCTION
