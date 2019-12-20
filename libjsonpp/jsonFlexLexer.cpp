#include "jsonFlexLexer.h"
#include <glibmm/convert.h>

namespace json {
	const std::string UTF8 { "utf-8" };

	jsonFlexLexer::jsonFlexLexer(std::istream & in, std::string enc, Value & v) :
		yyFlexLexer(&in, nullptr),
		encoding(enc != UTF8 ? std::move(enc) : std::string())
	{
		yy_push_state(0);
		acceptValues.push([&v](auto && value) {
			v = std::move(value);
			return &v;
		});
	}

	std::string
	jsonFlexLexer::encodeBuf() const
	{
		if (!encoding.empty()) {
			return Glib::convert(buf, UTF8, encoding);
		}
		return buf;
	}

	void
	jsonFlexLexer::BeginObject()
	{
		auto object = std::get_if<Object>(acceptValues.top()(Object()));
		acceptValues.push([object,this](auto && value) {
			return &object->emplace(std::move(name), std::move(value)).first->second;
		});
	}

	void
	jsonFlexLexer::BeginArray()
	{
		auto array = std::get_if<Array>(acceptValues.top()(Array()));
		acceptValues.push([array](auto && value) {
			return &array->emplace_back(std::move(value));
		});
	}

	void
	jsonFlexLexer::PushNull()
	{
		acceptValues.top()(Null());
	}

	void
	jsonFlexLexer::PushBoolean(bool value)
	{
		acceptValues.top()(value);
	}

	void
	jsonFlexLexer::PushNumber(double value)
	{
		acceptValues.top()(value);
	}

	void
	jsonFlexLexer::PushText(std::string && value)
	{
		acceptValues.top()(std::move(value));
	}

	void
	jsonFlexLexer::PushArray()
	{
		acceptValues.pop();
	}

	void
	jsonFlexLexer::PushObject()
	{
		acceptValues.pop();
	}

	void
	jsonFlexLexer::LexerError(const char * msg)
	{
		throw ParseError(msg, 0, 0);
	}
}

