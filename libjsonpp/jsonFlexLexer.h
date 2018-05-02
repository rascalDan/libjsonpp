#ifndef JSONFLEXLEXER_H
#define JSONFLEXLEXER_H

#include <string>
#include "jsonpp.h"
#include <stack>
#include <functional>

namespace json {
	class jsonFlexLexer : public yyFlexLexer {
		public:
			jsonFlexLexer(std::istream &, const std::string & enc, Value & v);

			int yylex();
			void LexerError(const char * msg) override;

			void BeginObject();
			void BeginArray();

			void PushBoolean(bool);
			void PushNumber(double);
			void PushNull();
			void PushText(const std::string &);
			void PushArray();
			void PushObject();

		private:
			std::string encodeBuf() const;

			std::string buf, name, encoding;

			typedef std::function<Value *(const Value &)> AcceptValue;
			std::stack<AcceptValue> acceptValues;
	};
}

#endif

