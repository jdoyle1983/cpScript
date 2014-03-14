extern __StdLib__Utilities__Array__New
extern __StdLib__Utilities__Array__Free
extern __StdLib__Utilities__Array__Count
extern __StdLib__Utilities__Array__Resize
extern __StdLib__Utilities__Array__SetItem
extern __StdLib__Utilities__Array__GetItem

class Array
{
	var ArrayId;
	
	Array(var Size)
	{
		ArrayId = __StdLib__Utilities__Array__New(Size);
	}
	
	~Array()
	{
		__StdLib__Utilities__Array__Free(ArrayId);
	}
	
	void Resize(var NewSize)
	{
		__StdLib__Utilities__Array__Resize(ArrayId, NewSize);
	}
	
	var Size()
	{
		return __StdLib__Utilities__Array__Count(ArrayId);
	}
	
	void SetItem(var Index, var Value)
	{
		__StdLib__Utilities__Array__SetItem(ArrayId, Index, Value);
	}
	
	var GetItem(var Index)
	{
		return __StdLib__Utilities__Array__GetItem(ArrayId, Index);
	}
}
