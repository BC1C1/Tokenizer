#include "Parser.h"
namespace AST {
	using pointer = AST::pointer;
	Parser::Parser(const vector<Token> tokens)
		: tokens(tokens), current(0), currentEnvironment(nullptr)
	{
		if (!currentEnvironment)
			currentEnvironment = make_shared<Py::Environment>();
	}

	pointer Parser::parse()
	{
		vector<pointer> statements;
		while (!isAtEnd()) {
			statements.push_back(parseStatement());
		}
		shared_ptr<Block> block = 
			make_shared<Block>(statements, currentEnvironment);
		return block;
	}

	pointer Parser::parseBlock(bool isCreateNewEnvironment)
	{
		comsume(TokenType::INDENT, "意外的缩进");
		vector<pointer> statements;
		if (isCreateNewEnvironment) {
			auto preEnvir = currentEnvironment;
			auto childEnvir = make_shared<Py::Environment>(currentEnvironment);
			currentEnvironment = childEnvir;
			while (!isAtEnd() && !check(TokenType::DEDENT)) {
				statements.push_back(parseStatement());
			}
			comsume(TokenType::DEDENT, "意外的缩进");

			shared_ptr<Block> block = make_shared<Block>(statements, childEnvir);
			currentEnvironment = preEnvir;
			return block;
		} 
		else {
			while (!isAtEnd() && !check(TokenType::DEDENT)) {
				statements.push_back(parseStatement());
			}
			comsume(TokenType::DEDENT, "意外的缩进");
			shared_ptr<Block> block = make_shared<Block>(statements, currentEnvironment);
			return block;
		}
	}

	pointer Parser::parseStatement()
	{
		auto assigment = parseAssignment();
		if (assigment) return assigment;
		auto printStmt = parsePrintStmt();
		if (printStmt) return printStmt;
		auto ifStmt = parseIfStmt();
		if (ifStmt) return ifStmt;
		auto whileStmt = parseWhileStmt();
		if (whileStmt) return whileStmt;

		return parseExpression();
	}

	pointer Parser::parseExpression()
	{
		return parseOr();
	}

	pointer Parser::parseOr()
	{
		auto left = parseAnd();

		while (!isAtEnd() && match(TokenType::OR)) {
			//级联结点
			auto right = parseAnd();
			left = make_shared<Binary>(TokenType::OR, left, right);
		}
		return left;
	}

	pointer Parser::parseAnd()
	{
		auto left = parseComparison();

		while (!isAtEnd() && match(TokenType::AND)) {
			auto right = parseComparison();
			left = make_shared<Binary>(TokenType::AND, left, right);
		}
		return left;
	}

	pointer Parser::parseComparison()
	{
		auto left = parseTerm();

		while (!isAtEnd() && (
			check(TokenType::EQ) || check(TokenType::NEQ) ||
			check(TokenType::LT) || check(TokenType::LTE) ||
			check(TokenType::GT) || check(TokenType::GTE)
			)) {
			auto opTokenType = tokens[current].type;
			current++; // 上面用的是check所以要加一
			auto right = parseTerm();
			left = make_shared<Binary>(opTokenType, left, right);
		}
		return left;
	}

	pointer Parser::parseTerm()
	{
		auto left = parseFactor();
		
		while (!isAtEnd() && (check(TokenType::PLUS) || check(TokenType::MINUS))) {
			auto opTokenType = tokens[current].type;
			current++;
			auto right = parseFactor();
			left = make_shared<Binary>(opTokenType, left, right);
		}
		return left;
	}

	pointer Parser::parseFactor()
	{
		auto left = parseUnary();

		while (!isAtEnd() && (
			check(TokenType::STAR) || check(TokenType::SLASH) ||
			check(TokenType::MOD)
			)) {
			auto opTokenType = tokens[current].type;
			current++;
			auto right = parseUnary();
			left = make_shared<Binary>(opTokenType, left, right);
		}
		return left;
	}

	pointer Parser::parseUnary()
	{
		if (check(TokenType::MINUS) || check(TokenType::NOT)) {
			auto opType = tokens[current].type;
			current++;
			pointer inner = parseUnary();
			return make_shared<Unary>(opType, inner);
		}
		return parsePrimary();
	}

	pointer Parser::parsePrimary()
	{
		// 最小元素
		auto token = tokens[current];
		auto type = token.type;
		pointer ret = nullptr;
		switch (type)
		{
		case Lexer::TokenType::STRING: {
			current += 1;
			ret = make_shared<String>(token.lexeme);
			break;
		}
		case Lexer::TokenType::NUMBER: {
			current += 1;
			double value = std::stod(token.lexeme);
			ret = make_shared<Number>(value);
			break;
		}
		case Lexer::TokenType::TRUE: {
			current += 1;
			ret = make_shared<Bool>(true);
			break;
		}
		case Lexer::TokenType::FALSE: {
			current += 1;
			ret = make_shared<Bool>(false);
			break;
		}
		case Lexer::TokenType::IDENTIFIER: {
			current += 1;
			ret = make_shared<Variable>(token.lexeme);
			break;
		}
		case Lexer::TokenType::LPAREN: {
			// 开始匹配
			comsume(TokenType::LPAREN, "缺失的左括号");
			auto inner = parseExpression();
			comsume(TokenType::RPAREN, "缺失的右括号");
			ret = inner;
			break;
			// return之前就consume了右括号了，这个语句只匹配了一层括号
		}
		case Lexer::TokenType::LBRACKET: {
			auto inner = parseList();
			ret = inner;
			break;
		}
		case Lexer::TokenType::COMMA: {
			break;
		}
		case Lexer::TokenType::RPAREN: {
			// 直接提交错误
			throwErrorMsg("缺失的左括号");
			current += 1;
			break;
		}
		case Lexer::TokenType::EOF_TOKEN: {
			throwErrorMsg("不完整的表达式");
			current += 1;
			break;
		}
		case Lexer::TokenType::ERROR: {
			throwErrorMsg("错误的Token");
			current += 1;
			break;
		}
		default:
			throwErrorMsg("未知的符号");
			current += 1;
			break;
		}
		while (match(TokenType::LBRACKET)) {
			auto indexExpr = parseExpression();
			comsume(TokenType::RBRACKET, "索引缺少右分隔符 ']'");
			ret = make_shared<AST::Index>(ret, indexExpr);
		}

		return ret;
	}

	pointer Parser::parseList()
	{
		// 参照这个 
		/*		
		// 确认是不是print语句
		if (!match(TokenType::PRINT)) return nullptr;
		comsume(TokenType::LPAREN, "缺失的左括号");
		vector<pointer> text;
		do {
			if (match(TokenType::RPAREN))
				throwErrorMsg("print缺少参数");
			text.push_back(parseExpression());
		} while (match(TokenType::COMMA));
		comsume(TokenType::RPAREN, "缺失的右括号");
		return make_shared<Print>(text);
		*/
		comsume(TokenType::LBRACKET, "缺失的左中括号");
		vector<pointer> expressions;
		while (!check(TokenType::RBRACKET)) {
			auto ele = parseExpression();
			if (ele)
				expressions.push_back(ele);
			if (match(TokenType::COMMA)) {
				continue;
			}
			else if (!check(TokenType::RBRACKET)) {
				throwErrorMsg("元素之间缺少逗号");
			}
		}
		comsume(TokenType::RBRACKET, "缺失的右中括号");
		return make_shared<List>(std::move(expressions));
	}

	void Parser::throwErrorMsg(string text)
	{
		std::cerr << "Error (line " << tokens[current].line << "): " << text << "\n";
		exit(1);
	}

	pointer Parser::parsePrintStmt()
	{
		// 确认是不是print语句
		if (!match(TokenType::PRINT)) return nullptr;
		comsume(TokenType::LPAREN, "缺失的左括号");
		vector<pointer> text;
		do {
			if (match(TokenType::RPAREN))
				throwErrorMsg("print缺少参数");
			text.push_back(parseExpression());
		} while (match(TokenType::COMMA));
		comsume(TokenType::RPAREN, "缺失的右括号");
		return make_shared<Print>(text);
	}

	pointer Parser::parseAssignment()
	{
		// 确认是不是赋值语句
		if (!match(TokenType::IDENTIFIER)) return nullptr;
		// 到这里current是=
		if (!match(TokenType::ASSIGN)) {
			// 需要回退
			current -= 1;
			return nullptr;
		}
		// 到这里已经是表达式的开头，先取出变量名
		auto varName = tokens[current - 2].lexeme;
		// 解析表达式
		auto value = parseExpression();
		// 构造assignment结点
		return make_shared<Assignment>(varName, value);
	}

	pointer Parser::parseIfStmt()
	{
		if (!match(TokenType::IF)) return nullptr;
		auto condition = parseExpression();
		comsume(TokenType::COLON, "if语句缺少冒号");
		auto block = parseBlock();
		vector<pointer> elifs;
		while (check(TokenType::ELIF)) {
			auto elif = parseElifStmt();
			if (elif)
				elifs.push_back(elif);
		}
		auto Else = parseElseStmt();
		return make_shared<If>(condition, block, std::move(elifs), Else);
	}

	pointer Parser::parseElifStmt()
	{
		if (!match(TokenType::ELIF)) return nullptr;
		auto condition = parseExpression();
		comsume(TokenType::COLON, "elif语句缺少冒号");
		auto block = parseBlock();
		return make_shared<If>(condition, block);
	}

	pointer Parser::parseElseStmt()
	{
		if (!match(TokenType::ELSE)) return nullptr;
		comsume(TokenType::COLON, "else语句缺少冒号");
		auto block = parseBlock();
		return make_shared<If>(block);
	}

	pointer Parser::parseWhileStmt()
	{
		if (!match(TokenType::WHILE)) return nullptr;
		auto condition = parseExpression();
		comsume(TokenType::COLON, "while语句缺少冒号");
		auto block = parseBlock(true);
		return make_shared<While>(condition, block);
	}

	bool Parser::isAtEnd()
	{
		return current >= tokens.size() || tokens[current].type == TokenType::EOF_TOKEN;
	}

	const Token& Parser::peek()
	{
		return tokens[current];
	}

	const Token& Parser::peekNext()
	{
		if (current + 1 < tokens.size())
			return tokens[current + 1];
		return Token(TokenType::ERROR, "out_of_range", -1);
	}

	const Token& Parser::comsume(TokenType type, const string& errorMsg)
	{
		if (!match(type)) {
			throwErrorMsg(errorMsg);
		}
		return tokens[current - 1];
	}

	bool Parser::check(TokenType type)
	{
		if (!isAtEnd()) {
			return type == tokens[current].type;
		}
		return false;
	}

	bool Parser::match(TokenType type)
	{
		if (check(type)) {
			current++;
			return true;
		}
		return false;
	}


}
