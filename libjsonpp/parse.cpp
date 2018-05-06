#include "jsonpp.h"
#include <FlexLexer.h>
#include "jsonFlexLexer.h"

namespace json {
	ParseError::ParseError(const char * at, int l, int s) :
		std::invalid_argument(Glib::ustring::compose("Parse error at or near %1 (line %2, state %3)", at, l, s))
	{
	}

	Value parseValue(std::istream & s) {
		return parseValue(s, std::string());
	}

	Value parseValue(std::istream & s, const std::string & enc) {
		Value v;
		jsonFlexLexer jfl(s, enc, v);
		while (jfl.yylex()) ;
		return v;
	}

	Value parseValue(const Glib::ustring & s) {
		std::stringstream stream(s);
		return parseValue(stream);
	}

	Value parseValue(Glib::ustring::const_iterator & s) {
		Glib::ustring::const_iterator start = s;
		while (*s++) ;
		return parseValue(Glib::ustring(start, --s));
	}
}

