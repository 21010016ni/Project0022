#pragma once
#include <string>
#include <DxLib.h>

namespace ext
{
	constexpr size_t find_first_of_mb(const std::string& t, const char _Ch, size_t _Off = 0)
	{
		while((_Off = t.find_first_of(_Ch, _Off)) != std::string::npos)
		{
			if (_Off == 0 || GetCharBytes(DX_CHARCODEFORMAT_SHIFTJIS, (unsigned char*)&t[_Off - 1]) == 1)
				break;
			++_Off;
		}
		return _Off;
	}
	constexpr size_t find_first_of_mb(const std::string& t, const char* _Ch, size_t _Off = 0)
	{
		while ((_Off = t.find_first_of(_Ch, _Off)) != std::string::npos)
		{
			if (_Off == 0 || GetCharBytes(DX_CHARCODEFORMAT_SHIFTJIS, (unsigned char*)&t[_Off - 1]) == 1)
				break;
			++_Off;
		}
		return _Off;
	}
	constexpr size_t find_last_of_mb(const std::string& t, const char _Ch, size_t _Off = std::string::npos)
	{
		while((_Off = t.find_last_of(_Ch, _Off)) != std::string::npos)
		{
			if(_Off == 0 || GetCharBytes(DX_CHARCODEFORMAT_SHIFTJIS, (unsigned char*)&t[_Off - 1]) == 1)
				break;
			--_Off;
		}
		return _Off;
	}
	constexpr size_t find_last_of_mb(const std::string& t, const char* _Ch, size_t _Off = std::string::npos)
	{
		while((_Off = t.find_last_of(_Ch, _Off)) != std::string::npos)
		{
			if(_Off == 0 || GetCharBytes(DX_CHARCODEFORMAT_SHIFTJIS, (unsigned char*)&t[_Off - 1]) == 1)
				break;
			--_Off;
		}
		return _Off;
	}
}

