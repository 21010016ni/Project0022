#include "convert_string.hpp"

std::string operator<<=(std::string& s, const std::u8string& u8)
{
	return s = reinterpret_cast<const char*>(u8.c_str());
}

std::u8string operator<<=(std::u8string& u8, const std::string& s)
{
	return u8 = reinterpret_cast<const char8_t*>(s.c_str());
}

std::string ext::convert(const std::u8string& u8)
{
	return reinterpret_cast<const char*>(u8.c_str());
}

std::u8string ext::convert(const std::string& s)
{
	return reinterpret_cast<const char8_t*>(s.c_str());
}

const char* ext::tochar(const std::u8string& u8)
{
	return reinterpret_cast<const char*>(u8.c_str());
}

const char8_t* ext::tochar(const std::string& s)
{
	return reinterpret_cast<const char8_t*>(s.c_str());
}

std::u8string ext::to_u8string(int v)
{
	return convert(std::to_string(v));
}

int ext::stoi(const std::u8string& u8)
{
	return std::stoi(convert(u8));
}

