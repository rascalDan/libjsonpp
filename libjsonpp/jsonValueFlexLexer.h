#ifndef JSONVALUEFLEXLEXER_H
#define JSONVALUEFLEXLEXER_H

#include "jsonFlexLexer.h"
#include "jsonpp.h"
#include <stack>
#include <functional>

namespace json {
	class jsonValueFlexLexer : public jsonFlexLexer {
		public:
			jsonValueFlexLexer(std::istream &, std::string enc, Value & v);

			void BeginObject() override;
			void BeginArray() override;

			void PushBoolean(bool) override;
			void PushNumber(double) override;
			void PushNull() override;
			void PushText(std::string &&) override;
			void PushKey(std::string &&) override;

			void EndArray() override;
			void EndObject() override;

		private:
			using AcceptValue = std::function<Value *(Value &&)>;
			std::stack<AcceptValue> acceptValues;
			std::string name;
	};
}

#endif

