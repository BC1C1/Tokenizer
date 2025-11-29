#include "Block.h"
namespace AST {
	Block::Block(vector<pointer>& statements
		, shared_ptr<Environment> envir) 
		: ANode(NodeType::Block),
		statements(std::move(statements)),
		environment(envir)
	{
		if (!environment)
			environment = make_shared<Environment>();
	}
}

