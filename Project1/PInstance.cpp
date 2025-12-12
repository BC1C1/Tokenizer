#include "PInstance.h"
namespace Py {
	PInstance::PInstance(PClass* parent, const pointer params, shared_ptr<Environment> envir)
		: PObject(typeMap.at(Type::Instance)), pclass(parent), currentEnvir(envir)
	{
		// params 是初始化参数，在初始化实例的时候调用init以获得正确实例
		pclass->getMember("__init__")->__call__(params, envir);
	}

	pointer PInstance::__getattribute__(const string& attributeName)
	{
		auto ret = getMember(attributeName);
		if (!ret)
			throw std::runtime_error("类 " + pclass->toString() + "不存在属性" + attributeName);
		return ret;
	}

	void PInstance::__setattribute__(const string& attributeName, const pointer& obj)
	{
		members[attributeName] = obj;
	}

	pointer PInstance::getMember(const string& name)
	{
		auto iter = members.find(name);
		if (iter == members.end())
			return pclass->getMember(name);
		return iter->second;
	}

	string PInstance::toString() const
	{
		// 这里应当提供重载，但是这个还没有做到就先不管
		return pclass->toString() + "的实例化";
	}

}
