#pragma once
#include <memory>
#include <vector>
namespace AST {
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
		Binary,
		Unary,
		Variable,
		Number,
		String,
		List,
		Index,
		Bool,
		Print 
	};

	class ANode {
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


