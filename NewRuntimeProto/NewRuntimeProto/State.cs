using System;
using System.Collections.Generic;
using System.Text;

namespace NewRuntimeProto
{
    internal class MemoryBlock
    {
        public bool Used { get; set; }
        public string Value { get; set; }

        public MemoryBlock()
        {
            Used = false;
            Value = "";
        }

        public MemoryBlock(string initialValue)
        {
            Used = true;
            Value = initialValue;
        }

        public void ReAlloc(string newValue)
        {
            Used = true;
            Value = newValue;
        }

        public void Free()
        {
            Used = false;
            Value = "";
        }
    }

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
                                                '-'
                                            };
        internal string[] Script;
        internal List<List<string>> ScriptPart;
        internal string[] Register = { "", "", "", "", "", "", "", "", "", "", "", "" };
        internal MemoryBlock[] Memory;
        internal List<string> Stack;
        internal Dictionary<string, int> LabelOffsets;
        internal int CurrentScriptOffset = -1;

        public State()
        {
            Script = new string[0];
            ScriptPart = new List<List<string>>();
            Memory = new MemoryBlock[0];
            Stack = new List<string>();
            LabelOffsets = new Dictionary<string, int>();
        }

        public void Load(string AsmText)
        {
            //Split script by line breaks
            string[] SrcLines = AsmText.Split(new char[] { '\n', '\r' }, StringSplitOptions.RemoveEmptyEntries);

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

            //Find each label offset
            //Example:
            //MyLabel:
            for (int i = 0; i < Script.Length; i++)
                if (Script[i].EndsWith(":"))
                    LabelOffsets.Add(Script[i].Replace(":", ""), i);
        }

        public void Reset()
        {
            Register[0] = Register[1] = Register[2] = Register[3] = Register[4] = Register[5] = Register[6] = Register[7] = Register[8] = Register[9] = Register[10] = Register[11] = "";
            Memory = new MemoryBlock[0];
            CurrentScriptOffset = -1;
            Stack.Clear();
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

            switch(ScriptPart[CurrentScriptOffset][0].ToLower())
            {
                case "push":
                    {

                    } break;
                case "pop":
                    {

                    } break;
                case "pushr":
                    {
                        for (int i = 0; i < 12; i++)
                            StackPush(Register[i]);
                    } break;
                case "popr":
                    {
                        for (int i = 11; i >= 0; i--)
                            Register[i] = StackPop();
                    } break;
                case "alloc":
                    {

                    } break;
                case "free":
                    {

                    } break;
            }

            return true;
        }

        #region Memory Management
        public long AllocateMemory(long allocationSize)
        {
            long blockStart = -1;
            long blockSize = -1;

            for(long i = 0; i < Memory.LongLength; i++)
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

                    if (blockSize == allocationSize)
                        break;
                }
                else
                {
                    blockStart = -1;
                    blockSize = -1;
                }
            }

            if(blockStart != -1 && blockSize == allocationSize)
            {
                for (long i = blockSize; i < blockStart + blockSize; i++)
                    Memory[i].ReAlloc("");
                return blockStart;
            }
            else
            {
                blockStart = Memory.LongLength;
                MemoryBlock[] NewMemory = new MemoryBlock[Memory.LongLength + blockSize];
                for (long i = 0; i < blockStart; i++)
                    NewMemory[i] = Memory[i];
                for (long i = blockStart; i < NewMemory.LongLength; i++)
                    NewMemory[i] = new MemoryBlock("");
                Memory = NewMemory;
                return blockStart;
            }
        }

        public void FreeMemory(long offset, long size)
        {
            if (offset < 0 || offset >= Memory.LongLength || (offset + size) > Memory.LongLength)
                throw new Exception("Free Outside Memory Allocated.");
            for(long i = offset; i < offset + size; i++)
            {
                if (!Memory[i].Used)
                    throw new Exception("Attempt to Free Unused Memory.");
                Memory[i].Free();
            }
        }

        public string ReadMemoryOffset(long Offset)
        {
            if (Offset < 0 || Offset >= Memory.LongLength)
                throw new Exception("Read Outside Memory Allocated.");
            if(!Memory[Offset].Used)
                throw new Exception("Attempt to Read UnInitialized Memory.");
            return Memory[Offset].Value;
        }

        public void WriteMemoryOffset(long Offset, string Value)
        {
            if (Offset < 0 || Offset >= Memory.LongLength)
                throw new Exception("Write Outside Bounds.");

            if (!Memory[Offset].Used)
                throw new Exception("Attemp to Write to UnInitialized Memory.");

            Memory[Offset].Value = Value;
        }

        #endregion

        #region Register Management

        public string ReadRegister(short RegisterIndex)
        {
            if (RegisterIndex < 0 || RegisterIndex >= 12)
                throw new Exception("Invalid Register");
            return Register[RegisterIndex];
        }

        internal string ReadRegister(string RegisterText)
        {
            switch(RegisterText.Trim())
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

        public void SetRegister(short RegisterIndex, string Value)
        {
            if (RegisterIndex < 0 || RegisterIndex >= 12)
                throw new Exception("Invalid Register");
            Register[RegisterIndex] = Value;
        }

        internal void WriteRegister(string RegisterText, string Value)
        {
            switch (RegisterText.Trim())
            {
                case "@0": SetRegister(0, Value); break;
                case "@1": SetRegister(1, Value); break;
                case "@2": SetRegister(2, Value); break;
                case "@3": SetRegister(3, Value); break;
                case "@4": SetRegister(4, Value); break;
                case "@5": SetRegister(5, Value); break;
                case "@6": SetRegister(6, Value); break;
                case "@7": SetRegister(7, Value); break;
                case "@8": SetRegister(8, Value); break;
                case "@9": SetRegister(9, Value); break;
                default: SetRegister(-1, Value); break;
            }
        }

        #endregion

        #region Stack Management

        public void StackPush(string Value)
        {
            Stack.Add(Value);
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
