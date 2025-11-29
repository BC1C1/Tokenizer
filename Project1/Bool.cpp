#include "Bool.h"
namespace AST {
	Bool::Bool(bool value) : ANode(NodeType::Bool), value(value)
	{
	}
}

