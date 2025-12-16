#pragma once
#include "PObject.h"
#include "TypeDefine.h"
#include "fstream"
namespace Py {
	class PFile : public PObject
	{
	public:
		PFile(const string& filename, char mode);

		pointer read();

		virtual string toString() const override;

	private:
		string filename;
		char mode;
		bool isOpen;
		shared_ptr<std::ifstream> PIfs; // read file stream
		shared_ptr<std::ofstream> POfs; // write file stream
	};
}


