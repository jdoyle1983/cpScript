using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NewRuntimeProto
{
    public class MemoryBlock
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
}
