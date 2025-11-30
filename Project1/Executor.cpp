#include "Executor.h"
namespace exe {
	using std::vector;
	exe::Executor::Executor(NPointer& ast) : ast(ast)
	{
	}
	PPointer Executor::run(EPointer envir, const NPointer& node)
	{
		auto currentNode = node ? node : ast;
		// 参照print ast的方法
		auto type = currentNode->getType();
		switch (type)
		{
		case AST::NodeType::Block: {
			// 按顺序执行所有语句
			auto now = dynamic_pointer_cast<AST::Block>(currentNode);
			for (auto& statement : now->statements) {
				run(now->environment, statement);
			}
			break;
		}
		case AST::NodeType::Assignment: {
			auto now = dynamic_pointer_cast<AST::Assignment>(currentNode);
			// 首先解析now->name，确认是变量还是容器
			if (auto var = dynamic_pointer_cast<AST::Variable>(now->name))
			{
				auto obj = run(envir, now->value);
				envir->assign(var->name, obj);
			} 
			else if (auto var = dynamic_pointer_cast<AST::Index>(now->name))
			{
				auto obj = run(envir, now->value);
				auto target = run(envir, var->obj);
				auto index = run(envir, var->expression);
				target->__setitem__(index, obj);
			}
			break;
		}
		case AST::NodeType::If: {
			auto now = dynamic_pointer_cast<AST::If>(currentNode);
			auto condition = run(envir, now->condition);
			auto result = dynamic_pointer_cast<Py::PBool>(condition);
			if (!result) {
				throwErrorMsg("表达式必须是布尔值");
			}
			if (result->getValue()) {
				auto block = dynamic_pointer_cast<AST::Block>(now->block);
				if (block)
					run(block->environment, block);
				else {
					throwErrorMsg("if 语句缺少有效代码块");
				}
			}
			else {
				// 开始运行elif
				bool flag = false;
				for (auto& elif : now->elifs) {
					run(envir, elif);
					auto isExecuted = dynamic_pointer_cast<AST::If>(elif)->isExecuted;
					flag = isExecuted;
					if (flag)
						break;
				}
				if (now->Else && !flag)
					run(envir, now->Else);
			}
			// 本次if执行结束，开始清空elif里那些执行标志符
			for (auto& elif : now->elifs) {
				dynamic_pointer_cast<AST::If>(elif)->isExecuted = false;
			}
			break;
		}
		case AST::NodeType::Elif: {
			auto now = dynamic_pointer_cast<AST::If>(currentNode);
			auto condition = run(envir, now->condition);
			auto result = dynamic_pointer_cast<Py::PBool>(condition);
			if (!result)
				throwErrorMsg("表达式必须是布尔值");
			if (result->getValue()) {
				now->isExecuted = true;
				auto block = dynamic_pointer_cast<AST::Block>(now->block);
				run(block->environment, block);
			}
			break;
		}
		case AST::NodeType::Else: {
			auto now = dynamic_pointer_cast<AST::If>(currentNode);
			auto block = dynamic_pointer_cast<AST::Block>(now->block);
			run(block->environment, block);
			break;
		}
		case AST::NodeType::While: {
			auto now = dynamic_pointer_cast<AST::While>(currentNode);
			auto condition = run(envir, now->condition);
			auto result = dynamic_pointer_cast<Py::PBool>(condition);
			if (!result)
				throwErrorMsg("表达式必须是布尔值");
			while (result->getValue()) {
				auto block = dynamic_pointer_cast<AST::Block>(now->block);
				run(block->environment, block);
				condition = run(envir, now->condition);
				result = dynamic_pointer_cast<Py::PBool>(condition);
			}
			break;
		}
		case AST::NodeType::For: {
			auto now = dynamic_pointer_cast<AST::For>(currentNode);
			auto block = dynamic_pointer_cast<AST::Block>(now->block);
			auto innerEnvir = block->environment;
			auto var = dynamic_pointer_cast<AST::Variable>(now->var);
			auto iterable = run(envir, now->iterable);
			auto iter = iterable->__iter__();
			auto iterator = dynamic_pointer_cast<Py::Iterator>(iter);
			while (!iterator->isEnd()) {
				// 给var赋值
				auto obj = iterator->get();
				innerEnvir->createThere(var->name, obj);
				run(innerEnvir, block);
			}

			break;
		}
		case AST::NodeType::Binary: {
			auto now = dynamic_pointer_cast<AST::Binary>(currentNode);
			auto left = run(envir, now->left);
			auto right = run(envir, now->right);
			switch (now->op)
			{
			case Lexer::TokenType::PLUS: return left->__add__(right);
			case Lexer::TokenType::MINUS: return left->__sub__(right);
			case Lexer::TokenType::STAR: return left->__mul__(right);
			case Lexer::TokenType::SLASH: return left->__truediv__(right);
			case Lexer::TokenType::MOD: return left->__mod__(right);
			case Lexer::TokenType::EQ: return left->__eq__(right);
			case Lexer::TokenType::NEQ: return left->__ne__(right);
			case Lexer::TokenType::LT: return left->__lt__(right);
			case Lexer::TokenType::GT: return left->__gt__(right);
			case Lexer::TokenType::LTE: return left->__le__(right);
			case Lexer::TokenType::GTE: return left->__ge__(right);
			case Lexer::TokenType::AND: return left->__and__(right);
			case Lexer::TokenType::OR: return left->__or__(right);
			case Lexer::TokenType::NOT: return left->__not__(right);
			case Lexer::TokenType::IN: {
				auto iter = right->__iter__();
				auto iterator = dynamic_pointer_cast<Py::Iterator>(iter);
				while (!iterator->isEnd()) {
					auto result = left->__eq__(iterator->get());
					auto temp = dynamic_pointer_cast<Py::PBool>(result);
					if (!temp)
						return make_shared<Py::PBool>(false);
					if (temp->getValue())
					{
						return make_shared<Py::PBool>(true);
					}
				}
				return make_shared<Py::PBool>(false);
			}
			default:
				// 这里暂时这样，其实需要try这个内层swich然后catch错误继续转交
				// 错误文本，这里也一样，是“未知的运算符”
				throwErrorMsg("未知的运算符");
			}
			break;
		}
		case AST::NodeType::Unary: {
			auto now = dynamic_pointer_cast<AST::Unary>(currentNode);
			auto type = now->op;
			auto right = run(envir, now->right);
			switch (type)
			{
			case Lexer::TokenType::MINUS: 
				return right->__sub__(make_shared<Py::PNone>());
			case Lexer::TokenType::NOT:
				return right->__not__(make_shared<Py::PNone>());
			default:
				throwErrorMsg("未知的运算符");
			}
		}
		case AST::NodeType::Variable: {
			auto now = dynamic_pointer_cast<AST::Variable>(currentNode);
			return envir->getObj(now->name);
			break;
		}
		case AST::NodeType::Int: {
			auto now = dynamic_pointer_cast<AST::Int>(currentNode);
			return make_shared<Py::PInt>(now->value);
			break;
		}
		case AST::NodeType::Float: {
			auto now = dynamic_pointer_cast<AST::Float>(currentNode);
			return make_shared<Py::PFloat>(now->value);
			break;
		}
		case AST::NodeType::String: {
			auto now = dynamic_pointer_cast<AST::String>(currentNode);
			return make_shared<Py::PStr>(now->text);
			break;
		}
		case AST::NodeType::Bool: {
			auto now = dynamic_pointer_cast<AST::Bool>(currentNode);
			return make_shared<Py::PBool>(now->value);
			break;
		}
		case AST::NodeType::Print: {
			auto now = dynamic_pointer_cast<AST::Print>(currentNode);
			string text = "";
			for (size_t i = 0; i < now->text.size(); ++i) {
				text += run(envir, now->text[i])->toString();
				if (i != now->text.size() - 1) {
					text += " ";
				}
			}
			puts(text.c_str());
			break;
		}
		case AST::NodeType::Index: {
			auto now = dynamic_pointer_cast<AST::Index>(currentNode);
			auto obj = run(envir, now->obj);
			auto index = run(envir, now->expression);
			auto IntObj = index->asInt();
			int value = dynamic_pointer_cast<Py::PInt>(IntObj)->getValue();
			auto iter = obj->__iter__();
			auto iterator = dynamic_pointer_cast<Py::Iterator>(iter);
			if (!iterator) {
				throwErrorMsg("不可迭代对象不可索引");
			}
			return iterator->get(value);
			break;
		}
		case AST::NodeType::List: {
			auto now = dynamic_pointer_cast<AST::List>(currentNode);
			vector<Py::pointer> iterable;
			for (auto& ele : now->elements) {
				auto e = run(envir, ele);
				if (e)
					iterable.push_back(e);
				else
					throwErrorMsg("无效的元素");
			}
			return make_shared<Py::PList>(std::move(iterable));
		}
		default:
			throwErrorMsg("未知的结点类型");
		}
		return nullptr;
	}
	void Executor::throwErrorMsg(const string& text)
	{
		std::cerr << "运行时错误: " << text << "\n";
		exit(0);
	}
}

