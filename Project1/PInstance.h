#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	class PInstance : public PObject
	{
	public:
		PInstance(PClass* parent, const pointer params, shared_ptr<Environment> envir); 

		// 一些基本getset方法
		virtual pointer __getattribute__(const string& attributeName) override;
		virtual void __setattribute__(const string& attributeName, const pointer& obj) override;

		pointer getMember(const string& name);

		virtual string toString() const override;

	private:
		// raw pointer not shared_ptr to avoid cycle referance
		PClass* pclass; // pointer to class which create it.

		std::unordered_map<string, pointer> members;
		shared_ptr<Environment> currentEnvir;
	};
}


