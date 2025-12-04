#pragma once
#include "PObject.h"
#include "TypeDefine.h"
namespace Py {
	using std::vector;
	using std::string;
	using EPointer = shared_ptr<Environment>;
	using NPointer = shared_ptr<AST::ANode>;
	class PFunction : public PObject
	{
	public:
		// define构造的时候需要参数表和执行方法
		PFunction(vector<string>& params, AST::pointer block, EPointer definition_env);

		virtual string toString() const override;

		virtual NPointer __call__(const pointer& params, EPointer envir) override;

		AST::pointer getBlock() const;

		const vector<string>& getParams() const;

		EPointer getDifinitionEnvir() const;

	private:
		vector<string> params;
		AST::pointer block; // 仅执行方法
		EPointer definition_env;
	};
}


