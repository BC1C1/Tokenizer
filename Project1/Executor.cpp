#include "Executor.h"
namespace exe {
	using std::vector;
	exe::Executor::Executor(NPointer& ast) : ast(ast)
	{
	}
	void Executor::preScan(NPointer b)
	{
		preScanClass(b);
		auto block = dynamic_pointer_cast<AST::Block>(b);
		auto OutEnvir = block->environment; // shared_ptr复制了也没关系
		for (auto& s : block->statements) {
			if (s->getType() == AST::NodeType::FunctionDefine) {
				auto function = dynamic_pointer_cast<AST::FunctionDefine>(s);
				// 构造函数对象
				auto obj = make_shared<Py::PFunction>(function->parmaNames, function->block);
				OutEnvir->assign(function->name, obj);
				// 对于def中嵌套的def也要处理
				preScan(function->block);
				// 提升函数内的类
				preScanClass(function->block);
			}
		}
	}
	void Executor::preScanClass(NPointer b)
	{
		auto block = dynamic_pointer_cast<AST::Block>(b);
		auto OutEnvir = block->environment;
		for (auto& s : block->statements) {
			if (s->getType() == AST::NodeType::Class) {
				auto Class = dynamic_pointer_cast<AST::Class>(s);
				auto obj = make_shared<Py::PClass>(Class->name);
				OutEnvir->assign(Class->name, obj);
				// 这里需要根据block中的def来为obj初始化成员表
				preScanFunctionInClass(obj, Class->block);
				// 提升嵌套类
				preScanClass(Class->block);
			}
		}

	}
	void Executor::preScanFunctionInClass(PPointer c, NPointer b)
	{
		// b is the block in class define
		// c is the class object
		auto block = dynamic_pointer_cast<AST::Block>(b);
		auto Class = dynamic_pointer_cast<Py::PClass>(c);
		if (!Class || !block) return;
		for (auto& s : block->statements) {
			if (s->getType() == AST::NodeType::FunctionDefine) {
				auto function = dynamic_pointer_cast<AST::FunctionDefine>(s);
				auto obj = make_shared<Py::PFunction>(function->parmaNames, function->block);
				Class->addMember(function->name, obj);
				// 提升内部的类和函数
				preScan(function->block);
			}
		}
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
			// 先预扫描所有函数和类
			preScan(now);
			for (auto& statement : now->statements) {
				auto result = run(now->environment, statement);
				if (!result)
					continue;
				if (result->getType().type == Py::Type::ReturnValue)
					return result;
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
			else if (auto var = dynamic_pointer_cast<AST::Attribute>(now->name))
			{
				auto obj = run(envir, now->value);
				auto caller = run(envir, var->caller);
				caller->__setattribute__(var->attribute, obj);
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
		case AST::NodeType::Call: {
			auto now = dynamic_pointer_cast<AST::Call>(currentNode);
			auto caller = run(envir, now->caller);
			vector<Py::pointer> params;
			for (auto& p : now->params) {
				params.push_back(run(envir, p));
			}
			if (caller->getType() == Py::Type::FunctionDefine)
				return caller->__call__(make_shared<Py::PList>(std::move(params)), envir);
			else if (caller->getType() == Py::Type::Class)
				return caller->__instance__(make_shared<Py::PList>(std::move(params)), envir);
			else throwErrorMsg(caller->toString() + "既不是类，也不是函数");
			break;
		}
		case AST::NodeType::Attribute: {
			auto now = dynamic_pointer_cast<AST::Attribute>(currentNode);
			auto caller = run(envir, now->caller);
			return caller->__getattribute__(now->attribute);
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
		case AST::NodeType::FunctionDefine: {
			// 跳过，什么也不做
			// 主要是预扫描构造的时候对于函数的参数名表是用移动构造
			break;
		}
		case AST::NodeType::Return: {
			auto now = dynamic_pointer_cast<AST::Return>(currentNode);
			if (now->iscontain) {
				auto obj = run(envir, now->expression);
				return make_shared<Py::PReturnValue>(obj);
			}
			else {
				auto none = make_shared<Py::PNone>();
				return make_shared<Py::PReturnValue>(none);
			}

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

