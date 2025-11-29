#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
#include "Tokenizer.h"
namespace AST {
	using Lexer::TokenType;
	class Binary : public ANode
	{
	public:
		Binary(TokenType op, pointer& left, pointer& right);
		pointer left;
		pointer right;
		TokenType op;
	};
}


