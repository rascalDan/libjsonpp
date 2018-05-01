%option batch
%option c++
%option noyywrap
%option 8bit
%option stack
%option yylineno
%option yyclass="json::jsonFlexLexer"
%option prefix="jsonBase"

%{
#include "jsonFlexLexer.h"
#include <boost/lexical_cast.hpp>
#pragma GCC diagnostic ignored "-Wsign-compare"
%}

beginobj "{"
endobj "}"
beginarray "["
endarray "]"
beginstr "\""
endstr "\""
true "true"
false "false"
bool {true|false}
null "null"
number [-+]?[0-9]+(\.[0-9]+)?
colon ":"
separator ","
escape "\\"

%x OBJECT_ITEM
%x OBJECT_NEXT
%x ARRAY_ITEM
%x ARRAY_NEXT
%x COLON
%x TEXT
%x VALUE
%x STRING
%x ESCAPE

%%

<ARRAY_ITEM,VALUE>{true} {
	PushBoolean(true);
	yy_pop_state();
}

<ARRAY_ITEM,VALUE>{false} {
	PushBoolean(false);
	yy_pop_state();
}

<ARRAY_ITEM,VALUE>{number} {
	PushNumber(boost::lexical_cast<double>(YYText()));
	yy_pop_state();
}

<ARRAY_ITEM,VALUE>{null} {
	PushNull();
	yy_pop_state();
}

<ARRAY_ITEM,VALUE,OBJECT_ITEM>{beginstr} {
	yy_push_state(STRING);
}

<ARRAY_ITEM,VALUE>{beginobj} {
	BeginObject();
	BEGIN(OBJECT_ITEM);
}

<ARRAY_ITEM,VALUE>{beginarray} {
	BeginArray();
	BEGIN(ARRAY_NEXT);
	yy_push_state(ARRAY_ITEM);
}

<STRING>{endstr} {
	yy_pop_state();
	switch (YY_START) {
		case ARRAY_ITEM:
		case VALUE:
			PushText(encodeBuf());
			yy_pop_state();
			break;
		case OBJECT_ITEM:
			name = encodeBuf();
			BEGIN(COLON);
			break;
	}
	buf.clear();
}

<OBJECT_NEXT>{endobj} {
	PushObject();
	yy_pop_state();
}

<ARRAY_ITEM>{endarray} {
	PushArray();
	yy_pop_state();
	yy_pop_state();
}

<ARRAY_NEXT>{endarray} {
	PushArray();
	yy_pop_state();
}

<COLON>{colon} {
	BEGIN(OBJECT_NEXT);
	yy_push_state(VALUE);
}

<OBJECT_NEXT>{separator} {
	BEGIN(OBJECT_ITEM);
}

<ARRAY_NEXT>{separator} {
	yy_push_state(VALUE);
}

<STRING>{escape} {
	yy_push_state(ESCAPE);
}

<ESCAPE>"\"" { buf += "\""; yy_pop_state(); }
<ESCAPE>"\\" { buf += "\\"; yy_pop_state(); }
<ESCAPE>"/" { buf += "/"; yy_pop_state(); }
<ESCAPE>"b" { buf += "\b"; yy_pop_state(); }
<ESCAPE>"f" { buf += "\f"; yy_pop_state(); }
<ESCAPE>"n" { buf += "\n"; yy_pop_state(); }
<ESCAPE>"r" { buf += "\r"; yy_pop_state(); }
<ESCAPE>"t" { buf += "\t"; yy_pop_state(); }

<ESCAPE>"u"[0-9a-fA-Z]{4} {
	buf += Glib::ustring(1, gunichar(strtol(YYText() + 1, NULL, 16)));
	yy_pop_state();
}

<STRING>. {
	buf += YYText();
}

<*>[ \t\r\n\f] {
}

<*>. {
	throw ParseError(YYText(), yylineno, YY_START);
}

%%

json::jsonFlexLexer::jsonFlexLexer(std::istream & in, const std::string & enc) :
	yyFlexLexer(&in, NULL),
	encoding(enc)
{
	yy_push_state(VALUE);
  acceptValues.push([this](const auto & value) {
		return values.emplace(std::make_shared<Value>(value)).get();
  });
}

