#include "PFunction.h"
namespace Py {
	PFunction::PFunction(vector<string>& params, AST::pointer block)
		: PObject(typeMap.at(Type::FunctionDefine)), params(params), block(block)
	{
	}
	string PFunction::toString() const
	{
		return "Function";
	}
}

