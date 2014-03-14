extern __StdLib__Math__Abs
extern __StdLib__Math__Floor
extern __StdLib__Math__Ceiling

class Math
{
	static var ABS(var Val)
	{
		return __StdLib__Math__Abs(Val);
	}
	
	static var FLOOR(var Val)
	{
		return __StdLib__Math__Floor(Val);
	}
	
	static var CEILING(var Val)
	{
		return __StdLib__Math__Ceiling(Val);
	}
}
