using System;
using System.Collections.Generic;
using System.Text;

namespace NewRuntimeProto
{
    public class State
    {
        public string[] Registers = { "", "", "", "", "", "", "", "", "", "", "", "" };
        public string[] Memory;
        public List<string> Stack;

        public State()
        {
            Memory = new string[0];
            Stack = new List<string>();
        }

        #region Memory Management

        internal string ReadMemoryOffset(long Offset)
        {
            if (Offset < 0 || Offset >= Memory.LongLength)
                throw new Exception("Read Outside Memory Allocated.");
            return Memory[Offset];
        }

        internal void WriteMemoryOffset(long Offset, string Value)
        {
            if (Offset < 0)
                throw new Exception("Write Outside Bounds.");

            if(Offset >= Memory.LongLength)
            {
                long OldSize = Memory.LongLength;
                long NewSize = Offset + 1;

                string[] NewMemory = new string[NewSize];

                for (long i = 0; i < OldSize; i++)
                    NewMemory[i] = Memory[i];
                for (long i = OldSize; i < NewSize; i++)
                    NewMemory[i] = "";

                Memory = NewMemory;
            }

            Memory[Offset] = Value;
        }

        #endregion

        #region Register Management

        internal string ReadRegister(short RegisterIndex)
        {
            if (RegisterIndex < 0 || RegisterIndex >= 12)
                throw new Exception("Invalid Register");
            return Registers[RegisterIndex];
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

        internal void SetRegister(short RegisterIndex, string Value)
        {
            if (RegisterIndex < 0 || RegisterIndex >= 12)
                throw new Exception("Invalid Register");
            Registers[RegisterIndex] = Value;
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

        internal void StackPush(string Value)
        {
            Stack.Add(Value);
        }

        internal string StackPeek()
        {
            if (Stack.Count < 1)
                throw new Exception("Stack Exception, Peek / Pop Empty Stack.");
            return Stack[Stack.Count - 1];
        }

        internal string StackPop()
        {
            string Val = StackPeek();
            Stack.RemoveAt(Stack.Count - 1);
            return Val;
        }

        #endregion


    }
}
