#include "Call.h"

AST::Call::Call(pointer caller, vector<pointer>&& params)
	: ANode(NodeType::Call), caller(caller), params(params)
{
}
