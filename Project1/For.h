#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	using PPointer = Py::pointer;
	class For : public ANode
	{
	public:
		For(pointer var, pointer iterable, pointer block);
		pointer var;
		pointer iterable;
		pointer block;
		PPointer iterator;
	};
}


