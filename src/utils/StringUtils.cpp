#include "marco/utils/StringUtils.h"

void Marco::TrimFront(std::string &s)
{
	s.erase(0, s.find_first_not_of(" \t\n\v\f\r"));
}

void Marco::TrimBack(std::string& s)
{
	auto pos = s.find_last_not_of(" \t\n\v\f\r");

	if (pos == std::string::npos)
	{
		s.clear();
		return;
	}
	
	s.erase(pos + 1);
}