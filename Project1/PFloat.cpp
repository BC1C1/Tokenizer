#include "PFloat.h"
namespace Py {
	PFloat::PFloat(double val) : PObject(typeMap.at(Type::Float)), value(val)
	{
	}
	string PFloat::toString() const
	{
		return std::to_string(value);
	}
	double PFloat::getValue() const
	{
		return value;
	}
	void PFloat::setValue(double val)
	{
		value = val;
	}
	pointer PFloat::asInt() const
	{
		// 可能丢失数据
		return make_shared<PInt>(static_cast<int>(value));
	}
	pointer PFloat::asFloat() const
	{
		return make_shared<PFloat>(value);
	}
	pointer PFloat::asBool() const
	{
		bool val = value != 0;
		return make_shared<PBool>(val);
	}
	pointer PFloat::asList() const
	{
		vector<pointer> iterable{ asFloat() };
		return make_shared<PList>(std::move(iterable));
	}
	pointer PFloat::__add__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PFloat>(value + double(intOther->getValue()));
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			return make_shared<PFloat>(value + floatOther->getValue());
		}
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			int val = boolOther->getValue() ? 1 : 0;
			return make_shared<PInt>(static_cast<int>(value + double(val)));
		}
		case Type::Undefined:
		default:
			defaultOpError("+", other);
		}
		return nullptr;
	}
	pointer PFloat::__sub__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PFloat>(value - intOther->getValue());
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			return make_shared<PFloat>(double(value) - floatOther->getValue());
		}
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			int val = boolOther->getValue() ? 1 : 0;
			return make_shared<PInt>(static_cast<int>(value - val));
		}
		case Type::None: {
			return make_shared<PFloat>(-value);
		}
		case Type::Undefined:
		default:
			defaultOpError("-", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__mul__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PFloat>(value * intOther->getValue());
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			return make_shared<PFloat>(value * floatOther->getValue());
		}
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			int val = boolOther->getValue() ? 1 : 0;
			return make_shared<PInt>(static_cast<int>(value * val));
		}
		case Type::Undefined:
		default:
			defaultOpError("*", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__truediv__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			if (intOther->getValue() == 0)
				throw runtime_error("不可除以0");
			return make_shared<PFloat>(value / intOther->getValue());
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			if (floatOther->getValue() == 0)
				throw runtime_error("不可除以0");
			return make_shared<PFloat>(value / floatOther->getValue());
		}
		case Type::Undefined:
		default:
			defaultOpError("/", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__pow__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			if (intOther->getValue() == 0)
				throw runtime_error("不可除以0");
			return make_shared<PFloat>(pow(value, intOther->getValue()));
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			if (floatOther->getValue() == 0)
				throw runtime_error("不可除以0");
			return make_shared<PFloat>(pow(value, floatOther->getValue()));
		}
		case Type::Undefined:
		default:
			defaultOpError("**", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__eq__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			bool val = value == intOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			bool val = value == floatOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("==", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__ne__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			bool val = value != intOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			bool val = value != floatOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("!=", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__lt__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			bool val = value < intOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			bool val = value < floatOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError(">", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__le__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			bool val = value <= intOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			bool val = value <= floatOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError("<=", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__gt__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			bool val = value > intOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			bool val = value > floatOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError(">", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
	pointer PFloat::__ge__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			bool val = value >= intOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			bool val = value >= floatOther->getValue();
			return make_shared<PBool>(val);
		}
		case Type::Undefined:
		default:
			defaultOpError(">=", other);
			return nullptr; // 无实际作用仅用于消除警告
		}
		return nullptr;
	}
}