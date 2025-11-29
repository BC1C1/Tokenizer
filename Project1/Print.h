#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	using std::vector;
	class Print : public ANode
	{
	public:
		Print(vector<pointer>& text);
		vector<pointer> text;
	};
}


