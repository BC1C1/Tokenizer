#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
#include "Tokenizer.h"
namespace AST {
	using Lexer::TokenType;
	class Number : public ANode
	{
	public:
		Number(double value);
		double value;
	};
}


