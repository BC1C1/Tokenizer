#include "Assignment.h"
namespace AST {
	Assignment::Assignment(pointer& name, pointer& value) : ANode(NodeType::Assignment),
		name(name), value(value)
	{
	}
}
