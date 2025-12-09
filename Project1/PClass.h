#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	class PClass : public PObject
	{
	public:
		PClass(const string& className);

		// 实例化方法，params默认用PList
		// python似乎不接受多个构造函数这回事，那就直接用这个表，默认参数还没做先不管
		// 默认参数这里实现不了，但是在PFunction那里解决这个问题就完全ok了，思路
		// 的话PFunction内部保存默认参数表？在传参的时候有参数按照参数，没有按照默认的？
		virtual pointer __instance__(const pointer& params) override;

		void addMember(const string& name, pointer memeber);

		pointer getMember(const string& name) const;

	private:
		string className;
		std::unordered_map<string, pointer> members;
		// cache, initialize it when create a PClass, need a default function
		pointer initFunc; 
		
	};
}


