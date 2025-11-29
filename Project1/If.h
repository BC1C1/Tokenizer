#pragma once
#include "TypeDefine.h"
namespace AST {
	using pointer = AST::pointer;
	class If : public ANode
	{
	public:
		// if构造方法
		If(pointer& condition, pointer& block, vector<pointer>&& elifs, pointer& Else);
		// elif构造方法
		If(pointer& condition, pointer& block);
		// else构造方法
		If(pointer& block);

		// 如果为nullptr就是不存在，类型由ANode构造时提供
		pointer condition; // 本if条件
		pointer block; // 本if代码块
		vector<pointer> elifs; // elif
		pointer Else; // else代码块

		bool isExecuted; // elif语句用于标记是否执行过
	};
}


