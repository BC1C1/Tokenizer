#include "While.h"
namespace AST {
	While::While(pointer& condition, pointer& block) : ANode(NodeType::While),
		condition(condition), block(block)
	{
	}
}

