#define BOOST_TEST_MODULE serialise
#include <boost/test/unit_test.hpp>

#include "jsonpp.h"
#include <fstream>
#include <boost/filesystem/path.hpp>

#define XSTR(s) STR(s)
#define STR(s) #s
const boost::filesystem::path root(XSTR(ROOT));

template <typename T>
static
json::ValuePtr
jvp(const T & t)
{
	return json::ValuePtr(new json::Value(t));
}

static
std::string
writeString(const json::Value & v)
{
	std::stringstream ss;
	json::serializeValue(v, ss, "utf-8");
	return ss.str();
}

BOOST_AUTO_TEST_CASE( serialise_true )
{
	BOOST_REQUIRE_EQUAL("true", writeString(true));
}

BOOST_AUTO_TEST_CASE( serialise_false )
{
	BOOST_REQUIRE_EQUAL("false", writeString(false));
}

BOOST_AUTO_TEST_CASE( serialise_0 )
{
	BOOST_REQUIRE_EQUAL("0", writeString(0.0));
}

BOOST_AUTO_TEST_CASE( serialise_48 )
{
	BOOST_REQUIRE_EQUAL("48", writeString(48.0));
}

BOOST_AUTO_TEST_CASE( serialise_neg48 )
{
	BOOST_REQUIRE_EQUAL("-48", writeString(-48.0));
}

BOOST_AUTO_TEST_CASE( serialise_pi )
{
	BOOST_REQUIRE_EQUAL("3.14159", writeString(3.14159));
}

BOOST_AUTO_TEST_CASE( serialise_emptyArray )
{
	BOOST_REQUIRE_EQUAL("[]", writeString(json::Array()));
}

BOOST_AUTO_TEST_CASE( serialise_emptyObject )
{
	BOOST_REQUIRE_EQUAL("{}", writeString(json::Object()));
}

BOOST_AUTO_TEST_CASE( serialise_null )
{
	BOOST_REQUIRE_EQUAL("null", writeString(json::Null()));
}

BOOST_AUTO_TEST_CASE( serialise_unicode )
{
	BOOST_REQUIRE_EQUAL("\"A Űņĩćőđē string.\"", writeString(Glib::ustring("A Űņĩćőđē string.")));
}

BOOST_AUTO_TEST_CASE( serialise_whitespace )
{
	BOOST_REQUIRE_EQUAL("\"\\r\\n\\t\"", writeString(Glib::ustring("\r\n\t")));
}

BOOST_AUTO_TEST_CASE( serialise_quotes )
{
	BOOST_REQUIRE_EQUAL("\"string with \\\" in\"", writeString(Glib::ustring("string with \" in")));
}

BOOST_AUTO_TEST_CASE( serialise_complexObject )
{
	BOOST_REQUIRE_EQUAL("{\"a\":\"string\",\"b\":null,\"c\":64,\"d\":true,\"e\":{\"suba\":\"suba-val\",\"subb\":\"subb-val\"},\"f\":[true,false,-4.9,\"item\"]}", writeString(json::Object({
					{"a", jvp(Glib::ustring("string"))},
					{"b", jvp(json::Null())},
					{"c", jvp(64.0)},
					{"d", jvp(true)},
					{"e", jvp(
								json::Object({
									{"suba", jvp(Glib::ustring("suba-val"))},
									{"subb", jvp(Glib::ustring("subb-val"))}
									})
								)},
					{"f", jvp(
								json::Array({
									jvp(true),
									jvp(false),
									jvp(-4.9),
									jvp(Glib::ustring("item"))
									})
								)},
					})));
}

