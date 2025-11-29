#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	using std::string;
	class PNone : public Py::PObject
	{
	public:
		PNone();
		virtual string toString() const override;
	};
}
