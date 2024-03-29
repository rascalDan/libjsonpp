#include "jsonpp.h"
#include "ustring_wrap.h"
#include <glibmm/convert.h>
#include <iomanip>
#include <ios>

namespace json {
	class JsonSerialize {
	public:
		JsonSerialize(std::ostream & out, const std::string & encoding) : s(out), e(encoding)
		{
			s << std::boolalpha // for Boolean
			  << std::defaultfloat // for Number
			  << std::setfill('0') // for String \uNNNN
					;
		}
		void
		// NOLINTNEXTLINE(misc-no-recursion)
		operator()(const Value & v) const
		{
			std::visit(*this, v);
		}
		void
		operator()(const std::string & str) const
		{
			(*this)(str, e);
		}
		void
		operator()(const String & str) const
		{
			(*this)(str, e);
		}
		void
		operator()(const String & str, const std::string & enc) const
		{
			if (!enc.empty()) {
				serializeString(Glib::convert(str, enc, utf8));
			}
			else {
				serializeString(str);
			}
		}
		void
		serializeString(const String & str) const
		{
			s << '"';
			for (auto i = str.begin(); i != str.end();) {
				const auto start = i;
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
					const gunichar & c = *i;
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
							s << "\\u" << std::setw(4) << std::hex << c << std::setw(1);
							break;
					}
					i++;
				}
			}
			s << '"';
		}
		void
		operator()(const Number & n) const
		{
			s << std::dec << n;
		}
		void
		// NOLINTNEXTLINE(misc-no-recursion)
		operator()(const Array & a) const
		{
			s << '[';
			for (const Array::value_type & v : a) {
				if (&v != &*a.begin()) {
					s << ',';
				}
				(*this)(v);
			}
			s << ']';
		}
		void
		// NOLINTNEXTLINE(misc-no-recursion)
		operator()(const Object & o) const
		{
			s << '{';
			for (const Object::value_type & v : o) {
				if (&v != &*o.begin()) {
					s << ',';
				}
				(*this)(v.first);
				s << ':';
				(*this)(v.second);
			}
			s << '}';
		}
		void
		operator()(const Null &) const
		{
			s << null;
		}
		void
		operator()(const Boolean & b) const
		{
			s << b;
		}

	private:
		std::ostream & s;
		const std::string & e;
	};

	void
	serializeValue(const Value & v, std::ostream & s, const std::string & enc)
	{
		JsonSerialize(s, enc)(v);
	}
}
