#include "PNone.h"

namespace Py
{
	PNone::PNone() : PObject(typeMap.at(Type::None))
	{
	}

	string PNone::toString() const
	{
		return "None";
	}
}

