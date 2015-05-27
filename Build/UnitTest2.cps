#include Lib/Math.obh
#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

FUNCTION Main()
	
	Console.WriteLine("Asm Block Below")
	var T1 = 14
	var T2 = 10
	var T3 = 0
	Array testArray
	testArray.Resize(10)
	ASMPUSH testArray
	ASMPUSH T1
	ASMPUSH T2
	ASMBLOCK
	ADD
	END ASMBLOCK
	ASMPOP T3
	ASMPOP testArray
	testArray.Free()
	Console.WriteLine("T1: " .. T1)
	Console.WriteLine("T2: " .. T2)
	Console.WriteLine("T3: " .. T3)
END FUNCTION