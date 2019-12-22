#ifndef JSONPP_H
#define JSONPP_H

#include <glibmm/ustring.h>
#include <variant>
#include <map>
#include <vector>
#include "jsonFlexLexer.h"

namespace json {
	extern const std::string utf8;
	extern const std::string null;

	typedef Glib::ustring String;
	typedef double Number;
	typedef bool Boolean;
#pragma GCC visibility push(default)
	class Null { };
	class Object;
	class Array;
	typedef std::variant<Null, String, Number, Object, Array, Boolean> Value;
	typedef std::map<std::string, Value, std::less<>> M;
	class Object : public M {
		public:
			using M::M;
	};
	typedef std::vector<Value> A;
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

