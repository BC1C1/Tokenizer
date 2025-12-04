#pragma once
#include "TypeDefine.h"
#include "NodeDefine.h"
#include "Tokenizer.h"
namespace AST {
	using Lexer::Token;
	using Lexer::TokenType;
	using AST::pointer;
	using std::make_shared;
	using std::dynamic_pointer_cast;
	using EPointer = shared_ptr<Py::Environment>;
	class Parser
	{
	public:
		Parser(const vector<Token> tokens);
		pointer parse();
	private:
		pointer parseBlock();


		pointer parseStatement();


		pointer parseReturn();

		pointer parseFunction();

		pointer parsePrintStmt();

		pointer parseAssignment();
		pointer parseLValue();

		pointer parseIfStmt();
		pointer parseElifStmt();
		pointer parseElseStmt();

		pointer parseWhileStmt();
		pointer parseForStmt();
	private:
		pointer parseExpression();

		pointer parseOr();

		pointer parseAnd();

		pointer parseIn();

		pointer parseComparison();

		pointer parseTerm();

		pointer parseFactor();

		pointer parseUnary();

		pointer parsePrimary();

		pointer parseList();
	private:
		void throwErrorMsg(string text);
	private:
		bool isAtEnd();

		const Token& peek();

		const Token& peekNext();

		const Token& comsume(TokenType type, const string& errorMsg);

		bool check(TokenType type);

		bool match(TokenType type);

	private:
		const vector<Token> tokens;
		size_t current;
	};
}


