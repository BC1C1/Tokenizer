#pragma once
#include "TypeDefine.h"
namespace exe {
	using NPointer = AST::pointer;
	using PPointer = Py::pointer;
	using EPointer = std::shared_ptr<Py::Environment>;
	using std::string;
	using std::dynamic_pointer_cast;
	using std::make_shared;
	class Executor
	{
	public:
		Executor(NPointer& ast);
		void preScan(NPointer block);
		void preScanClass(NPointer block);
		void preScanFunctionInClass(PPointer c, NPointer b);
		PPointer run(EPointer envir = nullptr, const NPointer& node = nullptr);
	private:
		void throwErrorMsg(const string& text = "");
	private:
		const NPointer ast;
	};
}


