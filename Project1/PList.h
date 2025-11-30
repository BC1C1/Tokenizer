#pragma once
#include "PObject.h"
#include "TypeDefine.h"
#include "ListIterator.h"
namespace Py {
	using std::vector;
	class PList : public PObject
	{
	public:
		PList();
		PList(vector<pointer>&& list);
		PList(std::initializer_list<pointer> list);

		pointer get(int index);
		pointer operator[](int index) const;

		int size() const;

		virtual string toString() const override;

	public:
		virtual pointer __iter__() override;

	private:
		vector<pointer> list;
	};
}


