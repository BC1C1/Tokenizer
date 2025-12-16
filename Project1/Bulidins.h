#pragma once
#include "PFunction.h"
#include "fstream"
namespace Py {
	namespace Bulidins {
		using EPointer = std::shared_ptr<Py::Environment>;
		pointer bulidin_open(const pointer& params, EPointer envir);

		void register_open(EPointer globalEnvir);

		void register_all_function(EPointer globalEnvir);
}
}




