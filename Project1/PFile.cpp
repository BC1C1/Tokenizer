#include "PFile.h"
namespace Py {
	PFile::PFile(const string& filename, char mode)
		: PObject(typeMap.at(Type::File)), filename(filename), mode(mode),
		isOpen(false), PIfs(nullptr), POfs(nullptr)
	{
		switch (mode)
		{
		case 'r': {
			PIfs = make_shared<std::ifstream>();
			PIfs->open(filename);
			if (!PIfs->is_open())
				throw std::runtime_error("打开文件: " + filename + "失败");
			isOpen = true;
			break;
		}
		case 'w': {
			break;
		}
		default:
			break;
		}
	}

}
