#include <pch.hpp>
#include "jsonpp.h"

namespace json {
	class ParseError { };
	String parseString(Glib::ustring::const_iterator & s) {
		while (Glib::Unicode::isspace(*s)) s++;
		if (*s++ != '"') throw ParseError();
		String str;
		while (*s != '"') {
			if (*s == '\\') {
				switch (*s) {
					case '"':
						str += '"';
						break;
					case '\\':
						str += '\\';
						break;
					case '/':
						str += '/';
						break;
					case 'b':
						str += gunichar(8);
						break;
					case 'f':
						str += gunichar(12);
						break;
					case 'n':
						str += gunichar(10);
						break;
					case 'r':
						str += gunichar(13);
						break;
					case 't':
						str += gunichar(9);
						break;
					case 'u':
						{
							unsigned int c = 0;
							for (int n = 0; n < 4; n += 1) {
								c *= 16;
								if (*s >= '0' && *s <= '9') {
									c += (*s - '0');
								}
								else if (*s >= 'a' && *s <= 'f') {
									c += (*s - 'a');
								}
								else if (*s >= 'A' && *s <= 'F') {
									c += (*s - 'A');
								}
								else {
									throw ParseError();
								}
								s++;
							}
							str += gunichar(c);
							s--;
						}
						break;
					default:
						throw ParseError();
				}
			}
			else {
				str += *s;
			}
			s++;
		}
		if (*s++ != '"') throw ParseError();
		return str;
	}
	Number parseNumber(Glib::ustring::const_iterator & s) {
		while (Glib::Unicode::isspace(*s)) s++;
		bool neg = false;
		double v = 0;
		if (*s == '-') {
			neg = true;
			s++;
		}
		bool dot = false, e = false;
		double frac = 1;
		unsigned int digits = 0;
		while (true) {
			if (Glib::Unicode::isdigit(*s)) {
				if (dot) {
					frac /= 10;
					v += (frac * (*s - '0'));
				}
				else {
					v *= 10;
					v += (*s - '0');
					digits += 1;
				}
			}
			else if (*s == '.') {
				if (dot || e) throw ParseError();
				dot = true;
			}
			else if (*s == 'e' || *s == 'E') {
				e = true;
				s++;
				bool eneg = false;
				if (*s == '+') {
				}
				else if (*s == '-') {
					eneg = true;
					s++;
				}
				int ev = 0;
				while (Glib::Unicode::isdigit(*s)) {
					ev *= 10;
					ev += (*s - '0');
					s++;
				}
				while (ev--) {
					if (eneg) {
						v /= 10;
					}
					else {
						v *= 10;
					}
				}
				break;
			}
			else {
				break;
			}
			s++;
		}
		if (digits < 1) throw ParseError();
		return neg ? -v : v;
	}
	Value parseValue(Glib::ustring::const_iterator & s) {
		while (Glib::Unicode::isspace(*s)) s++;
		switch (*s) {
			case '"':
				return parseString(s);
			case '{':
				return parseObject(s);
			case '[':
				return parseArray(s);
			case 'n':
				return parseNull(s);
			case 't':
			case 'f':
				return parseBoolean(s);
			default:
				return parseNumber(s);
		}
	}
	Object parseObject(const Glib::ustring & s) {
		Glib::ustring::const_iterator i = s.begin();
		Object o = parseObject(i);
		if (i != s.end()) throw ParseError();
		return o;
	}
	Object parseObject(Glib::ustring::const_iterator & s) {
		Object o;
		while (Glib::Unicode::isspace(*s)) s++;
		if (*s != '{') throw ParseError();
		do {
			s++;
			while (Glib::Unicode::isspace(*s)) s++;
			if (*s == '}') return o;
			String key = parseString(s);
			while (Glib::Unicode::isspace(*s)) s++;
			if (*s++ != ':') throw ParseError();
			if (!o.insert(Object::value_type(key, ValuePtr(new Value(parseValue(s))))).second) throw ParseError();
			while (Glib::Unicode::isspace(*s)) s++;
		} while (*s == ',');
		if (*s == '}') {
			s++;
			while (Glib::Unicode::isspace(*s)) s++;
			return o;
		}
		throw ParseError();
	}
	Array parseArray(Glib::ustring::const_iterator & s) {
		Array a;
		while (Glib::Unicode::isspace(*s)) s++;
		if (*s != '[') throw ParseError();
		do {
			s++;
			while (Glib::Unicode::isspace(*s)) s++;
			if (*s == ']') {
				s++;
				return a;
			}
			a.push_back(ValuePtr(new Value(parseValue(s))));
			while (Glib::Unicode::isspace(*s)) s++;
		} while (*s == ',');
		if (*s == ']') {
			s++;
			return a;
		}
		throw ParseError();
	}
	Null parseNull(Glib::ustring::const_iterator & s) {
		if (*s++ != 'n') throw ParseError();
		if (*s++ != 'u') throw ParseError();
		if (*s++ != 'l') throw ParseError();
		if (*s++ != 'l') throw ParseError();
		return Null();
	}
	Boolean parseBoolean(Glib::ustring::const_iterator & s) {
		if (*s == 't') {
			s++;
			if (*s++ != 'r') throw ParseError();
			if (*s++ != 'u') throw ParseError();
			if (*s++ != 'e') throw ParseError();
			return true;
		}
		else if (*s == 'f') {
			s++;
			if (*s++ != 'a') throw ParseError();
			if (*s++ != 'l') throw ParseError();
			if (*s++ != 's') throw ParseError();
			if (*s++ != 'e') throw ParseError();
			return false;
		}
		throw ParseError();
	}
}
