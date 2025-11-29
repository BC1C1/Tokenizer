#include "NodeDefine.h"
namespace AST {
	ANode::ANode(NodeType type) : type(type)
	{
	}

	NodeType ANode::getType() const
	{
		return type;
	}
	ANode::~ANode()
	{
	}
}

