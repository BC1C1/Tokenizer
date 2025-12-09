#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Attribute : public ANode
	{
	public:
		Attribute(pointer caller, pointer attribute);
		pointer caller;
		pointer attribute;
	};
}


