#include "PList.h"
namespace Py {
	Py::PList::PList() : PObject(typeMap.at(Type::List))
	{
	}
	PList::PList(vector<pointer>&& list) : PObject(typeMap.at(Type::List)),
		list(list)
	{
	}
	pointer PList::get(int index)
	{
		if (index < 0 || index >= size())
			throw std::runtime_error("list out of range");
		return list[index];
	}
	pointer PList::operator[](int index) const
	{
		return list[index];
	}
	int PList::size() const
	{
		return list.size();
	}
	string PList::toString() const
	{
		string result = "[ ";
		for (int i = 0; i < size(); i++) {
			result += list[i]->toString();
			if (i != size() - 1) {
				result += ", ";
			}
		}
		return result + " ]";
	}
	pointer PList::__iter__()
	{
		return make_shared<ListIterator>(shared_from_this());
	}
}

