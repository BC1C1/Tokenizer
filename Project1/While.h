#pragma once
#include "TypeDefine.h"
namespace AST {
	class While : public ANode
	{
	public:
		While(pointer& condition, pointer& block);
		pointer condition;
		pointer block;
	};
}


