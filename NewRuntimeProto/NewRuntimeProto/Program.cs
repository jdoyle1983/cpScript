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
            tState.OnExecutionException += TState_OnExecutionException;
            tState.RegisterFunction("_console_writeline", External_Console_WriteLine);
            tState.RegisterFunction("_console.readline", External_Console_ReadLine);
            tState.Load(AppDomain.CurrentDomain.BaseDirectory + "TestScript.asm");

            //State addonState = new State();
            //addonState.Load(AppDomain.CurrentDomain.BaseDirectory + "TestScript_Addon.asm");
            //tState = tState.MergeIntoThisScript(addonState);

            DateTime runTime = DateTime.Now;

            while (tState.Iterate()) ;

            DateTime endTime = DateTime.Now;

            Console.WriteLine();
            Console.WriteLine("Done.");
            Console.WriteLine("Init Time    : " + (runTime - startTime).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Run Time     : " + (endTime - runTime).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Total Time   : " + (endTime - startTime).TotalMilliseconds.ToString() + "ms");
            Console.WriteLine("Native Time  : " + (nativeEnd - nativeStart).TotalMilliseconds.ToString() + "ms");
            Console.ReadLine();
        }

        private static void TState_OnExecutionException(ExecutionExceptionEventArgs e)
        {
            string r = e.SourceFile + ":" + e.SrcLine.ToString() + " - " + e.ExceptionDetails;
            Console.WriteLine(r);
            throw new Exception(r);
        }

        private static void External_Console_WriteLine(State state)
        {
            Console.WriteLine(state.StackPop());
        }

        private static void External_Console_ReadLine(State state)
        {
            state.StackPush(Console.ReadLine());
        }
    }
}
