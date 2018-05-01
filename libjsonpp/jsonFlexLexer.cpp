#include <FlexLexer.h>
#include "jsonFlexLexer.h"
#include <glibmm/convert.h>

namespace json {
	jsonFlexLexer::jsonFlexLexer(std::istream & in, const std::string & enc) :
		yyFlexLexer(&in, NULL),
		encoding(enc)
	{
		yy_push_state(0);
		acceptValues.push([this](const auto & value) {
			return values.emplace(std::make_shared<Value>(value)).get();
		});
	}

	ValuePtr
	jsonFlexLexer::getValue() const
	{
		return values.top();
	}

	std::string
	jsonFlexLexer::encodeBuf() const
	{
		if (!encoding.empty()) {
			return Glib::convert(buf, "utf-8", encoding);
		}
		return buf;
	}

	void
	jsonFlexLexer::BeginObject()
	{
		auto object = std::get_if<Object>(acceptValues.top()(Object()));
		acceptValues.push([object,this](const auto & value) {
			return object->insert_or_assign(name, std::make_shared<Value>(value)).first->second.get();
		});
	}

	void
	jsonFlexLexer::BeginArray()
	{
		auto array = std::get_if<Array>(acceptValues.top()(Array()));
		acceptValues.push([array](const auto & value) {
			return array->emplace_back(std::make_shared<Value>(value)).get();
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
	jsonFlexLexer::PushText(const std::string & value)
	{
		acceptValues.top()(value);
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

