using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NewRuntimeProto
{
    public delegate void ExecutionExceptionHandler(ExecutionExceptionEventArgs e);
    public class ExecutionExceptionEventArgs
    {
        public string SourceFile { get; set; }
        public int SrcLine { get; set; }
        public string ExceptionDetails { get; set; }

        public ExecutionExceptionEventArgs(string sourceFile, int srcLine, string exceptionDetails)
        {
            SourceFile = sourceFile;
            SrcLine = srcLine;
            ExceptionDetails = exceptionDetails;
        }
    }
}
