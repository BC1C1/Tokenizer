#include "PBool.h"

namespace Py {
	PBool::PBool(bool val) : PObject(typeMap.at(Type::Bool)), value(val)
	{
	}
	string PBool::toString() const
	{
		return value ? "True" : "False";
	}
	bool PBool::getValue() const
	{
		return value;
	}
	void PBool::setValue(bool val)
	{
		value = val;
	}
	pointer PBool::__eq__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			bool val = value == boolOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("==", other);
		}
	}
	pointer PBool::__ne__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			bool val = value != boolOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("!=", other);
		}
	}
	pointer PBool::__lt__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			bool val = value < boolOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("<", other);
		}
	}
	pointer PBool::__le__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			bool val = value <= boolOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("<=", other);
		}
	}
	pointer PBool::__gt__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			bool val = value > boolOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError(">", other);
		}
	}
	pointer PBool::__ge__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			bool val = value >= boolOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError(">=", other);
		}
	}
	pointer PBool::__and__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			bool val = value && boolOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("and", other);
		}
	}
	pointer PBool::__or__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			bool val = value || boolOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("or", other);
		}
	}
	pointer PBool::__not__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::None: {
			return make_shared<PBool>(!value);
		}
		case Type::Undefined:
		default:
			defaultOpError("!=", other);
		}
	}
}
