#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Block : public ANode
	{
	public:
		Block(vector<pointer>&& statements);
		vector<pointer> statements;
	};
}


