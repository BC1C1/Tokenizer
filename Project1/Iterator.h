#pragma once
#include "PObject.h"
namespace Py {
	class Iterator : public PObject
	{
	public:
		Iterator();

		virtual ~Iterator() = default;

		virtual pointer get() = 0;
		virtual pointer get(int index) = 0;

		virtual void next() = 0;

		virtual void before() = 0;

		virtual bool isEnd() const = 0;

		virtual bool isRend() const = 0;

		virtual string toString() const override;
	};
}



