#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class For : public ANode
	{
	public:
		For(pointer var, pointer iterable, pointer block);
		pointer var;
		pointer iterable;
		pointer block;
	};
}


