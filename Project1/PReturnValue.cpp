#include "PReturnValue.h"
namespace Py {
	PReturnValue::PReturnValue(pointer value) 
		: PObject(typeMap.at(Type::ReturnValue)), value(value)
	{
	}

	string PReturnValue::toString() const
	{
		return value->toString();
	}
}

