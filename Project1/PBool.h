#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	class PBool : public PObject
	{
	public:
		PBool(bool val);
		virtual string toString() const override;
		bool getValue() const;
		void setValue(bool val);
	public:
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
		bool value;
	};
}


