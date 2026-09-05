#include "marco/toml/TomlDate.h"
#include "marco/toml/TomlError.h"
#include <expected>
#include <stdexcept>


static std::expected<int, Marco::TomlError> StringToInt(const std::string& s);

Marco::TomlDate::TomlDate(unsigned int year, unsigned int month, unsigned int day, std::string timeStartString, std::string timeEndString): year(year), month(month), day(day)
{
	this->offsetTimeStart.FromString(timeStartString);
	this->offsetTimeEnd.FromString(timeEndString);
}

Marco::TomlError Marco::TomlDate::FromString(const std::string& dateString)
{
	if (dateString.length() < 10) // minimal character count for YYYY:MM:DD
	{
		return TomlError{TomlErrorType::InvalidDateFormat, 0};
	}

	size_t i = 0;
	
	std::string s{};
	
	for (; i < 4; i++) // the YYYY part
	{
		s.push_back(dateString[i]);
	}

	i++; // skip the '-'
	
	auto result = StringToInt(s);
	if (! result.has_value())
	{
		return result.error();
	}

	this->year = result.value();
	
	s = "";

	for (; i < 7; i++)
	{
		s.push_back(dateString[i]);
	}

	i++; // skip the '-'
	
	result = StringToInt(s);
	if (! result.has_value())
	{
		return result.error();
	}

	this->month = result.value();
	
	s = "";

	for (; i < 10; i++)
	{
		s.push_back(dateString[i]);
	}

	result = StringToInt(s);
	if (! result.has_value())
	{
		return result.error();
	}

	this->day = result.value();

	if (dateString.length() == 10) // is exactly YYYY-MM-DD
	{
		return TomlError{TomlErrorType::NoError, i};
	}

	if (dateString.length() > 11) // is more than just YYYY-MM-DDT
	{
		if (dateString[10] != 'T' && dateString[10] != ' ') // dateString[10] could be 'T' / ' ' delemiter for time offset
		{
			return TomlError{TomlErrorType::InvalidDateFormat, i};
		}
	}

	i++;

	s = "";

	for (; i < dateString.length(); i++)
	{
		char c = dateString[i];

		if (c == 'Z' || c == '-')
		{
			break;
		}

		s.push_back(c);
	}
	
	if (i >= dateString.length() || dateString[i] == 'Z') // construct the first TomlTime in a string form
	{
		TomlError error = this->offsetTimeStart.FromString(s);
		
		this->offsetTimeEnd.FromString(s);

		return error;
	}
	// has to be dateString[i] == '-'

	TomlError error = this->offsetTimeStart.FromString(s);

	if (error.errorType != TomlErrorType::NoError)
	{
		return error;
	}

	i++;

	s = "";

	for (; i < dateString.length(); i++) // construct the second TomlTime in string form
	{
		s.push_back(dateString[i]);
	}

	error = this->offsetTimeEnd.FromString(s);

	return error;
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