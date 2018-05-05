#ifndef JSON_H
#define JSON_H

#include <glibmm/ustring.h>
#include <variant>
#include <map>
#include <vector>
#include <stdexcept>

#pragma GCC visibility push(default)
namespace json {
	class ParseError : public std::invalid_argument {
		public:
			ParseError(const char *, int, int);
	};

	typedef Glib::ustring String;
	typedef double Number;
	typedef bool Boolean;
	class Null { };
	class Object;
	class Array;
	typedef std::variant<Null, String, Number, Object, Array, Boolean> Value;
	typedef std::map<std::string, Value> M;
	class Object : public M {
		public:
			using M::M;
	};
	typedef std::vector<Value> A;
	class Array : public A {
		public:
			using A::A;
	};

	static_assert(std::is_move_constructible<Value>::value);
	static_assert(std::is_nothrow_move_constructible<Object>::value);
	static_assert(std::is_nothrow_move_constructible<Array>::value);
	static_assert(std::is_move_assignable<Value>::value);
	static_assert(std::is_nothrow_move_assignable<Object>::value);
	static_assert(std::is_nothrow_move_assignable<Array>::value);

	Value parseValue(std::istream &);
	Value parseValue(std::istream &, const std::string & encoding);
	Value parseValue(const Glib::ustring & s);
	Value parseValue(Glib::ustring::const_iterator & s);

	void serializeValue(const Value &, std::ostream & s, const std::string & encoding);
}
#pragma GCC visibility pop

#endif

