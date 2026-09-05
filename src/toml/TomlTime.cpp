#include "marco/toml/TomlTime.h"
#include "marco/toml/TomlError.h"
#include "marco/utils/IntUtils.h"
#include <expected>
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

	result = StringToInt(s);
	if (! result.has_value())
	{
		return result.error();
	}

	this->minute = result.value();


	if (dateString.length() < 8) // Doesnt have the seconds in HH:MM:SS
	{
		return TomlError{TomlErrorType::NoError, 0};
	}

	if (dateString[i] != ':')
	{
		return TomlError{TomlErrorType::InvalidDateFormat, 0};
	}

	i++; // skip the ':'

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

	if (dateString.length() == 8)
	{
		return TomlError{TomlErrorType::NoError, i};
	}

	if (! (dateString.length() > 8 && dateString[8] == '.')) // Would be something like HH:MM:SS.
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
	std::string dateString{};

	if (IsSingleDigit(this->hour))
	{
		dateString.push_back('0');
	}

	dateString += std::to_string(this->hour);
	dateString.push_back(':');

	if (IsSingleDigit(this->minute))
	{
		dateString.push_back('0');
	}

	dateString += std::to_string(this->minute);

	if (this->second > 0)
	{
		dateString.push_back(':');

		if (IsSingleDigit(this->second))
		{
			dateString.push_back('0');
		}

		dateString += std::to_string(this->second);
	}

	if (this->millisecond > 0)
	{
		if (this->second == 0)
		{
			dateString += ":00";
		}

		dateString.push_back('.');
		dateString += std::to_string(this->millisecond);
	}

	return dateString;
}

std::expected<int, Marco::TomlError> StringToInt(const std::string& s)
{
	int n{};

	try
	{
		n = std::stoi(s);
	}
	catch (const std::invalid_argument&)
	{
		return std::unexpected(Marco::TomlError{Marco::TomlErrorType::InvalidDateFormat, 0});
	}
	catch (const std::out_of_range&)
	{
		return std::unexpected(Marco::TomlError{Marco::TomlErrorType::InvalidDateFormat, 0});
	}

	return n;
}