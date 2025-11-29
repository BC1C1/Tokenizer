#include "PStr.h"
namespace Py {
	PStr::PStr(const string& val) : PObject(typeMap.at(Type::Str)), value(val)
	{
	}
	string PStr::toString() const
	{
		return value;
	}
	string PStr::getValue() const
	{
		return value;
	}
	void PStr::setValue(const string& val)
	{
		value = val;
	}
	pointer PStr::__add__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Py::Type::Str: {
			auto strOther = dynamic_pointer_cast<PStr>(other);
			if (!strOther)
				throw runtime_error("尝试转换为Str时失败");
			return make_shared<PStr>(value + strOther->getValue());
		}
		case Py::Type::Undefined:
		default:
			defaultOpError("+", other);
		}
	}
	pointer PStr::__mul__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Py::Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther)
				throw runtime_error("尝试转换为Int时失败");
			string val = "";
			for (int i = 0; i < intOther->getValue(); ++i)
				val += value;
			return make_shared<PStr>(val);
		}
		case Py::Type::Undefined:
		default:
			defaultOpError("+", other);
		}
	}
	pointer PStr::__eq__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Py::Type::Str: {
			auto strOther = dynamic_pointer_cast<PStr>(other);
			if (!strOther)
				throw runtime_error("尝试转换为Str时失败");
			bool val = value == strOther->getValue();
			return make_shared<PBool>(val);
		}
		case Py::Type::Undefined:
		default:
			defaultOpError("==", other);
		}
	}
	pointer PStr::__ne__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Py::Type::Str: {
			auto strOther = dynamic_pointer_cast<PStr>(other);
			if (!strOther)
				throw runtime_error("尝试转换为Str时失败");
			bool val = value != strOther->getValue();
			return make_shared<PBool>(val);
		}
		case Py::Type::Undefined:
		default:
			defaultOpError("!=", other);
		}
	}
	pointer PStr::__lt__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Py::Type::Str: {
			auto strOther = dynamic_pointer_cast<PStr>(other);
			if (!strOther)
				throw runtime_error("尝试转换为Str时失败");
			bool val = value < strOther->getValue();
			return make_shared<PBool>(val);
		}
		case Py::Type::Undefined:
		default:
			defaultOpError("<", other);
		}
	}
	pointer PStr::__le__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Py::Type::Str: {
			auto strOther = dynamic_pointer_cast<PStr>(other);
			if (!strOther)
				throw runtime_error("尝试转换为Str时失败");
			bool val = value <= strOther->getValue();
			return make_shared<PBool>(val);
		}
		case Py::Type::Undefined:
		default:
			defaultOpError("<=", other);
		}
	}
	pointer PStr::__gt__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Py::Type::Str: {
			auto strOther = dynamic_pointer_cast<PStr>(other);
			if (!strOther)
				throw runtime_error("尝试转换为Str时失败");
			bool val = value > strOther->getValue();
			return make_shared<PBool>(val);
		}
		case Py::Type::Undefined:
		default:
			defaultOpError(">", other);
		}
	}
	pointer PStr::__ge__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Py::Type::Str: {
			auto strOther = dynamic_pointer_cast<PStr>(other);
			if (!strOther)
				throw runtime_error("尝试转换为Str时失败");
			bool val = value >= strOther->getValue();
			return make_shared<PBool>(val);
		}
		case Py::Type::Undefined:
		default:
			defaultOpError(">=", other);
		}
	}
}
