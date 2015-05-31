#ifndef JSONFLEXLEXER_H
#define JSONFLEXLEXER_H

#include <string>
#include "jsonpp.h"
#include <stack>
#include <boost/function.hpp>

namespace json {
	class jsonFlexLexer : public yyFlexLexer {
		public:
			jsonFlexLexer(std::istream &, const std::string & enc);

			int yylex();
			ValuePtr getValue() const;
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
			Value * RootValue(const Value &);
			Value * ArrayAppend(Array *, const Value &);
			Value * ObjectMember(Object *, const Value &);

			std::string encodeBuf() const;

			std::string buf, name, encoding;
			std::stack<ValuePtr> values;

			typedef boost::function<Value *(const Value &)> AcceptValue;
			std::stack<AcceptValue> acceptValues;
	};
}

#endif

