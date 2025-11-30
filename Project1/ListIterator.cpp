#include "ListIterator.h"
namespace Py {
	ListIterator::ListIterator(pointer list) : Iterator(), index(0)
	{
		this->list = dynamic_pointer_cast<PList>(list);
	}
	pointer ListIterator::get() const
	{
		if (!isEnd()) {
			return list->get(index);
		}
		return make_shared<Py::PNone>();
	}
	pointer ListIterator::get(int index)
	{
		if (-list->size() <= index && index < list->size()) {
			return list->get(index);
		}
		return make_shared<Py::PNone>();
	}
	void ListIterator::next()
	{
		if (!isEnd()) {
			index++;
		}
	}
	void ListIterator::before()
	{
		if (!isRend()) {
			index--;
		}
	}
	bool ListIterator::isEnd() const
	{
		return index >= list->size();
	}
	bool ListIterator::isRend() const
	{
		return index < 0;
	}
	void ListIterator::reset()
	{
		index = 0;
	}
	void ListIterator::rreset()
	{
		index = list->size() - 1;
	}
}

