#include "jsonValueFlexLexer.h"

namespace json {
	jsonValueFlexLexer::jsonValueFlexLexer(std::istream & in, std::string enc, Value & v) :
		jsonFlexLexer(in, std::move(enc))
	{
		acceptValues.push([&v](auto && value) {
			return &(v = std::forward<Value>(value));
		});
	}

	void
	jsonValueFlexLexer::BeginObject()
	{
		auto object = std::get_if<Object>(acceptValues.top()(Object()));
		acceptValues.push([object,this](auto && value) {
			return &object->emplace(std::move(name), std::forward<Value>(value)).first->second;
		});
	}

	void
	jsonValueFlexLexer::BeginArray()
	{
		auto array = std::get_if<Array>(acceptValues.top()(Array()));
		acceptValues.push([array](auto && value) {
			return &array->emplace_back(std::forward<Value>(value));
		});
	}

	void
	jsonValueFlexLexer::PushNull()
	{
		acceptValues.top()(Null());
	}

	void
	jsonValueFlexLexer::PushBoolean(bool value)
	{
		acceptValues.top()(value);
	}

	void
	jsonValueFlexLexer::PushNumber(double value)
	{
		acceptValues.top()(value);
	}

	void
	jsonValueFlexLexer::PushText(std::string && value)
	{
		acceptValues.top()(std::forward<std::string>(value));
	}

	void
	jsonValueFlexLexer::PushKey(std::string && value)
	{
		name = std::forward<std::string>(value);
	}

	void
	jsonValueFlexLexer::EndArray()
	{
		acceptValues.pop();
	}

	void
	jsonValueFlexLexer::EndObject()
	{
		acceptValues.pop();
	}
}

