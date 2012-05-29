/*
This file is part of libCpScript.

    libCpScript is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libCpScript is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with libCpScript.  If not, see <http://www.gnu.org/licenses/>.
	
	
	libCpScript
	Original Author: Jason Doyle (jdoyle1983@gmail.com)
*/

using System;
using System.Collections.Generic;

namespace libCpScript.Net.ObjectBasic
{
	
	public class ClassDef
	{
		public string Name = "";
		public ClassDef Extends = null;
		public List<string> Properties = new List<string>();
		public List<Function> Methods = new List<Function>();
		
		public string GetFirstMethodLabel(string Name)
		{
			foreach(Function d in Methods)
				if(d._Name == Name)
					return "_" + Name + "_" + d._Name;
			if(Extends != null)
				return Extends.GetFirstMethodLabel(Name);
			
			return "";
		}

        internal ClassConversion[] GetStaticMethods()
        {
            List<ClassConversion> Results = new List<ClassConversion>();
            foreach (Function f in Methods)
            {
                if (f._IsStatic)
                {
                    ClassConversion cnv = new ClassConversion();
                    cnv.input = Name + "." + f._Name;
                    cnv.output = "_" + Name + "_" + f._Name;
                    Results.Add(cnv);
                }
            }
            return Results.ToArray();
        }
		
		internal ClassConversion[] GetInstanceMethods()
		{
			List<ClassConversion> Results = new List<ClassConversion>();
			foreach(Function f in Methods)
			{
                if (!f._IsStatic)
                {
                    ClassConversion cnv = new ClassConversion();
                    cnv.input = f._Name;
                    cnv.output = "_" + Name + "_" + f._Name;
                    Results.Add(cnv);
                }
			}
			
			if(Extends != null)
			{
				ClassConversion[] parentResults = Extends.GetInstanceMethods();
				foreach(ClassConversion cnv in parentResults)
				{
					bool found = false;
					foreach(ClassConversion cnv2 in Results)
						if(cnv2.input == cnv.input)
							found = true;
					
					if(!found)
						Results.Add(cnv);
				}
			}
			
			return Results.ToArray();
		}
		
		public ClassDef ()
		{
		}
		
		internal static void RecurseGetProps(ref ClassDef def, ref ClassDef exDef)
		{
			if(exDef.Extends != null)
				RecurseGetProps(ref def, ref exDef.Extends);
			for(int i = 0; i < exDef.Properties.Count; i++)
				if(!def.Properties.Contains(exDef.Properties[i]))
					def.Properties.Add(exDef.Properties[i]);
		}
		
		public static ClassDef[] ParseClassDefs(CodeBlock[] Blocks)
		{
			List<ClassDef> defs = new List<ClassDef>();
			
			for(int i = 0; i < Blocks.Length; i++)
			{
				if(Blocks[i].Tokens[0].Type == TokenType.ExClass)
				{
					ClassDef c = new ClassDef();
					if(Blocks[i].Tokens.Length == 4)
					{
						string exClassName = Blocks[i].Tokens[3].Value;
						ClassDef exClass = null;
						for(int r = 0; r < defs.Count; r++)
							if(defs[r].Name == exClassName)
								exClass = defs[r];
						if(exClass == null)
							throw new Exception("Base Class Not Found.");
						RecurseGetProps(ref c, ref exClass);
						c.Extends = exClass;
					}
					
					c.Name = Blocks[i].Tokens[1].Value;
					
					while(Blocks[i].Tokens[0].Type != TokenType.ExEndClass)
					{
						if(Blocks[i].Tokens[0].Type == TokenType.ExProperty)
							c.Properties.Add(Blocks[i].Tokens[1].Value);
						else if(Blocks[i].Tokens[0].Type == TokenType.ExMethod)
						{
							int tIdx = 1;
							Function mdef = new Function();
							if(Blocks[i].Tokens[tIdx].Type == TokenType.ExOverride)
							{
								mdef._IsOverride = true;
								tIdx++;
							}
                            else if (Blocks[i].Tokens[tIdx].Type == TokenType.ExStatic)
                            {
                                mdef._IsStatic = true;
                                tIdx++;
                            }
							mdef._Name = Blocks[i].Tokens[tIdx].Value;
                            List<FunctionParam> Parameters = new List<FunctionParam>();
							tIdx++;
							while(Blocks[i].Tokens[tIdx].Type != TokenType.CloseParen)
							{
                                if (Blocks[i].Tokens[tIdx].Type == TokenType.ExTypeHint)
                                {
                                    Parameters[Parameters.Count - 1].isClassVar = true;
                                    Parameters[Parameters.Count - 1].ClassName = Blocks[i].Tokens[tIdx].Value;
                                }
                                else if (Blocks[i].Tokens[tIdx].Type != TokenType.OpenParen && Blocks[i].Tokens[tIdx].Type != TokenType.Comma)
                                {
                                    FunctionParam p = new FunctionParam();
                                    p.isClassVar = false;
                                    p.VarName = Blocks[i].Tokens[tIdx].Value;
                                    Parameters.Add(p);
                                }
								tIdx++;
							}
							Parameters.Reverse();
							mdef._Parameters = Parameters.ToArray();
							
							i++;
							List<CodeBlock> mBlocks = new List<CodeBlock>();
							while(Blocks[i].Tokens[0].Type != TokenType.ExEndMethod)
							{
								mBlocks.Add(Blocks[i]);
								i++;
							}
							mdef._Blocks = mBlocks.ToArray();
							c.Methods.Add(mdef);
						}
						i++;
					}
					defs.Add(c);
				}
			}
			
			return defs.ToArray();
		}
	}
}

