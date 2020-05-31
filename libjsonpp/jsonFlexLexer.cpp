#include "jsonValueFlexLexer.h"
#include <glibmm/convert.h>

namespace json {
	jsonFlexLexer::jsonFlexLexer(std::istream & in, std::string enc) :
		yyFlexLexer(&in, nullptr), encoding(enc != utf8 ? std::move(enc) : std::string())
	{
		yy_push_state(0);
	}

	std::string
	jsonFlexLexer::encodeBuf() const
	{
		if (!encoding.empty()) {
			return Glib::convert(buf, utf8, encoding);
		}
		return buf;
	}

	void
	jsonFlexLexer::LexerError(const char * msg)
	{
		throw ParseError(msg, 0, 0);
	}

	ParseError::ParseError(const char * at, int l, int s) :
		std::invalid_argument(Glib::ustring::compose("Parse error at or near %1 (line %2, state %3)", at, l, s))
	{
	}
}
