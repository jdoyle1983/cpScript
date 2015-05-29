#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

--https://raw.githubusercontent.com/logicchains/LPATHBench/master/cs.cs

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
		Dest = -1
		Cost = -1
	END METHOD
	
	METHOD SetData(inDest, inCost)
		Dest = inDest
		Cost = inCost
	END METHOD
	
	METHOD OVERRIDE ToDataString()
		VAR Enc = Dest .. "," .. Cost
		RETURN Enc
	END METHOD
	
	METHOD OVERRIDE FromDataString(DataString)
		Array Results
		String.Split(DataString, ",", Results)
		Dest = Results.GetItem(0)
		Cost = Results.GetItem(1)
		Results.Free()
	END METHOD
END CLASS

CLASS LongestPathFinder

	PROP Iterations

	METHOD Init()
	END METHOD
	
	METHOD ReadPlaces(FilePath)

		Array2D Nodes	
		File readFile
		readFile.OpenRead(FilePath)
		VAR FileArrPrim = readFile.ReadAllLines()
		readFile.Close()
		Array Lines
		Lines.FromPrimitiveType(FileArrPrim)
		
		VAR NumNodes = Lines.GetItem(0)
		
		Array NodeCount
		NodeCount.Resize(NumNodes)
		
		Nodes.Resize(NumNodes, 0)
		
		NumNodes = NumNodes - 1
		
		VAR i = 0
		FOR i = 0 TO NumNodes
			NodeCount.SetItem(i, 0)
		NEXT i
		
		VAR LineCount = Lines.Size()
		
		FOR i = 1 to LineCount - 1
			Array Nums
			String.Split(Lines.GetItem(i), " ", Nums)
			
			IF Nums.Size() >= 3 THEN
			
				VAR tNode = Nums.GetItem(0)
				VAR tNeighbor = Nums.GetItem(1)
				VAR tCost = Nums.GetItem(2)
				
				Route tRoute
				tRoute.SetData(tNeighbor, tCost)
				
				VAR NewSize = NodeCount.GetItem(tNode) + 1
				
				NodeCount.SetItem(tNode, NewSize)
				
				VAR MaxSize = Nodes.YSize()
				
				IF NewSize > MaxSize THEN
					Nodes.ResizeY(NewSize)
				END IF
				
				Console.WriteLine(".")
				VAR DataString = tRoute.ToDataString()
				Nodes.SetItem(tNode, NewSize - 1, DataString)
				Console.WriteLine("@")
			END IF
			
			Nums.Free()
		NEXT i
		
		VAR TotalY = Nodes.YSize() - 1
		VAR ThisItemSize = 9999999
		VAR e = 0
		FOR i = 0 TO NumNodes
			ThisItemSize = NodeCount.GetItem(i)
			FOR e = ThisItemSize TO TotalY
				Nodes.SetItem(i, e, "-1")
			NEXT e
		NEXT i
		
		NodeCount.Free()
		
		RETURN Nodes.GetPrimitiveType()
	END METHOD
	
	METHOD GetLongestPath(NodesArrPrim, NodeIndex, VisitedPrim)
		
		Iterations = Iterations + 1
		
		IF Iterations % 1000 == 0 THEN
			Console.WriteLine("Iterations: " .. Iterations)
		END IF
		
		VAR Max = 0

		Array VArr
		VArr.FromPrimitiveType(VisitedPrim)
		VArr.SetItem(NodeIndex, 1)

		Array2D Nodes
		Nodes.FromPrimitiveType(NodesArrPrim)
		
		VAR i = 0
		VAR NeighborsLen = Nodes.YSize() - 1
		
		Console.WriteLine(NeighborsLen)
		
		FOR i = 0 TO NeighborsLen	
			Console.WriteLine(NodeIndex .. "," .. i)
			VAR BaseData = Nodes.GetItem(NodeIndex, i)
			Console.WriteLine(BaseData)
			IF BaseData != "-1" THEN 
				Route ThisRoute
				ThisRoute.FromDataString(BaseData)
			
				IF VArr.GetItem(ThisRoute.Dest) != 1 THEN
					VAR Dist = ThisRoute.Cost + GetLongestPath(NodesArrPrim, ThisRoute.Dest, VisitedPrim)
					IF Dist > Max THEN
						Max = Dist
					END IF
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

	Array2D Nodes
	Nodes.FromPrimitiveType(NodePrim)
	
	Array Visited
	Visited.Resize(Nodes.Size())
	
	Console.WriteLine("Nodes.Size = " .. Nodes.XSize() .. "," .. Nodes.YSize())
	
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
	Console.WriteLine(Result .. " LANGUAGE ObjectBasic / AsmScript " .. TotalMs .. " ms")

	Nodes.Free()
	Visited.Free()
END FUNCTION
