using System;
using System.Collections.Generic;
using System.Text;

namespace NewRuntimeProto
{
    public class State
    {
        internal static char[] SplitChars = {
                                                ' ',
                                                '%',        //Memory Location
                                                '@',        //Register
                                                '&',        //Address Of
                                                '[',
                                                ']',
                                                ',',
                                                '+',
                                                '-',
                                                ':'
                                            };
        internal string[] Script;
        internal List<List<string>> ScriptPart;
        internal List<int> OriginalLines;
        internal List<string> OriginalFile;
        internal string[] Register = { "", "", "", "", "", "", "", "", "", "", "", "" };
        internal Dictionary<string, int> DataVars;
        internal MemoryBlock[] Memory;
        internal List<string> Stack;
        internal Dictionary<string, int> LabelOffsets;
        internal List<string> Externals;
        internal List<List<string>> DataVarDeclarations;
        internal int CurrentScriptOffset = -1;
        internal int ScriptSegmentStart = -1;
        internal int DataSegmentStart = -1;

        public event ExternalMethodHandler OnExternalMethodCalled;
        public event ExecutionExceptionHandler OnExecutionException;

        public State()
        {
            Script = new string[0];
            ScriptPart = new List<List<string>>();
            OriginalLines = new List<int>();
            OriginalFile = new List<string>();
            DataVars = new Dictionary<string, int>();
            Memory = new MemoryBlock[0];
            Stack = new List<string>();
            LabelOffsets = new Dictionary<string, int>();
            Externals = new List<string>();
            DataVarDeclarations = new List<List<string>>();
            OnExternalMethodCalled = null;
        }

        public void Load(string AsmFile)
        {
            //string asmText = System.IO.File.ReadAllText(AsmFile);
            //Split script by line breaks
            string[] SrcLines = System.IO.File.ReadAllLines(AsmFile);// asmText.Split(new char[] { '\n', '\r' });

            //Find genuine code lines, removing comments (lines beginning with ;) and the like.
            List<string> CleanLines = new List<string>();
            int lCounter = 0;
            foreach (string s in SrcLines)
            {
                if (s.Trim() != "" && !s.Trim().StartsWith(";"))
                {
                    CleanLines.Add(s.Trim());
                    OriginalLines.Add(lCounter);
                    OriginalFile.Add(AsmFile);
                }
                lCounter++;
            }

            //Store entire clean script
            Script = CleanLines.ToArray();

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
                ScriptPart.Add(fItems);
            }

            int ExternalSegmentStart = -1;

            for (int i = 0; i < ScriptPart.Count; i++)
            {
                if(ScriptPart[i].Count == 3 && ScriptPart[i][0] == "[" && ScriptPart[i][2] == "]")
                {
                    switch(ScriptPart[i][1].ToLower())
                    {
                        case "data": DataSegmentStart = i + 1; break;
                        case "external": ExternalSegmentStart = i + 1; break;
                        case "code": ScriptSegmentStart = i + 1; break;
                    }
                    
                }
            }

            if (ScriptSegmentStart == -1)
                ThrowExecutionException(AsmFile, 0, "Required Code Segment Missing.");

            CurrentScriptOffset = ScriptSegmentStart;

            //Parse Data Segment
            ParseDataVariables();
            AllocateInitialData();

            //Parse External Segment
            if(ExternalSegmentStart != -1)
            {
                for(int i = ExternalSegmentStart; i < ScriptPart.Count; i++)
                {
                    if(ScriptPart[i][0] == "[")
                        break;
                    Externals.Add(ScriptPart[i][0]);
                }
            }

            //Find each label offset
            //Example:
            //MyLabel:
            for (int i = ScriptSegmentStart; i < ScriptPart.Count; i++)
            {
                if (ScriptPart[i].Count == 2 && ScriptPart[i][1] == ":")
                {
                    string labelName = ScriptPart[i][0];
                    if (LabelOffsets.ContainsKey(labelName))
                        ThrowExecutionException(AsmFile, OriginalLines[i], "Label '" + labelName + "' Already Defined.");
                    if (Externals.Contains(labelName))
                        ThrowExecutionException(AsmFile, OriginalLines[i], "Label '" + labelName + "' Conflicts with External.");
                    if (DataVars.ContainsKey(labelName))
                        ThrowExecutionException(AsmFile, OriginalLines[i], "Label '" + labelName + "' Conflicts with Data Variable.");
                    LabelOffsets.Add(ScriptPart[i][0], i);
                }
            }
        }

        /*
        public State MergeIntoThisScript(State otherState, bool mergeDataVariables, bool mergeExternals)
        {
            List<string> newScript = new List<string>(Script);
            newScript.AddRange(otherState.Script);

            List<List<string>> newScriptPart = new List<List<string>>(ScriptPart);
            newScriptPart.AddRange(otherState.ScriptPart);

            List<int> newOriginalLines = new List<int>(OriginalLines);
            newOriginalLines.AddRange(otherState.OriginalLines);

            List<string> newOriginalFile = new List<string>(OriginalFile);
            newOriginalFile.AddRange(otherState.OriginalFile);

            List<List<string>> newDataVarDeclarations = new List<List<string>>();
            Dictionary<string, int> newDataVars = new Dictionary<string, int>(DataVars);
            foreach(string key in otherState.DataVars.Keys)
            {
                if (newDataVars.ContainsKey(key))
                {
                    if (!mergeDataVariables)
                        ThrowExecutionException("No File", 0, "Merge Disabled for Data Variables, Conflict Found.");
                }
                


            }

            for(int i = 0; i < otherState.DataVars.Keys.Count; i++)
            {
                
                if(newDataVars.ContainsKey(otherState.DataVars.Keys[i]) && !mergeDataVariables)
                                    
            }

            //Find each label offset
            //Example:
            //MyLabel:
            for (int i = ScriptSegmentStart; i < ScriptPart.Count; i++)
            {
                if (ScriptPart[i].Count == 2 && ScriptPart[i][1] == ":")
                {
                    string labelName = ScriptPart[i][0];
                    if (LabelOffsets.ContainsKey(labelName))
                        ThrowExecutionException(AsmFile, OriginalLines[i], "Label '" + labelName + "' Already Defined.");
                    if (Externals.Contains(labelName))
                        ThrowExecutionException(AsmFile, OriginalLines[i], "Label '" + labelName + "' Conflicts with External.");
                    if (DataVars.ContainsKey(labelName))
                        ThrowExecutionException(AsmFile, OriginalLines[i], "Label '" + labelName + "' Conflicts with Data Variable.");
                    LabelOffsets.Add(ScriptPart[i][0], i);
                }
            }

            return this;
        }
        */

        internal void ParseDataVariables()
        {
            //Parse Data Segment
            if (DataSegmentStart != -1)
            {
                for (int i = DataSegmentStart; i < ScriptPart.Count; i++)
                {
                    if (ScriptPart[i][0] == "[")
                        break;
                    DataVarDeclarations.Add(ScriptPart[i]);
                }
            }
        }

        internal void AllocateInitialData()
        {
            for(int i = 0; i < DataVarDeclarations.Count; i++)
            {
                string varName = DataVarDeclarations[i][0];
                if (DataVars.ContainsKey(varName))
                    ThrowExecutionException(OriginalFile[i], OriginalLines[i], "Data Variable '" + varName + "' is Already Defined.");
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
                        ThrowExecutionException(OriginalFile[i], OriginalLines[i], "Integer Value Expected.");
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
            CurrentScriptOffset = ScriptSegmentStart;
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

                return ReadMemoryOffset(memoryOffset);
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

            if(CurrentScriptOffset >= Script.Length)
            {
                CurrentScriptOffset = Script.Length - 1;
                return false;
            }

            if (ScriptPart[CurrentScriptOffset][0].EndsWith(":"))
                CurrentScriptOffset++;

            if (CurrentScriptOffset >= Script.Length)
            {
                CurrentScriptOffset = Script.Length - 1;
                return false;
            }

            List<string> Step = ScriptPart[CurrentScriptOffset];

            switch(Step[0].ToLower())
            {
                case "push":                                            // PUSH [VALUE]             -    Push value onto stack
                    {
                        StackPush(ResolveGetValue(Step.GetRange(1, Step.Count - 1)));
                    } break;
                case "pop":                                             // POP [DEST]               -   Pop value from stack
                    {
                        ResolveSetValue(Step.GetRange(1, Step.Count - 1), StackPop());
                    } break;
                case "pushr":                                           // PUSHR                    -   Push all registers onto stack
                    {
                        for (int i = 0; i < 12; i++)
                            StackPush(Register[i]);
                    } break;
                case "popr":                                            // POPR                     -   Pop all registers from stack
                    {
                        for (int i = 11; i >= 0; i--)
                            Register[i] = StackPop();
                    } break;
                case "mov":                                             // MOV [SRC], [DEST]        -   Move value from one location to another
                    {
                        int commaBreak = Step.IndexOf(",");
                        int destStart = commaBreak + 1;
                        int destLen = Step.Count - destStart;
                        ResolveSetValue(Step.GetRange(destStart, destLen), ResolveGetValue(Step.GetRange(1, commaBreak - 1)));
                    } break;
                case "alloc":                                           // ALLOC [SIZE]             -   Allocate blocks of memory, pushes location on stack
                    {
                        int allocSize = 0;
                        string sizeVal = ResolveGetValue(Step.GetRange(1, Step.Count - 1));
                        if (!Int32.TryParse(sizeVal, out allocSize))
                            throw new Exception("Resolved Alloc Value is Not a Number.");
                        StackPush(AllocateMemory(allocSize).ToString());
                    } break;
                case "free":                                            // FREE [MEMLOC], [SIZE]    -   Free blocks of memory
                    {
                        int commaBreak = Step.IndexOf(",");
                        int sizeStart = commaBreak + 1;
                        int sizeLen = Step.Count - sizeStart;

                        string memLocVal = ResolveGetValue(Step.GetRange(1, commaBreak - 1));
                        string sizeVal = ResolveGetValue(Step.GetRange(sizeStart, sizeLen));

                        int memoryLocation = -1;
                        int memorySize = -1;

                        if (!Int32.TryParse(memLocVal, out memoryLocation))
                            ThrowExecutionException("Resolved Free Memory Location is Not a Number.");
                        if (!Int32.TryParse(sizeVal, out memorySize))
                            ThrowExecutionException("Resolved Free Memory Size is Not a Number.");
                        FreeMemory(memoryLocation, memorySize);
                    } break;
                case "add":                                             // ADD                      -   Pop last 2 values from stack, add and push result onto stack (PUSH 2, PUSH 1 -> 2 + 1 = 3)
                case "sub":                                             // SUB                      -   Pop last 2 values from stack, subtract and push result onto stack (PUSH 2, PUSH 1 -> 2 - 1 = 1)
                case "mul":                                             // MUL                      -   Pop last 2 values from stack, multiply and push result onto stack (PUSH 2, PUSH 1 -> 2 * 1 = 2)
                case "div":                                             // DIV                      -   Pop last 2 values from stack, divide and push result onto stack (PUSH 2, PUSH 1 -> 2 / 1 = 2)
                case "mod":                                             // MOD                      -   Pop last 2 values from stack, modulo and push result onto stack (PUSH 2, PUSH 1 -> 2 % 1 = 0)
                case "con":                                             // CON                      -   Pop last 2 values from stack, concat and push result onto stack (PUSH 2, PUSH 1 -> 2 .. 1 = "21")
                    {
                        string strValue1 = StackPop();
                        string strValue2 = StackPop();

                        if(Step[0].ToLower() == "con")
                        {
                            StackPush(strValue2 + strValue1);
                        }
                        else
                        {
                            double dValue1 = 0;
                            double dValue2 = 0;

                            if (!double.TryParse(strValue1, out dValue1) || !double.TryParse(strValue2, out dValue2))
                                ThrowExecutionException("Calculation Attempted on Non-Numeric Value.");

                            switch(Step[0].ToLower())
                            {
                                case "add": StackPush((dValue2 + dValue1).ToString()); break;
                                case "sub": StackPush((dValue2 + dValue1).ToString()); break;
                                case "mul": StackPush((dValue2 * dValue1).ToString()); break;
                                case "div":
                                case "mod":
                                    {
                                        if (dValue1 == 0)
                                            throw new Exception("Calculation Attempted Divided by Zero.");
                                        if (Step[0].ToLower() == "div") StackPush((dValue2 / dValue1).ToString());
                                        else StackPush((dValue2 % dValue1).ToString());
                                    } break;
                            }
                        }
                    } break;
                case "neg":                                             // NEG                      -   Pop last value from stack, multiply by -1 and push result onto stack (PUSH 1 -> 1 * -1 = -1)
                    {
                        string strValue = StackPop();
                        double dValue = 0;
                        if (!double.TryParse(strValue, out dValue))
                            ThrowExecutionException("Attempted To Inverse Non-Numeric Value.");
                        StackPush((dValue * -1.0).ToString());
                    } break;
                case "inc":                                             // INC [VAR], [INC AMOUNT]  -   Increment VAR by INC AMOUNT
                case "dec":                                             // DEC [VAR], [DEC AMOUNT]  -   Decrement VAR by INC AMOUNT
                    {
                        int commaBreak = Step.IndexOf(",");
                        int amtStart = commaBreak + 1;
                        int amtLen = Step.Count - amtStart;

                        string currentValue = ResolveGetValue(Step.GetRange(1, commaBreak - 1));
                        string amtValue = ResolveGetValue(Step.GetRange(amtStart, amtLen));

                        double numericValue = 0;
                        double numericAmt = 0;

                        if (!double.TryParse(currentValue, out numericValue) || !double.TryParse(amtValue, out numericAmt))
                            ThrowExecutionException("Invalid Data Types Specified for INC/DEC Values.");

                        if (Step[0].ToLower() == "inc")
                            numericValue += numericAmt;
                        else
                            numericValue -= numericAmt;

                        ResolveSetValue(Step.GetRange(1, commaBreak - 1), numericValue.ToString());
                    } break;
                case "cmpa":                                            // CMPA                     -   Pop Last 2 values, push true if they AND (PUSH 0, PUSH 1 -> 1 && 0, STACK PUSH 0)
                case "cmpo":                                            // CMPO                     -   Pop Last 2 values, push true if they OR (PUSH 0, PUSH 1 -> 1 || 0, STACK PUSH 1)
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

                        if (Step[0].ToLower() == "cmpa") StackPush((bValue2 && bValue1) ? "1" : "0");
                        else StackPush((bValue2 || bValue1) ? "1" : "0");
                    } break;
                case "cmpe":                                            // CMPE                     -   Pop Last 2 values, push true if they are equal (PUSH 2, PUSH 1 -> 2 == 1, STACK PUSH 0)
                case "cmpn":                                            // CMPN                     -   Pop Last 2 values, push true if they are not equal (PUSH 2, PUSH 1 -> 2 != 1, STACK PUSH 1)
                case "cmpg":                                            // CMPG                     -   Pop Last 2 values, push true if value 2 is greater than value 1 (PUSH 2, PUSH 1 -> 2 > 1, STACK PUSH 1)
                case "cmpge":                                           // CMPGE                    -   Pop Last 2 values, push true if value 2 is greater than or equal to value 1 (PUSH 2, PUSH 1 -> 2 >= 1, STACK PUSH 1)
                case "cmpl":                                            // CMPEL                    -   Pop Last 2 values, push true if value 2 is less than value 1 (PUSH 2, PUSH 1 -> 2 < 1, STACK PUSH 0)
                case "cmple":                                           // CMPLE                     -   Pop Last 2 values, push true if value 2 is less than or equal to value 1 (PUSH 2, PUSH 1 -> 2 <= 1, STACK PUSH 0)
                    {
                        string strValue1 = StackPop();
                        string strValue2 = StackPop();

                        if (Step[0].ToLower() == "cmpe")
                            StackPush((strValue2 == strValue1) ? "1" : "0");
                        else if (Step[0].ToLower() == "cmpn")
                            StackPush((strValue2 != strValue1) ? "1" : "0");
                        else
                        {
                            double dValue1 = 0;
                            double dValue2 = 0;

                            if (!double.TryParse(strValue1, out dValue1) || !double.TryParse(strValue2, out dValue2))
                                ThrowExecutionException("Numeric Comparison Attempted on Non-Numeric Value.");

                            switch(Step[0].ToLower())
                            {
                                case "cmpg": StackPush((dValue2 > dValue1) ? "1" : "0"); break;
                                case "cmpge": StackPush((dValue2 >= dValue1) ? "1" : "0"); break;
                                case "cmpl": StackPush((dValue2 < dValue1) ? "1" : "0"); break;
                                case "cmple": StackPush((dValue2 <= dValue1) ? "1" : "0"); break;
                            }
                        }
                    }
                    break;
                case "jmpl":                                            // JMPL LABEL               -   Move execution pointer to LABEL
                    {
                        DoLabelJump(Step);                
                    } break;
                case "jmpo":                                            // JMPO [OFFSET]            -   Move execution pointer to position [OFFSET] from current location
                    {
                        DoOffsetJump(Step);
                    } break;
                case "jmplt":                                           // JMPLT LABEL              -   Jump to LABEL if poped value is true
                    {
                        string strValue = StackPop();
                        if (strValue != "1" && strValue != "0")
                            ThrowExecutionException("Atemped Bitwise Comparison on Invalid Value.");
                        if(strValue == "1")
                        {
                            DoLabelJump(Step);
                        }
                    } break;
                case "jmpot":                                           // JMPOT [OFFSET]           -   Jump to [OFFSET] if poped value is true
                    {
                        string strValue = StackPop();
                        if (strValue != "1" && strValue != "0")
                            ThrowExecutionException("Atemped Bitwise Comparison on Invalid Value.");
                        if (strValue == "1")
                        {
                            DoOffsetJump(Step);
                        }
                    } break;
            }

            return true;
        }

        internal void DoLabelJump(List<string> Step)
        {
            if (!LabelOffsets.ContainsKey(Step[1]) && !Externals.Contains(Step[1]))
                ThrowExecutionException("Jump Label Requested Does Not Exist.");

            if (LabelOffsets.ContainsKey(Step[1]))
                CurrentScriptOffset = LabelOffsets[Step[1]];
            else
            {
                if (OnExternalMethodCalled == null)
                    ThrowExecutionException("External Method Event Handler Not Defined and an External Method was Requested.");
                else
                    OnExternalMethodCalled(new ExternalMethodEventArgs(Step[1], this));
            }
        }

        internal void DoOffsetJump(List<string> Step)
        {
            string strValue = ResolveGetValue(Step.GetRange(1, Step.Count - 1));
            int offsetValue = 0;
            if (!Int32.TryParse(strValue, out offsetValue))
                ThrowExecutionException("Attmpted Offset Jump With Non-Numeric Offset.");
            int newOffset = CurrentScriptOffset + offsetValue;
            if (newOffset < ScriptSegmentStart || newOffset >= ScriptPart.Count)
                ThrowExecutionException("Attmpted Offset Jump Beyond Bounds.");
            CurrentScriptOffset = newOffset;
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
            ThrowExecutionException(OriginalFile[CurrentScriptOffset], OriginalLines[CurrentScriptOffset], exceptionDetails);
        }

        public void ThrowExecutionException(string srcFile, int srcLine, string exceptionDetails)
        {
            if (OnExecutionException == null)
                throw new Exception("Exception Handler Not Enabled: [" + srcFile + ":" + srcLine.ToString() + " - " + exceptionDetails);
            else
                OnExecutionException(new ExecutionExceptionEventArgs(srcFile, srcLine, exceptionDetails));
        }
        #endregion


    }
}
