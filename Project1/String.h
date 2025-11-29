#pragma once
#include "NodeDefine.h"
#include "TypeDefine.h"
namespace AST {
	using std::string;
	class String : public ANode
	{
	public:
		String(const string& text);
		string text;
	};
}


