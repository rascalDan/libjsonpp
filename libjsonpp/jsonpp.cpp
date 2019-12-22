#include "jsonpp.h"

namespace json {
	const std::string null("null");
	const std::string utf8("utf-8");

	static_assert(std::is_move_constructible<Value>::value);
	static_assert(std::is_nothrow_move_constructible<Object>::value);
	static_assert(std::is_nothrow_move_constructible<Array>::value);
	static_assert(std::is_move_assignable<Value>::value);
	static_assert(std::is_nothrow_move_assignable<Object>::value);
	static_assert(std::is_nothrow_move_assignable<Array>::value);
}

