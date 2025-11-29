#include "Iterator.h"
namespace Py {
	Iterator::Iterator() : PObject(typeMap.at(Type::Iterator))
	{
	}
	string Iterator::toString() const
	{
		return "Iterator";
	}
}

