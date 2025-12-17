#include "PFunction.h"
namespace Py {
	PFunction::PFunction(vector<string> params,
		AST::pointer block,
		bool isUseBlock,
		FuncPointer func,
		std::unordered_map<string, AST::pointer>&& defaultValue)
		: PObject(typeMap.at(Type::FunctionDefine)), params(std::move(params)), block(block)
		, isUseBlock(isUseBlock), c_function(func), defaultValueMap(std::move(defaultValue))
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
		// 按顺序传，只会少参数，看看少几个就补充几个
		// 参数检查
		auto param = dynamic_pointer_cast<Py::PList>(params);
		if (!param)
			throw std::runtime_error("错误的参数表，转换成PList时失败");
		if (this->params.size() > param->size()) {
			for (size_t i = param->size(); i < this->params.size(); i++) {
				exe::Executor e(defaultValueMap[this->params[i]]);
				auto obj = e.run(envir);
				param->content().push_back(obj);
			}
		}
		if (this->params.size() != param->size())
			throw std::runtime_error("参数数量不匹配");
		if (isUseBlock) {
			auto iterator = dynamic_pointer_cast<Py::ListIterator>(param->__iter__());
			// 初始化block环境参数
			auto b = dynamic_pointer_cast<AST::Block>(block);
			// 环境用调用时候的环境
			b->environment->setParent(envir);
			for (auto& name : this->params) {
				b->environment->createThere(name, iterator->get());
			}
			exe::Executor e(block);
			auto result = e.run(b->environment);
			b->environment->clear();
			return result;
		}
		else {
			return (*c_function)(params, envir);
		}
	}
}

