#define BOOST_TEST_MODULE encoding
#include <boost/test/unit_test.hpp>

#include "jsonpp.h"
#include <fstream>
#include <filesystem>

#define XSTR(s) STR(s)
#define STR(s) #s
const std::filesystem::path root(XSTR(ROOT));

static
std::string
writeString(const json::Value & v, const std::string & enc)
{
	std::stringstream ss;
	json::serializeValue(v, ss, enc);
	return ss.str();
}

BOOST_AUTO_TEST_CASE( parse_latin1 )
{
	std::stringstream ss("\"A \xD9\xF1\xEE\xE7\xF4\xD0\xE8 string.\"");
	BOOST_REQUIRE_EQUAL(19, ss.str().length());
	BOOST_REQUIRE_EQUAL("A ÙñîçôÐè string.", std::get<Glib::ustring>(json::parseValue(ss, "latin1")));
}

BOOST_AUTO_TEST_CASE( write_latin1 )
{
	BOOST_REQUIRE_EQUAL("\"A \xD9\xF1\xEE\xE7\xF4\xD0\xE8 string.\"", writeString(Glib::ustring("A ÙñîçôÐè string."), "latin1"));
}

BOOST_AUTO_TEST_CASE( defaultEncoding )
{
	json::String s("Some string value");
	std::stringstream ss;
	json::serializeValue(s, ss, "");
	BOOST_REQUIRE_EQUAL(ss.str(), R"S("Some string value")S");
}

