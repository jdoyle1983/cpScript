--This is very much a work in progress

#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

--https://raw.githubusercontent.com/logicchains/LPATHBench/master/cs.cs

CLASS Node
	PROP RouteArrPrim
	
	METHOD Init()
		Array Arr
		RouteArrPrim = Arr.GetPrimitiveType()
	END METHOD
	
	METHOD GetRouteArrayPrim()
		RETURN RouteArrPrim
	END METHOD
	
	METHOD AddRouteStr(strData)
		Array Arr
		Arr.FromPrimitiveType(RouteArrPrim)
		Arr.Resize(Arr.Size() + 1)
		Arr.SetItem(Arr.Size() - 1, strData)
	END METHOD
	
	METHOD GetRouteStr(index)
		Array Arr
		Arr.FromPrimitiveType(RouteArrPrim)
		RETURN Arr.GetItem(index)
	END METHOD
	
	METHOD ToDataString()
		RETURN RouteArrPrim
	END METHOD
	
	METHOD FromDataString(DataString)
		RouteArrPrim = DataString
	END METHOD
	
	METHOD Free()
		Array Arr
		Arr.FromPrimitiveType(RouteArrPrim)
		Arr.Free()
	END METHOD
END CLASS

CLASS LongestPathFinder

	PROP Iterations

	METHOD Init()
	END METHOD
	
	METHOD ReadPlaces(FilePath)

		File readFile
		readFile.OpenRead(FilePath)
		VAR FileArrPrim = readFile.ReadAllLines()
		readFile.Close()
		Array Lines
		Lines.FromPrimitiveType(FileArrPrim)
		
		VAR NumNodes = Lines.GetItem(0)
		
		Array Nodes
		Nodes.Resize(NumNodes)
		
		Array Routes
		Routes.Resize(NumNodes)
		
		VAR i = 0
		VAR e = 0
		
		FOR i = 0 TO NumNodes - 1
			Node thisNode
			Nodes.SetItem(i, thisNode.ToDataString())
			Array thisRoute
			Routes.SetItem(i, thisRoute.GetPrimitiveType())
		NEXT i
		
		FOR i = 1 TO LineCount - 1
			Array Nums
			String.Split(Lines.GetItem(i), " ", Nums)
			
			IF Nums.Size() >= 3 THEN
				VAR node = Nums.GetItem(0)
				VAR neighbour = Nums.GetItem(1)
				VAR cost = Nums.GetItem(2)
				
				Array routesArray
				routesArray.FromPrimitiveType(Routes.GetItem(node))
				routesArray.Resize(routesArray.Size() + 1)
				routesArray.SetItem(routesArray.Size() - 1, neighbour .. "," .. cost)
			END IF
			
			Nums.Free()
		NEXT i
		
		FOR i = 0 TO Routes.Size() - 1
			Node thisNode
			thisNode.FromDataString(Nodes.GetItem(i))
			Array thisRoute
			thisRoute.FromPrimitiveType(Routes.GetItem(i))
			FOR e = 0 TO thisRoute.Size() - 1
				thisNode.AddRouteStr(thisRoute.GetItem(e))
			NEXT e
			thisRoute.Free()
		NEXT i
		
		Routes.Free()
		
		RETURN Nodes.GetPrimitiveType()
		
	END METHOD
	
	METHOD GetLongestPath(NodesArrPrim, NodeIndex, VisitedPrim)
		VAR Max = 0

		Array VArr
		VArr.FromPrimitiveType(VisitedPrim)
		VArr.SetItem(NodeIndex, 1)

		Array Nodes
		Nodes.FromPrimitiveType(NodesArrPrim)
		
		Array Neighbors
		Neighbors.FromPrimitiveType(Nodes.GetItem(NodeIndex))
		
		VAR i = 0
		
		FOR i = 0 TO Neighbors.Size() - 1
			Array DestCost
			String.Split(Neighbors.GetItem(i), ",", DestCost)
			IF VArr.GetItem(DestCost.GetItem(0)) <> 1 THEN
				VAR Dist = DestCost.GetItem(1) + GetLongestPath(NodesArrPrim, DestCost.GetItem(0), VisitedPrim)
				IF Dist > Max THEN
					Max = Dist
				END IF
			END IF
			DestCost.Free()
		NEXT i
		VArr.SetItem(NodeIndex, 0)
		RETURN Max
	END METHOD
END CLASS

FUNCTION Main()
	LongestPathFinder PathFinder
	VAR NodePrim = PathFinder.ReadPlaces("./agraph")
	Array Nodes
	Nodes.FromPrimitiveType(NodePrim)
		
	Array Visited
	Visited.Resize(Nodes.XSize())
	
	VAR i = 0
	VAR c = Nodes.XSize() - 1
	
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
	Console.WriteLine(Result .. " LANGUAGE ObjectBasic / AsmScript " .. TotalMs .. " ms, Iterations = " .. PathFinder.Iterations)

	Nodes.Free()
	Visited.Free()
END FUNCTION
