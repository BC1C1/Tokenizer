#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Call : public ANode
	{
	public:
		Call(pointer caller, vector<pointer>&& params);
		pointer caller;
		vector<pointer> params;
	};

}

