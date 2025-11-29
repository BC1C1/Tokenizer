#include "String.h"
namespace AST {
	String::String(const string& text) : ANode(NodeType::String)
		, text(std::move(text))
	{
	}
}

