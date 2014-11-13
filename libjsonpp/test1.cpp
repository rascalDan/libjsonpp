#include <pch.hpp>

#define BOOST_TEST_MODULE parsing
#include <boost/test/unit_test.hpp>

#include "jsonpp.h"
#include <fstream>
#include <boost/filesystem/path.hpp>

#define XSTR(s) STR(s)
#define STR(s) #s
const boost::filesystem::path root(XSTR(ROOT));

BOOST_AUTO_TEST_CASE( parse_bool_true )
{
	const Glib::ustring val(" true ");
	auto itr = val.begin();
	BOOST_REQUIRE_EQUAL(true, json::parseBoolean(itr));
}

BOOST_AUTO_TEST_CASE( parse_bool_false )
{
	const Glib::ustring val(" false ");
	auto itr = val.begin();
	BOOST_REQUIRE_EQUAL(false, json::parseBoolean(itr));
}

BOOST_AUTO_TEST_CASE( parse_bool_invalid )
{
	const Glib::ustring val(" meh ");
	auto itr = val.begin();
	BOOST_CHECK_THROW(json::parseBoolean(itr), json::ParseError );
}

BOOST_AUTO_TEST_CASE( parse_int_48 )
{
	const Glib::ustring val(" 48 ");
	auto itr = val.begin();
	BOOST_REQUIRE_EQUAL(48, json::parseNumber(itr));
}

BOOST_AUTO_TEST_CASE( parse_float_pi )
{
	const Glib::ustring val(" 3.14159265359 ");
	auto itr = val.begin();
	BOOST_REQUIRE_CLOSE(3.14159, json::parseNumber(itr), 0.001);
}

BOOST_AUTO_TEST_CASE( parse_array )
{
	const Glib::ustring val(" [ 1, 2, 3, [ 4, 5 ], {\"val\": 6} ] ");
	auto itr = val.begin();
	auto arr = json::parseArray(itr);
	BOOST_REQUIRE_EQUAL(5, arr.size());
}

BOOST_AUTO_TEST_CASE( parse_null )
{
	const Glib::ustring val(" null ");
	auto itr = val.begin();
	json::parseNull(itr);
}

BOOST_AUTO_TEST_CASE( parse_null_invalid )
{
	const Glib::ustring val(" meh ");
	auto itr = val.begin();
	BOOST_CHECK_THROW(json::parseBoolean(itr), json::ParseError);
}

BOOST_AUTO_TEST_CASE( parse_object )
{
	const Glib::ustring val(" { \"a\": 1, \"b\": 2 } ");
	auto itr = val.begin();
	auto obj = json::parseObject(itr);
	BOOST_REQUIRE_EQUAL(2, obj.size());
	BOOST_REQUIRE_EQUAL(1, boost::get<json::Number>(*obj["a"]));
	BOOST_REQUIRE_EQUAL(2, boost::get<json::Number>(*obj["b"]));
}

BOOST_AUTO_TEST_CASE( parse_string_simple )
{
	const Glib::ustring val(" \"simple string\" ");
	auto itr = val.begin();
	BOOST_REQUIRE_EQUAL("simple string", json::parseString(itr));
}

BOOST_AUTO_TEST_CASE( parse_object_withStringContainingQuote )
{
	const Glib::ustring val(" { \"key1\": \"value1\", \"key2\": \"value\\\"2\\\"\", \"key3\": 3 } ");
	auto itr = val.begin();
	auto obj = json::parseObject(itr);
	BOOST_REQUIRE_EQUAL(3, obj.size());
	BOOST_REQUIRE_EQUAL("value1", boost::get<json::String>(*obj["key1"]));
	BOOST_REQUIRE_EQUAL("value\"2\"", boost::get<json::String>(*obj["key2"]));
	BOOST_REQUIRE_EQUAL(3, boost::get<json::Number>(*obj["key3"]));
}

BOOST_AUTO_TEST_CASE( parse_string_invalid_missingOpeningQuote )
{
	const Glib::ustring val(" simple string\" ");
	auto itr = val.begin();
	BOOST_CHECK_THROW(json::parseString(itr), json::ParseError);
}

BOOST_AUTO_TEST_CASE( parse_string_escapedQuote )
{
	const Glib::ustring val(" \"A \\\"quoted\\\" string.\" ");
	auto itr = val.begin();
	BOOST_REQUIRE_EQUAL("A \"quoted\" string.", json::parseString(itr));
}

BOOST_AUTO_TEST_CASE( parse_sample_complexFile )
{
	std::ifstream inFile((root / "initial" / "sample1.json").string());
	std::stringstream buffer;
	buffer << inFile.rdbuf();
	Glib::ustring doc(buffer.str());
	Glib::ustring::const_iterator itr = doc.begin();
	json::Value obj = json::parseValue(itr);
}

