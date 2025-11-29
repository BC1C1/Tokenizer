#include "Unary.h"

AST::Unary::Unary(TokenType op, pointer& right)
	:ANode(NodeType::Unary), op(op), right(right)
{
}
