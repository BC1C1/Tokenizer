#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	using std::string;
	class Assignment : public ANode
	{
	public:
		Assignment(const string& name, pointer& value);
		string name;
		pointer value;
	};
}


