#include "PObject.h"
namespace Py {
	using pointer = shared_ptr<PObject>;
	std::string TypeToString(Type type) {
		switch (type)
		{
		case Type::Int: return "Int";
		case Type::Float: return "Float";
		case Type::Str: return "Str";
		case Type::Bool: return "Bool";
		case Type::None: return "None";
		default: return "Undefined";
		}
	}

	PyType::PyType(Type type = Type::Undefined) : type(type)
	{
	}

	std::string PyType::__str__()
	{
		return TypeToString(type);
	}

	PObject::PObject(PyType type) : type(type)
	{
	}

	PyType PObject::getType() const
	{
		return type;
	}

	PObject::~PObject()
	{
	}

	pointer PObject::__add__(const pointer& other) const
	{
		defaultOpError("+", other);
		return pointer();
	}

	pointer PObject::__sub__(const pointer& other) const
	{
		defaultOpError("-", other);
		return pointer();
	}

	pointer PObject::__mul__(const pointer& other) const
	{
		defaultOpError("*", other);
		return pointer();
	}

	pointer PObject::__truediv__(const pointer& other) const
	{
		defaultOpError("/", other);
		return pointer();
	}

	pointer PObject::__mod__(const pointer& other) const
	{
		defaultOpError("%", other);
		return pointer();
	}

	pointer PObject::__pow__(const pointer& other) const
	{
		defaultOpError("**", other);
		return pointer();
	}

	pointer PObject::__eq__(const pointer& other) const
	{
		defaultOpError("==", other);
		return pointer();
	}

	pointer PObject::__ne__(const pointer& other) const
	{
		defaultOpError("!=", other);
		return pointer();
	}

	pointer PObject::__lt__(const pointer& other) const
	{
		defaultOpError("<", other);
		return pointer();
	}

	pointer PObject::__le__(const pointer& other) const
	{
		defaultOpError("<=", other);
		return pointer();
	}

	pointer PObject::__gt__(const pointer& other) const
	{
		defaultOpError(">", other);
		return pointer();
	}

	pointer PObject::__ge__(const pointer& other) const
	{
		defaultOpError(">=", other);
		return pointer();
	}

	pointer PObject::__and__(const pointer& other) const
	{
		defaultOpError("and", other);
		return pointer();
	}

	pointer PObject::__or__(const pointer& other) const
	{
		defaultOpError("or", other);
		return pointer();
	}

	pointer PObject::__not__(const pointer& other) const
	{
		defaultOpError("not", other);
		return pointer();
	}

	Environment::Environment(EPointer parent) : parent(parent)
	{
	}

	Environment::~Environment()
	{
	}
	void Environment::assign(const string& name, shared_ptr<PObject> value)
	{
		auto iter = vars.find(name);
		if (iter != vars.end()) {
			vars[name] = value;
			return;
		}
		// 查父表
		auto currentEnvir = this;
		while (currentEnvir->parent) {
			currentEnvir = currentEnvir->parent.get();
			auto it = currentEnvir->vars.find(name);
			if (it != currentEnvir->vars.end()) {
				currentEnvir->vars[name] = value;
				return;
			}
		}
		vars[name] = value;
	}
	shared_ptr<PObject> Environment::getObj(const string& name)
	{
		auto iter = vars.find(name);
		if (iter != vars.end()) {
			return iter->second;
		}
		shared_ptr<PObject> ret = nullptr;
		if (parent) {
			ret = parent->getObj(name);
			if (ret) return ret;
		}
		throw runtime_error("变量 " + name + " 未定义");
	}
	pointer PObject::defaultOpError(const std::string& op, const pointer& other) const
	{
		throw runtime_error("没有匹配的运算符");
	}

	
}

