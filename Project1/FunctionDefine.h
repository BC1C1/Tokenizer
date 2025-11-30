#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class FunctionDefine : public ANode
	{
	public:
		FunctionDefine(const string& name, vector<string>&& parmaNames, pointer block);
		string name;
		vector<string> parmaNames;
		pointer block;
	};
}


