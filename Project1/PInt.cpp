#include "PInt.h"
namespace Py {
	using pointer = std::shared_ptr<Py::PObject>;
	PInt::PInt(int val) : PObject(typeMap.at(Type::Int)), value(val)
	{
	}
	string PInt::toString() const
	{
		return std::to_string(value);
	}
	int PInt::getValue() const
	{
		return value;
	}
	void PInt::setValue(int val)
	{
		value = val;
	}
	pointer PInt::asInt() const
	{
		return make_shared<PInt>(value);
	}
	pointer PInt::asFloat() const
	{
		return make_shared<PFloat>(value);
	}
	pointer PInt::asBool() const
	{
		return make_shared<PBool>(value != 0);
	}
	pointer PInt::asList() const
	{
		vector<pointer> iterable{ asInt() };
		return make_shared<PList>(std::move(iterable));
	}
	pointer PInt::__add__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PInt>(value + intOther->getValue());
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			return make_shared<PFloat>(double(value) + floatOther->getValue());
		}
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			int val = boolOther->getValue() ? 1 : 0;
			return make_shared<PInt>(value + val);
		}
		case Type::Undefined:
		default:
			defaultOpError("+", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__sub__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PInt>(value - intOther->getValue());
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
			return make_shared<PInt>(value - val);
		}
		case Type::None: {
			return make_shared<PInt>(-value);
		}
		case Type::Undefined:
		default:
			defaultOpError("-", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__mul__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PInt>(value * intOther->getValue());
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			return make_shared<PFloat>(double(value) * floatOther->getValue());
		}
		case Type::Bool: {
			auto boolOther = dynamic_pointer_cast<PBool>(other);
			if (!boolOther) {
				throw runtime_error("尝试转换为Bool时失败");
			}
			int val = boolOther->getValue() ? 1 : 0;
			return make_shared<PInt>(value * val);
		}
		case Type::Undefined:
		default:
			defaultOpError("*", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__truediv__(const pointer& other) const
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
			return make_shared<PInt>(value / intOther->getValue());
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			if (floatOther->getValue() == 0)
				throw runtime_error("不可除以0");
			return make_shared<PFloat>(double(value) / floatOther->getValue());
		}
		case Type::Undefined:
		default:
			defaultOpError("/", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__mod__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PInt>(value % intOther->getValue());
		}
		case Type::Undefined:
		default:
			defaultOpError("%", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__pow__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PInt>(int(pow(value, intOther->getValue())));
		}
		case Type::Float: {
			auto floatOther = dynamic_pointer_cast<PFloat>(other);
			if (!floatOther) {
				throw runtime_error("尝试转换为Float时失败");
			}
			return make_shared<PFloat>(pow(double(value), floatOther->getValue()));
		}
		case Type::Undefined:
		default:
			defaultOpError("**", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__eq__(const pointer& other) const
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
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__ne__(const pointer& other) const
	{
		return __eq__(other)->__not__(make_shared<PNone>());
	}
	pointer PInt::__lt__(const pointer& other) const
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
			defaultOpError("<", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__le__(const pointer& other) const
	{
		return __lt__(other)->__or__(__eq__(other));
	}
	pointer PInt::__gt__(const pointer& other) const
	{
		return __le__(other)->__not__(make_shared<PNone>());
	}
	pointer PInt::__ge__(const pointer& other) const
	{
		return __lt__(other)->__not__(make_shared<PNone>());
	}
	pointer PInt::__and__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PInt>(value & intOther->getValue());
		}
		case Type::Undefined:
		default:
			defaultOpError("and", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__or__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::Int: {
			auto intOther = dynamic_pointer_cast<PInt>(other);
			if (!intOther) {
				throw runtime_error("尝试转换为Int时失败");
			}
			return make_shared<PInt>(value | intOther->getValue());
		}
		case Type::Undefined:
		default:
			defaultOpError("|", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
	pointer PInt::__not__(const pointer& other) const
	{
		auto type = other->getType().type;
		switch (type)
		{
		case Type::None: {
			return make_shared<PInt>(~value);
		}
		case Type::Undefined:
		default:
			defaultOpError("~", other);
			return nullptr; // 无效语句用于消除警告
		}
	}
}

