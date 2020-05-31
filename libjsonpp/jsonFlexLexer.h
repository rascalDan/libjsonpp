#ifndef JSONFLEXLEXER_H
#define JSONFLEXLEXER_H

#include <stdexcept>
#include <string>
#ifndef FLEX_SCANNER
#	define yyFlexLexer jsonBaseFlexLexer
#	include <FlexLexer.h>
#endif

namespace json {
#pragma GCC visibility push(default)
	class ParseError : public std::invalid_argument {
	public:
		ParseError(const char *, int, int);
	};

	class jsonFlexLexer : public yyFlexLexer {
	public:
		jsonFlexLexer(std::istream &, std::string enc);

		int yylex() override;

	private:
		virtual void BeginObject() = 0;
		virtual void BeginArray() = 0;

		virtual void PushBoolean(bool) = 0;
		virtual void PushNumber(double) = 0;
		virtual void PushNull() = 0;
		virtual void PushText(std::string &&) = 0;
		virtual void PushKey(std::string &&) = 0;

		virtual void EndArray() = 0;
		virtual void EndObject() = 0;

		void LexerError(const char * msg) override;
		std::string encodeBuf() const;

		std::string buf;
		const std::string encoding;
	};
#pragma GCC visibility pop
}

#endif
