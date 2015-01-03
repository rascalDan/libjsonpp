#include <pch.hpp>
#include "jsonpp.h"
#include <glibmm/convert.h>

namespace json {
	class JsonSerialize : public boost::static_visitor<> {
		public:
			JsonSerialize(std::ostream & out, const std::string & encoding) :
				o(out),
				e(encoding) {
			}
			void operator()(const String & s) const {
				serializeString(s, o, e);
			}
			void operator()(const Number & s) const {
				serializeNumber(s, o, e);
			}
			void operator()(const Array & s) const {
				serializeArray(s, o, e);
			}
			void operator()(const Object & s) const {
				serializeObject(s, o, e);
			}
			void operator()(const Null & s) const {
				serializeNull(s, o, e);
			}
			void operator()(const Boolean & s) const {
				serializeBoolean(s, o, e);
			}
		private:
			std::ostream & o;
			std::string e;
	};

	void serializeObject(const Object & o, std::ostream & s, const std::string & renc) {
		std::string enc(renc == "utf-8" ? std::string() : renc);
		s << std::boolalpha;
		s << std::fixed;
		s << '{';
		for (const Object::value_type & v : o) {
			if (&v != &*o.begin()) {
				s << ',';
			}
			serializeString(v.first, s, enc);
			s << ':';
			serializeValue(*v.second, s, enc);
		}
		s << '}';
	}

	void serializeValue(const Value & v, std::ostream & s, const std::string & enc) {
		boost::apply_visitor(JsonSerialize(s, enc), v);
	}

	void serializeArray(const Array & a, std::ostream & s, const std::string & enc) {
		s << '[';
		for (const Array::value_type & v : a) {
			if (&v != &*a.begin()) {
				s << ',';
			}
			serializeValue(*v, s, enc);
		}
		s << ']';
	}

	void serializeString(const String & str, std::ostream & s) {
		s << '"';
		for (auto i = str.begin(); i != str.end(); ) {
			auto start = i;
			while (i != str.end() && *i >= 32 && *i != '/' && *i != '"' && *i != '\\') {
				i++;
			}
			if (start == str.begin() && i == str.end()) {
				s << str.raw();
				break;
			}
			else if (start != i) {
				s << Glib::ustring(start, i).raw();
			}
			while (i != str.end() && (*i < 32 || *i == '/' || *i == '"' || *i == '\\')) {
				gunichar c = *i;
				switch (c) {
					case '\f':
						s << "\\f";
						break;
					case '\t':
						s << "\\t";
						break;
					case '\n':
						s << "\\n";
						break;
					case '\b':
						s << "\\b";
						break;
					case '\r':
						s << "\\r";
						break;
					case '/':
						s << "\\/";
						break;
					case '\\':
						s << "\\\\";
						break;
					case '"':
						s << "\\\"";
						break;
					default:
						char buf[7];
						snprintf(buf, sizeof(buf), "\\u%04x", c);
						s << buf;
						break;
				}
				i++;
			}
		}
		s << '"';
	}

	void serializeString(const String & str, std::ostream & o, const std::string & encoding) {
		if (!encoding.empty()) {
			std::stringstream s;
			serializeString(str, s);
			o << Glib::convert(s.str(), encoding, "utf-8");
		}
		else {
			serializeString(str, o);
		}
	}

	void serializeNumber(const Number & n, std::ostream & s, const std::string & ) {
		s << n;
	}

	void serializeBoolean(const Boolean & b, std::ostream & s, const std::string & ) {
		s << b;
	}

	void serializeNull(const Null &, std::ostream & s, const std::string & ) {
		s << "null";
	}

	Glib::ustring serializeObject(const Object & o, const std::string & enc) {
		std::stringstream out;
		serializeObject(o, out, enc);
		return out.str();
	}
}

