#pragma once
#include "PFunction.h"
#include "fstream"
namespace Py {
	namespace Bulidins {
		using EPointer = std::shared_ptr<Py::Environment>;
		pointer bulidin_print(const pointer& params, EPointer envir);

		void register_print(EPointer globalEnvir);


		// 在整体运行中调用下面这个函数来注册所有内置函数
		void register_all_function(EPointer globalEnvir);
}
}




