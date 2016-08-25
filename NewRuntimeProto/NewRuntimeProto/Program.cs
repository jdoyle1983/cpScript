using System;
using System.Collections.Generic;
using System.Text;

namespace NewRuntimeProto
{
    class Program
    {
        static void Main(string[] args)
        {
            DateTime nativeStart = DateTime.Now;
            for (int i = 0; i < 10000; i++)
                Console.WriteLine(i.ToString());
            DateTime nativeEnd = DateTime.Now;

            DateTime startTime = DateTime.Now;
            State tState = new State();
            tState.OnExternalMethodCalled += TState_OnExternalMethodCalled;
            tState.OnExecutionException += TState_OnExecutionException;
            tState.Load(AppDomain.CurrentDomain.BaseDirectory + "TestScript.asm");

            DateTime runTime = DateTime.Now;

            while (tState.Iterate()) ;

            DateTime endTime = DateTime.Now;

            DateTime native2Start = DateTime.Now;
            for (int i = 0; i < 10000; i++)
                Console.WriteLine(i.ToString());
            DateTime native2End = DateTime.Now;

            Console.WriteLine();
            Console.WriteLine("Done.");
            Console.WriteLine("Init Time    : " + (runTime - startTime).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Run Time     : " + (endTime - runTime).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Total Time   : " + (endTime - startTime).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Native Time  : " + (nativeEnd - nativeStart).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Native2 Time : " + (native2End - native2Start).TotalMilliseconds.ToString() + "ms");
            Console.ReadLine();
        }

        private static void TState_OnExecutionException(ExecutionExceptionEventArgs e)
        {
            string r = e.SourceFile + ":" + e.SrcLine.ToString() + " - " + e.ExceptionDetails;
            Console.WriteLine(r);
            throw new Exception(r);
        }

        private static void TState_OnExternalMethodCalled(ExternalMethodEventArgs e)
        {
            if (e.ExternalMethodRequested == "_console_writeline")
                Console.WriteLine(e.ScriptState.StackPop());
            else if (e.ExternalMethodRequested == "_console_readline")
                e.ScriptState.StackPush(Console.ReadLine());
        }
    }
}
