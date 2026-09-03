#include "marco/toml/TomlTime.h"
#include "marco/toml/TomlError.h"
#include <expected>
#include <format>
#include <stdexcept>
#include <string>

static std::expected<int, Marco::TomlError> StringToInt(const std::string& s);

Marco::TomlTime::TomlTime(unsigned int hour, unsigned int minute, unsigned int second, unsigned int millisecond): hour(hour), minute(minute), second(second), millisecond(millisecond) {}

Marco::TomlError Marco::TomlTime::FromString(const std::string& dateString)
{
	if (dateString.length() < 5) // Not enough chars for format HH:MM
	{
		return TomlError{TomlErrorType::InvalidDateFormat, 0};
	}

	if (dateString[2] != ':')
	{
		return TomlError{TomlErrorType::InvalidDateFormat, 0};
	}
	
	std::size_t i = 0;

	std::string s{};

	for (; i < 2; i++) // hour digits are 0 and 1
	{
		s.push_back(dateString[i]);
	}

	i++; // skip the ':'

	auto result = StringToInt(s);
	if (! result.has_value())
	{
		return result.error();
	}
	
	this->hour = result.value();

	s = "";
	
	for (; i < 5; i++) // minute digits are 3 and 4
	{
		s.push_back(dateString[i]);
	}


	if (dateString.length() < 8) // Doesnt have the seconds in HH:MM:SS
	{
		return TomlError{TomlErrorType::NoError, 0};
	}

	if (dateString[i] != ':')
	{
		return TomlError{TomlErrorType::InvalidDateFormat, 0};
	}
	
	i++; // skip the ':'

	result = StringToInt(s);
	if (! result.has_value())
	{
		return result.error();
	}
	
	this->minute = result.value();

	s = "";

	for (; i < 8; i++) // second digits are 6 and 7
	{
		s.push_back(dateString[i]);
	}

	result = StringToInt(s);
	if (! result.has_value())
	{
		return result.error();
	}
	
	this->second = result.value();

	if (s.length() <= 9) // Would be something like HH:MM:SS.
	{
		return TomlError{TomlErrorType::InvalidDateFormat, 0};
	}

	i++; // skip the '.'

	s = "";

	for (; i < dateString.length(); i++)
	{
		s.push_back(dateString[i]);
	}

	if (s.length() > 6) // milliseconds digits count is greater than 6
	{
		return TomlError{TomlErrorType::InvalidDateFormat, 0};
	}

	result = StringToInt(s);
	if (! result.has_value())
	{
		return result.error();
	}
	
	this->millisecond = result.value();

	return TomlError{TomlErrorType::NoError, 0};
}

std::string Marco::TomlTime::AsString()
{
	return std::format("");
}

std::expected<int, Marco::TomlError> StringToInt(const std::string& s)
{
	int n{};
	
	try
	{
		n = std::stoi(s);
	}
	catch (std::invalid_argument e)
	{
		return std::unexpected(Marco::TomlError{Marco::TomlErrorType::InvalidDateFormat, 0});
	}
	catch (std::out_of_range e)
	{
		return std::unexpected(Marco::TomlError{Marco::TomlErrorType::InvalidDateFormat, 0});
	}

	return n;
}