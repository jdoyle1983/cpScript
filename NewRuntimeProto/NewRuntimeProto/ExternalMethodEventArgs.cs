using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace NewRuntimeProto
{
    public delegate void ExternalMethodHandler(ExternalMethodEventArgs e);

    public class ExternalMethodEventArgs
    {
        public string ExternalMethodRequested { get; set; }
        public State ScriptState { get; set; }

        public ExternalMethodEventArgs(string externalMethodRequested, State scriptState)
        {
            ExternalMethodRequested = externalMethodRequested;
            ScriptState = scriptState;
        }
    }
}
