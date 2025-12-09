#pragma once
#include <memory>
#include <vector>
namespace AST {
	using std::enable_shared_from_this;
	using std::shared_ptr;
	using std::vector;
	enum class NodeType {
		Program,
		Block,
		Assignment,
		If,
		Elif,
		Else,
		While,
		For,
		Binary,
		Unary,
		Variable,
		Number, Int, Float,
		String,
		List,
		Index,
		Bool,
		Print,
		FunctionDefine,
		Return,
		Call,
		Attribute,
	};

	class ANode : public enable_shared_from_this<ANode>
	{
	public:
		using pointer = shared_ptr<ANode>;
	public:
		ANode(NodeType type);
		NodeType getType() const;
		virtual ~ANode();
	private:
		NodeType type;
	};
	using pointer = shared_ptr<ANode>;
}


