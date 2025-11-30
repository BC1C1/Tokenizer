#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Int : public ANode
	{
	public:
		Int(int val);
		int value;
	};
}


