#include "PFunction.h"
namespace Py {
	PFunction::PFunction(vector<string>& params, AST::pointer block, EPointer definition_env)
		: PObject(typeMap.at(Type::FunctionDefine)), 
		params(params), block(block),
		definition_env(definition_env)
	{
	}
	string PFunction::toString() const
	{
		return "Function";
	}
	shared_ptr<AST::ANode> PFunction::__call__(const pointer& params, EPointer envir)
	{
		auto list = dynamic_pointer_cast<Py::PList>(params);
		// 参数检查
		if (list->size() != this->params.size())
			throw std::runtime_error("参数不匹配");
		auto iter = dynamic_pointer_cast<Py::Iterator>(list->__iter__());
		// 要闭包可以在这里把definitionEnvir的参数先createThere
		if (!this->definition_env)
			throw std::runtime_error("定义环境缺失");
		auto& defineVars = this->definition_env->vars;
		for (auto& iterator : defineVars) {
			envir->createThere(iterator.first, iterator.second);
		}
		// 在本环境中赋值
		for (auto& name : this->params) {
			envir->createThere(name, iter->get());
		}
		// 返回block
		return this->block;
	}
	AST::pointer PFunction::getBlock() const
	{
		return block;
	}
}

