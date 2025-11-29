#include "List.h"
namespace AST {
	List::List(vector<pointer>&& elements) : ANode(NodeType::List),
		elements(elements)
	{
	}
}

