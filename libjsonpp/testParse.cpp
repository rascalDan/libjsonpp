#define BOOST_TEST_MODULE parsing
#include <boost/test/unit_test.hpp>

#include "jsonpp.h"
#include <filesystem>
#include <fstream>

#define XSTR(s) STR(s)
#define STR(s) #s
const std::filesystem::path root(XSTR(ROOT));

BOOST_AUTO_TEST_CASE(parse_bool_true)
{
	const Glib::ustring val(" true ");
	BOOST_REQUIRE_EQUAL(true, std::get<bool>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_bool_false)
{
	const Glib::ustring val(" false ");
	BOOST_REQUIRE_EQUAL(false, std::get<bool>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_invalid_value)
{
	const Glib::ustring val(" meh ");
	BOOST_CHECK_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_int_1)
{
	const Glib::ustring val(" 1 ");
	BOOST_REQUIRE_EQUAL(1, std::get<json::Number>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_int_neg48)
{
	const Glib::ustring val(" -48 ");
	BOOST_REQUIRE_EQUAL(-48, std::get<json::Number>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_int_48)
{
	const Glib::ustring val(" 48 ");
	BOOST_REQUIRE_EQUAL(48, std::get<json::Number>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_float_pi)
{
	const Glib::ustring val(" 3.14159265359 ");
	BOOST_REQUIRE_CLOSE(3.14159, std::get<json::Number>(json::parseValue(val)), 0.001);
}

BOOST_AUTO_TEST_CASE(parse_array)
{
	const Glib::ustring val(" [ 1, 2, 3, [ 4, 5 ], {\"val1\": 6, \"val2\": [7, 8]}, 9, 10 ] ");
	auto arr = std::get<json::Array>(json::parseValue(val));
	BOOST_REQUIRE_EQUAL(7, arr.size());
	auto itr = arr.begin();
	BOOST_REQUIRE_EQUAL(1, std::get<json::Number>(*itr++));
	BOOST_REQUIRE_EQUAL(2, std::get<json::Number>(*itr++));
	BOOST_REQUIRE_EQUAL(3, std::get<json::Number>(*itr++));
	std::get<json::Array>(*itr++);
	std::get<json::Object>(*itr++);
	BOOST_REQUIRE_EQUAL(9, std::get<json::Number>(*itr++));
	BOOST_REQUIRE_EQUAL(10, std::get<json::Number>(*itr++));
}

BOOST_AUTO_TEST_CASE(parse_array_of_strings)
{
	const Glib::ustring val(" [ \"en\", \"de\", \"ro\", \"es\", \"fa\" ] ");
	const auto arr = std::get<json::Array>(json::parseValue(val));
	BOOST_REQUIRE_EQUAL(arr.size(), 5);
	BOOST_CHECK_EQUAL(std::get<json::String>(arr.front()), "en");
	BOOST_CHECK_EQUAL(std::get<json::String>(arr.back()), "fa");
}

BOOST_AUTO_TEST_CASE(parse_null)
{
	const Glib::ustring val(" null ");
	std::get<json::Null>(json::parseValue(val));
}

BOOST_AUTO_TEST_CASE(parse_empty_object)
{
	const Glib::ustring val(" {  } ");
	BOOST_REQUIRE(std::get<json::Object>(json::parseValue(val)).empty());
}

BOOST_AUTO_TEST_CASE(parse_broken_object_lone_number)
{
	const Glib::ustring val(" { 1 } ");
	BOOST_REQUIRE_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_broken_object_lone_string)
{
	const Glib::ustring val(" { \"string\" } ");
	BOOST_REQUIRE_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_broken_object_missing_value)
{
	const Glib::ustring val(" { \"string\": } ");
	BOOST_REQUIRE_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_broken_object_missing_second_name)
{
	const Glib::ustring val(" { \"string\": 1, } ");
	BOOST_REQUIRE_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_broken_object_missing_second_colon)
{
	const Glib::ustring val(" { \"string\": 1, \"string2\" } ");
	BOOST_REQUIRE_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_broken_object_missing_second_value)
{
	const Glib::ustring val(" { \"string\": 1, \"string2\": } ");
	BOOST_REQUIRE_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_broken_object_lone_bool)
{
	const Glib::ustring val(" { true } ");
	BOOST_REQUIRE_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_empty_array)
{
	const Glib::ustring val(" [  ] ");
	BOOST_REQUIRE(std::get<json::Array>(json::parseValue(val)).empty());
}

BOOST_AUTO_TEST_CASE(parse_empty_arrary_in_object)
{
	const Glib::ustring val(" { \"v1\": [ ], \"v2\": 100 } ");
	auto value = json::parseValue(val);
	BOOST_REQUIRE_EQUAL(3, value.index());
	auto obj = std::get<json::Object>(value);
	BOOST_REQUIRE_EQUAL(2, obj.size());
	BOOST_REQUIRE(std::get<json::Array>(obj["v1"]).empty());
	BOOST_REQUIRE_EQUAL(100, std::get<json::Number>(obj["v2"]));
}

BOOST_AUTO_TEST_CASE(parse_broken_array)
{
	const Glib::ustring val(" [ 1, 2, ] ");
	BOOST_CHECK_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_nonsense_in_array)
{
	const Glib::ustring val(" [ 1, 2, nonsense ] ");
	BOOST_CHECK_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_object)
{
	const Glib::ustring val(" { \"a\": 1, \"b\": 2 } ");
	auto value = json::parseValue(val);
	BOOST_REQUIRE_EQUAL(3, value.index());
	auto obj = std::get<json::Object>(value);
	BOOST_REQUIRE_EQUAL(2, obj.size());
	BOOST_REQUIRE_EQUAL(1, std::get<json::Number>(obj["a"]));
	BOOST_REQUIRE_EQUAL(2, std::get<json::Number>(obj["b"]));
}

BOOST_AUTO_TEST_CASE(parse_string_simple)
{
	const Glib::ustring val(" \"simple string\" ");
	BOOST_REQUIRE_EQUAL("simple string", std::get<json::String>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_object_withStringContainingQuote)
{
	const Glib::ustring val(" { \"key1\": \"value1\", \"key2\": \"value\\\"2\\\"\", \"key3\": 3 } ");
	auto obj = std::get<json::Object>(json::parseValue(val));
	BOOST_REQUIRE_EQUAL(3, obj.size());
	BOOST_REQUIRE_EQUAL("value1", std::get<json::String>(obj["key1"]));
	BOOST_REQUIRE_EQUAL("value\"2\"", std::get<json::String>(obj["key2"]));
	BOOST_REQUIRE_EQUAL(3, std::get<json::Number>(obj["key3"]));
}

BOOST_AUTO_TEST_CASE(parse_string_invalid_missingOpeningQuote)
{
	const Glib::ustring val(" simple string\" ");
	BOOST_CHECK_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_string_escapedQuote)
{
	const Glib::ustring val(" \"A \\\"quoted\\\" string.\" ");
	BOOST_REQUIRE_EQUAL("A \"quoted\" string.", std::get<json::String>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_string_escapedWhitespace)
{
	const Glib::ustring val(" \"A whitespace\\t\\r\\n\\b\\f string.\" ");
	BOOST_REQUIRE_EQUAL("A whitespace\t\r\n\b\f string.", std::get<json::String>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_string_escapedSlashes)
{
	const Glib::ustring val(" \"A whitespace\\\\ \\/ string.\" ");
	BOOST_REQUIRE_EQUAL("A whitespace\\ / string.", std::get<json::String>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_string_literalUnicode)
{
	const Glib::ustring val(" \"A Űņĩćőđē string.\" ");
	BOOST_REQUIRE_EQUAL("A Űņĩćőđē string.", std::get<json::String>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_string_unknownEscape)
{
	const Glib::ustring val(" \"A \\z string.\" ");
	BOOST_CHECK_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_string_shortUnicodeEscape)
{
	const Glib::ustring val(" \"A \\u017 string.\" ");
	BOOST_CHECK_THROW(json::parseValue(val), json::ParseError);
}

BOOST_AUTO_TEST_CASE(parse_string_escapedUnicode)
{
	const Glib::ustring val(" \"A \\u0170\\u0146\\u0129\\u0107\\u0151\\u0111\\u0113 string.\" ");
	BOOST_REQUIRE_EQUAL("A Űņĩćőđē string.", std::get<json::String>(json::parseValue(val)));
}

BOOST_AUTO_TEST_CASE(parse_sample_complexFile1)
{
	std::ifstream inFile((root / "initial" / "sample1.json").string());
	json::Value obj = json::parseValue(inFile, "utf-8");
	BOOST_CHECK_EQUAL(obj.index(), 3);
}

BOOST_AUTO_TEST_CASE(parse_sample_complexFile2)
{
	std::ifstream inFile((root / "initial" / "sample2.json").string());
	json::Value obj = json::parseValue(inFile, "utf-8");
	BOOST_CHECK_EQUAL(obj.index(), 3);
}

BOOST_AUTO_TEST_CASE(parse_sample_complexFile2_bulk, *boost::unit_test::disabled())
{
	for (int x = 0; x < 100; x++) {
		std::ifstream inFile((root / "initial" / "sample2.json").string());
		std::ignore = json::parseValue(inFile, "utf-8");
	}
}

BOOST_AUTO_TEST_CASE(parse_from_itr)
{
	const Glib::ustring val(" \"A \\u0170\\u0146\\u0129\\u0107\\u0151\\u0111\\u0113 string.\" ");
	auto itr = val.begin();
	BOOST_REQUIRE_EQUAL("A Űņĩćőđē string.", std::get<json::String>(json::parseValue(itr)));
}
