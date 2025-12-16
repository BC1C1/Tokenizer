#include "PFunction.h"
namespace Py {
	PFunction::PFunction(vector<string>& params, 
		AST::pointer block, 
		bool isUseBlock,
		FuncPointer func)
		: PObject(typeMap.at(Type::FunctionDefine)), params(params), block(block)
		, isUseBlock(isUseBlock), c_function(func)
	{
		if (isUseBlock && block) {
			auto B = dynamic_pointer_cast<AST::Block>(block);
			B->environment = make_shared<Py::Environment>();
		}
	}
	string PFunction::toString() const
	{
		return "Function";
	}
	pointer PFunction::__call__(const pointer& params, EPointer envir)
	{
		if (isUseBlock) {
			// 参数检查
			auto param = dynamic_pointer_cast<Py::PList>(params);
			if (!param)
				throw std::runtime_error("错误的参数表，转换成PList时失败");
			if (this->params.size() != param->size())
				throw std::runtime_error("参数数量不匹配");
			auto iterator = dynamic_pointer_cast<Py::ListIterator>(param->__iter__());
			// 初始化block环境参数
			auto b = dynamic_pointer_cast<AST::Block>(block);
			// 环境用调用时候的环境
			b->environment->setParent(envir);
			for (auto& name : this->params) {
				b->environment->createThere(name, iterator->get());
			}
			exe::Executor e(block);
			auto result = e.run();
			b->environment->clear();
			return result;
		}
		else {
			return (*c_function)(params, envir);
		}
	}
}

