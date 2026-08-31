#include "marco/toml/TomlReader.h"
#include "marco/toml/TomlError.h"
#include "marco/toml/TomlValue.h"
#include "marco/utils/FileUtils.h"
#include <cctype>
#include <expected>
#include <string>


Marco::TomlReader::TomlReader(): m_error(TomlError{TomlErrorType::NoError, 0}), m_valid(false) {}

Marco::Toml Marco::TomlReader::Parse(const std::string& tomlString)
{
	size_t start = tomlString.find_first_not_of(" \t\r\n");

	TomlValue toml{TomlObject{}};

	if (start == std::string::npos)
	{
		this->m_error = TomlError{TomlErrorType::InvalidFormat, 0};
		this->m_valid = false;

		return Toml{};
	}

	size_t index = start + 1;

	this->m_error = FormTomlFromString(toml, toml, tomlString, index);

	if (this->m_error.errorType == TomlErrorType::NoError)
	{
		this->m_valid = true;
	}

	return toml.AsObject()->get();
}

Marco::Toml Marco::TomlReader::Parse(std::istream& tomlFile)
{
	return this->Parse(ReadFile(tomlFile));
}

Marco::TomlError Marco::TomlReader::Error()
{
	return this->m_error;
}

bool Marco::TomlReader::IsValid()
{
	return this->m_valid;
}

Marco::TomlError Marco::TomlReader::FormTomlFromString(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	TomlError error{};

	while (index < tomlString.length() && error.errorType == TomlErrorType::NoError)
	{
		for (; index < tomlString.length(); index++)
		{
			if (! std::isspace(tomlString[index]))
			{
				break;
			}
		}
	
		if (index >= tomlString.length())
		{
			return TomlError{TomlErrorType::NoError, 0};
		}
	
		switch (tomlString[index])
		{
			case '#':
				error = HandleComment(tomlString, index);
				break;
			case '[':
				error = HandleTables(rootTomlValue, currTomlValue, tomlString, index);
				break;
			case '\"':
			{
				auto result = HandleStringKey(tomlString, index);
	
				if (! result.has_value())
				{
					error = result.error();
					break;
				}
				
				std::string key = result.value();
				error = FormTomlValue(currTomlValue[key], tomlString, index);
				break;
			}
			case '\'':
			{
				auto result = HandleStringKey(tomlString, index);
	
				if (! result.has_value())
				{
					error = result.error();
					break;
				}
				
				std::string key = result.value();
				error = FormTomlValue(currTomlValue[key], tomlString, index);
				break;
			}
			default:
			{
				auto result = HandleStringKey(tomlString, index);
	
				if (! result.has_value())
				{
					error = result.error();
					break;
				}
				
				std::string key = result.value();
				error = FormTomlValue(currTomlValue[key], tomlString, index);
				break;
			}
		}
	}

	return error;
}

Marco::TomlError Marco::TomlReader::HandleTables(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	index++;
	
	currTomlValue = rootTomlValue;
	std::string key{};

	if (tomlString[index] == '[')
	{
		TomlError error = HandleArrayOfTables(rootTomlValue, currTomlValue, tomlString, index);

		return error;
	}

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '#')
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		if (tomlString[index] == ']')
		{
			break;
		}

		if (isspace(tomlString[index]))
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		if (tomlString[index] == '.')
		{
			currTomlValue = currTomlValue[key];
			key = "";

			index++;

			if (index >= tomlString.length() || tomlString[index] == ']')
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}
		}

		key.push_back(tomlString[index]);
	}

	if (index >= tomlString.length())
	{
		return TomlError{TomlErrorType::InvalidFormat, index};
	}

	currTomlValue = currTomlValue[key];

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '\n')
		{
			break;
		}

		if (! std::isspace(tomlString[index]))
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
	}

	return TomlError{TomlErrorType::NoError, index};
}

Marco::TomlError Marco::TomlReader::HandleInlineTables(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleArrayOfTables(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	index++;

	currTomlValue = rootTomlValue;
	std::string key{};

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '#')
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		if (tomlString[index] == ']')
		{
			break;
		}

		if (isspace(tomlString[index]))
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		if (tomlString[index] == '.')
		{
			currTomlValue = currTomlValue[key];
			key = "";

			index++;

			if (index >= tomlString.length() || tomlString[index] == ']')
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}
		}

		key.push_back(tomlString[index]);
	}

	if (index >= tomlString.length())
	{
		return TomlError{TomlErrorType::InvalidFormat, index};
	}

	currTomlValue = currTomlValue[key];

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '\n')
		{
			break;
		}

		if (! std::isspace(tomlString[index]))
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
	}

	currTomlValue = currTomlValue.PushBack(nullptr);
	
	return TomlError{TomlErrorType::NoError, index};
}

Marco::TomlError Marco::TomlReader::FormTomlValue(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	TomlError error{};
	
	if (tomlString[index] == '\'')
	{
		index++;
		
		bool isMultiline = false;
		if (index + 1 < tomlString.length() && tomlString[index] == '\'' && tomlString[index + 1] == '\'')
		{
			index += 2;
			
			isMultiline = true;
		}
		
		error = HandleStringLiteral(currTomlValue, tomlString, index, isMultiline);
	}
	else if (tomlString[index] == '\"')
	{
		bool isMultiline = false;
		if (index + 2 < tomlString.length() && tomlString[index + 1] == '\"' && tomlString[index + 2] == '\"')
		{
			isMultiline = true;
		}
		
		error = HandleString(currTomlValue, tomlString, index, isMultiline);
	}
	else if (tomlString[index] >= '0' && tomlString[index] <= '9')
	{
		error = HandleNumber(currTomlValue, tomlString, index);
	}
	else if (tomlString[index] == '[')
	{
		error = HandleArray(currTomlValue, tomlString, index);
	}
	else if (tomlString[index] == '{')
	{
		error = HandleInlineTables(currTomlValue, tomlString, index);
	}
	else
	{
		error = TomlError{TomlErrorType::InvalidFormat, index};
	}

	return error;
}

Marco::TomlError Marco::TomlReader::HandleNumber(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	std::string value{};
	
	for (; index < tomlString.length(); index++)
	{
		if (std::isspace(tomlString[index]))
		{
			break;
		}

		value.push_back(tomlString[index]);
	}
	
	auto result = ParseTomlInt(value);

	if (! result.has_value())
	{
		return TomlError{result.error().errorType, index};
	}

	currTomlValue = result.value();
	
	return TomlError{TomlErrorType::NoError, index};
}

Marco::TomlError Marco::TomlReader::HandleString(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index, bool isMultiline)
{
	std::string value{};
	
	if (isMultiline)
	{
		for (; index < tomlString.length(); index++)
		{
			if (tomlString[index] == '\\')
			{
				index++;
				for (; index < tomlString.length(); index++)
				{
					if (! std::isspace(tomlString[index]))
					{
						break;
					}
				}
			}

			if (tomlString[index] == '\"')
			{
				if (index + 2 < tomlString.length() && tomlString[index + 1] == '\"' && tomlString[index + 2] == '\"')
				{
					currTomlValue = value;
					return TomlError{TomlErrorType::NoError, index};
				}
			}

			value.push_back(tomlString[index]);
		}

		return TomlError{TomlErrorType::InvalidFormat, index};
	}
	else
	{
		for (; index < tomlString.length(); index++)
		{
			if (tomlString[index] == '\n')
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}
			
			if (tomlString[index] == '\\')
			{
				TomlError error = HandleEscape(value, tomlString, index);

				if (error.errorType != TomlErrorType::NoError)
				{
					return error;
				}
			}

			if (tomlString[index] == '\"')
			{
				currTomlValue = value;
				return TomlError{TomlErrorType::NoError, index};
			}

			value.push_back(tomlString[index]);
		}
	}

	return TomlError{TomlErrorType::InvalidFormat, index};
}

Marco::TomlError Marco::TomlReader::HandleStringLiteral(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index, bool isMultiline)
{
	
}

Marco::TomlError Marco::TomlReader::HandleBool(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleNull(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}



Marco::TomlError Marco::TomlReader::HandleArray(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

std::expected<std::string, Marco::TomlError> Marco::TomlReader::HandleStringKey(const std::string& tomlString, size_t& index)
{
	index++;

	std::string key{};

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '\"')
		{
			break;
		}
		
		if (tomlString[index] == '\\')
		{
			TomlError error = HandleEscape(key, tomlString, index);

			if (error.errorType != TomlErrorType::NoError)
			{
				return std::unexpected(error);
			}
		}
		
		key.push_back(tomlString[index]);
	}

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '#')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}
		
		if (! std::isspace(tomlString[index]) && tomlString[index] != '=')
		{
			break;
		}
	}

	if (index >= tomlString.length())
	{
		return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
	}

	return key;
}

std::expected<std::string, Marco::TomlError> Marco::TomlReader::HandleStringLiteralKey(const std::string& tomlString, size_t& index)
{
	index++;

	std::string key{};
	
	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '\'')
		{
			break;
		}

		key.push_back(tomlString[index]);
	}

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '#')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}
		
		if (! std::isspace(tomlString[index]) && tomlString[index] != '=')
		{
			break;
		}
	}
	
	if (index >= tomlString.length())
	{
		return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
	}

	return key;
}

std::expected<std::string, Marco::TomlError> Marco::TomlReader::HandleBareKey(const std::string& tomlString, size_t& index)
{
	std::string key{};

	for (; index < tomlString.length(); index++)
	{
		if (std::isspace(tomlString[index]))
		{
			break;
		}
		
		if (!(tomlString[index] >= 'a' && tomlString[index] <= 'z') && !(tomlString[index] >= 'A' && tomlString[index] <= 'Z') && !(tomlString[index] >= '0' && tomlString[index] <= '9') && tomlString[index] != '_' && tomlString[index] != '-')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}

		key.push_back(tomlString[index]);
	}

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '#')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}
		
		if (! std::isspace(tomlString[index]) && tomlString[index] != '=')
		{
			break;
		}
	}

	if (index >= tomlString.length())
	{
		return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
	}

	return key;
}

Marco::TomlError Marco::TomlReader::HandleComment(const std::string& tomlString, size_t& index)
{
	index++;

	for (; index < tomlString.length(); index++)
	{
		if (tomlString[index] == '\n')
		{
			break;
		}
	}

	return TomlError{TomlErrorType::NoError, index};
}

Marco::TomlError Marco::TomlReader::HandleEscape(std::string& value, const std::string& tomlString, size_t& index)
{
	index++;

	if (index >= tomlString.length())
	{
		return TomlError{TomlErrorType::InvalidFormat, index};
	}

	switch (tomlString[index])
	{
		case '\"': value.push_back('\"'); index++;  break;
		case '\\': value.push_back('\\'); index++;  break;
		case '/':  value.push_back ('/'); index++;  break;
		case 'b': value.push_back ('\b'); index++;  break;
		case 'f': value.push_back ('\f'); index++;  break;
		case 'n': value.push_back ('\n'); index++;  break;
		case 'r': value.push_back ('\r'); index++;  break;
		case 't': value.push_back ('\t'); index++;  break;
		default:
			return TomlError{TomlErrorType::InvalidFormat, index};
	}

	return TomlError{TomlErrorType::NoError, index};
}

std::expected<int, Marco::TomlError> Marco::TomlReader::ParseTomlInt(std::string_view s)
{
	bool isNegative = false;

	if (!s.empty() && (s[0] == '+' || s[0] == '-'))
	{
		isNegative = s[0] == '-';

		s.remove_prefix(1);
	}

	int base = 10;
	if (s.size() > 2 && s[0] == '0')
	{
		switch (s[1])
		{
			case 'x': base = 16; s.remove_prefix(2); break;
			case 'o': base = 8;  s.remove_prefix(2); break;
			case 'b': base = 2;  s.remove_prefix(2); break;
		}
	}

	if (s.empty())
	{
		return std::unexpected(TomlError{TomlErrorType::InvalidFormat, 0});
	}

	int  value        = 0;
	bool prevWasDigit = false;

	for (std::size_t i = 0; i < s.length(); i++)
	{
		if (s[i] == '_')
		{
			if (!prevWasDigit)
			{
				return std::unexpected(TomlError{TomlErrorType::InvalidFormat, 0});
			}

			prevWasDigit = false;
			continue;
		}

		int digit{};
		if (s[i] >= '0' && s[i] <= '9')
		{
			digit = s[i] - '0';
		}
		else if (s[i] >= 'a' && s[i] <= 'f')
		{
			digit = s[i] - 'a' + 10;
		}
		else if (s[i] >= 'A' && s[i] <= 'f')
		{
			digit = s[i] - 'A' + 10;
		}
		else
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, 0});
		}

		if (digit >= base)
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, 0});
		}

		value = value * base + digit;
	}

	if (!prevWasDigit)
	{
		return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, 0});
	}

	return isNegative ? -value : value;
}