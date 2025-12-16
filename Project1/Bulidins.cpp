#include "Bulidins.h"
using namespace Py;
namespace Bulidins {
	// file = open(file_name, mode='r', buffering=-1, encoding=None)
	// 先搞只有前两个参数的，第二个参数只限r
	pointer bulidin_open(const pointer& params, EPointer envir)
	{
		auto paramsList = dynamic_pointer_cast<PList>(params);
		// 参数检查
		if (paramsList->size() != 2) throw std::runtime_error("参数不匹配");
		auto iter = paramsList->__iter__();
		auto iterator = dynamic_pointer_cast<ListIterator>(iter);
		auto filenameObj = iterator->get(); // get会自动推进
		auto filename = dynamic_pointer_cast<PStr>(filenameObj)->getValue();
		auto modeObj = iterator->get();
		auto mode = dynamic_pointer_cast<PStr>(modeObj)->getValue();
		if (mode == "r") {
			return make_shared<PFile>(filename, 'r');
		}
		else if (mode == "w") {
			return make_shared<PFile>(filename, 'w');
		}
	}

	void register_open(EPointer globalEnvir)
	{
		vector<string> params{ "filename", "mode" };
		auto openFunc = make_shared<InnerFunc>(bulidin_open);
		auto obj = make_shared<PFunction>(params, nullptr, false, openFunc);
		globalEnvir->createThere("open", obj);
	}
	void register_all_function(EPointer globalEnvir)
	{
		register_open(globalEnvir);
	}
}

