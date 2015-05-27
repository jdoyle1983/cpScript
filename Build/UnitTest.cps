#include Lib/Math.obh
#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Utilities.obh

FUNCTION Main()
	
	Console.Write("Console.Write #1 ")
	Console.Write("Console.Write #2 ")
	Console.WriteLine("")
	Console.WriteLine("Console.WriteLine #1 ")
	Console.WriteLine("Console.WriteLine #2 ")
	Console.WriteLine("")
	Console.Write("Enter a String: ")
	VAR inputValue = Console.ReadLine()
	Console.WriteLine("Entered Value: '" .. inputValue .. "'")
	Console.WriteLine("")
	Console.WriteLine("")
	Console.WriteLine("ABS of -45: " .. Math.ABS(-45))
	Console.WriteLine("CEILING of 35.89: " .. Math.CEILING(35.89))
	Console.WriteLine("FLOOR of 35.89: " .. Math.FLOOR(35.89))
	Console.WriteLine("")
	Console.WriteLine("Writing Text File...")
	File textWriteFile
	textWriteFile.OpenWrite("UnitTestText.txt")
	textWriteFile.WriteLine("Line #1")
	textWriteFile.WriteLine("Line #2")
	textWriteFile.WriteLine("Line #3")
	textWriteFile.Close()
	Console.WriteLine("Wrote Text File")
	Console.WriteLine("")
	Console.WriteLine("Reading Text File...")
	File textReadFile
	textReadFile.OpenRead("UnitTestText.txt")
	WHILE(textReadFile.Eof() == false)
		Console.WriteLine(textReadFile.ReadLine())
	LOOP
	textReadFile.Close()
	Console.WriteLine("Read Text File")
	Console.WriteLine("")
	Console.WriteLine("Creating Array")
	Array testArray
	Console.WriteLine("Setting Array Size To 10")
	testArray.Resize(10)
	Console.WriteLine("Array Size: " .. testArray.Size())
	Console.WriteLine("Setting Array Size To 5")
	testArray.Resize(5)
	Console.WriteLine("Array Size: " .. testArray.Size())
	Console.WriteLine("Setting Array Values...")
	testArray.SetItem(0, "Item #1")
	testArray.SetItem(1, "Item #2")
	testArray.SetItem(2, "Item #3")
	testArray.SetItem(3, "Item #4")
	testArray.SetItem(4, "Item #5")
	Console.WriteLine("Array Values Set")
	Console.WriteLine("")
	Console.WriteLine("Reading Array Values")
	VAR iter = 0
	VAR term = testArray.Size()
	FOR iter = 0 TO term
		Console.WriteLine(testArray.GetItem(iter))
	NEXT
	Console.WriteLine("Read Array Values")
	Console.WriteLine("Resizing Array...")
	testArray.Resize(7)
	Console.WriteLine("Array Resized")
	Console.WriteLine("Setting Additional Values...")
	testArray.SetItem(5, "Item #6")
	testArray.SetItem(6, "Item #7")
	Console.WriteLine("Values Set")
	Console.WriteLine("Reading Array Values...")
	term = testArray.Size();
	FOR iter = 0 TO term
		Console.WriteLine(testArray.GetItem(iter))
	NEXT
	Console.WriteLine("Read Array Values")
	Console.WriteLine("Deleting Array...")
	testArray.Free()
	Console.WriteLine("Array Deleted.")
	Console.WriteLine("")
	Console.WriteLine("")
	Console.WriteLine("Unit Test Complete.")
	
END FUNCTION