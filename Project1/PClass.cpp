#include "PClass.h"

namespace Py {
	PClass::PClass(const string& className) : PObject(typeMap.at(Type::Class)),
		className(className)
	{
		// 构造一个空的init函数
		vector<string> params{ "self" };
		vector<AST::pointer> temp;
		auto block = make_shared<AST::Block>(temp);
		pointer default_func = make_shared<PFunction>(params, block);
		addMember("__init__", default_func);
		initFunc = default_func;
	}
	pointer PClass::__instance__(const pointer& params, shared_ptr<Environment> envir)
	{
		auto obj = make_shared<PInstance>(this, envir);
		vector<pointer> nowParams{ obj };
		auto& p = dynamic_pointer_cast<PList>(params)->content();
		nowParams.insert(nowParams.end(), p.begin(), p.end());
		auto nowList = make_shared<PList>(std::move(nowParams));
		this->initFunc->__call__(nowList, envir);
		return obj;
	}
	void PClass::addMember(const string& name, pointer member)
	{
		members[name] = member;
		if (name == "__init__")
			initFunc = member;
	}
	pointer PClass::getMember(const string& name) const
	{
		auto iter = members.find(name);
		if (iter == members.end())
			return nullptr;
		return iter->second;
	}
	string PClass::toString() const
	{
		return className;
	}
}

