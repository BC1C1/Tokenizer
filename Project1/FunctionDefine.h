#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
namespace AST {
	class FunctionDefine : public ANode
	{
	public:
		FunctionDefine(const string& name, 
					   vector<string>&& paramNames,
					   pointer block);
		FunctionDefine(const string& name,
			vector<string>&& paramNames,
			pointer block,
			std::unordered_map<string, pointer>&& defaultValue);

		string name;
		vector<string> paramNames;
		pointer block;
		std::unordered_map<string, pointer> defaultValue;
	};
}


