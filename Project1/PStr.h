#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	class PStr : public PObject
	{
	public:
		PStr(const string& val);
		virtual string toString() const override;
		string getValue() const;
		void setValue(const string& val);
	public:
		// 算术运算符声明
		virtual pointer __add__(const pointer& other) const override;
		virtual pointer __mul__(const pointer& other) const override;

		// 比较运算符声明
		virtual pointer __eq__(const pointer& other) const override;
		virtual pointer __ne__(const pointer& other) const override;
		virtual pointer __lt__(const pointer& other) const override;
		virtual pointer __le__(const pointer& other) const override;
		virtual pointer __gt__(const pointer& other) const override;
		virtual pointer __ge__(const pointer& other) const override;

	private:
		string value;
	};
}


