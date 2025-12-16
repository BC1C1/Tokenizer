#pragma once
#include "PObject.h"
#include "TypeDefine.h"
#include <functional>
namespace Py {
	using std::vector;
	using std::string;
	using EPointer = shared_ptr<Environment>;
	using InnerFunc = std::function<pointer(pointer, EPointer)>;
	using FuncPointer = shared_ptr<InnerFunc>;
	class PFunction : public PObject
	{
	public:
		// define构造的时候需要参数表和执行方法
		// 若想要使用内置, 传入的参数block为nullptr,
		// isUseBlock为false, 最后传入函数指针
		PFunction(vector<string>& params, 
			      AST::pointer block, 
			      bool isUseBlock = true,
				  FuncPointer func = nullptr);

		virtual string toString() const override;

		virtual pointer __call__(const pointer& params, EPointer envir) override;
	
	private:
		vector<string> params;
		AST::pointer block;
		bool isUseBlock;
		FuncPointer c_function;
	};
}


