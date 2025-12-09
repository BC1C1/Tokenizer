#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	class PInstance : public PObject
	{
	public:
		PInstance(PClass* parent); 

		// 一些基本getset方法
	private:
		// raw pointer not shared_ptr to avoid cycle referance
		PClass* pclass; // pointer to class which create it.

		std::unordered_map<string, pointer> members;

	};
}


