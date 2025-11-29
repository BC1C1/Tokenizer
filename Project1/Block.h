#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	using Py::Environment;
	class Block : public ANode
	{
	public:
		Block(vector<pointer>& statements,
			shared_ptr<Environment> envir = nullptr);
		vector<pointer> statements;
		shared_ptr<Environment> environment;
	};
}


