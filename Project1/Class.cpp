#include "Class.h"
namespace AST {
	Class::Class(const string& name, pointer block) : ANode(NodeType::Class),
		name(name), block(block)
	{
	}
}

