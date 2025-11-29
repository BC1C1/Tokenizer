#include "Binary.h"
namespace AST {
	Binary::Binary(TokenType op, pointer& left, pointer& right) : ANode(NodeType::Binary),
		op(op), left(left), right(right)
	{
	}
}

