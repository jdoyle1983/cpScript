using System;
using System.Collections.Generic;
using System.Xml;
using System.IO;
using System.IO.Compression;
using System.Reflection;

namespace libCpScript.Net.Asm
{
	public delegate void UserFunction(State state);
	
	public class State
	{
		internal Stack<int> _CursorStack = new Stack<int>();
		internal Stack<string> _Stack = new Stack<string>();
		internal List<MemoryBlock> _Memory = new List<MemoryBlock>();
		internal List<LabelDef> _Labels = new List<LabelDef>();
		internal List<UserFunction> _UserFunctions = new List<UserFunction>();
		internal Stack<List<string>> _Registers = new Stack<List<string>>();
		internal Stack<List<MemoryBlockHeader>> _Headers = new Stack<List<MemoryBlockHeader>>();
		internal Stack<List<MemoryBlockSetHeader>> _BlockHeaders = new Stack<List<MemoryBlockSetHeader>>();
		
		internal AssemblyToken[] _Tokens = null;
		
		internal int _Offset = -1;

        internal byte[] Compress(byte[] buffer)
        {
            var memoryStream = new MemoryStream();
            using (var gZipStream = new GZipStream(memoryStream, CompressionMode.Compress, true))
                gZipStream.Write(buffer, 0, buffer.Length);
            memoryStream.Position = 0;
            var compressedData = new byte[memoryStream.Length];
            memoryStream.Read(compressedData, 0, compressedData.Length);
            var gZipBuffer = new byte[compressedData.Length + 4];
            Buffer.BlockCopy(compressedData, 0, gZipBuffer, 4, compressedData.Length);
            Buffer.BlockCopy(BitConverter.GetBytes(buffer.Length), 0, gZipBuffer, 0, 4);
            return gZipBuffer;
        }

        internal byte[] Decompress(byte[] gZipBuffer)
        {
            using (var memoryStream = new MemoryStream())
            {
                int dataLength = BitConverter.ToInt32(gZipBuffer, 0);
                memoryStream.Write(gZipBuffer, 4, gZipBuffer.Length - 4);
                var buffer = new byte[dataLength];
                memoryStream.Position = 0;
                using (var gZipStream = new GZipStream(memoryStream, CompressionMode.Decompress))
                {
                    gZipStream.Read(buffer, 0, buffer.Length);
                }
                return buffer;
            }
        }
		
		internal void DoInit()
		{
            //Find Library Calls
            List<AssemblyToken> _NewToks = new List<AssemblyToken>();
            for (int i = 0; i < _Tokens.Length; i++)
            {
                if (_Tokens[i].Tok == Token.Lib && _Tokens[i + 1].Tok == Token.QuotedLiteral)
                {
                    i++;
                    string libFile = _Tokens[i].Val;
                    Assembly a = Assembly.LoadFrom(AppDomain.CurrentDomain.BaseDirectory + System.IO.Path.DirectorySeparatorChar + libFile);
					foreach(var type in a.GetTypes ())
					{
                        if(!type.IsClass || type.IsNotPublic) continue;
                        foreach (var it in type.GetInterfaces())
                        {
                            if(it.FullName != null && it.FullName == "libCpScript.Net.Asm.IScriptLib")
                            {
                                IScriptLib thisLib = Activator.CreateInstance(type) as IScriptLib;
                                thisLib.Init(this);
                            }
                        }
                    }
                }
                else
                    _NewToks.Add(_Tokens[i]);
            }

            _Tokens = _NewToks.ToArray();

			//Find Labels
			for(int i = 0; i < _Tokens.Length; i++)
			{
				if(_Tokens[i].Tok == Token.Label && i + 1 < _Tokens.Length && _Tokens[i + 1].Tok == Token.Literal)
				{
					LabelDef d = new LabelDef(_Tokens[i + 1].Val, i + 2);
					i+=2;
					_Labels.Add(d);
				}
			}
			
			_Registers.Push(new List<string>());
			_Headers.Push(new List<MemoryBlockHeader>());
			_BlockHeaders.Push(new List<MemoryBlockSetHeader>());
		}
		
		public State (string AssemblyScript)
		{
			_Tokens = Parser.Parse(AssemblyScript);
			DoInit ();
		}
		
		#region BinaryFormat
		
		public State (byte[] AssemblyBinary)
		{
			short tShort = 0;
			int tInt = 0;
			
			int shortLen = BitConverter.GetBytes(tShort).Length;
			int intLen = BitConverter.GetBytes(tInt).Length;
			List<AssemblyToken> _Toks = new List<AssemblyToken>();

            byte[] _src = Decompress(AssemblyBinary);
			int currentOffset = 8;
			if(_src[0] == (byte)'C' && _src[1] == (byte)'p' && _src[2] == (byte)'S' && _src[3] == (byte)'c' && _src[4] == (byte)'R' && _src[5] == (byte)'i' && _src[6] == (byte)'P' && _src[7] == (byte)'t')
			{
				while(currentOffset < _src.Length)
				{
					short srcTok = BitConverter.ToInt16(_src, currentOffset);
					currentOffset += shortLen;
                    string val = "";
                    if ((Token)srcTok == Token.Literal || (Token)srcTok == Token.QuotedLiteral || (Token)srcTok == Token.Register || (Token)srcTok == Token.MemoryVar)
                    {
                        int valLen = BitConverter.ToInt32(_src, currentOffset);
                        currentOffset += intLen;
                        val = System.Text.Encoding.UTF8.GetString(_src, currentOffset, valLen);
                        //val = System.Text.Encoding.ASCII.GetString(_src, currentOffset, valLen);
                        currentOffset += valLen;
                    }
					_Toks.Add (new AssemblyToken((Token)srcTok, val));
				}
                _Tokens = _Toks.ToArray();
				DoInit();
			}
		}
		
		public byte[] CompileScript()
		{
			List<byte> _SrcBytes = new List<byte>();
			_SrcBytes.Add((byte)'C');
			_SrcBytes.Add((byte)'p');
			_SrcBytes.Add((byte)'S');
			_SrcBytes.Add((byte)'c');
			_SrcBytes.Add((byte)'R');
			_SrcBytes.Add((byte)'i');
			_SrcBytes.Add((byte)'P');
			_SrcBytes.Add((byte)'t');
			for(int i = 0; i < _Tokens.Length; i++)
			{
				short thisToken = (short)_Tokens[i].Tok;
                byte[] thisTokenb = BitConverter.GetBytes(thisToken);
                _SrcBytes.AddRange(thisTokenb);
                if (_Tokens[i].Tok == Token.Literal || _Tokens[i].Tok == Token.QuotedLiteral || _Tokens[i].Tok == Token.Register || _Tokens[i].Tok == Token.MemoryVar)
                {
                    byte[] thisValb = System.Text.Encoding.UTF8.GetBytes(_Tokens[i].Val);
                    //byte[] thisValb = System.Text.Encoding.ASCII.GetBytes(_Tokens[i].Val);
                    byte[] thisValLengthb = BitConverter.GetBytes(thisValb.Length);
                    _SrcBytes.AddRange(thisValLengthb);
                    _SrcBytes.AddRange(thisValb);
                }
			}
            return Compress(_SrcBytes.ToArray());
		}
		
		#endregion 
		
		public void RegisterFunction(string Name, UserFunction Function, bool StubFunction)
		{
            bool wasFound = false;
            for (int i = 0; i < _Labels.Count; i++)
            {
                if (_Labels[i].Label == Name)
                {
                    wasFound = true;
                    if (_Labels[i].Function != null)
                    {
                        if((_Labels[i].IsStub && !StubFunction))
                            _Labels[i].Function = Function;
                    }
                }
            }
            if(!wasFound)
			    _Labels.Add(new LabelDef(Name, Function, StubFunction));
		}
		
		#region Literal Evaluator
		
		bool ptMemoryBlockVar(string Val)
		{
			if(Val.StartsWith("$"))
				return true;
			return false;
		}
		
		bool ptMemoryBlockVarIndex(string Val)
		{
			if(Val.StartsWith("$") && Val.Contains(":"))
				return true;
			return false;
		}
		
		bool ptMemoryVar(string Val)
		{
			if(Val.StartsWith("%"))
				return true;
			return false;
		}
		
		bool ptRegister(string Val)
		{
			if(Val.StartsWith("@"))
			{
				try
				{
					#pragma warning disable 219
					int tVal = Convert.ToInt32(Val.Replace("@", ""));
					#pragma warning restore 219
					return true;
				}
				catch {}
			}
			return false;
		}
		
		bool ptLabel(string Val)
		{
			foreach(LabelDef d in _Labels)
				if(d.Label == Val)
					return true;
			return false;
		}
		
		bool ptIntegerNonNegative(string Val)
		{
			try
			{
				int v = Convert.ToInt32(Val);
				if(v >= 0)
					return true;
			}
			catch{}
			return false;
		}
		
		bool ptInteger(string Val)
		{
			try
			{
				#pragma warning disable 219
				int v = Convert.ToInt32(Val);
				#pragma warning restore 219
				return true;
			}
			catch{}
			return false;
		}
		
		bool pt1(string Val)
		{
			return ptIntegerNonNegative(Val);
		}
		
		bool pt2(string Val)
		{
			return ptInteger(Val);
		}
		
		bool pt3(string Val)
		{
			return ptLabel(Val);
		}
		
		bool pt4(string Val)
		{
			return ptRegister(Val) || ptMemoryVar(Val) || ptMemoryBlockVarIndex(Val);
		}
		
		bool pt5(string Val)
		{
			return (!ptLabel(Val) && !ptRegister(Val) && !ptMemoryVar(Val));
		}
		
		bool pt8(string Val)
		{
			return !ptLabel(Val);
		}
		
		bool pt9(string Val)
		{
			return ptMemoryVar(Val);
		}
		
		#endregion
		
		#region Register Management
		
		void SetRegister(int id, string Value)
		{
			int targetId = id - 1;
			if(targetId < _Registers.Peek().Count)
				_Registers.Peek()[targetId] = Value;
			else
			{
				while(targetId != _Registers.Peek().Count)
					_Registers.Peek().Add("");
				_Registers.Peek().Add(Value);
			}
		}
		
		string ReadRegister(int id)
		{
			int targetId = id - 1;
			if(targetId < _Registers.Peek().Count)
				return _Registers.Peek()[targetId];
			return "";
		}
		
		#endregion
		
		#region Memory Management
		
		void AllocateMemoryBlockSet(string Id, int Count)
		{
			MemoryBlockSetHeader h = new MemoryBlockSetHeader(Id, Count);
			for(int i = 0; i < Count; i++)
			{
				bool found = false;
				foreach(MemoryBlock m in _Memory)
				{
					if(!m.Used)
					{
						h.IndexOffset[i] = _Memory.IndexOf(m);
						found = true;
					}
				}
				if(!found)
				{
					MemoryBlock mb = new MemoryBlock();
					mb.SetValue("");
					_Memory.Add(mb);
					h.IndexOffset[i] = _Memory.Count - 1;
				}
			}
			_BlockHeaders.Peek().Add(h);
		}
		
		void AllocateMemoryBlock(string Id)
		{
			bool found = false;
			foreach(MemoryBlock m in _Memory)
			{
				if(!m.Used)
				{
					MemoryBlockHeader h = new MemoryBlockHeader(Id, _Memory.IndexOf(m));
					m.SetValue("");
					_Headers.Peek().Add(h);
					found = true;
				}
			}
			
			if(!found)
			{
				MemoryBlock mb = new MemoryBlock();
				mb.SetValue("");
				_Memory.Add(mb);
				MemoryBlockHeader h = new MemoryBlockHeader(Id, _Memory.Count - 1);
				_Headers.Peek().Add(h);
			}		
		}
		
		void ReferenceMemoryBlockSet(string OrigId, string NewId)
		{
			MemoryBlockSetHeader hed = null;
			foreach(MemoryBlockSetHeader h in _BlockHeaders.Peek())
				if(h.Name == NewId)
					hed = h;
			MemoryBlockSetHeader hed2 = null;
			foreach(MemoryBlockSetHeader h in _BlockHeaders.Peek())
				if(h.Name == OrigId)
					hed2 = h;
			if(hed2 != null)
			{
				bool shouldAdd = true;
				if(hed == null)
					hed = new MemoryBlockSetHeader(NewId, hed2.IndexOffset.Count);
				else
					shouldAdd = false;
				for(int i = 0; i < hed2.IndexOffset.Count; i++)
					hed.IndexOffset[i] = hed2.IndexOffset[i];
				
				if(shouldAdd)
					_BlockHeaders.Peek().Add(hed);
			}
		}
		
		void ReferenceMemoryBlock(string OrigId, string NewId)
		{
			MemoryBlockHeader hed = null;
			foreach(MemoryBlockHeader h in _Headers.Peek())
				if(h.Name == NewId)
					hed = h;
			int targetOffset = -1;
			foreach(MemoryBlockHeader h in _Headers.Peek())
				if(h.Name == OrigId)
					targetOffset = h.Offset;
			
			if(hed == null)
			{
				hed = new MemoryBlockHeader(NewId, targetOffset);
				_Headers.Peek().Add(hed);
			}
			else
				hed.Offset = targetOffset;
		}
		
		void SetMemoryBlockSetIndex(string Id, string Value)
		{
			string[] parts = Id.Split(new char[] { ':' });
			string Name = parts[0];
			int Index = Convert.ToInt32(parts[1]);
			foreach(MemoryBlockSetHeader h in _BlockHeaders.Peek())
				if(h.Name == Name)
					_Memory[h.IndexOffset[Index - 1]].Value = Value;
		}
		
		void SetMemoryBlock(string Id, string Value)
		{
			foreach(MemoryBlockHeader h in _Headers.Peek())
				if(h.Name == Id)
					_Memory[h.Offset].Value = Value;
		}
		
		string ReadMemoryBlockSetIndex(string Id)
		{
			string[] parts = Id.Split(new char[] { ':' });
			string Name = parts[0];
			int Index = Convert.ToInt32(parts[1]);
			foreach(MemoryBlockSetHeader h in _BlockHeaders.Peek())
				if(h.Name == Name)
					return _Memory[h.IndexOffset[Index - 1]].Value;
			return "";
		}
		
		string ReadMemoryBlock(string Id)
		{
			foreach(MemoryBlockHeader h in _Headers.Peek())
				if(h.Name == Id)
					return _Memory[h.Offset].Value;
			return "";
		}
		
		void FreeMemoryBlockSet(string Id)
		{
			MemoryBlockSetHeader hed = null;
			foreach(MemoryBlockSetHeader h in _BlockHeaders.Peek())
				if(h.Name == Id)
					hed = h;
			if(hed != null)
			{
				for(int i = 0; i < hed.IndexOffset.Count; i++)
					_Memory[hed.IndexOffset[i]].Free();
			}
		}
		
		void FreeMemoryBlock(string Id)
		{
			foreach(MemoryBlockHeader h in _Headers.Peek())
				if(h.Name == Id)
					_Memory[h.Offset].Free();
		}
		
		#endregion
		
		public bool Iterate()
		{
			if(_Offset >= _Tokens.Length)
				_Offset = -1;
			
			if(_Offset > -1)
			{
			switch(_Tokens[_Offset].Tok)
			{
				case Token.Label: _Offset+= 2; break;

				case Token.PushB:
				{
					_Offset++;
					if(!ptMemoryBlockVar(_Tokens[_Offset].Val))
						throw new Exception("Expecting Memory Block Var");
					foreach(MemoryBlockSetHeader h in _BlockHeaders.Peek())
					{
						if(h.Name == _Tokens[_Offset].Val)
						{
							for(int i = 0; i < h.IndexOffset.Count; i++)
								_Stack.Push(h.IndexOffset[i].ToString());
							_Stack.Push(h.IndexOffset.Count.ToString());
						}
					}
					_Offset++;
				}break;
					
				case Token.PushA:
				{
					_Offset++;
					if(!ptMemoryVar(_Tokens[_Offset].Val))
						throw new Exception("Expecting Memory Var.");
					int targetOffset = -1;
					foreach(MemoryBlockHeader h in _Headers.Peek())
						if(h.Name == _Tokens[_Offset].Val)
							targetOffset = h.Offset;
					if(targetOffset == -1)
						throw new Exception("No Address Found In Memory Var.");
					_Stack.Push(targetOffset.ToString());
					_Offset++;
				}break;
					
				case Token.PopB:
				{
					_Offset++;
					if(!ptMemoryBlockVar(_Tokens[_Offset].Val))
						throw new Exception("Expecting Memory Var");
					MemoryBlockSetHeader hed = null;
					foreach(MemoryBlockSetHeader h in _BlockHeaders.Peek())
						if(h.Name == _Tokens[_Offset].Val)
							hed = h;
					bool shouldAdd = true;
					if(hed == null)
						hed = new MemoryBlockSetHeader(_Tokens[_Offset].Val, 0);
					else
					{
						hed.IndexOffset.Clear();
						shouldAdd = false;
					}
					
					int Count = Convert.ToInt32(_Stack.Pop());
					for(int i = 0; i < Count; i++)
						hed.IndexOffset.Add(-1);
					for(int i = Count - 1; i >= 0; i--)
						hed.IndexOffset[i] = Convert.ToInt32(_Stack.Pop());
					
					if(shouldAdd)
						_BlockHeaders.Peek().Add(hed);
					_Offset++;
				}break;
					
				case Token.PopA:
				{
					_Offset++;
					if(!ptMemoryVar(_Tokens[_Offset].Val))
						throw new Exception("Expecting Memory Var.");
					bool found = false;
					foreach(MemoryBlockHeader h in _Headers.Peek())
					{
						if(h.Name == _Tokens[_Offset].Val)
						{
							h.Offset = Convert.ToInt32(_Stack.Pop());
							found = true;
						}
					}
					
					if(!found)
					{
						MemoryBlockHeader h = new MemoryBlockHeader(_Tokens[_Offset].Val, Convert.ToInt32(_Stack.Pop()));
						_Headers.Peek().Add(h);
					}
					_Offset++;		
				}break;
					
				case Token.Move:
				{
					_Offset++;
					AssemblyToken destTok = _Tokens[_Offset];
					_Offset++;
					AssemblyToken commaTok = _Tokens[_Offset];
					_Offset++;
					AssemblyToken srcTok = _Tokens[_Offset];
					
					if(!pt4(destTok.Val))
						throw new Exception("Invalid Dest Parameter (MOV).");
					if(commaTok.Tok != Token.Comma)
						throw new Exception("Comma Expected.");
					if(!pt8(srcTok.Val))
						throw new Exception("Invalid Source Parameter (MOV).");
					
					string sVal = "";
					if(ptRegister(srcTok.Val))
					{
						int regId = Convert.ToInt32(srcTok.Val.Replace("@",""));
						sVal = ReadRegister(regId);
					}
					else if(ptMemoryVar(srcTok.Val))
						sVal = ReadMemoryBlock(srcTok.Val);
					else if(ptMemoryBlockVarIndex(srcTok.Val))
						sVal = ReadMemoryBlockSetIndex(srcTok.Val);
					else
						sVal = srcTok.Val;
					
					if(ptRegister(destTok.Val))
					{
						int regId = Convert.ToInt32(destTok.Val.Replace("@",""));
						SetRegister(regId, sVal);
					}
					else if(ptMemoryBlockVarIndex(destTok.Val))
						SetMemoryBlockSetIndex(destTok.Val, sVal);    
					else
						SetMemoryBlock(destTok.Val, sVal);
					_Offset++;
				}break;
				
				case Token.Push:
				{
					_Offset++;
					if(!pt8(_Tokens[_Offset].Val))
						throw new Exception("Invalid Parameter (PUSH).");
					if(ptRegister(_Tokens[_Offset].Val))
					{
						int regId = Convert.ToInt32(_Tokens[_Offset].Val.Replace("@",""));
						_Stack.Push(ReadRegister(regId));
					}
					else if(ptMemoryVar(_Tokens[_Offset].Val))
						_Stack.Push(ReadMemoryBlock(_Tokens[_Offset].Val));
					else if(ptMemoryBlockVarIndex(_Tokens[_Offset].Val))
						_Stack.Push(ReadMemoryBlockSetIndex(_Tokens[_Offset].Val));
					else
						_Stack.Push(_Tokens[_Offset].Val);
					_Offset++;
				}break;
				case Token.Pop:
				{
					_Offset++;
					if(!pt4(_Tokens[_Offset].Val))
						throw new Exception("Invalid Parameter (POP).");
					if(ptRegister(_Tokens[_Offset].Val))
					{
						int regId = Convert.ToInt32(_Tokens[_Offset].Val.Replace("@",""));
						SetRegister(regId, _Stack.Pop());
					}
					else if(ptMemoryBlockVarIndex(_Tokens[_Offset].Val))
						SetMemoryBlockSetIndex(_Tokens[_Offset].Val, _Stack.Pop());
					else
						SetMemoryBlock(_Tokens[_Offset].Val, _Stack.Pop());
					_Offset++;
				}break;
				case Token.Add: case Token.Sub:
				case Token.Mul: case Token.Div:
				case Token.Mod:
				{
					string s1 = _Stack.Pop();
					string s2 = _Stack.Pop();
					double v2 = 0;
					double v1 = 0;
					try
					{
						v2 = Convert.ToDouble(s2);
						v1 = Convert.ToDouble(s1);
					}
					catch{ throw new Exception("Invalid Parameter (ADD,SUB,MUL,DIV,MOD)"); }
					double v3 = 0;
					switch(_Tokens[_Offset].Tok)
					{
						case Token.Add: v3 = v2 + v1; break;
						case Token.Sub: v3 = v2 - v1; break;
						case Token.Mul: v3 = v2 * v1; break;
						case Token.Div: v3 = v2 / v1; break;
						case Token.Mod: v3 = v2 % v1; break;
					}
					_Stack.Push(v3.ToString());
					_Offset++;
				}break;
				case Token.Cat:
				{
					string v2 = _Stack.Pop();
					string v1 = _Stack.Pop();
					_Stack.Push(v1 + v2);
					_Offset++;
				}break;
                case Token.Neg:
                {
                    string s1 = _Stack.Pop();
                    double v1 = 0;
                    try
                    {
                        v1 = Convert.ToDouble(s1);
                    }
                    catch { throw new Exception("Invalid Parameter (NEG)"); }
                    v1 = v1 * -1;
                    _Stack.Push(v1.ToString());
                    _Offset++;
                } break;
				case Token.JumpOffset:
				{
					_Registers.Push(new List<string>());
					_Headers.Push(new List<MemoryBlockHeader>());
					_Offset++;
					string val = _Tokens[_Offset].Val;
					_Offset++;
					_CursorStack.Push(_Offset);
					int newOffset = -1;
					
					if(ptRegister(val))
					{
						try
						{
							newOffset = Convert.ToInt32(ReadRegister(Convert.ToInt32(val.Replace("@",""))));
						} catch { throw new Exception("Invalid Parameter."); }
					}
					else if(ptMemoryVar(val))
					{
						try
						{
							newOffset = Convert.ToInt32(ReadMemoryBlock(val));
						} catch { throw new Exception("Invalid Parameter."); }
					}
					else
					{
						try
						{
							newOffset = Convert.ToInt32(val);
						} catch { throw new Exception("Invalid Parameter."); }
					}
					
					_Offset = newOffset;
				}break;
				case Token.Jump:
				{
					_Registers.Push(new List<string>());
					_Headers.Push(new List<MemoryBlockHeader>());
					_Offset++;
					string labelTitle = _Tokens[_Offset].Val;
					_Offset++;
					_CursorStack.Push(_Offset);
					LabelDef l = null;
					foreach(LabelDef d in _Labels)
						if(d.Label == labelTitle)
							l = d;
					if(l == null)
						throw new Exception("Label Not Found.");
					if(l.Function != null)
					{
						l.Function(this);
						_CursorStack.Pop();
						_Registers.Pop();
						_Headers.Pop();
					}
					else
						_Offset = l.Offset;
				}break;
				case Token.Return:
				{
					_Headers.Pop();
					if(_Headers.Count < 1)
						_Offset = -1;
					else
					{
						_Registers.Pop();
						_Offset = _CursorStack.Pop();
					}
				}break;
				case Token.LongJumpOffset:
				{
					_Offset++;
					string val = _Tokens[_Offset].Val;
					_Offset++;
					_CursorStack.Push(_Offset);
					int newOffset = -1;
					
					if(ptRegister(val))
					{
						try
						{
							newOffset = Convert.ToInt32(ReadRegister(Convert.ToInt32(val.Replace("@",""))));
						} catch { throw new Exception("Invalid Parameter."); }
					}
					else if(ptMemoryVar(val))
					{
						try
						{
							newOffset = Convert.ToInt32(ReadMemoryBlock(val));
						} catch { throw new Exception("Invalid Parameter."); }
					}
					else
					{
						try
						{
							newOffset = Convert.ToInt32(val);
						} catch { throw new Exception("Invalid Parameter."); }
					}
					
					_Offset = newOffset;
				}break;
				case Token.LongJump:
				{
					_Offset++;
					string labelTitle = _Tokens[_Offset].Val;
					_Offset++;
					LabelDef l = null;
					foreach(LabelDef d in _Labels)
						if(d.Label == labelTitle)
							l = d;
					if(l == null)
						throw new Exception("Label Not Found.");
					if(l.Function != null)
						l.Function(this);
					else
						_Offset = l.Offset;
				}break;
				case Token.CmpI:
				{
					string v = _Stack.Pop();
					bool b = false;
					try
					{
						b = Convert.ToBoolean(v);
					}
					catch { throw new Exception("Invalid Parameter"); }
					_Stack.Push((!b).ToString());
				}break;
				case Token.CmpA: case Token.CmpE:
				case Token.CmpG: case Token.CmpGE:
				case Token.CmpL: case Token.CmpLE:
				case Token.CmpN: case Token.CmpO:
				{
					string v2 = _Stack.Pop();
					string v1 = _Stack.Pop();
					
					switch(_Tokens[_Offset].Tok)
					{
						case Token.CmpA: case Token.CmpO:
						{
							bool b1 = false;
							bool b2 = false;
							try
							{
								b1 = Convert.ToBoolean(v1);
								b2 = Convert.ToBoolean(v2);
							}
							catch { throw new Exception("Invalid Parameters (C Operation)"); }
							if(_Tokens[_Offset].Tok == Token.CmpA)
								_Stack.Push((b1 && b2).ToString());
							else
								_Stack.Push((b1 || b2).ToString());
						}break;
						case Token.CmpE: case Token.CmpN:
						{
							if(_Tokens[_Offset].Tok == Token.CmpE)
								_Stack.Push((v1 == v2).ToString());
							else
								_Stack.Push((v1 != v2).ToString());
						}break;
						default:
						{
							double d1 = 0;
							double d2 = 0;
							try
							{
								d1 = Convert.ToDouble(v1);
								d2 = Convert.ToDouble(v2);
							}
							catch { throw new Exception("Invalid Paraters (C Operation)"); }
						
							switch(_Tokens[_Offset].Tok)
							{
								case Token.CmpG: _Stack.Push((d1 > d2).ToString()); break;
								case Token.CmpGE: _Stack.Push((d1 >= d2).ToString()); break;
								case Token.CmpL: _Stack.Push((d1 < d2).ToString()); break;
								case Token.CmpLE: _Stack.Push((d1 <= d2).ToString()); break;
							}
								
						}break;
					}
					_Offset++;
				}break;
				case Token.JumpE: case Token.JumpN:
				case Token.JumpG: case Token.JumpGE:
				case Token.JumpL: case Token.JumpLE:
				{
					Token cmpTok = _Tokens[_Offset].Tok;
					_Offset++;
					AssemblyToken p1 = _Tokens[_Offset];
					_Offset++;
					AssemblyToken c1 = _Tokens[_Offset];
					_Offset++;
					AssemblyToken p2 = _Tokens[_Offset];
					_Offset++;
					AssemblyToken c2 = _Tokens[_Offset];
					_Offset++;
					AssemblyToken p3 = _Tokens[_Offset];
					
					if(c1.Tok != Token.Comma || c2.Tok != Token.Comma)
						throw new Exception("Commas Expected.");
					
					if(!pt8(p1.Val) || !pt8(p2.Val) || !pt3(p3.Val))
						throw new Exception("Invalid Parameters.");
					
					string val1 = "";
					string val2 = "";
					int newOffset = -1;
					
					if(ptRegister(p1.Val))
						val1 = ReadRegister(Convert.ToInt32(p1.Val.Replace("@","")));
					else if(ptMemoryVar(p1.Val))
						val1 = ReadMemoryBlock(p1.Val);
					else if(ptMemoryBlockVarIndex(p1.Val))
						val1 = ReadMemoryBlockSetIndex(p1.Val);
					else
						val1 = p1.Val;
					
					if(ptRegister(p2.Val))
						val2 = ReadRegister(Convert.ToInt32(p2.Val.Replace("@","")));
					else if(ptMemoryVar(p2.Val))
						val2 = ReadMemoryBlock(p2.Val);
					else if(ptMemoryBlockVarIndex(p2.Val))
						val2 = ReadMemoryBlockSetIndex(p2.Val);
					else
						val2 = p2.Val;
					
					foreach(LabelDef l in _Labels)
						if(l.Label == p3.Val)
							newOffset = l.Offset;
					
					if(newOffset == -1)
						throw new Exception("Label Not Found");
					
					bool didEval = false;
					switch(cmpTok)
					{
					case Token.JumpE: if(val1 == val2) didEval = true; break;
					case Token.JumpN: if(val1 != val2) didEval = true; break;
					case Token.JumpG: if(Convert.ToDouble(val1) > Convert.ToDouble(val2)) didEval = true; break;
					case Token.JumpGE: if(Convert.ToDouble(val1) >= Convert.ToDouble(val2)) didEval = true; break;
					case Token.JumpL: if(Convert.ToDouble(val1) < Convert.ToDouble(val2)) didEval = true; break;
					case Token.JumpLE: if(Convert.ToDouble(val1) <= Convert.ToDouble(val2)) didEval = true; break;
					}
					if(didEval)
						_Offset = newOffset;
					else
						_Offset++;
				}break;
				case Token.Alloc:
				{
					_Offset++;
					AllocateMemoryBlock(_Tokens[_Offset].Val);
					_Offset++;
				}break;
				case Token.AllocBlockSet:
				{
					_Offset++;
					string BlockName = _Tokens[_Offset].Val;
					_Offset++;
					_Offset++;
					int Count = Convert.ToInt32(_Tokens[_Offset].Val);
					AllocateMemoryBlockSet(BlockName, Count);
					_Offset++;
				}break;
				case Token.Ref:
				{
					_Offset++;
					AssemblyToken src = _Tokens[_Offset];
					_Offset++;
					#pragma warning disable 219
					AssemblyToken c = _Tokens[_Offset];
					#pragma warning restore 219
					_Offset++;
					AssemblyToken dest = _Tokens[_Offset];
					ReferenceMemoryBlock(src.Val, dest.Val);
					_Offset++;
				}break;
				case Token.RefBlockSet:
				{
					_Offset++;
					AssemblyToken src = _Tokens[_Offset];
					_Offset++;
					_Offset++;
					AssemblyToken dest = _Tokens[_Offset];
					ReferenceMemoryBlockSet(src.Val, dest.Val);
					_Offset++;
				}break;
				case Token.Dealloc:
				{
					_Offset++;
					FreeMemoryBlock(_Tokens[_Offset].Val);
					_Offset++;
				}break;
				case Token.DeallocBlockSet:
				{
					_Offset++;
					FreeMemoryBlockSet(_Tokens[_Offset].Val);
					_Offset++;
				}break;
				case Token.EndOfExec:
				{
					_Offset = -1;
				}break;
			}
			}
			
			if(_Offset > -1)
				return true;
			return false;
		}
		
		public void RunFromMethod(string Name)
		{
			LoadMethod(Name);
			while(Iterate());
		}
		
		public void LoadMethod(string Name)
		{
			_Offset = -1;
			for(int i = 0; i < _Labels.Count && _Offset == -1; i++)
				if(_Labels[i].Label == Name && _Labels[i].Offset >= 0)
					_Offset = _Labels[i].Offset;
			if(_Offset == -1)
				throw new Exception("Method Not Found.");
		}
		
		public void ResetState()
		{
			_CursorStack = new Stack<int>();
			_Stack = new Stack<string>();
			_Memory = new List<MemoryBlock>();
			_Offset = -1;
			_Registers = new Stack<List<string>>();
			_Registers.Push(new List<string>());
		}
		
		#region User Stack Functions
		
		public int PopInt()
		{
			return Convert.ToInt32(_Stack.Pop());
		}
		
		public double PopDouble()
		{
			return Convert.ToDouble(_Stack.Pop());
		}
		
		public bool PopBool()
		{
			return Convert.ToBoolean(_Stack.Pop());
		}
		
		public string PopString()
		{
			return _Stack.Pop();
		}
		
		public void PushInt(int v)
		{
			_Stack.Push(v.ToString());
		}
		
		public void PushDouble(double v)
		{
			_Stack.Push(v.ToString());
		}
		
		public void PushBool(bool v)
		{
			_Stack.Push(v.ToString());
		}
		
		public void PushString(string v)
		{
			_Stack.Push(v);
		}
		
		#endregion
		
		public string SaveState()
		{
			XmlDocument d = new XmlDocument();
			XmlNode RootNode = d.CreateNode(XmlNodeType.Element, "cpState", "");
			d.AppendChild(RootNode);
			
			XmlNode cursorStackNode = d.CreateNode(XmlNodeType.Element, "CursorStack", "");
			RootNode.AppendChild(cursorStackNode);
			int[] cursorStackArray = _CursorStack.ToArray();
			for(int i = 0; i < cursorStackArray.Length; i++)
			{
				XmlNode n = d.CreateNode(XmlNodeType.Element, "Stack", "");
				n.InnerText = cursorStackArray[i].ToString();
				cursorStackNode.AppendChild(n);
			}
			
			XmlNode stackNode = d.CreateNode(XmlNodeType.Element, "Stack", "");
			RootNode.AppendChild(stackNode);
			string[] stackArray = _Stack.ToArray();
			for(int i = 0; i < stackArray.Length; i++)
			{
				XmlNode n = d.CreateNode(XmlNodeType.Element, "Stack", "");
				n.InnerText = stackArray[i];
				stackNode.AppendChild(n);
			}
			
			XmlNode memoryNode = d.CreateNode(XmlNodeType.Element, "Memory", "");
			RootNode.AppendChild(memoryNode);
			MemoryBlock[] blocksArray = _Memory.ToArray();
			for(int i = 0; i < blocksArray.Length; i++)
			{
				XmlNode n = d.CreateNode(XmlNodeType.Element, "Mem", "");
				XmlAttribute usedAtt = d.CreateAttribute("Used");
				usedAtt.Value = blocksArray[i].Used.ToString();
				n.Attributes.Append(usedAtt);
				n.InnerText = blocksArray[i].Value;
				memoryNode.AppendChild(n);
			}

            return "";
			
		}
		
		public void RestoreState(string savedState)
		{
		}
	}
}

