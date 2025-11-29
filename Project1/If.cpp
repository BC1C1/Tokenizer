#include "If.h"
namespace AST {
	If::If(pointer& condition, pointer& block,
		vector<pointer>&& elifs, pointer& Else) : ANode(NodeType::If),
		condition(condition), block(block), elifs(elifs), Else(Else),
		isExecuted(false)
	{
	}
	If::If(pointer& condition, pointer& block) : ANode(NodeType::Elif),
		condition(condition), block(block), Else(nullptr), isExecuted(false)
	{
	}
	If::If(pointer& block) : ANode(NodeType::Else), condition(nullptr),
		block(block), Else(nullptr), isExecuted(false)
	{
	}
}

