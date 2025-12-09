#include "Attribute.h"

AST::Attribute::Attribute(pointer caller, pointer attribute)
	: ANode(NodeType::Attribute), caller(caller), attribute(attribute)
{
}
