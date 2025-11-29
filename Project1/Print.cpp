#include "Print.h"
namespace AST {
	Print::Print(vector<pointer>& text) 
		: ANode(NodeType::Print), text(std::move(text))
	{
	}
}
