#ifndef JSON_H
#define JSON_H

#include <glibmm/ustring.h>
#include <memory>
#include <boost/variant.hpp>
#include <map>
#include <list>
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
	class Value;
	typedef std::shared_ptr<Value> ValuePtr;
	typedef std::map<std::string, ValuePtr> Object;
	typedef std::list<ValuePtr> Array;
	typedef boost::variant<Null, String, Number, Object, Array, Boolean> VT;
	class Value : public VT {
		public:
			Value() : VT(Null()) { }

			template <class X>
			Value(const X & x) : VT(x) { }
	};

	Value parseValue(std::istream &);
	Value parseValue(std::istream &, const std::string & encoding);
	Value parseValue(const Glib::ustring & s);
	Value parseValue(Glib::ustring::const_iterator & s);

	void serializeValue(const Value &, std::ostream & s, const std::string & encoding);
}
#pragma GCC visibility pop

#endif

