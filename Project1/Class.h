#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Class : public ANode
	{
	public:
		Class(const string& name, pointer block);
		string name;
		pointer block;
	};
}


