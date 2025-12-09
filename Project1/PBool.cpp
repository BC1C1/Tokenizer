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
	pointer PBool::asInt() const
	{
		auto val = value ? 1 : 0;
		return make_shared<PInt>(val);
	}
	pointer PBool::asFloat() const
	{
		auto val = value ? 1 : 0;
		return make_shared<PFloat>(val);
	}
	pointer PBool::asBool() const
	{
		return make_shared<PBool>(value);
	}
	pointer PBool::asList() const
	{
		vector<pointer> val{ asBool() };
		return make_shared<PList>(std::move(val));
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
		return nullptr;
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
		return nullptr;
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
		return nullptr;
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
		return nullptr;
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
		return nullptr;
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
		return nullptr;
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
		return nullptr;
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
		return nullptr;
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
		return nullptr;
	}
}
