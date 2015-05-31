#include <FlexLexer.h>
#include "jsonFlexLexer.h"
#include <boost/bind.hpp>
#include <glibmm/convert.h>

namespace json {
	ValuePtr
	jsonFlexLexer::getValue() const
	{
		return values.top();
	}

	Value *
	jsonFlexLexer::RootValue(const Value & value)
	{
		auto v = ValuePtr(new Value(value));
		values.push(v);
		return v.get();
	}

	Value *
	jsonFlexLexer::ArrayAppend(Array * array, const Value & value)
	{
		auto v = ValuePtr(new Value(value));
		array->push_back(v);
		return v.get();
	}

	Value *
	jsonFlexLexer::ObjectMember(Object * object, const Value & value)
	{
		auto v = ValuePtr(new Value(value));
		(*object)[name] = v;
		return v.get();
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
		auto object = boost::get<Object>(acceptValues.top()(Object()));
		acceptValues.push(boost::bind(&jsonFlexLexer::ObjectMember, this, object, _1));
	}

	void
	jsonFlexLexer::BeginArray()
	{
		auto array = boost::get<Array>(acceptValues.top()(Array()));
		acceptValues.push(boost::bind(&jsonFlexLexer::ArrayAppend, this, array, _1));
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

