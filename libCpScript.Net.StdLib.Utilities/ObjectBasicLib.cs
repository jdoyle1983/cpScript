using System;
using System.Threading;
using System.Collections;
using System.Collections.Generic;
using libCpScript.Net.Asm;

namespace libCpScript.Net.StdLib.Utilities
{
	internal class ArrayObject
	{
		public int Id = -1;
		public List<string> Items = new List<string>();
	}
	
	public class UtilitiesLib : IScriptLib
	{
		internal static Mutex _ArrayLocker = new Mutex();
		internal static List<ArrayObject> _Arrays = new List<ArrayObject>();
		internal static int _NextArrayId = 1;
		
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
			Script.RegisterFunction("__Utilities__Array__New", UtilitiesLib.ArrayNew, false);
			Script.RegisterFunction("__Utilities__Array__Free", UtilitiesLib.ArrayFree, false);
			Script.RegisterFunction("__Utilities__Array__Count", UtilitiesLib.ArrayItemCount, false);
			Script.RegisterFunction("__Utilities__Array__SetItem", UtilitiesLib.ArraySetItem, false);
			Script.RegisterFunction("__Utilities__Array__GetItem", UtilitiesLib.ArrayGetItem, false);
			/*
            Script.RegisterFunction("__StdLib__Math__Abs", MathLib.Abs, false);
            Script.RegisterFunction("__StdLib__Math__Floor", MathLib.Floor, false);
            Script.RegisterFunction("__StdLib__Math__Ceiling", MathLib.Ceiling, false);
            */
        }

        #endregion
		
		#region Array Members
		
		public static void ArrayNew(State state)
        {
			int Size = state.PopInt();
			ArrayObject obj = new ArrayObject();
			for(int i = 0; i < Size; i++)
				obj.Items.Add("");
			_ArrayLocker.WaitOne();
			obj.Id = _NextArrayId;
			_NextArrayId++;
			_Arrays.Add(obj);
			_ArrayLocker.ReleaseMutex();
			state.PushInt(obj.Id);
		}
		
		private static ArrayObject GetArrayById(int Id)
		{
			ArrayObject rValue = null;
			_ArrayLocker.WaitOne();
			foreach(ArrayObject obj in _Arrays)
			{
				if(obj.Id == Id)
				{
					rValue = obj;
					break;
				}
			}
			_ArrayLocker.ReleaseMutex();
			return rValue;
		}
		
		public static void ArraySetItem(State state)
		{	
			int ArrayIndex = state.PopInt();
			string ArrayValue = state.PopString();
			int ArrayId = state.PopInt();

			ArrayObject obj = GetArrayById(ArrayId);
			if(ArrayIndex >= 0 && ArrayIndex <= obj.Items.Count)
			{
				_ArrayLocker.WaitOne();
				obj.Items[ArrayIndex] = ArrayValue;
				_ArrayLocker.ReleaseMutex();
			}
		}
		
		public static void ArrayGetItem(State state)
		{
			int ArrayIndex = state.PopInt();
			int ArrayId = state.PopInt();
			string rValue = "";
			ArrayObject obj = GetArrayById(ArrayId);
			if(ArrayIndex >= 0 && ArrayIndex < obj.Items.Count)
				rValue = obj.Items[ArrayIndex];
			state.PushString(rValue);
		}
		
		public static void ArrayItemCount(State state)
		{
			int ArrayId = state.PopInt ();
			ArrayObject obj = GetArrayById(ArrayId);
			state.PushInt(obj.Items.Count);
		}
		
		public static void ArrayFree(State state)
		{
			int ArrayId = state.PopInt();
			_ArrayLocker.WaitOne();
			ArrayObject obj = null;
			int ArrayIdx = -1;
			for(int i = 0; i < _Arrays.Count; i++)
			{
				if(_Arrays[i].Id == ArrayId)
				{
					obj = _Arrays[i];
					ArrayIdx = i;
					i = _Arrays.Count;
				}
			}
			
			if(obj != null)
				_Arrays.RemoveAt(ArrayIdx);
			_ArrayLocker.ReleaseMutex();
		}
		
		#endregion
	}
}

