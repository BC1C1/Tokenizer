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
		PList(vector<pointer>&& iterable);
		PList(std::initializer_list<pointer> iterable);

		pointer get(int index);
		pointer operator[](int index) const;

		int size() const;

		virtual string toString() const override;

		vector<pointer>& content();

	public:
		virtual pointer __eq__(const pointer& other) const override;

		virtual pointer __iter__() override;

		virtual void __setitem__(const pointer& index, pointer obj) override;
		virtual pointer __getitem__(const pointer& index) override;
	private:
		vector<pointer> iterable;

	private:
		int findIndex(const pointer& index);
	};
}


