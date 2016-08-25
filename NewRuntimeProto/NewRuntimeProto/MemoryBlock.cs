using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NewRuntimeProto
{
    public class MemoryBlock
    {
        public bool Used { get; set; }
        public bool Fixed { get; set; }
        public string Value { get; set; }

        public MemoryBlock()
        {
            Used = false;
            Fixed = false;
            Value = "";
        }

        public MemoryBlock(string initialValue, bool isFixed = false)
        {
            Fixed = isFixed;
            Used = true;
            Value = initialValue;
        }

        public void ReAlloc(string newValue)
        {
            Used = true;
            Value = newValue;
        }

        public bool Free()
        {
            if (Fixed)
                return false;
            Used = false;
            Value = "";
            return true;
        }
    }
}
