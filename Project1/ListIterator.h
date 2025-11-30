#pragma once
#include "TypeDefine.h"
#include "Iterator.h"
namespace Py {
	class ListIterator : public Iterator
	{
	public:
		// 这个传入的是受控的对象，本身是const但是下属元素不const
		ListIterator(pointer list);

		virtual pointer get() const override;
		virtual pointer get(int index) override;

		virtual void next() override;

		virtual void before() override;

		virtual bool isEnd() const override;

		virtual bool isRend() const override;

		void reset();
		void rreset();

		~ListIterator() = default;
	private:
		int index;
		shared_ptr<PList> list;
	};
}



