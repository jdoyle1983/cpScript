using System;
using System.Collections.Generic;
using System.Text;

namespace NewRuntimeProto
{
    public class State
    {
        internal static char[] SplitChars = {
                                                ' ',
                                                '%',
                                                '@',
                                                '&',
                                                '[',
                                                ']',
                                                ',',
                                                '+',
                                                '-',
                                                ':'
                                            };
        internal string[] Script;
        internal List<List<string>> ScriptPart;
        internal string[] Register = { "", "", "", "", "", "", "", "", "", "", "", "" };
        internal Dictionary<string, int> DataVars;
        internal MemoryBlock[] Memory;
        internal List<string> Stack;
        internal Dictionary<string, int> LabelOffsets;
        internal List<string> Externals;
        internal int CurrentScriptOffset = -1;
        internal int ScriptSegmentStart = -1;
        internal int DataSegmentStart = -1;

        public State()
        {
            Script = new string[0];
            ScriptPart = new List<List<string>>();
            DataVars = new Dictionary<string, int>();
            Memory = new MemoryBlock[0];
            Stack = new List<string>();
            LabelOffsets = new Dictionary<string, int>();
            Externals = new List<string>();
        }

        public void Load(string asmText)
        {
            //Split script by line breaks
            string[] SrcLines = asmText.Split(new char[] { '\n', '\r' }, StringSplitOptions.RemoveEmptyEntries);

            //Find genuine code lines, removing comments (lines beginning with ;) and the like.
            List<string> CleanLines = new List<string>();
            foreach (string s in SrcLines)
                if (s.Trim() != "" && !s.Trim().StartsWith(";"))
                    CleanLines.Add(s.Trim());

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
                            fItems.Add(QuoteString);
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
                throw new Exception("Required Code Segment Missing.");

            //Parse Data Segment
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
                        throw new Exception("Label '" + labelName + "' Already Defined.");
                    if (Externals.Contains(labelName))
                        throw new Exception("Label '" + labelName + "' Conflicts with External.");
                    if (DataVars.ContainsKey(labelName))
                        throw new Exception("Label '" + labelName + "' Conflicts with Data Variable.");
                    LabelOffsets.Add(ScriptPart[i][0], i);
                }
            }
        }

        internal void AllocateInitialData()
        {
            //Parse Data Segment
            if (DataSegmentStart != -1)
            {
                for (int i = DataSegmentStart; i < ScriptPart.Count; i++)
                {
                    if (ScriptPart[i][0] == "[")
                        break;
                    string varName = ScriptPart[i][0];
                    if (DataVars.ContainsKey(varName))
                        throw new Exception("Data Variable '" + varName + "' is Already Defined.");
                    string initialValue = "";
                    if (ScriptPart[i].Count == 3)
                        initialValue = ScriptPart[i][2];
                    int memoryLocation = AllocateMemory(1);
                    WriteMemoryOffset(memoryLocation, initialValue);
                    DataVars.Add(varName, memoryLocation);
                }
            }
        }

        public void Reset()
        {
            Register[0] = Register[1] = Register[2] = Register[3] = Register[4] = Register[5] = Register[6] = Register[7] = Register[8] = Register[9] = Register[10] = Register[11] = "";
            Memory = new MemoryBlock[0];
            AllocateInitialData();
            CurrentScriptOffset = -1;
            Stack.Clear();
        }

        public string ResolveGetValue(List<string> valueParts)
        {
            return "";
        }

        public void ResolveSetValue(List<string> valueParts, string setValue)
        {
            
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
                case "pop":                                             // POP {DEST]               -   Pop value from stack
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
                            throw new Exception("Resolved Free Memory Location is Not a Number.");
                        if (!Int32.TryParse(sizeVal, out memorySize))
                            throw new Exception("Resolved Free Memory Size is Not a Number.");
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
                                throw new Exception("Calculation Attempted on Non-Numeric Value.");

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
                            throw new Exception("Attempted To Inverse Non-Numeric Value.");
                        StackPush((dValue * -1.0).ToString());
                    } break;
                case "cmpa":                                            // CMPA                     -   Pop Last 2 values, push true if they AND (PUSH 0, PUSH 1 -> 1 && 0, STACK PUSH 0)
                    {

                    } break;
                case "cmpo":                                            // CMPO                     -   Pop Last 2 values, push true if they OR (PUSH 0, PUSH 1 -> 1 || 0, STACK PUSH 1)
                    {

                    } break;
                case "cmpe":                                            // CMPE                     -   Pop Last 2 values, push true if they are equal (PUSH 2, PUSH 1 -> 2 == 1, STACK PUSH 0)
                    {

                    } break;
                case "cmpn":                                            // CMPN                     -   Pop Last 2 values, push true if they are not equal (PUSH 2, PUSH 1 -> 2 != 1, STACK PUSH 1)
                    {

                    }
                    break;
                case "cmpg":                                            // CMPG                     -   Pop Last 2 values, push true if value 2 is greater than value 1 (PUSH 2, PUSH 1 -> 2 > 1, STACK PUSH 1)
                    {

                    }
                    break;
                case "cmpge":                                           // CMPGE                    -   Pop Last 2 values, push true if value 2 is greater than or equal to value 1 (PUSH 2, PUSH 1 -> 2 >= 1, STACK PUSH 1)
                    {

                    }
                    break;
                case "cmpl":                                            // CMPEL                    -   Pop Last 2 values, push true if value 2 is less than value 1 (PUSH 2, PUSH 1 -> 2 < 1, STACK PUSH 0)
                    {

                    }
                    break;
                case "cmple":                                           // CMPLE                     -   Pop Last 2 values, push true if value 2 is less than or equal to value 1 (PUSH 2, PUSH 1 -> 2 <= 1, STACK PUSH 0)
                    {

                    }
                    break;
                case "jmpl":                                            // JMPL LABEL               -   Move execution pointer to LABEL
                    {

                    } break;
                case "jmpo":                                            // JMPO [OFFSET]            -   Move execution pointer to position [OFFSET] from current location
                    {

                    } break;
                case "jmplt":                                           // JMPLT LABEL              -   Jump to LABEL if poped value is true
                    {

                    } break;
                case "jmpot":                                           // JMPOT [OFFSET]           -   Jump to [OFFSET] if poped value is true
                    {

                    } break;
            }

            return true;
        }

        #region Memory Management
        public int AllocateMemory(int size)
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
                    Memory[i].ReAlloc("");
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
                    NewMemory[i] = new MemoryBlock("");
                Memory = NewMemory;
                return blockStart;
            }
        }

        public void FreeMemory(int offset, int size)
        {
            if (offset < 0 || offset >= Memory.Length || (offset + size) > Memory.Length)
                throw new Exception("Free Outside Memory Allocated.");
            for(int i = offset; i < offset + size; i++)
            {
                if (!Memory[i].Used)
                    throw new Exception("Attempt to Free Unused Memory.");
                Memory[i].Free();
            }
        }

        public string ReadMemoryOffset(int offset)
        {
            if (offset < 0 || offset >= Memory.Length)
                throw new Exception("Read Outside Memory Allocated.");
            if(!Memory[offset].Used)
                throw new Exception("Attempt to Read UnInitialized Memory.");
            return Memory[offset].Value;
        }

        public void WriteMemoryOffset(int offset, string value)
        {
            if (offset < 0 || offset >= Memory.Length)
                throw new Exception("Write Outside Bounds.");

            if (!Memory[offset].Used)
                throw new Exception("Attemp to Write to UnInitialized Memory.");

            Memory[offset].Value = value;
        }

        #endregion

        #region Register Management

        public string ReadRegister(short registerIndex)
        {
            if (registerIndex < 0 || registerIndex >= 12)
                throw new Exception("Invalid Register");
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
                throw new Exception("Invalid Register");
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
                throw new Exception("Stack Exception, Peek / Pop Empty Stack.");
            return Stack[Stack.Count - 1];
        }

        public string StackPop()
        {
            string Val = StackPeek();
            Stack.RemoveAt(Stack.Count - 1);
            return Val;
        }

        #endregion


    }
}
