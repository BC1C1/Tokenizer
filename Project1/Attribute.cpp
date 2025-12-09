#include "Attribute.h"
namespace AST {
	Attribute::Attribute(pointer caller, const string& attribute)
		: ANode(NodeType::Attribute), caller(caller), attribute(attribute)
	{
	}
}
