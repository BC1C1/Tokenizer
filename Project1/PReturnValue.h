#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	class PReturnValue : public PObject
	{
	public:
		PReturnValue(pointer value);

		virtual string toString() const override;

	private:
		pointer value;

	};
}


