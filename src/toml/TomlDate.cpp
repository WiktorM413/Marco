#include "marco/toml/TomlDate.h"

Marco::TomlDate::TomlDate(unsigned int year, unsigned int month, unsigned int day, std::string timeStartString, std::string timeEndString): year(year), month(month), day(day)
{
	this->offsetTimeStart.FromString(timeStartString);
	this->offsetTimeEnd.FromString(timeEndString);
}