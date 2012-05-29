using System;
using System.IO;

namespace cpScriptTest.Net
{
	class MainClass
	{
		public static void ParamCheck(libCpScript.Net.Asm.State State)
		{
			string p5 = State.PopString();
			string p4 = State.PopString();
			string p3 = State.PopString();
			string p2 = State.PopString();
			string p1 = State.PopString();
			
			Console.WriteLine("p1 = " + p1);
			Console.WriteLine("p2 = " + p2);
			Console.WriteLine("p3 = " + p3);
			Console.WriteLine("p4 = " + p4);
			Console.WriteLine("p5 = " + p5);
		}
		
		public static void DoWrite(libCpScript.Net.Asm.State State)
		{
			Console.WriteLine(State.PopString());
		}
		
		public static void Main (string[] args)
		{
			string Script = "LBL Add2Ints\n" +
							"POP @1\n" + 
							"POP @2\n" +
							"PUSH @1\n" +
							"PUSH @2\n" +
							"ADD\n" +
							"POP @3\n" +
							"PUSH @3\n" +
							"RTN\n" +
							"LBL Main\n" +
							"AB %Var1\n" +
							"AB %Var2\n" +
							"MOV %Var1, 10\n" +
							"MOV %Var2, 20\n" +
							"PUSH %Var1\n" +
							"PUSH %Var2\n" +
							"JMP Add2Ints\n" +
							"AB %Var3\n" +
							"POP %Var3\n" +
							"PUSH %Var1\n" +
							"JMP DoWrite\n" +					
							"PUSH %Var2\n" +
							"JMP DoWrite\n" +
							"PUSH %Var3\n" +
							"JMP DoWrite\n" +
							"LBL _StartWhile\n" +
							"JE %Var3, 100, _EndWhile\n" +
							"PUSH 1\n" +
							"PUSH %Var3\n" +
							"ADD\n" + 
							"POP %Var3\n" +
							"PUSH %Var3\n" +
							"JMP DoWrite\n" +
							"LJMP _StartWhile\n" +
							"LBL _EndWhile\n" +
							"PUSHA %Var3\n" +
							"POPA %Var4\n" +
							"PUSH %Var3\n" +
							"JMP DoWrite\n" +
							"PUSH %Var4\n" +
							"JMP DoWrite\n" +
							"ABS $TestVar, 3\n" +
							"MOV $TestVar:1, \"Value 1\"\n" +
							"MOV $TestVar:2, \"Value 2\"\n" +
							"MOV $TestVar:3, \"Value 3\"\n" +
							"PUSH $TestVar:1\n" +
							"JMP DoWrite\n" +
							"PUSH $TestVar:2\n" +
							"JMP DoWrite\n" +
							"PUSH $TestVar:3\n" +
							"JMP DoWrite\n" +
							"PUSH \"End Of Exec\"\n" +
							"JMP DoWrite\n" +
							"RTN\n";
			
			/*libCpScript.Net.Asm.State state = new libCpScript.Net.Asm.State(Script);
			state.RegisterFunction("DoWrite", DoWrite);
			state.RunFromMethod("Main");*/
			
			
			
			//counter = counter + 3
			//counter == AllResults
			/*string Script = "FUNCTION Add2Ints (Var1, Var2)\n" +
							"	 RETURN Var1 + Var2\n" + 
							"END FUNCTION\n\n" +
							"FUNCTION MAIN()\n" + 
							"   VAR TestValue = 0 / 32\n" +
							"   WriteLine(TestValue)\n" + 
							"	VAR AllResults = Add2Ints(1,99)\n" +
							"	VAR counter = 1\n" +
							"   WHILE counter < AllResults\n" + 
							"		IF counter < 10\n" +
							"			WriteLine(\"counter < 10 : \" .. counter)\n" +
							"		ELSE IF (counter >= 10) && (counter <= 50)\n" +
							"			WriteLine(\"counter >=10 AND <= 50 : \" .. counter)\n" +
							"		ELSE\n" +
							"			WriteLine(\"counter > 50 : \" .. counter)\n" +
							"		END IF\n" +
							"		counter = counter + 1\n" +
							"	LOOP\n" +
							"END FUNCTION";*/

            string inputScript = File.ReadAllText(AppDomain.CurrentDomain.BaseDirectory + Path.DirectorySeparatorChar + "Fib.cps");
            libCpScript.Net.ObjectBasic.ObjectBasicScript script = new libCpScript.Net.ObjectBasic.ObjectBasicScript();
            script.LoadScript(inputScript);

            File.WriteAllText(AppDomain.CurrentDomain.BaseDirectory + Path.DirectorySeparatorChar + "Fib.asm", script.AsmScript);

            script.LoadMethod("Main");
            script.RunScript();

            /*

            string inputScript = File.ReadAllText(AppDomain.CurrentDomain.BaseDirectory + Path.DirectorySeparatorChar + "TestScript.cps");
            libCpScript.Net.ObjectBasic.ObjectBasicScript script = new libCpScript.Net.ObjectBasic.ObjectBasicScript();
            DateTime ParseStart = DateTime.Now;
            script.LoadScript(inputScript);
            DateTime ParseEnd = DateTime.Now;
            File.WriteAllText(AppDomain.CurrentDomain.BaseDirectory + Path.DirectorySeparatorChar + "Script.casm", script.AsmScript);
            DateTime CompileStart = DateTime.Now;
            byte[] compiledBytes = script.Compile();
            DateTime CompileEnd = DateTime.Now;
            File.WriteAllBytes(AppDomain.CurrentDomain.BaseDirectory + Path.DirectorySeparatorChar + "Script.cmp", compiledBytes);
            libCpScript.Net.ObjectBasic.ObjectBasicScript script2 = new libCpScript.Net.ObjectBasic.ObjectBasicScript();
            
            byte[] compiledBytes2 = File.ReadAllBytes(AppDomain.CurrentDomain.BaseDirectory + Path.DirectorySeparatorChar + "Script.cmp");
            DateTime CompileLoadStart = DateTime.Now;
            script2.LoadScript(compiledBytes);
            DateTime CompileLoadEnd = DateTime.Now;
            script2.RegisterFunction("WriteLine", DoWrite);
            script2.RegisterFunction("ParamCheck", ParamCheck);
            script2.LoadMethod("Main");
            DateTime RunStart = DateTime.Now;
            script2.RunScript();
            DateTime RunEnd = DateTime.Now;


            Console.WriteLine();
            Console.WriteLine();
            Console.WriteLine("Parse Time     : " + (ParseEnd - ParseStart).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Compile Time   : " + (CompileEnd - CompileStart).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Decompile Time : " + (CompileLoadEnd - CompileLoadStart).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Run Time       : " + (RunEnd - RunStart).TotalMilliseconds.ToString() + "ms");*/
            /*
			
			string inLines = System.IO.File.ReadAllText(AppDomain.CurrentDomain.BaseDirectory + System.IO.Path.DirectorySeparatorChar + "TestScript.cps");
			libCpScript.Net.ObjectBasic.ObjectBasicScript sc = new libCpScript.Net.ObjectBasic.ObjectBasicScript();
			sc.RegisterFunction("WriteLine", DoWrite);
			sc.RegisterFunction("ParamCheck", ParamCheck);
			sc.LoadScript(inLines);
            
			Console.WriteLine(sc.AsmScript);
			libCpScript.Net.Asm.State state = new libCpScript.Net.Asm.State(sc.AsmScript);
			System.IO.File.WriteAllBytes(AppDomain.CurrentDomain.BaseDirectory + System.IO.Path.DirectorySeparatorChar + "CompiledScript.cpsc", state.CompileScript());
			state.RegisterFunction("WriteLine", DoWrite);
			state.RegisterFunction("ParamCheck", ParamCheck);
			state.RunFromMethod("Main");
			
			libCpScript.Net.Asm.State state2 = new libCpScript.Net.Asm.State(System.IO.File.ReadAllBytes(AppDomain.CurrentDomain.BaseDirectory + System.IO.Path.DirectorySeparatorChar + "CompiledScript.cpsc"));
			state2.RegisterFunction("WriteLine", DoWrite);
			state2.RegisterFunction("ParamCheck", ParamCheck);
			state2.RunFromMethod("Main");*/
			
			//Console.WriteLine(sc.AsmScript);
			
			/*DateTime startTime = DateTime.Now;
			libCpScript.Net.ScriptState state = new libCpScript.Net.ScriptState(System.IO.File.ReadAllText("TestScript.cps"));
			state.AddFunction("WriteLine", WriteLine);
			state.RunFromFunction("Main");
			DateTime endTime = DateTime.Now;
			
			Console.WriteLine("Execution Time: " + (endTime - startTime).TotalMilliseconds.ToString() + "ms");*/
			
			/*foreach(libCpScript.Net.FunctionDef def in state._Defs)
			{
				Console.WriteLine(def._Name);
			}*/
			
			/*
			DateTime ParseStart = DateTime.Now;
			libCpScript.Net.Token[] Toks = libCpScript.Net.Tokenizer.Tokenize(Script);
			DateTime ExParseStart = DateTime.Now;
			libCpScript.Net.Token[] ExToks = libCpScript.Net.Tokenizer.ParseExtended(Toks);
			DateTime BlockParseStart = DateTime.Now;
			libCpScript.Net.CodeBlock[] blocks = libCpScript.Net.BlockBuilder.ConvertToBlocks(ExToks);
			DateTime EndTime = DateTime.Now;
			
			foreach(libCpScript.Net.CodeBlock b in blocks)
			{
				foreach(libCpScript.Net.Token t in b.Tokens)
				{
					if(t.Type == libCpScript.Net.TokenType.QuotedLiteral)
						Console.Write("\"" + t.Value + "\" ");
					else
						Console.Write(t.Value + " ");
				}
				Console.WriteLine();
			}

			Console.WriteLine("Parse Time    : " + (ExParseStart - ParseStart).TotalMilliseconds.ToString() + "ms");
			Console.WriteLine("Ex Parse Time : " + (BlockParseStart - ExParseStart).TotalMilliseconds.ToString() + "ms");
			Console.WriteLine("Block Time    : " + (EndTime - BlockParseStart).TotalMilliseconds.ToString() + "ms");
			Console.WriteLine("Total Time    : " + (EndTime - ParseStart).TotalMilliseconds.ToString() + "ms");
			
			*/
			/*foreach(libCpScript.Net.Token t in ExToks)
				Console.WriteLine(t.Type.ToString() + ": " + t.Value);*/
		}
	}
}

