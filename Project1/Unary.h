#pragma once
#include "TypeDefine.h"
#include "Tokenizer.h"
namespace AST {
	using Lexer::TokenType;
	using pointer = AST::pointer;
	class Unary : public ANode
	{
	public:
		Unary(TokenType op, pointer& right);
		TokenType op;
		pointer right;
	};
}


