#ifndef JSONPP_H
#define JSONPP_H

#include "jsonFlexLexer.h"
#include "ustring_wrap.h"
#include <map>
#include <variant>
#include <vector>

namespace json {
	extern const std::string utf8;
	extern const std::string null;

	using String = Glib::ustring;
	using Number = double;
	using Boolean = bool;
#pragma GCC visibility push(default)
	class Null { };
	class Object;
	class Array;
	using Value = std::variant<Null, String, Number, Object, Array, Boolean>;
	using M = std::map<std::string, Value, std::less<>>;
	class Object : public M {
	public:
		using M::M;
	};
	using A = std::vector<Value>;
	class Array : public A {
	public:
		using A::A;
	};

	Value parseValue(std::istream &);
	Value parseValue(std::istream &, const std::string & encoding);
	Value parseValue(const Glib::ustring & s);
	Value parseValue(Glib::ustring::const_iterator & s);

	void serializeValue(const Value &, std::ostream & s, const std::string & encoding);
#pragma GCC visibility pop
}

#endif
