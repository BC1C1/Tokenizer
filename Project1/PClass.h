#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	class PClass : public PObject
	{
	public:
		PClass(const string& className);

		virtual pointer __instance__(const pointer& params, shared_ptr<Environment> envir) override;

		void addMember(const string& name, pointer memeber);

		pointer getMember(const string& name) const;

		virtual string toString() const override;

	private:
		string className;
		std::unordered_map<string, pointer> members;
		// cache, initialize it when create a PClass, need a default function
		pointer initFunc; 
		
	};
}


