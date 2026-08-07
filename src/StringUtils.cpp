#include "marco/StringUtils.h"

void TrimFront(std::string &s)
{
	s.erase(0, s.find_first_not_of(" \t\n\v\f\r"));
}

void TrimBack(std::string& s)
{
	s.erase(s.find_last_not_of(" \t\n\v\f\r") + 1);
}