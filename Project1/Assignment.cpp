#include "Assignment.h"
namespace AST {
	Assignment::Assignment(const string& name, pointer& value) : ANode(NodeType::Assignment),
		name(std::move(name)), value(value)
	{
	}
}
