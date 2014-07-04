#ifndef JSON_H
#define JSON_H

#include <glibmm/ustring.h>
#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <map>
#include <list>

namespace json {
	typedef Glib::ustring String;
	typedef double Number;
	typedef bool Boolean;
	class Null { };
	class Value;
	typedef boost::shared_ptr<Value> ValuePtr;
	typedef std::map<std::string, ValuePtr> Object;
	typedef std::list<ValuePtr> Array;
	typedef boost::variant<Null, String, Number, Object, Array, Boolean> VT;
	class Value : public VT {
		public:
			Value() : VT(Null()) { }

			template <class X>
			Value(const X & x) : VT(x) { }
	};

	Object parseObject(Glib::ustring::const_iterator &);
	Object parseObject(const Glib::ustring &);
	String parseString(Glib::ustring::const_iterator & s);
	Number parseNumber(Glib::ustring::const_iterator & s);
	Boolean parseBoolean(Glib::ustring::const_iterator & s);
	Null parseNull(Glib::ustring::const_iterator & s);
	Value parseValue(Glib::ustring::const_iterator & s);
	Array parseArray(Glib::ustring::const_iterator &);

	void serializeObject(const Object &, std::ostream & s, const std::string & encoding);
	void serializeValue(const Value &, std::ostream & s, const std::string & encoding);
	void serializeArray(const Array &, std::ostream & s, const std::string & encoding);
	void serializeString(const String &, std::ostream & s, const std::string & encoding);
	void serializeNumber(const Number &, std::ostream & s, const std::string & encoding);
	void serializeBoolean(const Boolean &, std::ostream & s, const std::string & encoding);
	void serializeNull(const Null &, std::ostream & s, const std::string & encoding);
	Glib::ustring serializeObject(const Object &, const std::string & encoding);
}

#endif

