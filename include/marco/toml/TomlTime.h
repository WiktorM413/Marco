#pragma once

#include "marco/toml/TomlError.h"
#include <string>


namespace Marco
{
	class TomlTime
	{
	public:
		TomlTime(unsigned int hour = 0, unsigned int minute = 0, unsigned int second = 0, unsigned int millisecond = 0);

		
		TomlError FromString(const std::string& dateString);

		std::string AsString();
		
		bool IsWIthinBoundry();
		
		unsigned int hour;
		unsigned int minute;
		unsigned int second;
		unsigned int millisecond;
	};
}