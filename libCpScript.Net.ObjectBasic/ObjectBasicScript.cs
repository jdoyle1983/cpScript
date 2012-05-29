using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace libCpScript.Net.ObjectBasic
{
	internal class ClassConversion
	{
        public bool isstatic = false;
		public string input = "";
		public string output = "";
	}
	
	public class ObjectBasicScript
	{
        internal List<string> Libraries = new List<string>();
		internal List<Function> Functions = new List<Function>();
		internal List<ClassDef> Classes = new List<ClassDef>();
		internal StringBuilder AsmResult = new StringBuilder();
		internal CodeBlock[] Blocks = null;
		internal long _NextLabel = 1;
		
		internal long NextLabelId
		{
			get
			{
				long rValue = _NextLabel;
				_NextLabel++;
				return rValue;
			}
		}
		internal bool ScriptLoaded = false;
		internal libCpScript.Net.Asm.State asmState = null;
		
		internal long RegisterCount = 0;
		
		Function currentFunction = null;
		internal ClassDef CurrentClass = null;
		long currentBlock = -1;
		
		internal List<string> currentVars = new List<string>();
        internal List<string> currentClassVars = new List<string>();
		internal List<ClassConversion> currentClassProperties = new List<ClassConversion>();
		internal List<ClassConversion> currentClassMethods = new List<ClassConversion>();
		
		internal string[] FunctionList
		{
			get
			{
				List<string> Names = new List<string>();
				foreach(Function f in Functions)
					Names.Add(f._Name);
				foreach(ClassConversion cMethod in currentClassMethods)
					Names.Add(cMethod.input);
                foreach (ClassDef cd in Classes)
                {
                    foreach (ClassConversion cMethod in cd.GetStaticMethods())
                        Names.Add(cMethod.input);
                }
				return Names.ToArray();
			}
		}
		
		public string AsmScript { get { return AsmResult.ToString(); } }
		
		public ObjectBasicScript ()
		{
		}

        internal void MethodStub(libCpScript.Net.Asm.State State)
        {
        }

        internal string ParsePreProcessor(string Script)
        {
            List<string> _OutLines = new List<string>();
            List<string> _PreProcLines = new List<string>();
            string[] _SrcLines = Script.Split(new char[] { '\n' }, StringSplitOptions.RemoveEmptyEntries);
            foreach (string s in _SrcLines)
            {
                if (s.Trim().StartsWith("#"))
                    _PreProcLines.Add(s);
                else
                    _OutLines.Add(s);
            }

            List<string> includedFiles = new List<string>();
            List<string> toIncludeFiles = new List<string>();

            for (int i = 0; i < _PreProcLines.Count; i++)
                if (_PreProcLines[i].ToLower().StartsWith("#include "))
                    toIncludeFiles.Add(_PreProcLines[i].Substring(9).Trim());

            while (toIncludeFiles.Count > 0)
            {
                string filePath = AppDomain.CurrentDomain.BaseDirectory + System.IO.Path.DirectorySeparatorChar + toIncludeFiles[0];
                bool found = false;
                foreach (string incf in includedFiles)
                    if (incf == filePath)
                        found = true;
                if (!found)
                {
                    string[] theseLines = System.IO.File.ReadAllLines(filePath);
                    foreach (string s in theseLines)
                    {
                        if (s.Trim().StartsWith("#"))
                        {
                            if (s.Trim().ToLower().StartsWith("#include "))
                                toIncludeFiles.Add(s.Substring(9).Trim());
                            else
                                _PreProcLines.Add(s);
                        }
                        else
                            _OutLines.Add(s);
                    }
                    includedFiles.Add(filePath);
                }
                toIncludeFiles.RemoveAt(0);
            }

            for(int i = 0; i < _PreProcLines.Count; i++)
            {
                string preProc = _PreProcLines[i];
                if (preProc.ToLower().StartsWith("#hostfunction "))
                    RegisterFunction(preProc.Substring(14).Trim(), MethodStub, true);
                else if (preProc.ToLower().StartsWith("#lib "))
                {
                    string filePath = preProc.Substring(5).Trim();
                    bool found = false;
                    foreach (string inclib in Libraries)
                        if (inclib == filePath)
                            found = true;
                    if(!found)
                        Libraries.Add(filePath);
                    /*
                    Assembly a = Assembly.LoadFrom(AppDomain.CurrentDomain.BaseDirectory + System.IO.Path.DirectorySeparatorChar + filePath);
                    Type type = a.GetType("libCpScript.Net.ObjectBasic.ObjectBasicLib", true);
                    IObjectBasicLib thisLib = Activator.CreateInstance(type) as IObjectBasicLib;
                    string[] Lines = thisLib.IncludeScript();
                    foreach (string s in Lines)
                    {
                        if (s.Trim().StartsWith("#"))
                            _PreProcLines.Add(s);
                        else
                            _OutLines.Add(s);
                    }
                    thisLib.Init(this);*/
                }
            }

            string rValue = "";
            foreach (string s in _OutLines)
                rValue += s + '\n';
            return rValue;
        }
		
		internal void EvaluateExpression(int bStart)
		{
			Token[] toEval = new Token[currentFunction._Blocks[currentBlock].Tokens.Length - bStart];
			for(int i = bStart; i < currentFunction._Blocks[currentBlock].Tokens.Length; i++)
				toEval[i - bStart] = currentFunction._Blocks[currentBlock].Tokens[i];
			Evaluator ev = new Evaluator();
			Token[] evaled = ev.Parse(toEval, FunctionList);
			foreach(Token t in evaled)
			{
				switch(t.Type)
				{
					case TokenType.ExClassAction:
					{
						foreach(ClassConversion con in currentClassProperties)
							if(t.Value == con.input)
								AsmResult.AppendLine("PUSH " + con.output);
						foreach(ClassConversion con in currentClassMethods)
						{
							if(con.input == t.Value)
							{
                                if (!con.isstatic)
                                {
                                    string varName = "$" + (con.input.Split(new char[] { '.' })[0]);
                                    AsmResult.AppendLine("PUSHB " + varName);
                                }
								AsmResult.AppendLine("JMP " + con.output);
							}
						}
                        foreach (ClassDef cd in Classes)
                        {
                            foreach (ClassConversion con in cd.GetStaticMethods())
                                if (con.input == t.Value)
                                    AsmResult.AppendLine("JMP " + con.output);
                        }
					}break;
					case TokenType.QuotedLiteral:
						AsmResult.AppendLine("PUSH \"" + t.Value + "\"");
					break;
					case TokenType.Literal:
						bool found = false;
						foreach(Function f in Functions)
						{
							if(f._Name == t.Value)
							{
								found = true;
								AsmResult.AppendLine("JMP " + t.Value);
							}
						}
					
						if(!found)
						{
							foreach(string v in currentVars)
							{
								if(v == t.Value)
								{
									found = true;
									AsmResult.AppendLine("PUSH %" + t.Value);
								}
							}
						}
					
						if(!found)
						{
							for(int i = 0; i < currentFunction._Parameters.Length; i++)
							{
								if(currentFunction._Parameters[i].VarName == t.Value)
								{
									found = true;
                                    if (currentFunction._Parameters[i].isClassVar)
                                        AsmResult.AppendLine("PUSHB $" + currentFunction._Parameters[i].VarName);
                                    else
									    AsmResult.AppendLine("PUSH @" + (i + 1).ToString());
								}
							}		
						}
					
						if(!found && CurrentClass != null)
						{
							for(int i = 0; i < CurrentClass.Properties.Count; i++)
							{
								if(CurrentClass.Properties[i] == t.Value)
								{
									found = true;
									AsmResult.AppendLine("PUSH $this:" + (i + 1).ToString());
								}
							}
						}

                        if (!found)
                        {
                            for (int i = 0; i < currentClassVars.Count; i++)
                            {
                                if (currentClassVars[i] == t.Value)
                                {
                                    found = true;
                                    AsmResult.AppendLine("PUSHB $" + currentClassVars[i]);
                                }
                            }
                        }
					
						if(!found)
							AsmResult.AppendLine("PUSH " + t.Value);
					break;
					case TokenType.OpAdd: AsmResult.AppendLine("ADD"); break;
					case TokenType.OpSub: AsmResult.AppendLine("SUB"); break;
					case TokenType.OpMul: AsmResult.AppendLine("MUL"); break;
					case TokenType.OpDiv: AsmResult.AppendLine("DIV"); break;
					case TokenType.OpMod: AsmResult.AppendLine("MOD"); break;
					case TokenType.OpCat: AsmResult.AppendLine("CAT"); break;
                    case TokenType.OpNeg: AsmResult.AppendLine("NEG"); break;
					case TokenType.OpOr: AsmResult.AppendLine("CO"); break;
					case TokenType.OpAnd: AsmResult.AppendLine("CA"); break;
					case TokenType.OpGreaterThan: AsmResult.AppendLine("CG"); break;
					case TokenType.OpGreaterThanOrEqualTo: AsmResult.AppendLine("CGE"); break;
					case TokenType.OpLessThan: AsmResult.AppendLine("CL"); break;
					case TokenType.OpLessThanOrEqualTo: AsmResult.AppendLine("CLE"); break;
					case TokenType.OpEqualTo: AsmResult.AppendLine("CE"); break;
					case TokenType.OpNotEqualTo: AsmResult.AppendLine("CE"); break;
					case TokenType.OpNot: AsmResult.AppendLine("CI"); break;
								
				}
				
			}
		}
		
		internal void ParseVarBlock()
		{
			string varName = currentFunction._Blocks[currentBlock].Tokens[1].Value;
			AsmResult.AppendLine("AB %" + varName);
			if(currentFunction._Blocks[currentBlock].Tokens.Length >= 3)
			{
				EvaluateExpression(3);
				AsmResult.AppendLine("POP %" + varName);
			}
			currentVars.Add(varName);
		}
		
		internal void ParseIfBlock()
		{
			long EndIfId = NextLabelId;
			long CmpReg = RegisterCount + 1;
			RegisterCount++;
			EvaluateExpression(1);
			AsmResult.AppendLine("POP @" + CmpReg.ToString());
			long SkipId = NextLabelId;
			AsmResult.AppendLine("JN @" + CmpReg.ToString() + ", True, _Skip" + SkipId.ToString());
			currentBlock++;
			while(currentFunction._Blocks[currentBlock].Tokens[0].Type != TokenType.ExEndIf)
			{
				if(currentFunction._Blocks[currentBlock].Tokens[0].Type == TokenType.ExElseIf)
				{
					AsmResult.AppendLine("LJMP _EndIf" + EndIfId.ToString());
					AsmResult.AppendLine("LBL _Skip" + SkipId.ToString());
					EvaluateExpression(1);
					AsmResult.AppendLine("POP @" + CmpReg.ToString());
					SkipId = NextLabelId;
					AsmResult.AppendLine("JN @" + CmpReg.ToString() + ", True, _Skip" + SkipId.ToString());
					currentBlock++;
				}
				else if(currentFunction._Blocks[currentBlock].Tokens[0].Type == TokenType.ExElse)
				{
					AsmResult.AppendLine("LJMP _EndIf" + EndIfId.ToString());
					AsmResult.AppendLine("LBL _Skip" + SkipId.ToString());
					currentBlock++;
				}
				else
					ParseBlock();
			}
			AsmResult.AppendLine("LBL _EndIf" + EndIfId.ToString());
		}
		
		internal string getVarOrLit(string input)
		{
            for (int i = 0; i < currentFunction._Parameters.Length; i++)
            {
                if (currentFunction._Parameters[i].VarName == input)
                {
                    if (currentFunction._Parameters[i].isClassVar)
                        return "$" + currentFunction._Parameters[i].ClassName;
                    else
                        return "@" + (i + 1).ToString();
                }
            }
			if(CurrentClass != null)
			{
				for(int i = 0; i < CurrentClass.Properties.Count; i++)
					if(CurrentClass.Properties[i] == input)
						return "$this:" + (i = 1).ToString();
			}
			for(int i = 0; i < currentVars.Count; i++)
				if(currentVars[i] == input)
					return "%" + currentVars[i];
			return input;
		}
		
		internal void ParseForBlock()
		{
			long forId = NextLabelId;
			Token forToken = currentFunction._Blocks[currentBlock].Tokens[0];
			Token varToken = currentFunction._Blocks[currentBlock].Tokens[1];
			Token eqToken = currentFunction._Blocks[currentBlock].Tokens[2];
			Token initToken = currentFunction._Blocks[currentBlock].Tokens[3];
			Token toToken = currentFunction._Blocks[currentBlock].Tokens[4];
			Token endValue = currentFunction._Blocks[currentBlock].Tokens[5];
			string stepVal = "1";
			string initialVal = getVarOrLit(initToken.Value);
			TokenType op = TokenType.OpAdd;
			string opVar = getVarOrLit(varToken.Value);
			string endVar = getVarOrLit(endValue.Value);
			
			if(!opVar.StartsWith("@") && !opVar.StartsWith("%"))
				throw new Exception("Variable Not Found.");
			
			Token stepToken = null;
			Token opToken = null;
			Token valToken = null;
			if(currentFunction._Blocks[currentBlock].Tokens.Length == 9)
			{
				stepToken = currentFunction._Blocks[currentBlock].Tokens[6];
				opToken = currentFunction._Blocks[currentBlock].Tokens[7];
				valToken = currentFunction._Blocks[currentBlock].Tokens[8];
				op = opToken.Type;
				stepVal = getVarOrLit(valToken.Value);
			}
			AsmResult.AppendLine("MOV " + opVar + ", " + initialVal);
			AsmResult.AppendLine("LBL _ForStart" + forId.ToString());
			currentBlock++;
			while(currentFunction._Blocks[currentBlock].Tokens[0].Type != TokenType.ExNext)
				ParseBlock();
			AsmResult.AppendLine("PUSH " + stepVal);
			AsmResult.AppendLine("PUSH " + opVar);
			if(op == TokenType.OpSub)
			{
				AsmResult.AppendLine("SUB");
				AsmResult.AppendLine("POP " + opVar);
				AsmResult.AppendLine("JGE " + opVar + ", " + endVar + ", _ForStart" + forId.ToString());
			}
			else
			{
				AsmResult.AppendLine("ADD");
				AsmResult.AppendLine("POP " + opVar);
				AsmResult.AppendLine("JLE " + opVar + ", " + endVar + ", _ForStart" + forId.ToString());
			}
		}
		
		internal void ParseWhileBlock()
		{
			long whileId = NextLabelId;
			long CmpReg = RegisterCount + 1;
			RegisterCount++;
			
			AsmResult.AppendLine("LBL _WhileTestStart" + whileId.ToString());
			EvaluateExpression(1);
			AsmResult.AppendLine("POP @" + CmpReg.ToString());
			AsmResult.AppendLine("JN @" + CmpReg.ToString() + ", True, _WhileEnd" + whileId.ToString());
			currentBlock++;
			while(currentFunction._Blocks[currentBlock].Tokens[0].Type != TokenType.ExLoop)
				ParseBlock();
			AsmResult.AppendLine("LJMP _WhileTestStart" + whileId.ToString());
			AsmResult.AppendLine("LBL _WhileEnd" + whileId.ToString());
		}
		
		internal void ParseBlock()
		{
			switch(currentFunction._Blocks[currentBlock].Tokens[0].Type)
			{
				case TokenType.ExIf:
				{
					ParseIfBlock();
				}break;
				case TokenType.ExWhile:
				{
					ParseWhileBlock();
				}break;
				case TokenType.ExVar:
				{
					ParseVarBlock();
				}break;
				case TokenType.ExFor:
				{
					ParseForBlock();
				}break;
				default: 
				{
					if(currentFunction._Blocks[currentBlock].Tokens.Length >= 2 && currentFunction._Blocks[currentBlock].Tokens[1].Type == TokenType.Assignment)
					{
						Token t = currentFunction._Blocks[currentBlock].Tokens[0];
						EvaluateExpression(2);
						bool found = false;
						foreach(string v in currentVars)
						{
							if(v == t.Value)
							{
								found = true;
								AsmResult.AppendLine("POP %" + t.Value);
							}
						}
						if(!found)
						{
							for(int i = 0; i < currentFunction._Parameters.Length; i++)
							{
								if(currentFunction._Parameters[i].VarName == t.Value)
								{
									found = true;
                                    if (currentFunction._Parameters[i].isClassVar)
                                        AsmResult.AppendLine("POPB $" + currentFunction._Parameters[i].VarName);
                                    else
									    AsmResult.AppendLine("POP @" + (i + 1).ToString());
								}
							}
						}
						if(!found && CurrentClass != null)
						{
							for(int i = 0; i < CurrentClass.Properties.Count; i++)
							{
								if(CurrentClass.Properties[i] == t.Value)
								{
									found = true;
									AsmResult.AppendLine("POP $this:" + (i + 1).ToString());
								}
							}
						}
					}
					else
					{
						bool isClass = false;
						for(int i = 0; i < Classes.Count; i++)
						{
							if(currentFunction._Blocks[currentBlock].Tokens[0].Value == Classes[i].Name)
							{
								isClass = true;
								string classVar = currentFunction._Blocks[currentBlock].Tokens[1].Value;
								AsmResult.AppendLine("ABS $" + classVar + ", " + Classes[i].Properties.Count.ToString());
                                currentClassVars.Add(classVar);
								RecurseInit(classVar, Classes[i]);
								for(int p = 0; p < Classes[i].Properties.Count; p++)
								{
									ClassConversion con = new ClassConversion();
									con.input = classVar + "." + Classes[i].Properties[p];
									con.output = "$" + classVar + ":" + (p + 1).ToString();
                                    con.isstatic = false;
									currentClassProperties.Add(con);
								}
								ClassConversion[] instanceMethods = Classes[i].GetInstanceMethods();
								foreach(ClassConversion con in instanceMethods)
								{
									ClassConversion cc = new ClassConversion();
									cc.input = classVar + "." + con.input;
									cc.output = con.output;
                                    cc.isstatic = false;
									currentClassMethods.Add(cc);
								}
                                ClassConversion[] staticMethods = Classes[i].GetStaticMethods();
                                foreach(ClassConversion con in staticMethods)
                                {
                                    ClassConversion cc = new ClassConversion();
                                    cc.input = Classes[i].Name + "." + con.input;
                                    cc.output = con.output;
                                    cc.isstatic = true;
                                    currentClassMethods.Add(cc);
                                }
								/*for(int p = 0; p < Classes[i].Methods.Count; p++)
								{
									ClassConversion con = new ClassConversion();
									con.input = classVar + "." + Classes[i].Methods[p]._Name;
									con.output = "_" + Classes[i].Name + "_" + Classes[i].Methods[p]._Name;
									currentClassMethods.Add(con);
								}*/
							}
						}
						if(!isClass)
							EvaluateExpression(0);
					}
				}break;
			}
			currentBlock++;
		}
		
		internal void RecurseInit(string varName, ClassDef cls)
		{
			if(cls.Extends != null)
				RecurseInit(varName, cls.Extends);
			AsmResult.AppendLine("PUSHB $" + varName);
			AsmResult.AppendLine("JMP _" + cls.Name + "_Init");
		}
		
		internal void ParseClasses()
		{
			foreach(ClassDef def in Classes)
			{
				foreach(Function mdef in def.Methods)
				{
					currentVars = new List<string>();
                    currentClassVars = new List<string>();
					RegisterCount = 0;
					AsmResult.AppendLine("LBL _" + def.Name + "_" + mdef._Name);
                    if(!mdef._IsStatic)
					    AsmResult.AppendLine("POPB $this");
					int total = mdef._Parameters.Length;
					while(total > 0)
					{
                        if (mdef._Parameters[total - 1].isClassVar)
                        {
                            AsmResult.AppendLine("POPB $" + mdef._Parameters[total - 1].VarName);
                            foreach (ClassDef cdef in Classes)
                            {
                                if (cdef.Name == mdef._Parameters[total - 1].ClassName)
                                {
                                    ClassConversion[] conv = cdef.GetInstanceMethods();
                                    foreach(ClassConversion cconv in conv)
                                    {
                                        ClassConversion cnv = new ClassConversion();
                                        cnv.input = mdef._Parameters[total - 1].VarName + "." + cconv.input;
                                        cnv.output = cconv.output;
                                        currentClassMethods.Add(cnv);
                                    }
                                }
                            }
                        }
                        else
                            AsmResult.AppendLine("POP @" + total.ToString());
						total--;
						RegisterCount++;
					}
					
					currentBlock = 0;
					CurrentClass = def;
					currentFunction = mdef;
					
					while(currentBlock < mdef._Blocks.Length)
						ParseBlock();
					
					AsmResult.AppendLine("RTN");
				}
			}
		}
		
		internal void ParseFunctions()
		{
			foreach(Function func in Functions)
			{
				if(!func._UserFunction)
				{
					currentVars = new List<string>();
                    currentClassVars = new List<string>();
					RegisterCount = 0;
					AsmResult.AppendLine("LBL " + func._Name);
                    List<FunctionParam> parms = new List<FunctionParam>();
					
					
					Token[] headerToks = func._Blocks[0].Tokens;
					for(int i = 3; i < headerToks.Length; i++)
					{
						Token t = headerToks[i];
                        if (t.Type == TokenType.ExTypeHint)
                        {
                            parms[parms.Count - 1].isClassVar = true;
                            parms[parms.Count - 1].ClassName = t.Value;
                        }
                        else if (t.Type != TokenType.OpenParen && t.Type != TokenType.CloseParen && t.Type != TokenType.Comma)
                        {
                            FunctionParam p = new FunctionParam();
                            p.isClassVar = false;
                            p.VarName = t.Value;
                            parms.Add(p);
                        }
					}
					parms.Reverse();
					func._Parameters = parms.ToArray();
					int total = parms.Count;
					while(total > 0)
					{
                        if (parms[total - 1].isClassVar)
                            AsmResult.AppendLine("POPB $" + parms[total - 1].VarName);
                        else
						    AsmResult.AppendLine("POP @" + total.ToString());
						total--;
						RegisterCount++;
					}
					
					currentBlock = 1;
					currentFunction = func;
					
					while(currentBlock < func._Blocks.Length)
						ParseBlock();
					
					
					AsmResult.AppendLine("RTN");
				}
			}
		}

        public void LoadMethod(string MethodName)
        {
            asmState.LoadMethod(MethodName);
        }
		
		public void RunScript()
		{
			while(Iterate());
		}
		
		public bool Iterate()
		{
			return asmState.Iterate();
		}

        public void LoadScript(byte[] CompiledScript)
        {
            if (!ScriptLoaded)
            {
                asmState = new libCpScript.Net.Asm.State(CompiledScript);
                ScriptLoaded = true;
            }
            else
                throw new Exception("Script Already Loaded.");
        }
		
		public void LoadScript(string Script)
		{
			if(!ScriptLoaded)
			{
                Script = ParsePreProcessor(Script);
				AsmResult = new StringBuilder();
                foreach (string thisLib in Libraries)
                    AsmResult.AppendLine("LIB \"" + thisLib + "\"");
				Token[] Stage1 = Tokenizer.Tokenize(Script);
				Token[] Stage2 = Tokenizer.ParseExtended(Stage1);
				Blocks = BlockBuilder.ConvertToBlocks(Stage2);
				//Blocks = BlockBuilder.ConvertToBlocks(Tokenizer.ParseExtended(Tokenizer.Tokenize(Script)));
				Function[] funcs = Function.ParseFunctionDefs(Blocks);
				ClassDef[] classes = ClassDef.ParseClassDefs(Blocks);
				foreach(Function f in funcs)
					Functions.Add(f);
				foreach(ClassDef d in classes)
					Classes.Add(d);
				ParseClasses();
				ParseFunctions();
				ScriptLoaded = true;
                asmState = new libCpScript.Net.Asm.State(AsmScript);
                foreach (Function f in Functions)
                    asmState.RegisterFunction(f._Name, f._Func, f._IsStub);
			}
			else
				throw new Exception("Script Already Loaded.");
		}
		
		public void RegisterFunction(string Name, libCpScript.Net.Asm.UserFunction Func, bool StubMethod)
		{
            if (asmState == null)
            {
                Function f = new Function();
                f._Func = Func;
                f._Name = Name;
                f._UserFunction = true;
                Functions.Add(f);
            }
            else
                asmState.RegisterFunction(Name, Func, StubMethod);
		}

        public byte[] Compile()
        {
            return asmState.CompileScript();
        }
	}
}

