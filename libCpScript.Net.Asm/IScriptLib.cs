using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace libCpScript.Net.Asm
{
    public interface IScriptLib
    {
        string Author();
        string Version();
        void Init(State Script);
    }
}
