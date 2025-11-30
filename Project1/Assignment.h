#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	using std::string;
	class Assignment : public ANode
	{
	public:
		Assignment(pointer& name, pointer& value);
		pointer name;
		pointer value;
	};
}


