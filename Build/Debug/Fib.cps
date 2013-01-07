#include Lib/ConsoleIO.obh

FUNCTION Main()
	VAR BoolTest = false
	VAR DoubleTest = 8.492
	VAR IntTest = 0
	VAR StringTest = "---"
	
	Console.WriteLine("From Script: BoolTest   : " .. BoolTest)
	Console.WriteLine("From Script: DoubleTest : " .. DoubleTest)
	Console.WriteLine("From Script: IntTest    : " .. IntTest)
	Console.WriteLine("From Script: StringTest : " .. StringTest)
	
	BoolTest = true
	DoubleTest = 6.232
	IntTest = 10
	StringTest = "-dfdf-"
	
	Console.WriteLine("From Script: BoolTest   : " .. BoolTest)
	Console.WriteLine("From Script: DoubleTest : " .. DoubleTest)
	Console.WriteLine("From Script: IntTest    : " .. IntTest)
	Console.WriteLine("From Script: StringTest : " .. StringTest)
	
	Console.WriteLine(" ")
	Console.Write("Enter Fibonacci Sequence Iteration Count: ")
	VAR fibMax = Console.ReadLine()
	VAR f1 = 0
	VAR f2 = 0
	VAR f3 = 0
	VAR i = 0
	FOR i = 1 to fibMax
		IF i == 1 THEN
			f1 = 1
			Console.WriteLine("Seq " .. i .. ": " .. f1)
		ELSE IF i == 2 THEN
			f2 = 1
			Console.WriteLine("Seq " .. i .. ": " .. f2)
		ELSE
			f3 = f1 + f2
			Console.WriteLine("Seq " .. i .. ": " .. f3)
			f1 = f2
			f2 = f3
		END IF
	NEXT
END FUNCTION