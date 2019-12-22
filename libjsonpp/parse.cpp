#include "jsonpp.h"
#include "jsonValueFlexLexer.h"

namespace json {
	Value parseValue(std::istream & s) {
		return parseValue(s, std::string());
	}

	Value parseValue(std::istream & s, const std::string & enc) {
		Value v;
		jsonValueFlexLexer jfl(s, enc, v);
		while (jfl.yylex()) {}
		return v;
	}

	Value parseValue(const Glib::ustring & s) {
		std::stringstream stream(s);
		return parseValue(stream);
	}

	Value parseValue(Glib::ustring::const_iterator & s) {
		Glib::ustring::const_iterator start = s;
		while (*s++) {}
		return parseValue(Glib::ustring(start, --s));
	}
}

