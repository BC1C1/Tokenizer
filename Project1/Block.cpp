#include "Block.h"
namespace AST {
	Block::Block(vector<pointer>&& statements) : ANode(NodeType::Block),
		statements(statements)
	{
	}
}

