#include "PList.h"
namespace Py {
	Py::PList::PList() : PObject(typeMap.at(Type::List))
	{
	}
	PList::PList(vector<pointer>&& iterable) : PObject(typeMap.at(Type::List)),
		iterable(iterable)
	{
	}
	PList::PList(std::initializer_list<pointer> iterable) : PObject(typeMap.at(Type::List)),
		iterable(iterable)
	{
	}
	pointer PList::get(int index)
	{
		if (index < 0 || index >= size())
			throw std::runtime_error("list out of range");
		return iterable[index];
	}
	pointer PList::operator[](int index) const
	{
		return iterable[index];
	}
	int PList::size() const
	{
		return iterable.size();
	}
	string PList::toString() const
	{
		string result = "[ ";
		for (int i = 0; i < size(); i++) {
			result += iterable[i]->toString();
			if (i != size() - 1) {
				result += ", ";
			}
		}
		return result + " ]";
	}
	pointer PList::__eq__(const pointer& other) const
	{
		auto otherList = dynamic_pointer_cast<PList>(other);
		if (!otherList || otherList->size() != size())
			return make_shared<PBool>(false);
		for (int i = 0; i < size(); i++) {
			if (this->iterable[i]->__ne__(otherList->iterable[i]))
				return make_shared<PBool>(false);
		}
		return make_shared<PBool>(true);
	}
	pointer PList::__iter__()
	{
		return make_shared<ListIterator>(shared_from_this());
	}
	void PList::__setitem__(const pointer& index, pointer obj)
	{
		iterable[findIndex(index)] = obj;
	}
	pointer PList::__getitem__(const pointer& index)
	{
		return this->iterable[findIndex(index)];
	}
	int PList::findIndex(const pointer& index)
	{
		auto type = index->getType().type;
		int indexValue = 0;
		double floatVal = 0.0f;
		switch (type)
		{
		case Py::Type::Int:
			indexValue = dynamic_pointer_cast<PInt>(index)->getValue();
			break;
		case Py::Type::Float:
			floatVal = dynamic_pointer_cast<PFloat>(index)->getValue();
			if (floatVal != std::floor(floatVal)) {
				throw std::runtime_error("类型错误：列表索引浮点数必须是整数形式（如 3.0），当前是 " + std::to_string(floatVal));
			}
			indexValue = static_cast<int>(floatVal);
			break;
		case Py::Type::Bool:
			indexValue = dynamic_pointer_cast<PInt>(index->asInt())->getValue();
			break;
		default:
			throw std::runtime_error("类型错误：列表索引必须是整数、整数形式浮点数或布尔值，当前是 '" + index->getType().__str__() + "'");
			break;
		}
		if (indexValue < 0)
			indexValue += size();
		if (indexValue >= size() || indexValue < 0)
			throw std::runtime_error("列表索引越界：索引 " + std::to_string(indexValue) +
				" 超出范围 [0, " + std::to_string(iterable.size() - 1) + "]");
		return indexValue;
	}
}

