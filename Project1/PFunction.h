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
		PFunction(vector<string> params, 
			      AST::pointer block, 
			      bool isUseBlock = true,
				  FuncPointer func = nullptr);
		PFunction(vector<string> params,
				  AST::pointer block,
				  bool isUseBlock = true,
				  FuncPointer func = nullptr,
				  std::unordered_map<string, AST::pointer>&& defaultValue);

		virtual string toString() const override;

		virtual pointer __call__(const pointer& params, EPointer envir) override;
	
	private:
		vector<string> params;
		AST::pointer block;
		bool isUseBlock;
		FuncPointer c_function;
		std::unordered_map<string, AST::pointer> defaultValueMap;
	};
}


