#pragma once
#include "NodeDefine.h"
#include "TypeDefine.h"
namespace AST {
	using AST::ANode;
	class Variable : public ANode
	{
	public:
		Variable(const string& name);
		string name;
	};
}


