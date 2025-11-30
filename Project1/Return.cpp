#include "Return.h"
namespace AST {
	Return::Return(bool iscontain, pointer expression) 
		: ANode(NodeType::Return), iscontain(iscontain), expression(expression)
	{
	}
}

