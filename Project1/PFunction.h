#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	class PFunction : public PObject
	{
	public:
		// define构造的时候需要参数表和执行方法
		PFunction(vector<string>&& params, AST::pointer block);

		// 执行函数的时候Call结点保存函数名和参数表，
		// 执行的时候传参数，从调用函数的环境中获取引用，然后shared_from_this地
		// 传过来，不用担心可变对象什么的，只要有名字就是可变对象，不可变对象
		// 在ast分析赋值语句的时候就直接报错了
		// 传过来以后就按照block记录的run就可以了，直接利用run函数即可，PFuntion
		// 不需要内置run函数

		virtual string toString() const override;
	private:
		vector<string> params;
		AST::pointer block;
	};
}


