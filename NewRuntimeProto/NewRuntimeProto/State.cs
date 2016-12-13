using System;
using System.Collections.Generic;
using System.Text;

namespace NewRuntimeProto
{
    public delegate void ExternalFunction(State state);

    public enum AsmInstruction
    {
        NOOP = 0,
        LBL,                                    // LBL [LABEL]              -   Creates a label for JMP instructions
        PUSH,                                   // PUSH [VALUE]             -   Push value onto stack
        POP,                                    // POP [DEST]               -   Pop value from stack
        PUSHR,                                  // PUSHR                    -   Push all registers onto stack
        POPR,                                   // POPR                     -   Pop all registers from stack
        PUSHA,                                  // PUSHA                    -   Push the code execution offset onto the stack
        MOV,                                    // MOV [SRC], [DEST]        -   Move value from one location to another
        MOVA,                                   // MOVA [DEST]              -   Move the code execution offset into variable
        ALLOC,                                  // ALLOC [SIZE]             -   Allocate adjacent blocks of memory, pushes location on stack
        FREE,                                   // FREE [MEMLOC], [SIZE]    -   Free adjacent blocks of memory
        ADD,                                    // ADD                      -   Pop last 2 values from stack, add and push result onto stack (PUSH 2, PUSH 1 -> 2 + 1 = 3)
        SUB,                                    // SUB                      -   Pop last 2 values from stack, subtract and push result onto stack (PUSH 2, PUSH 1 -> 2 - 1 = 1)
        MUL,                                    // MUL                      -   Pop last 2 values from stack, multiply and push result onto stack (PUSH 2, PUSH 1 -> 2 * 1 = 2)
        DIV,                                    // DIV                      -   Pop last 2 values from stack, divide and push result onto stack (PUSH 2, PUSH 1 -> 2 / 1 = 2)
        MOD,                                    // MOD                      -   Pop last 2 values from stack, modulo and push result onto stack (PUSH 2, PUSH 1 -> 2 % 1 = 0)
        CON,                                    // CON                      -   Pop last 2 values from stack, concat and push result onto stack (PUSH 2, PUSH 1 -> 2 .. 1 = "21")
        NEG,                                    // NEG                      -   Pop last value from stack, multiply by -1 and push result onto stack (PUSH 1 -> 1 * -1 = -1)
        INC,                                    // INC [VAR], [INC AMOUNT]  -   Increment VAR by INC AMOUNT
        DEC,                                    // DEC [VAR], [DEC AMOUNT]  -   Decrement VAR by INC AMOUNT
        CMPA,                                   // CMPA                     -   Pop Last 2 values, push true if they AND (PUSH 0, PUSH 1 -> 1 && 0, STACK PUSH 0)
        CMPO,                                   // CMPO                     -   Pop Last 2 values, push true if they OR (PUSH 0, PUSH 1 -> 1 || 0, STACK PUSH 1)
        CMPE,                                   // CMPE                     -   Pop Last 2 values, push true if they are equal (PUSH 2, PUSH 1 -> 2 == 1, STACK PUSH 0)
        CMPN,                                   // CMPN                     -   Pop Last 2 values, push true if they are not equal (PUSH 2, PUSH 1 -> 2 != 1, STACK PUSH 1)
        CMPG,                                   // CMPG                     -   Pop Last 2 values, push true if value 2 is greater than value 1 (PUSH 2, PUSH 1 -> 2 > 1, STACK PUSH 1)
        CMPGE,                                  // CMPGE                    -   Pop Last 2 values, push true if value 2 is greater than or equal to value 1 (PUSH 2, PUSH 1 -> 2 >= 1, STACK PUSH 1)
        CMPL,                                   // CMPL                     -   Pop Last 2 values, push true if value 2 is less than value 1 (PUSH 2, PUSH 1 -> 2 < 1, STACK PUSH 0)
        CMPLE,                                  // CMPLE                    -   Pop Last 2 values, push true if value 2 is less than or equal to value 1 (PUSH 2, PUSH 1 -> 2 <= 1, STACK PUSH 0)
        JMPL,                                   // JMPL LABEL               -   Move execution pointer to LABEL
        JMPO,                                   // JMPO [OFFSET]            -   Move execution pointer to position [OFFSET] realtive to current location
        JMPF,                                   // JMPF [OFFSET]            -   Move execution pointer to position fixed [OFFSET]
        JMPLT,                                  // JMPLT LABEL              -   Jump to LABEL if poped value is true
        JMPOT,                                  // JMPOT [OFFSET]           -   Jump to [OFFSET] relative to current location if poped value is true
        JMPFT,                                  // JMPFT [OFFSET]           -   Jump to [OFFSET] if poped value is true
    }

    public class State
    {
        internal static char[] SplitChars = { ' ', '%', '@', '&', '[', ']', ',', '+', '-', ':' };

        internal List<Tuple<AsmInstruction, List<string>>> CodePart;
        internal List<Tuple<string, int>> OriginalLocation;
        internal List<string> Externals;
        internal List<List<string>> DataVarDeclarations;

        internal Dictionary<string, int> LabelOffsets;

        internal string[] Register = { "", "", "", "", "", "", "", "", "", "", "", "" };
        internal Dictionary<string, int> DataVars;
        internal MemoryBlock[] Memory;
        internal List<string> Stack;       
        internal int CurrentScriptOffset = -1;

        internal Dictionary<string, ExternalFunction> ExternalFunctions;

        public event ExecutionExceptionHandler OnExecutionException;

        public State()
        {
            CodePart = new List<Tuple<AsmInstruction, List<string>>>();
            OriginalLocation = new List<Tuple<string, int>>();
            Externals = new List<string>();
            DataVarDeclarations = new List<List<string>>();

            LabelOffsets = new Dictionary<string, int>();

            DataVars = new Dictionary<string, int>();
            Memory = new MemoryBlock[0];
            Stack = new List<string>();
            CurrentScriptOffset = -1;

            ExternalFunctions = new Dictionary<string, ExternalFunction>();          
            
            OnExecutionException = null;
        }

        public void RegisterFunction(string functionName, ExternalFunction functionHandler)
        {
            if (!ExternalFunctions.ContainsKey(functionName) && functionHandler != null)
                ExternalFunctions.Add(functionName, functionHandler);
        }

        public void Load(string AsmFile)
        {
            //string asmText = System.IO.File.ReadAllText(AsmFile);
            //Split script by line breaks
            string[] SrcLines = System.IO.File.ReadAllLines(AsmFile);// asmText.Split(new char[] { '\n', '\r' });

            //Find genuine code lines, removing comments (lines beginning with ;) and the like.
            List<string> CleanLines = new List<string>();
            List<Tuple<string, int>> BaseOriginalLocation = new List<Tuple<string, int>>();
            int lCounter = 1;
            foreach (string s in SrcLines)
            {
                if (s.Trim() != "" && !s.Trim().StartsWith(";"))
                {
                    CleanLines.Add(s.Trim());
                    BaseOriginalLocation.Add(new Tuple<string, int>(AsmFile, lCounter));
                }
                lCounter++;
            }

            //Store entire clean script
            string[] Script = CleanLines.ToArray();

            List<List<string>> TotalCodeParts = new List<List<string>>();

            //Pre-Split all lines
            foreach (string s in Script)
            {
                List<string> tItems = new List<string>(s.SplitAndKeep(State.SplitChars));
                List<string> fItems = new List<string>();
                char QuoteChar = ' ';
                string QuoteString = "";
                foreach (string p in tItems)
                {
                    if(QuoteChar == ' ')
                    {
                        if(p.StartsWith("\""))
                        {
                            QuoteChar = '\"';
                            QuoteString += p;
                        }
                        else if(p.StartsWith("'"))
                        {
                            QuoteChar = '\'';
                            QuoteString += p;
                        }
                        else
                        {
                            if (p.Trim() != "")
                                fItems.Add(p.Trim());
                        }
                    }
                    else
                    {
                        QuoteString += p;
                        if(p.EndsWith(QuoteChar.ToString()))
                        {
                            QuoteChar = ' ';
                            fItems.Add(QuoteString.Substring(1, QuoteString.Length - 2));
                        }
                    }
                }
                TotalCodeParts.Add(fItems);
            }

            int targetArea = -1;

            for(int i = 0; i < TotalCodeParts.Count; i++)
            {
                if(TotalCodeParts[i].Count == 3 && TotalCodeParts[i][0] == "[" && TotalCodeParts[i][2] == "]")
                {
                    if (TotalCodeParts[i][1].ToLower() == "data")
                        targetArea = 1;
                    else if (TotalCodeParts[i][1].ToLower() == "external")
                        targetArea = 2;
                    else if (TotalCodeParts[i][1].ToLower() == "code")
                        targetArea = 3;
                }
                else
                {
                    if (targetArea == 1)
                        DataVarDeclarations.Add(TotalCodeParts[i]);
                    else if (targetArea == 2)
                        Externals.Add(TotalCodeParts[i][0]);
                    else if (targetArea == 3)
                    {
                        AsmInstruction Cmd = AsmInstruction.NOOP;

                        switch(TotalCodeParts[i][0].ToLower())
                        {
                            case "lbl": Cmd = AsmInstruction.LBL; break;
                            case "push": Cmd = AsmInstruction.PUSH; break;
                            case "pop": Cmd = AsmInstruction.POP; break;
                            case "pushr": Cmd = AsmInstruction.PUSHR; break;
                            case "popr": Cmd = AsmInstruction.POPR; break;
                            case "pusha": Cmd = AsmInstruction.PUSHA; break;
                            case "mov": Cmd = AsmInstruction.MOV; break;
                            case "mova": Cmd = AsmInstruction.MOVA; break;
                            case "alloc": Cmd = AsmInstruction.ALLOC; break;
                            case "free": Cmd = AsmInstruction.FREE; break;
                            case "add": Cmd = AsmInstruction.ADD; break;
                            case "sub": Cmd = AsmInstruction.SUB; break;
                            case "mul": Cmd = AsmInstruction.MUL; break;
                            case "div": Cmd = AsmInstruction.DIV; break;
                            case "mod": Cmd = AsmInstruction.MOD; break;
                            case "con": Cmd = AsmInstruction.CON; break;
                            case "neg": Cmd = AsmInstruction.NEG; break;
                            case "inc": Cmd = AsmInstruction.INC; break;
                            case "dec": Cmd = AsmInstruction.DEC; break;
                            case "cmpa": Cmd = AsmInstruction.CMPA; break;
                            case "cmpo": Cmd = AsmInstruction.CMPO; break;
                            case "cmpe": Cmd = AsmInstruction.CMPE; break;
                            case "cmpn": Cmd = AsmInstruction.CMPN; break;
                            case "cmpg": Cmd = AsmInstruction.CMPG; break;
                            case "cmpge": Cmd = AsmInstruction.CMPGE; break;
                            case "cmpl": Cmd = AsmInstruction.CMPL; break;
                            case "cmple": Cmd = AsmInstruction.CMPLE; break;
                            case "jmpl": Cmd = AsmInstruction.JMPL; break;
                            case "jmpo": Cmd = AsmInstruction.JMPO; break;
                            case "jmpf": Cmd = AsmInstruction.JMPF; break;
                            case "jmplt": Cmd = AsmInstruction.JMPLT; break;
                            case "jmpot": Cmd = AsmInstruction.JMPOT; break;
                            case "jmpft": Cmd = AsmInstruction.JMPFT; break;
                        }
                        CodePart.Add(new Tuple<AsmInstruction, List<string>>(Cmd, TotalCodeParts[i].GetRange(1, TotalCodeParts[i].Count - 1)));
                        OriginalLocation.Add(BaseOriginalLocation[i]);
                    }
                }
            }

            CurrentScriptOffset = 0;

            //Parse Data Segment
            AllocateInitialData();

            DiscoverLabels();
        }

        public State MergeIntoThisScript(State otherState)
        {
            List<Tuple<AsmInstruction, List<string>>> newCodePart = new List<Tuple<AsmInstruction, List<string>>>(CodePart);
            newCodePart.AddRange(otherState.CodePart);

            List<Tuple<string, int>> newOriginalLocation = new List<Tuple<string, int>>(OriginalLocation);
            newOriginalLocation.AddRange(otherState.OriginalLocation);

            List<List<string>> newDataVarDeclarations = new List<List<string>>(DataVarDeclarations);
            foreach(List<string> otherDataVarDeclaration in otherState.DataVarDeclarations)
            {
                bool foundMatch = false;
                foreach(List<string> existingDataVarDeclaration in newDataVarDeclarations)
                {
                    if(existingDataVarDeclaration[0] == otherDataVarDeclaration[0])
                    {
                        foundMatch = true;
                        break;
                    }
                }

                if (foundMatch)
                    ThrowExecutionException(new Tuple<string, int>("PARSER", 0), "Data Variables Conflict Found During Merge (" + otherDataVarDeclaration[0] + " Already Defined).");
                else
                    newDataVarDeclarations.Add(otherDataVarDeclaration);
            }
            
            List<String> newExternals = new List<string>(Externals);
            foreach(string otherExternal in otherState.Externals)
                if (!newExternals.Contains(otherExternal))
                    newExternals.Add(otherExternal);

            CodePart = newCodePart;
            OriginalLocation = newOriginalLocation;
            Externals = newExternals;
            DataVarDeclarations = newDataVarDeclarations;

            Reset();
            DiscoverLabels();

            return this;
        }

        internal void DiscoverLabels()
        {
            LabelOffsets = new Dictionary<string, int>();
            for (int i = 0; i < CodePart.Count; i++)
            {
                if(CodePart[i].Item1 == AsmInstruction.LBL)
                {
                    string labelName = CodePart[i].Item2[0];
                    if (LabelOffsets.ContainsKey(labelName))
                        ThrowExecutionException(OriginalLocation[i], "Label '" + labelName + "' Already Defined.");
                    if (Externals.Contains(labelName))
                        ThrowExecutionException(OriginalLocation[i], "Label '" + labelName + "' Conflicts with External.");
                    if (DataVars.ContainsKey(labelName))
                        ThrowExecutionException(OriginalLocation[i], "Label '" + labelName + "' Conflicts with Data Variable.");
                    LabelOffsets.Add(CodePart[i].Item2[0], i);
                }
            }
        }

        internal void AllocateInitialData()
        {
            DataVars.Clear();
            for(int i = 0; i < DataVarDeclarations.Count; i++)
            {
                string varName = DataVarDeclarations[i][0];
                if (DataVars.ContainsKey(varName))
                    ThrowExecutionException(OriginalLocation[i], "Data Variable '" + varName + "' is Already Defined.");
                string initialValue = "";
                if (DataVarDeclarations[i].Count == 3)
                {
                    initialValue = DataVarDeclarations[i][2];
                    int memoryLocation = AllocateMemory(1, true);
                    WriteMemoryOffset(memoryLocation, initialValue);
                    DataVars.Add(varName, memoryLocation);
                }
                else if (DataVarDeclarations[i].Count == 5 && DataVarDeclarations[i][2] == "[" && DataVarDeclarations[i][4] == "]")
                {
                    int cValue = 0;
                    if (!Int32.TryParse(DataVarDeclarations[i][3], out cValue))
                        ThrowExecutionException(OriginalLocation[i], "Integer Value Expected.");
                    int memoryLocation = AllocateMemory(cValue, true);
                    DataVars.Add(varName, memoryLocation);
                }
            }
        }

        public void Reset()
        {
            Register[0] = Register[1] = Register[2] = Register[3] = Register[4] = Register[5] = Register[6] = Register[7] = Register[8] = Register[9] = Register[10] = Register[11] = "";
            Memory = new MemoryBlock[0];
            AllocateInitialData();
            CurrentScriptOffset = 0;
            Stack.Clear();
        }

        public string ResolveGetValue(List<string> valueParts)
        {
            if(valueParts[0] == "@")
                return ReadRegister(Convert.ToInt16(valueParts[1]));
            else if(valueParts[0] == "&")
            {
                if (!DataVars.ContainsKey(valueParts[1]))
                {
                    ThrowExecutionException("Reference to Data Variable That is Not Defined.");
                    return "";
                }
                else
                {
                    int memoryOffset = DataVars[valueParts[1]];

                    if (valueParts.Count > 2 && (valueParts[2] == "+" || valueParts[2] == "-"))
                    {
                        string modValue = ResolveGetValue(valueParts.GetRange(3, valueParts.Count - 3));
                        int iModVal = 0;
                        if (!Int32.TryParse(modValue, out iModVal))
                            ThrowExecutionException("Memory Address Modifier Invalid Type.");
                        if (valueParts[2] == "+")
                            memoryOffset += iModVal;
                        else
                            memoryOffset -= iModVal;
                    }

                    return memoryOffset.ToString();
                }
            }
            else if(valueParts[0] == "%")
            {
                int memoryOffset = -1;
                if(!Int32.TryParse(valueParts[1], out memoryOffset))
                {
                    if (!DataVars.ContainsKey(valueParts[1]))
                        ThrowExecutionException("Failed To Find Target Memory Address Variable.");
                    string varValue = ReadMemoryOffset(DataVars[valueParts[1]]);
                    if (!Int32.TryParse(varValue, out memoryOffset))
                        ThrowExecutionException("Value Contained In Data Varible is Invalid For Memory Address.");
                }

                if(valueParts.Count > 2 && (valueParts[2] == "+" || valueParts[2] == "-"))
                {
                    string modValue = ResolveGetValue(valueParts.GetRange(3, valueParts.Count - 3));
                    int iModVal = 0;
                    if (!Int32.TryParse(modValue, out iModVal))
                        ThrowExecutionException("Memory Address Modifier Invalid Type.");
                    if (valueParts[2] == "+")
                        memoryOffset += iModVal;
                    else
                        memoryOffset -= iModVal;
                }

                return memoryOffset.ToString();
            }
            else
            {
                if (!DataVars.ContainsKey(valueParts[0])) //Return literal value
                    return valueParts[0];
                else
                    return ReadMemoryOffset(DataVars[valueParts[0]]);
            }
        }

        public void ResolveSetValue(List<string> valueParts, string setValue)
        {
            if (valueParts[0] == "@")
                SetRegister(Convert.ToInt16(valueParts[1]), setValue);
            else if (valueParts[0] == "%")
            {
                int memoryOffset = -1;
                if (!Int32.TryParse(valueParts[1], out memoryOffset))
                {
                    if (!DataVars.ContainsKey(valueParts[1]))
                        ThrowExecutionException("Failed To Find Target Memory Address Variable.");
                    string varValue = ReadMemoryOffset(DataVars[valueParts[1]]);
                    if (!Int32.TryParse(varValue, out memoryOffset))
                        ThrowExecutionException("Value Contained In Data Varible is Invalid For Memory Address.");
                }

                if (valueParts.Count > 2 && (valueParts[2] == "+" || valueParts[2] == "-"))
                {
                    string modValue = ResolveGetValue(valueParts.GetRange(3, valueParts.Count - 3));
                    int iModVal = 0;
                    if (!Int32.TryParse(modValue, out iModVal))
                        ThrowExecutionException("Memory Address Modifier Invalid Type.");
                    if (valueParts[2] == "+")
                        memoryOffset += iModVal;
                    else
                        memoryOffset -= iModVal;
                }

                WriteMemoryOffset(memoryOffset, setValue);
            }
            else
            {
                if (!DataVars.ContainsKey(valueParts[0]))
                    ThrowExecutionException("Invalid Parameters Specified.");



                WriteMemoryOffset(DataVars[valueParts[0]], setValue);
            }
        }

        public bool Iterate()
        {
            CurrentScriptOffset++;

            if(CurrentScriptOffset >= CodePart.Count)
            {
                CurrentScriptOffset = CodePart.Count - 1;
                return false;
            }

            while (CodePart[CurrentScriptOffset].Item1 == AsmInstruction.LBL && CurrentScriptOffset < CodePart.Count)
                CurrentScriptOffset++;

            if (CurrentScriptOffset >= CodePart.Count)
            {
                CurrentScriptOffset = CodePart.Count - 1;
                return false;
            }

            Tuple<AsmInstruction, List<string>> Step = CodePart[CurrentScriptOffset];

            switch(Step.Item1)
            {
                case AsmInstruction.PUSH:                                            // PUSH [VALUE]             -    Push value onto stack
                    {
                        StackPush(ResolveGetValue(Step.Item2));
                    } break;
                case AsmInstruction.POP:                                             // POP [DEST]               -   Pop value from stack
                    {
                        ResolveSetValue(Step.Item2, StackPop());
                    } break;  
                case AsmInstruction.PUSHR:                                           // PUSHR                    -   Push all registers onto stack
                    {
                        for (int i = 0; i < 12; i++)
                            StackPush(Register[i]);
                    } break;
                case AsmInstruction.POPR:                                            // POPR                     -   Pop all registers from stack
                    {
                        for (int i = 11; i >= 0; i--)
                            Register[i] = StackPop();
                    } break;
                case AsmInstruction.PUSHA:                                           // PUSHA                    -   Push the code execution offset onto the stack
                    {
                        StackPush(CurrentScriptOffset.ToString());
                    }
                    break;
                case AsmInstruction.MOV:                                             // MOV [SRC], [DEST]        -   Move value from one location to another
                    {
                        int commaBreak = Step.Item2.IndexOf(",");
                        int destStart = commaBreak + 1;
                        int destLen = Step.Item2.Count - destStart;
                        ResolveSetValue(Step.Item2.GetRange(destStart, destLen), ResolveGetValue(Step.Item2.GetRange(0, commaBreak)));
                    } break;
                case AsmInstruction.MOVA:                                            // MOVA [DEST]              -   Move the code execution offset into variable
                    {
                        ResolveSetValue(Step.Item2, CurrentScriptOffset.ToString());
                    } break;
                case AsmInstruction.ALLOC:                                           // ALLOC [SIZE]             -   Allocate blocks of memory, pushes location on stack
                    {
                        int allocSize = 0;
                        string sizeVal = ResolveGetValue(Step.Item2);
                        if (!Int32.TryParse(sizeVal, out allocSize))
                            throw new Exception("Resolved Alloc Value is Not a Number.");
                        StackPush(AllocateMemory(allocSize).ToString());
                    } break;
                case AsmInstruction.FREE:                                            // FREE [MEMLOC], [SIZE]    -   Free blocks of memory
                    {
                        int commaBreak = Step.Item2.IndexOf(",");
                        int sizeStart = commaBreak + 1;
                        int sizeLen = Step.Item2.Count - sizeStart;

                        string memLocVal = ResolveGetValue(Step.Item2.GetRange(0, commaBreak));
                        string sizeVal = ResolveGetValue(Step.Item2.GetRange(sizeStart, sizeLen));

                        int memoryLocation = -1;
                        int memorySize = -1;

                        if (!Int32.TryParse(memLocVal, out memoryLocation))
                            ThrowExecutionException("Resolved Free Memory Location is Not a Number.");
                        if (!Int32.TryParse(sizeVal, out memorySize))
                            ThrowExecutionException("Resolved Free Memory Size is Not a Number.");
                        FreeMemory(memoryLocation, memorySize);
                    } break;
                case AsmInstruction.ADD:                                             // ADD                      -   Pop last 2 values from stack, add and push result onto stack (PUSH 2, PUSH 1 -> 2 + 1 = 3)
                case AsmInstruction.SUB:                                             // SUB                      -   Pop last 2 values from stack, subtract and push result onto stack (PUSH 2, PUSH 1 -> 2 - 1 = 1)
                case AsmInstruction.MUL:                                             // MUL                      -   Pop last 2 values from stack, multiply and push result onto stack (PUSH 2, PUSH 1 -> 2 * 1 = 2)
                case AsmInstruction.DIV:                                             // DIV                      -   Pop last 2 values from stack, divide and push result onto stack (PUSH 2, PUSH 1 -> 2 / 1 = 2)
                case AsmInstruction.MOD:                                             // MOD                      -   Pop last 2 values from stack, modulo and push result onto stack (PUSH 2, PUSH 1 -> 2 % 1 = 0)
                case AsmInstruction.CON:                                             // CON                      -   Pop last 2 values from stack, concat and push result onto stack (PUSH 2, PUSH 1 -> 2 .. 1 = "21")
                    {
                        string strValue1 = StackPop();
                        string strValue2 = StackPop();

                        if(Step.Item1 == AsmInstruction.CON)
                        {
                            StackPush(strValue2 + strValue1);
                        }
                        else
                        {
                            double dValue1 = 0;
                            double dValue2 = 0;

                            if (!double.TryParse(strValue1, out dValue1) || !double.TryParse(strValue2, out dValue2))
                                ThrowExecutionException("Calculation Attempted on Non-Numeric Value.");

                            switch(Step.Item1)
                            {
                                case AsmInstruction.ADD: StackPush((dValue2 + dValue1).ToString()); break;
                                case AsmInstruction.SUB: StackPush((dValue2 + dValue1).ToString()); break;
                                case AsmInstruction.MUL: StackPush((dValue2 * dValue1).ToString()); break;
                                case AsmInstruction.DIV:
                                case AsmInstruction.MOD:
                                    {
                                        if (dValue1 == 0)
                                            throw new Exception("Calculation Attempted Divided by Zero.");
                                        if (Step.Item1 == AsmInstruction.DIV) StackPush((dValue2 / dValue1).ToString());
                                        else StackPush((dValue2 % dValue1).ToString());
                                    } break;
                            }
                        }
                    } break;
                case AsmInstruction.NEG:                                             // NEG                      -   Pop last value from stack, multiply by -1 and push result onto stack (PUSH 1 -> 1 * -1 = -1)
                    {
                        string strValue = StackPop();
                        double dValue = 0;
                        if (!double.TryParse(strValue, out dValue))
                            ThrowExecutionException("Attempted To Inverse Non-Numeric Value.");
                        StackPush((dValue * -1.0).ToString());
                    } break;
                case AsmInstruction.INC:                                             // INC [VAR], [INC AMOUNT]  -   Increment VAR by INC AMOUNT
                case AsmInstruction.DEC:                                             // DEC [VAR], [DEC AMOUNT]  -   Decrement VAR by INC AMOUNT
                    {
                        int commaBreak = Step.Item2.IndexOf(",");
                        int amtStart = commaBreak + 1;
                        int amtLen = Step.Item2.Count - amtStart;

                        string currentValue = ResolveGetValue(Step.Item2.GetRange(0, commaBreak));
                        string amtValue = ResolveGetValue(Step.Item2.GetRange(amtStart, amtLen));

                        double numericValue = 0;
                        double numericAmt = 0;

                        if (!double.TryParse(currentValue, out numericValue) || !double.TryParse(amtValue, out numericAmt))
                            ThrowExecutionException("Invalid Data Types Specified for INC/DEC Values.");

                        if (Step.Item1 == AsmInstruction.INC)
                            numericValue += numericAmt;
                        else
                            numericValue -= numericAmt;

                        ResolveSetValue(Step.Item2.GetRange(0, commaBreak), numericValue.ToString());
                    } break;
                case AsmInstruction.CMPA:                                            // CMPA                     -   Pop Last 2 values, push true if they AND (PUSH 0, PUSH 1 -> 1 && 0, STACK PUSH 0)
                case AsmInstruction.CMPO:                                            // CMPO                     -   Pop Last 2 values, push true if they OR (PUSH 0, PUSH 1 -> 1 || 0, STACK PUSH 1)
                    {
                        string strValue1 = StackPop();
                        string strValue2 = StackPop();

                        short sValue1 = -1;
                        short sValue2 = -1;

                        if (!short.TryParse(strValue1, out sValue1) || short.TryParse(strValue2, out sValue2))
                            ThrowExecutionException("Attempted Bitwise Operation on Invlaid Value (1).");

                        if ((sValue1 != 0 && sValue1 != 1) || (sValue2 != 0 && sValue2 != 1))
                            ThrowExecutionException("Attempted Bitwise Operation on Invalid Value (2).");

                        bool bValue1 = sValue1 == 1;
                        bool bValue2 = sValue2 == 1;

                        if (Step.Item1 == AsmInstruction.CMPA) StackPush((bValue2 && bValue1) ? "1" : "0");
                        else StackPush((bValue2 || bValue1) ? "1" : "0");
                    } break;
                case AsmInstruction.CMPE:                                            // CMPE                     -   Pop Last 2 values, push true if they are equal (PUSH 2, PUSH 1 -> 2 == 1, STACK PUSH 0)
                case AsmInstruction.CMPN:                                            // CMPN                     -   Pop Last 2 values, push true if they are not equal (PUSH 2, PUSH 1 -> 2 != 1, STACK PUSH 1)
                case AsmInstruction.CMPG:                                            // CMPG                     -   Pop Last 2 values, push true if value 2 is greater than value 1 (PUSH 2, PUSH 1 -> 2 > 1, STACK PUSH 1)
                case AsmInstruction.CMPGE:                                           // CMPGE                    -   Pop Last 2 values, push true if value 2 is greater than or equal to value 1 (PUSH 2, PUSH 1 -> 2 >= 1, STACK PUSH 1)
                case AsmInstruction.CMPL:                                            // CMPEL                    -   Pop Last 2 values, push true if value 2 is less than value 1 (PUSH 2, PUSH 1 -> 2 < 1, STACK PUSH 0)
                case AsmInstruction.CMPLE:                                           // CMPLE                     -   Pop Last 2 values, push true if value 2 is less than or equal to value 1 (PUSH 2, PUSH 1 -> 2 <= 1, STACK PUSH 0)
                    {
                        string strValue1 = StackPop();
                        string strValue2 = StackPop();

                        if (Step.Item1 == AsmInstruction.CMPE)
                            StackPush((strValue2 == strValue1) ? "1" : "0");
                        else if (Step.Item1 == AsmInstruction.CMPN)
                            StackPush((strValue2 != strValue1) ? "1" : "0");
                        else
                        {
                            double dValue1 = 0;
                            double dValue2 = 0;

                            if (!double.TryParse(strValue1, out dValue1) || !double.TryParse(strValue2, out dValue2))
                                ThrowExecutionException("Numeric Comparison Attempted on Non-Numeric Value.");

                            switch(Step.Item1)
                            {
                                case AsmInstruction.CMPG: StackPush((dValue2 > dValue1) ? "1" : "0"); break;
                                case AsmInstruction.CMPGE: StackPush((dValue2 >= dValue1) ? "1" : "0"); break;
                                case AsmInstruction.CMPL: StackPush((dValue2 < dValue1) ? "1" : "0"); break;
                                case AsmInstruction.CMPLE: StackPush((dValue2 <= dValue1) ? "1" : "0"); break;
                            }
                        }
                    }
                    break;
                case AsmInstruction.JMPL:                                            // JMPL LABEL               -   Move execution pointer to LABEL
                    {
                        DoLabelJump(Step);                
                    } break;
                case AsmInstruction.JMPO:                                            // JMPO [OFFSET]            -   Move execution pointer to position [OFFSET] from current location
                    {
                        DoOffsetJump(Step);
                    } break;
                case AsmInstruction.JMPF:
                    {
                        DoFixedJump(Step);
                    } break;
                case AsmInstruction.JMPLT:                                           // JMPLT LABEL              -   Jump to LABEL if poped value is true
                    {
                        if (DoConditionCheck(Step))
                            DoLabelJump(Step);
                    } break;
                case AsmInstruction.JMPOT:                                           // JMPOT [OFFSET]           -   Jump to [OFFSET] if poped value is true
                    {
                        if (DoConditionCheck(Step))
                            DoOffsetJump(Step);
                    } break;
                case AsmInstruction.JMPFT:
                    {
                        if (DoConditionCheck(Step))
                            DoFixedJump(Step);
                    } break;
            }

            return true;
        }

        internal bool DoConditionCheck(Tuple<AsmInstruction, List<string>> Step)
        {
            string strValue = StackPop();
            if (strValue != "1" && strValue != "0")
                ThrowExecutionException("Atemped Bitwise Comparison on Invalid Value.");
            if (strValue == "1")
            {
                return true;
            }

            return false;
        }

        internal void DoLabelJump(Tuple<AsmInstruction, List<string>> Step)
        {
            if (!LabelOffsets.ContainsKey(Step.Item2[0]) && !Externals.Contains(Step.Item2[0]))
                ThrowExecutionException("Jump Label Requested Does Not Exist.");

            if (LabelOffsets.ContainsKey(Step.Item2[0]))
                CurrentScriptOffset = LabelOffsets[Step.Item2[0]];
            else
            {
                if (Externals.Contains(Step.Item2[0]) && ExternalFunctions.ContainsKey(Step.Item2[0]))
                    ExternalFunctions[Step.Item2[0]](this);
                else
                {
                    if (!ExternalFunctions.ContainsKey(Step.Item2[0]))
                    {
                        if (Externals.Contains(Step.Item2[0]))
                            ThrowExecutionException("External Method Event Handler Not Registered for External '" + Step.Item2[0] + "'.");
                        else
                            ThrowExecutionException("Invalid Label (No Label or Defined External Found For '" + Step.Item2[0] + "'.");
                    }
                    else
                    {
                        if(!Externals.Contains(Step.Item2[0]))
                            ThrowExecutionException("External Method Event Handler Registered For '" + Step.Item2[0] + "', But Not Defined in External Segment.");
                    }
                }
            }
        }

        internal void DoOffsetJump(Tuple<AsmInstruction, List<string>> Step)
        {
            string strValue = ResolveGetValue(Step.Item2);
            int offsetValue = 0;
            if (!Int32.TryParse(strValue, out offsetValue))
                ThrowExecutionException("Attmpted Offset Jump With Non-Numeric Offset.");
            int newOffset = CurrentScriptOffset + offsetValue;
            if (newOffset < 0 || newOffset >= CodePart.Count)
                ThrowExecutionException("Attmpted Offset Jump Beyond Bounds.");
            CurrentScriptOffset = newOffset;
        }

        internal void DoFixedJump(Tuple<AsmInstruction, List<string>> Step)
        {
            string strValue = ResolveGetValue(Step.Item2);
            int offsetValue = 0;
            if (!Int32.TryParse(strValue, out offsetValue))
                ThrowExecutionException("Attmpted Fixed Jump With Non-Numeric Offset.");
            if(offsetValue < 0 || offsetValue >= CodePart.Count)
                ThrowExecutionException("Attmpted Fixed Jump Beyond Bounds.");
            CurrentScriptOffset = offsetValue;
        }

        #region Memory Management
        public int AllocateMemory(int size, bool isFixed = false)
        {
            int blockStart = -1;
            int blockSize = -1;

            for(int i = 0; i < Memory.Length; i++)
            {
                if(!Memory[i].Used)
                {
                    if (blockStart == -1)
                    {
                        blockStart = i;
                        blockSize = 1;
                    }
                    else
                        blockSize++;

                    if (blockSize == size)
                        break;
                }
                else
                {
                    blockStart = -1;
                    blockSize = -1;
                }
            }

            if(blockStart != -1 && blockSize == size)
            {
                for (long i = blockSize; i < blockStart + blockSize; i++)
                {
                    Memory[i].ReAlloc("");
                    if (isFixed)
                        Memory[i].Fixed = true;
                }
                return blockStart;
            }
            else
            {
                blockStart = Memory.Length;
                blockSize = size;
                MemoryBlock[] NewMemory = new MemoryBlock[Memory.Length + blockSize];
                for (int i = 0; i < blockStart; i++)
                    NewMemory[i] = Memory[i];
                for (int i = blockStart; i < NewMemory.Length; i++)
                    NewMemory[i] = new MemoryBlock("", isFixed);
                Memory = NewMemory;
                return blockStart;
            }
        }

        public void FreeMemory(int offset, int size)
        {
            if (offset < 0 || offset >= Memory.Length || (offset + size) > Memory.Length)
                ThrowExecutionException("Free Outside Memory Allocated.");
            for(int i = offset; i < offset + size; i++)
            {
                if (!Memory[i].Used)
                    ThrowExecutionException("Attempt to Free Unused Memory.");
                if (!Memory[i].Free())
                    ThrowExecutionException("Attempt to Free Fixed Memory.");
            }
        }

        public string ReadMemoryOffset(int offset)
        {
            if (offset < 0 || offset >= Memory.Length)
                ThrowExecutionException("Read Outside Memory Allocated.");
            if(!Memory[offset].Used)
                ThrowExecutionException("Attempt to Read UnInitialized Memory.");
            return Memory[offset].Value;
        }

        public void WriteMemoryOffset(int offset, string value)
        {
            if (offset < 0 || offset >= Memory.Length)
                ThrowExecutionException("Write Outside Bounds.");

            if (!Memory[offset].Used)
                ThrowExecutionException("Attemp to Write to UnInitialized Memory.");

            Memory[offset].Value = value;
        }

        #endregion

        #region Register Management

        public string ReadRegister(short registerIndex)
        {
            if (registerIndex < 0 || registerIndex >= 12)
                ThrowExecutionException("Invalid Register");
            return Register[registerIndex];
        }

        internal string ReadRegister(string registerText)
        {
            switch(registerText.Trim())
            {
                case "@0": return ReadRegister(0);
                case "@1": return ReadRegister(1);
                case "@2": return ReadRegister(2);
                case "@3": return ReadRegister(3);
                case "@4": return ReadRegister(4);
                case "@5": return ReadRegister(5);
                case "@6": return ReadRegister(6);
                case "@7": return ReadRegister(7);
                case "@8": return ReadRegister(8);
                case "@9": return ReadRegister(9);
                default: return ReadRegister(-1);
            }
        }

        public void SetRegister(short registerIndex, string value)
        {
            if (registerIndex < 0 || registerIndex >= 12)
                ThrowExecutionException("Invalid Register");
            Register[registerIndex] = value;
        }

        internal void WriteRegister(string registerText, string value)
        {
            switch (registerText.Trim())
            {
                case "@0": SetRegister(0, value); break;
                case "@1": SetRegister(1, value); break;
                case "@2": SetRegister(2, value); break;
                case "@3": SetRegister(3, value); break;
                case "@4": SetRegister(4, value); break;
                case "@5": SetRegister(5, value); break;
                case "@6": SetRegister(6, value); break;
                case "@7": SetRegister(7, value); break;
                case "@8": SetRegister(8, value); break;
                case "@9": SetRegister(9, value); break;
                default: SetRegister(-1, value); break;
            }
        }

        #endregion

        #region Stack Management

        public void StackPush(string value)
        {
            Stack.Add(value);
        }

        public string StackPeek()
        {
            if (Stack.Count < 1)
                ThrowExecutionException("Stack Exception, Peek / Pop Empty Stack.");
            return Stack[Stack.Count - 1];
        }

        public string StackPop()
        {
            string Val = StackPeek();
            Stack.RemoveAt(Stack.Count - 1);
            return Val;
        }

        #endregion


        #region Event Handlers

        public void ThrowExecutionException(string exceptionDetails)
        {
            ThrowExecutionException(OriginalLocation[CurrentScriptOffset], exceptionDetails);
        }

        public void ThrowExecutionException(Tuple<string, int> exceptionLocation, string exceptionDetails)
        {
            if (OnExecutionException == null)
                throw new Exception("Exception Handler Not Enabled: [" + exceptionLocation.Item1 + ":" + exceptionLocation.Item2.ToString() + " - " + exceptionDetails);
            else
                OnExecutionException(new ExecutionExceptionEventArgs(exceptionLocation.Item1, exceptionLocation.Item2, exceptionDetails));
        }
        #endregion


    }
}
