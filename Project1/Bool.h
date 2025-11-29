#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Bool : public ANode
	{
	public:
		Bool(bool value);
		bool value;
	};
}


