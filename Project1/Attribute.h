#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Attribute : public ANode
	{
	public:
		Attribute(pointer caller, const string& attribute);
		pointer caller;
		string attribute;
	};
}


