#pragma once
#include "NodeDefine.h"
#include "TypeDefine.h"
namespace AST {
	using pointer = AST::pointer;
	class List : public ANode
	{
	public:
		List(vector<pointer>&& elements);
		vector<pointer> elements;
	};

}

