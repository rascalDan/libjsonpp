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
		jsonFlexLexer jfl(s, enc);
		while (jfl.yylex()) ;
		return *jfl.getValue();
	}

	Value parseValue(const Glib::ustring & s) {
		std::stringstream stream(s);
		return parseValue(stream);
	}

	Object parseObject(const Glib::ustring & s) {
		return boost::get<Object>(parseValue(s));
	}

	Value parseValue(Glib::ustring::const_iterator & s) {
		Glib::ustring::const_iterator start = s;
		while (*s++) ;
		return parseValue(Glib::ustring(start, --s));
	}
}

