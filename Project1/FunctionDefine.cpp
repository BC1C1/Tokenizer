#include "FunctionDefine.h"
namespace AST {
	FunctionDefine::FunctionDefine(const string& name, vector<string>&& paramNames, pointer block)
		: ANode(NodeType::FunctionDefine), name(name), paramNames(paramNames),
		block(block)
	{
	}
	FunctionDefine::FunctionDefine(const string& name,
		vector<string>&& paramNames, 
		pointer block,
		std::unordered_map<string, pointer>&& defaultValue)
		: ANode(NodeType::FunctionDefine), name(name), paramNames(paramNames),
		block(block), defaultValue(defaultValue)
	{
	}

}
