//Example loading c lib
using System;

using System.Runtime.InteropServices;

public class Tester
{
        [DllImport("libtest.so", EntryPoint="print")]

        static extern void print(string message);

        public static void Main(string[] args)
        {

                print("Hello World C# => C++");
        }
}
