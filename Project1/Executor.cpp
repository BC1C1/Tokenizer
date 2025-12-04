#include "Executor.h"
namespace exe {
	using std::vector;
	exe::Executor::Executor(NPointer& ast)
	{
		this->ast = dynamic_pointer_cast<AST::Block>(ast);
		if (!this->ast) throw std::runtime_error("无效的ast");
		globalEnvir = make_shared<Py::Environment>();
	}
	void Executor::preScan()
	{
		// 1.注册内置函数（暂时没有）

		// 2.函数定义提升
		for (auto& s : ast->statements) {
			if (s->getType() == AST::NodeType::FunctionDefine) {
				auto function = dynamic_pointer_cast<AST::FunctionDefine>(s);
				// 构造函数对象
				auto obj = make_shared<Py::PFunction>(function->parmaNames, function->block, globalEnvir);
				globalEnvir->assign(function->name, obj);
			}
		}

		// 3.创建全局入口
		// 全局代码块默认创建全局上下文
		ExecutionContext context = {
			this->ast,
			globalEnvir,
			0,
			nullptr
		};
		// 压入栈中
		contextStack.push(context);




	}
	void Executor::run()
	{			
		preScan();
		while (!contextStack.empty()) {
			auto& curr_ctx = contextStack.top();
			bool is_finish = executeBlock(curr_ctx);
			if (is_finish) {
				contextStack.pop();
				if (!contextStack.empty() && curr_ctx.returnValue) {
					contextStack.top().returnValue = curr_ctx.returnValue;
				}
			}
		}
	}
	void Executor::throwErrorMsg(const string& text)
	{
		std::cerr << "运行时错误: " << text << "\n";
		exit(0);
	}
	bool Executor::executeBlock(ExecutionContext& ctx)
	{
		while (ctx.stmt_idx < ctx.currBlock->statements.size()) {
			auto& stmt = ctx.currBlock->statements[ctx.stmt_idx];
			ctx.stmt_idx++;
			auto type = stmt->getType();
			switch (type)
			{
			case AST::NodeType::Assignment: {
				executeAssignment(stmt, ctx.currentEnvir);
				break;
			}
			case AST::NodeType::Call: {
				auto isContinue = executeCall(stmt, ctx.currentEnvir);
				if (isContinue) break;
				return false;
			}
			case AST::NodeType::Return: {
				auto isContinue = executeReturn(stmt, ctx.currentEnvir);
				return isContinue;
			}
			case AST::NodeType::If: {
				auto isContinue = executeIf(stmt, ctx.currentEnvir);
				if (isContinue) break;
				return false;
			}
			case AST::NodeType::While: {
				auto isContinue = executeWhile(stmt, ctx.currentEnvir);
				if (isContinue) break;
				ctx.stmt_idx--;
				return false;
			}
			case AST::NodeType::For: {
				auto isContinue = executeFor(stmt, ctx.currentEnvir);
				if (isContinue) break;
				ctx.stmt_idx--;
				return false;
			}
			case AST::NodeType::Print: {
				executePrint(stmt, ctx.currentEnvir);
			}
			case AST::NodeType::FunctionDefine: {
				continue;
			}
			default:
				break;
			}
		}
		return true;
	}
	void Executor::executeAssignment(NPointer assignStmt, EPointer envir)
	{
		// 赋值本身不涉及新代码块，简单处理表达式返回即可
		auto now = dynamic_pointer_cast<AST::Assignment>(assignStmt);
		auto nameObj = dynamic_pointer_cast<AST::Variable>(now->name);
		auto name = nameObj->name;
		auto valueObj = executeExpr(now->value, envir);
		envir->assign(name, valueObj);
	}

	bool Executor::executeCall(NPointer callStmt, EPointer envir)
	{
		auto now = dynamic_pointer_cast<AST::Call>(callStmt);
		if (!now) throwErrorMsg("无效的call语句");
		if (!now->caller) throwErrorMsg("无效的调用者");
		// 获取函数对象
		auto& name = dynamic_pointer_cast<AST::Variable>(now->caller)->name;
		auto func = envir->getObj(name);
		auto function = dynamic_pointer_cast<Py::PFunction>(func);
		// 构造参数列表PList
		std::vector<Py::pointer> params;
		for (auto p : now->params) {
			auto o = executeExpr(p, envir);
			if (!o) throwErrorMsg("无效的参数");
			params.push_back(o);
		}
		auto list = make_shared<Py::PList>(std::move(params));
		// 构造函数内部环境
		auto innerEnvir = make_shared<Py::Environment>(envir);
		auto innerBlock = function->__call__(list, innerEnvir);
		ExecutionContext context = {
			dynamic_pointer_cast<AST::Block>(innerBlock),
			innerEnvir,
			0,
			nullptr
		};
		contextStack.push(context);
		return false;
	}

	bool Executor::executeReturn(NPointer returnStmt, EPointer envir)
	{
		PPointer returnValue;
		auto now = dynamic_pointer_cast<AST::Return>(returnStmt);
		if (!now) throwErrorMsg("无效的return语句");
		if (!now->expression) throwErrorMsg("无效的返回值");
		if (now->iscontain) {
			returnValue = executeExpr(now->expression, envir);
		}
		else {
			returnValue = make_shared<Py::PNone>();
		}
		contextStack.top().returnValue = returnValue;
		contextStack.top().isReturn = true;
		return true;
	}

	bool Executor::executeIf(NPointer ifStmt, EPointer envir)
	{
		auto now = dynamic_pointer_cast<AST::If>(ifStmt);
		if (!now) throwErrorMsg("无效的if语句");
		if (!now->block && now->elifs.empty() && !now->Else) throwErrorMsg("if 语句缺少有效代码块");
		auto condition = dynamic_pointer_cast<Py::PBool>(executeExpr(now->condition, envir));
		if (!condition) {
			throwErrorMsg("if条件缺少条件");
		}
		if (condition->getValue()) {
			// 要执行新的代码块，现在创建新环境并压入？
			ExecutionContext newContext = {
				dynamic_pointer_cast<AST::Block>(now->block),
				envir, // 不创建新环境
				0,
				nullptr
			};
			contextStack.push(newContext);
			return false;
		}
		auto elifs = now->elifs;
		for (auto& elif : elifs) {
			auto now = dynamic_pointer_cast<AST::If>(elif);
			if (!now) throwErrorMsg("elif语句无效");
			auto conditionObj = executeExpr(now->condition, envir);
			if (!conditionObj) throwErrorMsg("elif的表达式无效");
			auto condition = dynamic_pointer_cast<Py::PBool>(conditionObj);
			if (!condition)
				throwErrorMsg("无效的elif语句");
			if (condition->getValue()) {
				ExecutionContext context = {
					dynamic_pointer_cast<AST::Block>(now->block),
					envir,
					0,
					nullptr
				};
				contextStack.push(context);
				return false;
			}
		}
		if (now->Else) {
			auto innerNow = dynamic_pointer_cast<AST::If>(now->Else);
			auto condition = dynamic_pointer_cast<Py::PBool>(innerNow->condition);
			ExecutionContext context = {
				dynamic_pointer_cast<AST::Block>(innerNow->block),
				envir,
				0,
				nullptr
			};
			contextStack.push(context);
			return false;
		}
		return true; // 加入运行这句说明所有分支都没有命中，直接跳过算了
	}

	bool Executor::executeWhile(NPointer whileStmt, EPointer envir)
	{
		auto now = dynamic_pointer_cast<AST::While>(whileStmt);
		if (!now) throwErrorMsg("无效的while语句");
		if (!now->block) throwErrorMsg("while语句没有循环体");
		auto conditionObj = executeExpr(now->condition, envir);
		auto condition = dynamic_pointer_cast<Py::PBool>(conditionObj);
		if (!condition) throwErrorMsg("while语句无效的表达式");
		if (condition->getValue()) {
			ExecutionContext context = {
				dynamic_pointer_cast<AST::Block>(now->block),
				envir,
				0,
				nullptr
			};
			contextStack.push(context);
			return false;
		}
		return true;
	}

	bool Executor::executeFor(NPointer forStmt, EPointer envir)
	{
		auto now = dynamic_pointer_cast<AST::For>(forStmt);
		if (!now) throwErrorMsg("无效的for语句");
		if (!now->var) throwErrorMsg("for语句没有循环变量");
		auto nameObj = dynamic_pointer_cast<AST::Variable>(now->var);
		if (!nameObj) throwErrorMsg("for循环变量必须是合法变量名");
		auto& name = nameObj->name;
		if (!now->block) throwErrorMsg("for语句没有循环体");
		if (!now->iterable) throwErrorMsg("for语句没有循环范围");
		Py::pointer iterator = nullptr;
		if (now->iterator)
			iterator = now->iterator;
		else {
			iterator = executeExpr(now->iterable, envir)->__iter__();
			now->iterator = iterator;
		}
		auto iter = dynamic_pointer_cast<Py::Iterator>(iterator);
		if (!iter->isEnd()) {
			auto obj = iter->get();
			if (!obj) throwErrorMsg("迭代器获取元素失败");
			envir->assign(name, obj);
			ExecutionContext context = {
				dynamic_pointer_cast<AST::Block>(now->block),
				envir,
				0,
				nullptr
			};
			contextStack.push(context);
			return false;
		}
		else {
			now->iterator = nullptr;
		    return true;
		}
	}

	void Executor::executePrint(NPointer printStmt, EPointer envir)
	{
		auto now = dynamic_pointer_cast<AST::Print>(printStmt);
		string buffer = "";
		if (!now) throwErrorMsg("无效的print语句");
		for (int i = 0; i < now->text.size(); i++) {
			auto Obj = executeExpr(now->text[i], envir);
			auto text = Obj->toString();
			buffer += text;
			if (i != now->text.size() - 1)
				buffer += " ";
		}
		printf("%s\n", buffer.c_str());
	}

	PPointer Executor::executeExpr(NPointer expr, EPointer envir)
	{
		auto type = expr->getType();
		switch (type)
		{
		case AST::NodeType::Binary: {
			auto now = dynamic_pointer_cast<AST::Binary>(expr);
			auto op = now->op;
			auto left = executeExpr(now->left, envir);
			auto right = executeExpr(now->right, envir);
			switch (op)
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

				break;
			}
			default: {
				throwErrorMsg("未知的二元运算符");
				break;
			}
			}
		}
		case AST::NodeType::Unary: {
			auto now = dynamic_pointer_cast<AST::Unary>(expr);
			auto op = now->op;
			auto obj = executeExpr(now->right, envir);
			switch (op)
			{
			case Lexer::TokenType::MINUS: {
				return obj->__sub__(make_shared<Py::PNone>());
				break;
			}
			case Lexer::TokenType::NOT: {
				return obj->__not__(make_shared<Py::PNone>());
				break;
			}
			default: {
				throwErrorMsg("未知的一元运算符");
				break;
			}
			}
		}
		case AST::NodeType::Variable: {
			auto now = dynamic_pointer_cast<AST::Variable>(expr);
			return envir->getObj(now->name);
			break;
		}
		case AST::NodeType::Int: {
			auto now = dynamic_pointer_cast<AST::Int>(expr);
			return make_shared<Py::PInt>(now->value);
			break;
		}
		case AST::NodeType::Float: {
			auto now = dynamic_pointer_cast<AST::Float>(expr);
			return make_shared<Py::PFloat>(now->value);
			break;
		}
		case AST::NodeType::String: {
			auto now = dynamic_pointer_cast<AST::String>(expr);
			return make_shared<Py::PStr>(now->text);
			break;
		}
		case AST::NodeType::List: {
			auto now = dynamic_pointer_cast<AST::List>(expr);
			std::vector<Py::pointer> eles;
			for (auto e : now->elements) {
				auto temp = executeExpr(e, envir);
				if (!temp) {
					throwErrorMsg("无效的元素");
				}
				eles.push_back(temp);
			}
			return make_shared<Py::PList>(std::move(eles));
			break;
		}
		case AST::NodeType::Index: {
			auto now = dynamic_pointer_cast<AST::Index>(expr);
			auto obj = executeExpr(now->obj, envir);
			auto index = executeExpr(now->expression, envir);
			auto indexNum = dynamic_pointer_cast<Py::PInt>(index->asInt())->getValue();
			auto iter = obj->__iter__();
			auto iterator = dynamic_pointer_cast<Py::Iterator>(iter);
			return iterator->get(indexNum);
			break;
		}
		case AST::NodeType::Bool: {
			auto now = dynamic_pointer_cast<AST::Bool>(expr);
			return make_shared<Py::PBool>(now->value);
			break;
		}
		case AST::NodeType::Call: {
			executeCall(expr, envir);
			auto ret = contextStack.top().returnValue;
			contextStack.top().returnValue = nullptr;
			return ret;
			break;
		}
		default: {
			throwErrorMsg("未知的表达式节点类型");
			break;
		}
		}
		return nullptr;
	}

}

