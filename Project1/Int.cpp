#include "Int.h"
namespace AST {
	Int::Int(int val) : ANode(NodeType::Int), value(val) {}
}
