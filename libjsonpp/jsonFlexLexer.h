#ifndef JSONFLEXLEXER_H
#define JSONFLEXLEXER_H

#include <string>
#include "jsonpp.h"
#include <stack>
#include <functional>
#ifndef FLEX_SCANNER
#include <FlexLexer.h>
#endif

namespace json {
	class jsonFlexLexer : public yyFlexLexer {
		public:
			jsonFlexLexer(std::istream &, std::string enc, Value & v);

			int yylex() override;
			void LexerError(const char * msg) override;

			void BeginObject();
			void BeginArray();

			void PushBoolean(bool);
			void PushNumber(double);
			void PushNull();
			void PushText(std::string &&);

			void EndArray();
			void EndObject();

		private:
			std::string encodeBuf() const;

			std::string buf, name;
			const std::string encoding;

			using AcceptValue = std::function<Value *(Value &&)>;
			std::stack<AcceptValue> acceptValues;
	};
}

#endif

