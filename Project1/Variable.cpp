#include "Variable.h"
namespace AST {
	Variable::Variable(const string& name) : ANode(NodeType::Variable),
		name(std::move(name))
	{
	}
}
