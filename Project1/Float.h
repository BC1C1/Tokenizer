#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class Float : public ANode
	{
	public:
		Float(double val);
		double value;
	};

}

