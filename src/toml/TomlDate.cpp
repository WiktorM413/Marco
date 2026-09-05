#include "marco/toml/TomlDate.h"
#include "marco/toml/TomlError.h"
#include <expected>
#include <stdexcept>
#include <string>


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
		if (! this->IsWIthinBoundry())
		{
			return TomlError{TomlErrorType::InvalidDateFormat, i};
		}
		
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

		if (! this->IsWIthinBoundry())
		{
			return TomlError{TomlErrorType::InvalidDateFormat, i};
		}

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

	if (! this->IsWIthinBoundry())
	{
		return TomlError{TomlErrorType::InvalidDateFormat, i};
	}

	return error;
}

std::string Marco::TomlDate::AsString()
{
	std::string dateString{};

	dateString += std::to_string(this->year);

	dateString.push_back('-');

	if (this->month < 10)
	{
		dateString.push_back('0');
	}
		
	dateString += std::to_string(this->month);

	dateString.push_back('-');

	if (this->day < 10)
	{
		dateString.push_back('0');
	}

	dateString += std::to_string(this->day);

	std::string timeStartString = this->offsetTimeStart.AsString();
	std::string timeEndString   = this->offsetTimeEnd.AsString();

	if (timeStartString == timeEndString) // there is only one date offset or none
	{
		if (timeStartString == "00:00")
		{
			return dateString;
		}

		dateString.push_back('T');

		dateString += timeStartString;

		dateString.push_back('Z');
		
		return dateString;
	}

	dateString.push_back('T');

	dateString += timeStartString;

	dateString.push_back(':');

	dateString += timeEndString;
	
	return dateString;
}

bool Marco::TomlDate::IsWIthinBoundry()
{
	if (this->month > 12 || this->month < 1)
	{
		return false;
	}
	else if (this->day > 31 || this->day < 1)
	{
		return false;
	}
	else if (! this->offsetTimeStart.IsWIthinBoundry())
	{
		return false;
	}
	else if (! this->offsetTimeEnd.IsWIthinBoundry())
	{
		return false;
	}

	return true;
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
