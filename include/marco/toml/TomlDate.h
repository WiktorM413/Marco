#pragma once

#include "marco/toml/TomlError.h"
#include "marco/toml/TomlTime.h"
#include <string>


namespace Marco
{
	class TomlDate
	{
	public:
		TomlDate(unsigned int year = 0, unsigned int month = 0, unsigned int day = 0, std::string timeStartString = "", std::string timeEndString = "");

		TomlError FromString(const std::string& dateString);

		std::string AsString();

		unsigned int year;
		unsigned int month;
		unsigned int day;
		TomlTime offsetTimeStart;
		TomlTime offsetTimeEnd;
	};
}