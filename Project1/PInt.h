#pragma once
#include "PObject.h"
#include "TypeDefine.h"
#include <cmath>
namespace Py {
	using std::string;
	using pointer = shared_ptr<PObject>;
	class PInt : public PObject
	{
	public:
		PInt(int val);
		virtual string toString() const override;
		int getValue() const;
		void setValue(int val);

		// 类型转换函数
		virtual pointer asInt() const override;
		virtual pointer asFloat() const override;
		virtual pointer asBool() const override;
		virtual pointer asList() const override;

	public:
		// 算术运算符声明
		virtual pointer __add__(const pointer& other) const override;
		virtual pointer __sub__(const pointer& other) const override;
		virtual pointer __mul__(const pointer& other) const override;
		virtual pointer __truediv__(const pointer& other) const override;
		virtual pointer __mod__(const pointer& other) const override;
		virtual pointer __pow__(const pointer& other) const override;

		// 比较运算符声明
		virtual pointer __eq__(const pointer& other) const override;
		virtual pointer __ne__(const pointer& other) const override;
		virtual pointer __lt__(const pointer& other) const override;
		virtual pointer __le__(const pointer& other) const override;
		virtual pointer __gt__(const pointer& other) const override;
		virtual pointer __ge__(const pointer& other) const override;

		// 逻辑运算符声明
		virtual pointer __and__(const pointer& other) const override;
		virtual pointer __or__(const pointer& other) const override;
		virtual pointer __not__(const pointer& other) const override;

	private:
		int value;
	};
}


