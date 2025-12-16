#include "Bulidins.h"
namespace Py {
	namespace Bulidins {
		// 当前print的params是类似print('a=',a)这样的
		pointer bulidin_print(const pointer& params, EPointer envir)
		{
			auto iter = params->__iter__();
			auto iterator = dynamic_pointer_cast<ListIterator>(iter);
			if (!iterator) throw std::runtime_error("无效的参数");
			while (!iterator->isEnd()) {
				auto obj = iterator->get();
				printf("%s ", obj->toString().c_str());
			}
			printf("\n这是全局函数调用\n");
			return make_shared<PNone>();
		}
		void register_print(EPointer globalEnvir)
		{
			vector<string> params{ "print_list" };
			auto funcObj = make_shared<InnerFunc>(bulidin_print);
			auto func = make_shared<PFunction>(params, nullptr, false, funcObj);
			globalEnvir->createThere("print", func);
		}
		void register_all_function(EPointer globalEnvir)
		{
			register_print(globalEnvir);
		}
	}
}


