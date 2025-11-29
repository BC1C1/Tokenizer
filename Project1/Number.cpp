#include "Number.h"
namespace AST {
	Number::Number(double value) : ANode(NodeType::Number), value(value)
	{
	}
}

