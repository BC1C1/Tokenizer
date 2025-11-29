#pragma once
#include <string>
#include <map>
#include <map>
#include <memory>
#include <stdexcept>

namespace Py {
	using std::enable_shared_from_this;
	using std::dynamic_pointer_cast;
	using std::string;
	using std::map;
	using std::shared_ptr;
	using std::make_shared;
	using std::runtime_error;
	enum class Type {
		Int, Float, Str, Bool, None, Undefined, Iterator, List
	};
	std::string TypeToString(Type type);
	struct PyType {
		PyType(Type type);
		std::string __str__();
		Type type;
	};
	class PObject : public enable_shared_from_this<PObject>
	{
	public:
		using pointer = std::shared_ptr<Py::PObject>;
	public:
		PObject(PyType type);
		PyType getType() const;
		virtual ~PObject();
		virtual std::string toString() const = 0;

	public:
		// 算术运算符声明
		virtual pointer __add__(const pointer& other) const;
		virtual pointer __sub__(const pointer& other) const;
		virtual pointer __mul__(const pointer& other) const;
		virtual pointer __truediv__(const pointer& other) const;
		virtual pointer __mod__(const pointer& other) const;
		virtual pointer __pow__(const pointer& other) const;

		// 比较运算符声明
		virtual pointer __eq__(const pointer& other) const;
		virtual pointer __ne__(const pointer& other) const;
		virtual pointer __lt__(const pointer& other) const;
		virtual pointer __le__(const pointer& other) const;
		virtual pointer __gt__(const pointer& other) const;
		virtual pointer __ge__(const pointer& other) const;

		// 逻辑运算符声明
		virtual pointer __and__(const pointer& other) const;
		virtual pointer __or__(const pointer& other) const;
		virtual pointer __not__(const pointer& other) const;

		virtual pointer __iter__();

	protected:
		// 错误处理函数声明
		pointer defaultOpError(const std::string& op, const pointer& other) const;

		pointer defaultNoIterError() const;
	private:
		PyType type;
	};
	using pointer = std::shared_ptr<Py::PObject>;

	static const std::map<Type, PyType> typeMap = {
		{Type::Int, PyType(Type::Int)},
		{Type::Float, PyType(Type::Float)},
		{Type::Str, PyType(Type::Str)},
		{Type::Bool, PyType(Type::Bool)},
		{Type::None, PyType(Type::None)},
		{Type::Iterator, PyType(Type::Iterator)},
		{Type::List, PyType{Type::List}}
	};
	class PInt;
	class PFloat;
	class PBool;
	class PStr;
	class PNone;
	class Iterator;
	class PList;

	class Environment {
		using EPointer = std::shared_ptr<Environment>;
	public:
		Environment(EPointer parent = nullptr);
		map<string, shared_ptr<PObject>> vars;
		EPointer parent;
		~Environment();
		void assign(const string& name, shared_ptr<PObject> value);
		shared_ptr<PObject> getObj(const string& name);
	};
}


