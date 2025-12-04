#pragma once
#include "TypeDefine.h"
#include <stack>
namespace AST {
	class Block;
}
namespace exe {
	using NPointer = AST::pointer;
	using PPointer = Py::pointer;
	using EPointer = std::shared_ptr<Py::Environment>;
	using std::string;
	using std::dynamic_pointer_cast;
	using std::make_shared;
	using ReturnPair = std::pair<bool, PPointer>;
	struct ExecutionContext {
		std::shared_ptr<AST::Block> currBlock;
		EPointer currentEnvir;
		size_t stmt_idx;
		Py::pointer returnValue;
		bool isReturn = false;
	};

	class Executor
	{
	public:
		Executor(NPointer& ast);
		void preScan();
		void run();
	private:
		ReturnPair executeBlockOnce(ExecutionContext& ctx);

		void throwErrorMsg(const string& text = "");

		bool executeBlock(ExecutionContext& ctx);

		void executeAssignment(NPointer assignStmt, EPointer envir);

		bool executeCall(NPointer callStmt, EPointer envir);

		bool executeReturn(NPointer returnStmt, EPointer envir);

		bool executeIf(NPointer ifStmt, EPointer envir);

		bool executeWhile(NPointer callStmt, EPointer envir);

		bool executeFor(NPointer callStmt, EPointer envir);

		void executePrint(NPointer printStmt, EPointer envir);

		PPointer executeExpr(NPointer expr, EPointer envir);

	private:
		std::shared_ptr<AST::Block> ast;
		std::stack<ExecutionContext> contextStack;
		std::shared_ptr<Py::Environment> globalEnvir;
	};
}


