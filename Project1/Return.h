#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Return : public ANode
	{
	public:
		Return(bool iscontain, pointer expression);
		bool iscontain;
		pointer expression;
	};
}


