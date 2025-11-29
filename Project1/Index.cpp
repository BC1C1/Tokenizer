#include "Index.h"

AST::Index::Index(pointer obj, pointer expression) : ANode(NodeType::Index),
	obj(obj), expression(expression)
{
}
