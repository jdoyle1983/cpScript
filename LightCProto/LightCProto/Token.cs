﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace LightCProto
{
    public enum TokenType
    {
        ttINVALID = 0,

        OpAdd,
        OpSub,
        OpMul,
        OpDiv,
        OpMod,
        OpCat,
        OpNot,
        OpAnd,
        OpOr,
        OpNeg,
        OpEqualTo,
        OpNotEqualTo,
        OpGreaterThan,
        OpGreaterThanOrEqualTo,
        OpLessThan,
        OpLessThanOrEqualTo,
        OpenParen,
        CloseParen,
        ScopeIn,
        ScopeOut,
        OpenBracket,
        CloseBracket,
        Comma,
        Period,
        Assignment,
        Literal,
        QuotedLiteral,
        Or,
        And,
        Colon,
        SemiColon,
        Eol,


        //Extended Types

        ExExtern,

        ExReturn,

        ExVar,
        ExStruct,

        ExIf,
        ExElseIf,
        ExElse,

        ExFor,
        ExWhile,

        ExClass,
        ExOverride,
        ExStatic,

        ExPPInclude,
        ExPPDefine,
        ExPPIfDef,
        ExPPIfNDef,
        ExPPElseIfDef,
        ExPPElseIfNDef,
        ExPPElseDef,
        ExPPEndDef,
    }

    public class Token
    {
        public TokenType Type { get; set; }
        public string Value { get; set; }

        public Token(TokenType _type, string _value)
        {
            Type = _type;
            Value = _value;
        }
    }

    public class Tokenizer
    {
        private static Token LastToken
        {
            get { if (Toks.Count > 0) return Toks[Toks.Count - 1]; return new Token(TokenType.ttINVALID, ""); }
        }

        private static void ReplaceLastToken(Token t)
        {
            Toks[Toks.Count - 1] = t;
        }

        private static List<Token> Toks;

        public static Token[] Tokenize(string Input)
        {
            char[] cToks = { '+', '-', '*', '/', '%', '(', ')', '{', '}', '[', ']', '>', '<', ',', '.', '!', '=', '\"', '\'', '&', '|', ' ', '\n', '\r', '\t', ':', ';' };


            Toks = new List<Token>();

            bool inQuotes = false;
            string quoteString = "";
            string quoteChar = "\"";

            foreach (string t in Input.SplitAndKeep(cToks))
            {
                if (!inQuotes)
                {
                    switch (t)
                    {
                        case "\n": Toks.Add(new Token(TokenType.Eol, "\n")); break;
                        case "+": Toks.Add(new Token(TokenType.OpAdd, t)); break;
                        case "-": Toks.Add(new Token(TokenType.OpSub, t)); break;
                        case "*": Toks.Add(new Token(TokenType.OpMul, t)); break;
                        case "/": Toks.Add(new Token(TokenType.OpDiv, t)); break;
                        case "%": Toks.Add(new Token(TokenType.OpMod, t)); break;
                        case "(": Toks.Add(new Token(TokenType.OpenParen, t)); break;
                        case ")": Toks.Add(new Token(TokenType.CloseParen, t)); break;
                        case "{": Toks.Add(new Token(TokenType.ScopeIn, t)); break;
                        case "}": Toks.Add(new Token(TokenType.ScopeOut, t)); break;
                        case "[": Toks.Add(new Token(TokenType.OpenBracket, t)); break;
                        case "]": Toks.Add(new Token(TokenType.CloseBracket, t)); break;
                        case ">": Toks.Add(new Token(TokenType.OpGreaterThan, t)); break;
                        case "<": Toks.Add(new Token(TokenType.OpLessThan, t)); break;
                        case ",": Toks.Add(new Token(TokenType.Comma, t)); break;
                        case "!": Toks.Add(new Token(TokenType.OpNot, t)); break;
                        case ":": Toks.Add(new Token(TokenType.Colon, t)); break;
                        case ";": Toks.Add(new Token(TokenType.SemiColon, t)); break;
                        case ".": Toks.Add(new Token(TokenType.Period, t)); break;
                        case "=":
                            {
                                if (LastToken != null)
                                {
                                    if (LastToken.Type == TokenType.OpGreaterThan)
                                        ReplaceLastToken(new Token(TokenType.OpGreaterThanOrEqualTo, ">="));
                                    else if (LastToken.Type == TokenType.OpLessThan)
                                        ReplaceLastToken(new Token(TokenType.OpLessThanOrEqualTo, "<="));
                                    else if (LastToken.Type == TokenType.OpNot)
                                        ReplaceLastToken(new Token(TokenType.OpNotEqualTo, "!="));
                                    else if (LastToken.Type == TokenType.Assignment)
                                        ReplaceLastToken(new Token(TokenType.OpEqualTo, "=="));
                                    else
                                        Toks.Add(new Token(TokenType.Assignment, t));
                                }
                                else
                                    Toks.Add(new Token(TokenType.Assignment, t));
                            } break;
                        case "&":
                            {
                                if (LastToken != null && LastToken.Type == TokenType.And)
                                    ReplaceLastToken(new Token(TokenType.OpAnd, "&&"));
                                else
                                    Toks.Add(new Token(TokenType.And, "&"));
                            } break;
                        case "|":
                            {
                                if (LastToken != null && LastToken.Type == TokenType.Or)
                                    ReplaceLastToken(new Token(TokenType.OpOr, "||"));
                                else
                                    Toks.Add(new Token(TokenType.Or, "|"));
                            } break;
                        case "\"": inQuotes = true; quoteString = ""; quoteChar = "\""; break;
                        case "\'": inQuotes = true; quoteString = ""; quoteChar = "\'"; break;
                        default: if (t.Trim() != "") Toks.Add(new Token(TokenType.Literal, t)); break;
                    }
                }
                else
                {
                    if (t == quoteChar)
                    {
                        inQuotes = false;
                        Toks.Add(new Token(TokenType.QuotedLiteral, quoteString));
                    }
                    else
                        quoteString += t;
                }
            }

            return Toks.ToArray();
        }

        public static Token[] ParseExtended(Token[] Input)
        {
            Toks = new List<Token>();
            for (int count = 0; count < Input.Length; count++)
            {
                Token t = Input[count];
                if (t.Type == TokenType.Literal)
                {
                    switch (t.Value.ToLower().Trim())
                    {
                        case "if": Toks.Add(new Token(TokenType.ExIf, "if")); break;
                        case "elseif": Toks.Add(new Token(TokenType.ExElseIf, "elseif")); break;
                        case "else": Toks.Add(new Token(TokenType.ExElse, "else")); break;
                        case "while": Toks.Add(new Token(TokenType.ExWhile, "while")); break;
                        case "for": Toks.Add(new Token(TokenType.ExFor, "for")); break;
                        case "var": Toks.Add(new Token(TokenType.ExVar, "var")); break;
                        case "return": Toks.Add(new Token(TokenType.ExReturn, "return")); break;
                        case "class": Toks.Add(new Token(TokenType.ExClass, "class")); break;
                        case "override": Toks.Add(new Token(TokenType.ExOverride, "override")); break;
                        case "static": Toks.Add(new Token(TokenType.ExStatic, "static")); break;
                        case "extern": Toks.Add(new Token(TokenType.ExExtern, "extern")); break;
                        case "struct": Toks.Add(new Token(TokenType.ExStruct, "struct")); break;
                        case "#include": Toks.Add(new Token(TokenType.ExPPInclude, "#include")); break;
                        case "#define": Toks.Add(new Token(TokenType.ExPPDefine, "#define")); break;
                        case "#ifdef": Toks.Add(new Token(TokenType.ExPPIfDef, "#ifdef")); break;
                        case "#ifndef": Toks.Add(new Token(TokenType.ExPPIfNDef, "#ifndef")); break;
                        case "#elseifdef": Toks.Add(new Token(TokenType.ExPPElseIfDef, "#elseifdef")); break;
                        case "#elseifndef": Toks.Add(new Token(TokenType.ExPPElseIfNDef, "#elseifndef")); break;
                        case "#elsedef": Toks.Add(new Token(TokenType.ExPPElseDef, "#elsedef")); break;
                        case "#enddef": Toks.Add(new Token(TokenType.ExPPEndDef, "#enddef")); break;
                        default: Toks.Add(t); break;
                    }
                }
                else
                    Toks.Add(t);
            }

            return Toks.ToArray();
        }
    }
}