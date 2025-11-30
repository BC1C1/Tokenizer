#include "FunctionDefine.h"
namespace AST {
	FunctionDefine::FunctionDefine(const string& name, vector<string>&& parmaNames, pointer block)
		: ANode(NodeType::FunctionDefine), name(name), parmaNames(parmaNames),
		block(block)
	{
	}

}
