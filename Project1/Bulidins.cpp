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
			printf("\n");
			return make_shared<PNone>();
		}
		pointer bulidin_range(const pointer& params, EPointer envir) {
			auto list = dynamic_pointer_cast<PList>(params);
			if (!list) throw std::runtime_error("range参数必须是列表");

			int first = 0, second = 0, stepLength = 1;
			auto iter = params->__iter__();
			auto iterator = dynamic_pointer_cast<ListIterator>(iter);
			if (!iterator) throw std::runtime_error("range参数不可迭代");
			if (list->size() == 1) {
				auto secondObj = iterator->get();
				if (secondObj->getType().type != Py::Type::Int) 
					throw std::runtime_error("参数必须是int");
				second = dynamic_pointer_cast<PInt>(secondObj)->getValue();
			}
			else if (list->size() == 2) {
				auto firstObj = iterator->get();
				if (firstObj->getType().type != Py::Type::Int) 
					throw std::runtime_error("参数必须是int");
				first = dynamic_pointer_cast<PInt>(firstObj)->getValue();

				auto secondObj = iterator->get();
				if (secondObj->getType().type != Py::Type::Int) 
					throw std::runtime_error("参数必须是int");
				second = dynamic_pointer_cast<PInt>(secondObj)->getValue();
			}
			else if (list->size() == 3) {
				auto firstObj = iterator->get();
				if (firstObj->getType().type != Py::Type::Int) 
					throw std::runtime_error("参数必须是int");
				first = dynamic_pointer_cast<PInt>(firstObj)->getValue();

				auto secondObj = iterator->get();
				if (secondObj->getType().type != Py::Type::Int) 
					throw std::runtime_error("参数必须是int");
				second = dynamic_pointer_cast<PInt>(secondObj)->getValue();

				auto stepLengthObj = iterator->get();
				if (stepLengthObj->getType().type != Py::Type::Int) 
					throw std::runtime_error("参数必须是int");
				stepLength = dynamic_pointer_cast<PInt>(stepLengthObj)->getValue();
			}
			else {
				throw std::runtime_error("参数数量错误（range仅支持1/2/3个参数）");
			}
			vector<pointer> content;
			for (int i = first; i < second; i += stepLength) {
				content.push_back(make_shared<PInt>(i));
			}
			return make_shared<PList>(std::move(content));
		}
		void register_print(EPointer globalEnvir)
		{
			vector<string> params{ "print_list" };
			auto funcObj = make_shared<InnerFunc>(bulidin_print);
			auto func = make_shared<PFunction>(params, nullptr, false, funcObj);
			globalEnvir->createThere("print", func);
		}
		void register_range(EPointer globalEnvir)
		{
			vector<string> params{ "begin", "end", "stepLength" };
			auto funcObj = make_shared<InnerFunc>(bulidin_range);
			auto func = make_shared<PFunction>(
				params, 
				nullptr, 
				false, 
				funcObj
			);
			globalEnvir->createThere("range", func);
		}
		void register_all_function(EPointer globalEnvir)
		{
			register_print(globalEnvir);
			register_range(globalEnvir);
		}
	}
}


