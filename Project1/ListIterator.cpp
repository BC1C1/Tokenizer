#include "ListIterator.h"
namespace Py {
	ListIterator::ListIterator(pointer iterable) : Iterator(), index(0)
	{
		this->iterable = dynamic_pointer_cast<PList>(iterable);
	}
	pointer ListIterator::get()
	{
		if (!isEnd()) {
			auto ret = iterable->get(index);
			this->index++;
			return ret;
		}
		return make_shared<Py::PNone>();
	}
	pointer ListIterator::get(int index)
	{
		if (-iterable->size() <= index && index < iterable->size()) {
			return iterable->get(index);
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
		return index >= iterable->size();
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
		index = iterable->size() - 1;
	}
}

