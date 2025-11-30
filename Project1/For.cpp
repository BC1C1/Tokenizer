#include "FOR.h"
namespace AST {
	For::For(pointer var, pointer iterable, pointer block)
		: ANode(NodeType::For), var(var), iterable(iterable), block(block)
	{
	}
}

