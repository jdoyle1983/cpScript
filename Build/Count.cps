#include Lib/ConsoleIO.obh

FUNCTION Main()
	Console.WriteLine("")
	Console.WriteLine("")
	Console.Write("Enter Start Value: ")
	VAR startValue = Console.ReadLine()
	Console.Write("Enter End Value: ")
	VAR endValue = Console.ReadLine()
	Console.WriteLine("")
	IF startValue == endValue THEN
		Console.WriteLine("Values Match, No Counting Required.")
	ELSE IF startValue < endValue THEN
		VAR lc = 0
		FOR lc = startValue TO endValue STEP + 1
			Console.WriteLine("Value: " .. lc)
		NEXT
	ELSE
		VAR gc = 0
		FOR gc = startValue TO endValue STEP - 1
			Console.WriteLine("Value: " .. gc)
		NEXT
	END IF
END FUNCTION