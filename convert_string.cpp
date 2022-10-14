#include "convert_string.hpp"

std::string operator<<=(std::string& s, const std::u8string& u8)
{
	return s = (const char*)u8.c_str();
}

std::u8string operator<<=(std::u8string& u8, const std::string& s)
{
	return u8 = (const char8_t*)s.c_str();
}

std::string ext::convert(const std::u8string& u8)
{
	return (const char*)u8.c_str();
}

std::u8string ext::convert(const std::string& s)
{
	return (const char8_t*)s.c_str();
}

const char* ext::tochar(const std::u8string& u8)
{
	return (const char*)u8.c_str();
}

