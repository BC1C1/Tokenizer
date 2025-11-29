#pragma once
#include "NodeDefine.h"
#include "TypeDefine.h"
namespace AST {
	class Index : public ANode
	{
	public:
		Index(pointer obj, pointer expression);
		pointer obj;
		pointer expression;
	};
}


