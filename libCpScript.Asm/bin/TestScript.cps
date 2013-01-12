#include Lib/ConsoleIO.obh
#include Lib/FileIO.obh
#include Lib/Math.obh
#include Lib/Utilities.obh

CLASS BaseObject
	PROP ObjectId
	
	METHOD Init()
		ObjectId = 0
	END METHOD
END CLASS

CLASS String EXTENDS BaseObject
	PROP Value
	
	METHOD Init()
		ObjectId = 1
	END METHOD
END CLASS

CLASS StringBuilder EXTENDS String
	METHOD Init()
		ObjectId = 2
	END METHOD
	
	METHOD Append(val)
		Value = Value .. val
	END METHOD

	METHOD AppendLine(val)
		Value = Value .. val .. "\n"
	END METHOD
END CLASS

CLASS TestClass1
	METHOD Init()
	END METHOD
	
	METHOD ShouldDoSomething(myval)
		Console.WriteLine(myval)
	END METHOD
END CLASS

CLASS TestClass2
	METHOD Init()
	END METHOD
	
	METHOD DoSomething(myitem:TestClass1)
	
		myitem.ShouldDoSomething("Test Line")
	
	END METHOD
END CLASS

FUNCTION Main()

	TestClass1 ts1
	TestClass2 ts2
	ts2.DoSomething(ts1)

	File f1
	f1.OpenWrite("output.txt")
	f1.WriteLine("Line 1")
	f1.WriteLine("Line 2")
	f1.Close()
	File f2
	f2.OpenRead("output.txt")
	Console.WriteLine(f2.ReadLine())
	Console.WriteLine(f2.ReadLine())
	f2.Close()
	
	Console.WriteLine("Function Main() Start")
	StringBuilder builder
	builder.AppendLine("Line 1")
	builder.AppendLine("Line 2")
	builder.AppendLine("Line 3")
	builder.AppendLine("Live 4")
	Console.WriteLine(builder.Value)
	Console.WriteLine("Enter a String: ")
	VAR Input = Console.ReadLine()
	Console.Write("You Entered: ")
	Console.WriteLine(Input)
	Console.WriteLine("Goodbye.")
	Console.WriteLine("Function Main() End")
	
	VAR myVar
	myVar = 189 * (232 / 34 + 83 - 34) * 45
	Console.WriteLine(myVar)
	
	VAR myVar2 = 8 % 3
	Console.WriteLine(myVar2)
	
	Console.WriteLine(Math.ABS(-23))
	
	VAR myVar3 = 125 * ((4 + 3) * (-(28 - 4) * 5) + 88)
	Console.WriteLine(myVar3)

	Array ar1
	ar1.Alloc(5)
	ar1.SetItem(0, "Idx 0")
	ar1.SetItem(1, "Idx 1")
	ar1.SetItem(2, "Idx 2")
	ar1.SetItem(3, "Idx 3")
	ar1.SetItem(4, "Idx 4")

	VAR c = 0
	VAR s = ar1.Size() - 1
	FOR c = 0 to s
		Console.WriteLine(c .. " - " .. ar1.GetItem(c))
	NEXT

	ar1.Free()
END FUNCTION