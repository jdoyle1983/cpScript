using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Threading;
using libCpScript.Net.Asm;

namespace libCpScript.Net.StdLib
{
    public class ioObject
    {
        public StreamReader _Reader;
        public StreamWriter _Writer;
        public BinaryReader _BReader;
        public BinaryWriter _BWriter;
        public FileStream _Stream;
        public int _Id;
        public bool _BinaryMode;

        public ioObject()
        {
            _Reader = null;
            _Writer = null;
            _BReader = null;
            _BWriter = null;
            _Stream = null;
            _Id = -1;
            _BinaryMode = false;
        }
    }

    public class FileIO : IScriptLib
    {
        public static Mutex _ioLocker = new Mutex();
        public static List<ioObject> _objects = new List<ioObject>();

        internal static int NextFileHandle = 1;

        #region IScriptLib Members

        public string Author()
        {
            return "Jason Doyle";
        }

        public string Version()
        {
            return "1.0.0.0";
        }

        public void Init(State Script)
        {
            Script.RegisterFunction("__StdLib__FileIO__Open", FileIO.OpenFile, false);
            Script.RegisterFunction("__StdLib__FileIO__Close", FileIO.CloseFile, false);
            Script.RegisterFunction("__StdLib__FileIO__ReadLine", FileIO.FileReadLine, false);
            Script.RegisterFunction("__StdLib__FileIO__WriteLine", FileIO.FileWriteLine, false);
            Script.RegisterFunction("__StdLib__FileIO__Eof", FileIO.FileEof, false);
            Script.RegisterFunction("__StdLib__FileIO__ReadBool", FileIO.FileReadBool, false);
            Script.RegisterFunction("__StdLib__FileIO__ReadDouble", FileIO.FileReadDouble, false);
            Script.RegisterFunction("__StdLib__FileIO__ReadInt", FileIO.FileReadInt, false);
            Script.RegisterFunction("__StdLib__FileIO__ReadString", FileIO.FileReadString, false);
            Script.RegisterFunction("__StdLib__FileIO__WriteBool", FileIO.FileWriteBool, false);
            Script.RegisterFunction("__StdLib__FileIO__WriteDouble", FileIO.FileWriteDouble, false);
            Script.RegisterFunction("__StdLib__FileIO__WriteInt", FileIO.FileWriteInt, false);
            Script.RegisterFunction("__StdLib__FileIO__WriteString", FileIO.FileWriteString, false);
        }

        #endregion

        public static void OpenFile(State state)
        {
            string filePath = state.PopString();
            string readMode = state.PopString();

            ioObject obj = new ioObject();
            if (readMode.ToLower() == "r" || readMode.ToLower() == "rb")
            {
                obj._Stream = System.IO.File.OpenRead(filePath);
                obj._Stream.Seek(0, SeekOrigin.Begin);
            }
            else if (readMode.ToLower() == "w" || readMode.ToLower() == "wb")
                obj._Stream = System.IO.File.OpenWrite(filePath);

            if (readMode.ToLower().Contains("b"))
                obj._BinaryMode = true;

            if (obj._Stream.CanRead)
            {
                if(!obj._BinaryMode)
                    obj._Reader = new StreamReader(obj._Stream);
                else
                    obj._BReader = new BinaryReader(obj._Stream);
            }
            if (obj._Stream.CanWrite)
            {
                if(!obj._BinaryMode)
                    obj._Writer = new StreamWriter(obj._Stream);
                else
                    obj._BWriter = new BinaryWriter(obj._Stream);
            }
            _ioLocker.WaitOne();
            NextFileHandle++;
            obj._Id = NextFileHandle;
            _objects.Add(obj);
            _ioLocker.ReleaseMutex();

            state.PushInt(obj._Id);
        }

        private static ioObject GetObj(int objectId)
        {
            ioObject r = null;
            _ioLocker.WaitOne();
            for (int i = 0; i < _objects.Count && r == null; i++)
                if (_objects[i]._Id == objectId)
                    r = _objects[i];
            _ioLocker.ReleaseMutex();
            return r;
        }

        public static void CloseFile(State state)
        {
            int objectId = state.PopInt();
            ioObject obj = null;
            _ioLocker.WaitOne();
            for (int i = 0; i < _objects.Count && obj == null; i++)
            {
                if (_objects[i]._Id == objectId)
                {
                    obj = _objects[i];
                    _objects.RemoveAt(i);
                }
            }
            _ioLocker.ReleaseMutex();

            if (obj != null)
            {
                obj._Stream.Flush();
                obj._Stream.Close();
            }
        }

        public static void FileReadLine(State state)
        {
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            string readLine = "";
            if (obj != null)
            {
                readLine = obj._Reader.ReadLine();
            }
            state.PushString(readLine);
        }

        public static void FileWriteLine(State state)
        {
            string toWrite = state.PopString();
            int objectId = state.PopInt();

            ioObject obj = GetObj(objectId);
            if (obj != null)
            {
                obj._Writer.WriteLine(toWrite);
                obj._Writer.Flush();
            }
        }

        public static void FileEof(State state)
        {
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            bool rValue = true;
            if (obj != null)
            {
                FileStream stream = obj._Stream as FileStream;
                if (stream.Position >= stream.Length)
                    rValue = true;
                else
                    rValue = false;
            }
            state.PushBool(rValue);
        }

        public static void FileReadInt(State state)
        {
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            int read = 0;
            if (obj != null)
                read = obj._BReader.ReadInt32();
            state.PushInt(read);
        }

        public static void FileWriteInt(State state)
        {
            int toWrite = state.PopInt();
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            if (obj != null)
                obj._BWriter.Write(toWrite);
        }

        public static void FileReadDouble(State state)
        {
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            double read = 0.0;
            if (obj != null)
                read = obj._BReader.ReadDouble();
            state.PushDouble(read);
        }

        public static void FileWriteDouble(State state)
        {
            double toWrite = state.PopDouble();
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            if (obj != null)
                obj._BWriter.Write(toWrite);
        }

        public static void FileReadBool(State state)
        {
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            bool read = false;
            if (obj != null)
                read = obj._BReader.ReadBoolean();
            state.PushBool(read);
        }

        public static void FileWriteBool(State state)
        {
            bool toWrite = state.PopBool();
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            if (obj != null)
                obj._BWriter.Write(toWrite);
        }

        public static void FileReadString(State state)
        {
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            string read = "";
            if (obj != null)
                read = obj._BReader.ReadString();
            state.PushString(read);
        }

        public static void FileWriteString(State state)
        {
            string toWrite = state.PopString();
            int objectId = state.PopInt();
            ioObject obj = GetObj(objectId);
            if (obj != null)
                obj._BWriter.Write(toWrite);
        }
    }
}
