#include "marco/toml/TomlReader.h"
#include "marco/toml/TomlError.h"
#include "marco/toml/TomlValue.h"
#include "marco/utils/FileUtils.h"
#include <cctype>
#include <cmath>
#include <expected>
#include <string>


Marco::TomlReader::TomlReader(): m_error(TomlError{TomlErrorType::NoError, 0}), m_valid(false) {}

Marco::Toml Marco::TomlReader::Parse(const std::string& tomlString)
{
	size_t start = tomlString.find_first_not_of(" \t\r\n");

	TomlValue toml{TomlObject{}};

	if (start == std::string::npos)
	{
		this->m_error = TomlError{TomlErrorType::NoError, 0};
		this->m_valid = true;

		return Toml{};
	}

	this->m_error = FormTomlFromString(toml, toml, tomlString, start);

	if (this->m_error.errorType == TomlErrorType::NoError)
	{
		this->m_valid = true;
	}
	else
	{
		this->m_valid = false;
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
		MoveIndexUntilNotSpace(tomlString, index);
	
		if (index >= tomlString.length())
		{
			return TomlError{TomlErrorType::NoError, 0};
		}

		char c = tomlString[index];
	
		switch (c)
		{
			case '#':
				error = HandleComment(tomlString, index);
				break;
			case '[':
				error = HandleTables(rootTomlValue, currTomlValue, tomlString, index);
				break;
			case '\"':
			{
				error = FormKeyValuePair(HandleStringKey, currTomlValue, tomlString, index);
				break;
			}
			case '\'':
			{
				error = FormKeyValuePair(HandleStringLiteralKey, currTomlValue, tomlString, index);
				break;
			}
			default:
			{
				error = FormKeyValuePair(HandleBareKey, currTomlValue, tomlString, index);
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
	bool prevWasDot = true;

	char firstChar = tomlString[index];

	if (firstChar == '[')
	{
		TomlError error = HandleArrayOfTables(rootTomlValue, currTomlValue, tomlString, index);

		return error;
	}

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (c == '#')
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		if (c == ']')
		{
			break;
		}

		if (std::isspace(c))
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		if (c == '.')
		{
			if (prevWasDot)
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}
			
			currTomlValue = currTomlValue[key];
			key = "";
			
			index++;

			if (index >= tomlString.length() || tomlString[index] == ']')
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}

			prevWasDot = true;
			
			continue;
		}

		prevWasDot = false;
		key.push_back(c);
	}

	if (index >= tomlString.length())
	{
		return TomlError{TomlErrorType::InvalidFormat, index};
	}

	currTomlValue = currTomlValue[key];

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (c == '\n')
		{
			break;
		}

		if (! std::isspace(c))
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
	bool prevWasDot = true;

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (c == '#')
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		if (c == ']')
		{
			break;
		}

		if (std::isspace(c))
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		if (c == '.')
		{
			if (prevWasDot)
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}
			
			currTomlValue = currTomlValue[key];
			key = "";

			index++;

			if (index >= tomlString.length() || tomlString[index] == ']')
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}

			prevWasDot = true;

			continue;
		}

		prevWasDot = false;
		key.push_back(c);
	}

	if (index >= tomlString.length())
	{
		return TomlError{TomlErrorType::InvalidFormat, index};
	}

	currTomlValue = currTomlValue[key];

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (c == '\n')
		{
			break;
		}

		if (! std::isspace(c))
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
	char c = tomlString[index];
	
	if (c == '\'')
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
	else if (c == '\"')
	{
		index++;

		bool isMultiline = false;
		if (index + 2 < tomlString.length() && tomlString[index] == '\"' && tomlString[index + 1] == '\"')
		{
			index += 2;
			
			isMultiline = true;
		}
		
		error = HandleString(currTomlValue, tomlString, index, isMultiline);
	}
	else if ((c >= '0' && c <= '9') || c == '-' || c == '+')
	{
		error = HandleNumber(currTomlValue, tomlString, index);
	}
	else if (c == 't' || c == 'f')
	{
		error = HandleBool(currTomlValue, tomlString, index);
	}
	else if (c == '[')
	{
		error = HandleArray(currTomlValue, tomlString, index);
	}
	else if (c == '{')
	{
		error = HandleInlineTables(currTomlValue, tomlString, index);
	}
	else
	{
		error = TomlError{TomlErrorType::InvalidFormat, index};
	}

	return error;
}

Marco::TomlError Marco::TomlReader::FormKeyValuePair(TomlReader::KeyParser keyParser, TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	auto result = keyParser(tomlString, index);

	if (! result.has_value())
	{
		return result.error();
	}

	return FormTomlValue(currTomlValue[result.value()], tomlString, index);
}

Marco::TomlError Marco::TomlReader::HandleNumber(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	std::string value{};
	bool isFloat = false;

	if (tomlString[index] == '+' || tomlString[index] == '-')
	{
		value.push_back(tomlString[index]);
		index++;
	}
	
	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (IsValueDelimiter(c))
		{
			break;
		}

		// A sign is only valid right after 'e'/'E' (exponent), e.g. 1e-10
		bool isExponentSign = (c == '+' || c == '-') && ! value.empty()
			&& (value.back() == 'e' || value.back() == 'E');

		bool isDigit = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');

		if (! isDigit && c != '.' && c != '_' && c != 'e' && c != 'E' && ! isExponentSign)
		{
			return TomlError{TomlErrorType::InvalidNumberFormat, index};
		}

		if (c == '.' || c == 'e' || c == 'E')
		{
			if (isFloat)
			{
				return TomlError{TomlErrorType::InvalidNumberFormat, index};
			}

			isFloat = true;
		}

		value.push_back(c);
	}

	double parsedValue{};
	
	if (isFloat)
	{
		auto result = ParseTomlFloat(value);

		if (! result.has_value())
		{
			return TomlError{result.error().errorType, index};
		}

		parsedValue = result.value();
	}
	else
	{
		auto result = ParseTomlInt(value);

		if (! result.has_value())
		{
			return TomlError{result.error().errorType, index};
		}

		parsedValue = result.value();
	}

	currTomlValue = parsedValue;
	
	return TomlError{TomlErrorType::NoError, index};
}

Marco::TomlError Marco::TomlReader::HandleString(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index, bool isMultiline)
{
	std::string value{};
	
	if (isMultiline)
	{
		for (; index < tomlString.length(); index++)
		{
			char c = tomlString[index];

			if (c == '\\')
			{
				index++;
				for (; index < tomlString.length(); index++)
				{
					if (! std::isspace(tomlString[index]))
					{
						break;
					}
				}

				if (index >= tomlString.length())
				{
					return TomlError{TomlErrorType::InvalidFormat, index};
				}

				c = tomlString[index]; // refresh: index moved during the skip above
			}

			if (c == '\"')
			{
				if (index + 2 < tomlString.length() && tomlString[index + 1] == '\"' && tomlString[index + 2] == '\"')
				{
					currTomlValue = value;
					index += 3;
					
					return TomlError{TomlErrorType::NoError, index};
				}
			}

			value.push_back(c);
		}

		return TomlError{TomlErrorType::InvalidFormat, index};
	}
	else
	{
		for (; index < tomlString.length(); index++)
		{
			char c = tomlString[index];

			if (c == '\n')
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}
			
			if (c == '\\')
			{
				TomlError error = HandleEscape(value, tomlString, index);

				if (error.errorType != TomlErrorType::NoError)
				{
					return error;
				}
				
				continue;
			}

			if (c == '\"')
			{
				currTomlValue = value;
				index++;
				
				return TomlError{TomlErrorType::NoError, index};
			}

			value.push_back(c);
		}
	}

	return TomlError{TomlErrorType::InvalidFormat, index};
}

Marco::TomlError Marco::TomlReader::HandleStringLiteral(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index, bool isMultiline)
{
	std::string value{};

	if (isMultiline)
	{
		for (; index < tomlString.length(); index++)
		{
			char c = tomlString[index];

			if (c == '\'')
			{
				if (index + 2 < tomlString.length() && tomlString[index + 1] == '\'' && tomlString[index + 2] == '\'')
				{
					currTomlValue = value;
					index += 3;
					
					return TomlError{TomlErrorType::NoError, index};
				}
			}

			value.push_back(c);
		}

		return TomlError{TomlErrorType::InvalidFormat, index};
	}
	else
	{
		for (; index < tomlString.length(); index++)
		{
			char c = tomlString[index];

			if (c == '\n')
			{
				return TomlError{TomlErrorType::InvalidFormat, index};
			}

			if (c == '\'')
			{
				currTomlValue = value;
				index++;
				
				return TomlError{TomlErrorType::NoError, index};
			}

			value.push_back(c);
		}
	}

	return TomlError{TomlErrorType::InvalidFormat, index};
}

Marco::TomlError Marco::TomlReader::HandleBool(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	std::string key{};

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (IsValueDelimiter(c))
		{
			break;
		}

		key.push_back(c);
	}

	TomlError error{};

	if (key == "true")
	{
		currTomlValue = true;
		error = TomlError{TomlErrorType::NoError, index};
	}
	else if (key == "false")
	{
		currTomlValue = false;
		error = TomlError{TomlErrorType::NoError, index};
	}
	else
	{
		error = TomlError{TomlErrorType::InvalidFormat, index};
	}

	return error;
}

Marco::TomlError Marco::TomlReader::HandleNull(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleArray(Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	index++;

	bool isExpectingValue = true;

	while (index < tomlString.length())
	{
		MoveIndexUntilNotSpace(tomlString, index);

		if (index >= tomlString.length())
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}

		if (tomlString[index] == ']')
		{
			index++;

			return TomlError{TomlErrorType::NoError, index};
		}

		if (! isExpectingValue)
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}
		
		TomlError error = FormTomlValue(currTomlValue.PushBack(nullptr), tomlString, index);

		if (error.errorType != TomlErrorType::NoError)
		{
			return error;
		}

		MoveIndexUntilNotSpace(tomlString, index);

		if (index >= tomlString.length())
		{
			return TomlError{TomlErrorType::InvalidFormat, index};
		}

		if (tomlString[index] == ',')
		{
			index++;
			
			isExpectingValue = true;
		}
		else
		{
			isExpectingValue = false;
		}

	}

	return TomlError{TomlErrorType::InvalidFormat, index};
}

std::expected<std::string, Marco::TomlError> Marco::TomlReader::HandleStringKey(const std::string& tomlString, size_t& index)
{
	index++;

	std::string key{};

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (c == '\"')
		{
			break;
		}
		
		if (c == '\\')
		{
			TomlError error = HandleEscape(key, tomlString, index);

			if (error.errorType != TomlErrorType::NoError)
			{
				return std::unexpected(error);
			}

			continue;
		}
		
		key.push_back(c);
	}

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (c == '#')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}
		
		if (! std::isspace(c) && c != '=')
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
		char c = tomlString[index];

		if (c == '\'')
		{
			break;
		}

		key.push_back(c);
	}

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (c == '#')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}
		
		if (! std::isspace(c) && c != '=')
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
		char c = tomlString[index];

		if (std::isspace(c))
		{
			break;
		}
		
		if (!(c >= 'a' && c <= 'z') && !(c >= 'A' && c <= 'Z') && !(c >= '0' && c <= '9') && c != '_' && c != '-')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}

		key.push_back(c);
	}

	for (; index < tomlString.length(); index++)
	{
		char c = tomlString[index];

		if (c == '#')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}
		
		if (! std::isspace(c) && c != '=')
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
		char c = tomlString[index];

		if (c == '\n')
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

	char c = tomlString[index];

	switch (c)
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
		char c = s[i];

		if (c == '_')
		{
			if (!prevWasDigit)
			{
				return std::unexpected(TomlError{TomlErrorType::InvalidFormat, 0});
			}

			prevWasDigit = false;
			continue;
		}

		prevWasDigit = true;
		
		int digit{};
		if (c >= '0' && c <= '9')
		{
			digit = c - '0';
		}
		else if (c >= 'a' && c <= 'f')
		{
			digit = c - 'a' + 10;
		}
		else if (c >= 'A' && c <= 'F')
		{
			digit = c - 'A' + 10;
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

std::expected<double, Marco::TomlError> Marco::TomlReader::ParseTomlFloat(std::string_view s)
{
	bool isNegative = false;

	if (!s.empty() && (s[0] == '+' || s[0] == '-'))
	{
		isNegative = s[0] == '-';
		s.remove_prefix(1);
	}

	if (s.empty())
	{
		return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, 0});
	}

	double      integerPart  = 0.0;
	std::size_t i            = 0;
	bool        prevWasDigit = false;

	for (; i < s.length() && s[i] != '.' && s[i] != 'e' && s[i] != 'E'; i++)
	{
		if (s[i] == '_')
		{
			if (!prevWasDigit)
			{
				return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
			}
			prevWasDigit = false;
			continue;
		}

		if (s[i] < '0' || s[i] > '9')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
		}

		integerPart = integerPart * 10.0 + (s[i] - '0');
		prevWasDigit = true;
	}

	if (!prevWasDigit)
	{
		return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
	}

	double fractionPart = 0.0;

	if (i < s.length() && s[i] == '.')
	{
		i++;
		double divisor = 10.0;
		prevWasDigit = false;

		for (; i < s.length() && s[i] != 'e' && s[i] != 'E'; i++)
		{
			if (s[i] == '_')
			{
				if (!prevWasDigit)
				{
					return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
				}
				prevWasDigit = false;
				continue;
			}

			if (s[i] < '0' || s[i] > '9')
			{
				return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
			}

			fractionPart += (s[i] - '0') / divisor;
			divisor *= 10.0;
			prevWasDigit = true;
		}

		if (!prevWasDigit)
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
		}
	}

	double value = integerPart + fractionPart;

	if (i < s.length() && (s[i] == 'e' || s[i] == 'E'))
	{
		i++;

		bool exponentNegative = false;
		if (i < s.length() && (s[i] == '+' || s[i] == '-'))
		{
			exponentNegative = s[i] == '-';
			i++;
		}

		int  exponent = 0;
		prevWasDigit = false;

		for (; i < s.length(); i++)
		{
			if (s[i] == '_')
			{
				if (!prevWasDigit)
				{
					return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
				}
				prevWasDigit = false;
				continue;
			}

			if (s[i] < '0' || s[i] > '9')
			{
				return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
			}

			exponent = exponent * 10 + (s[i] - '0');
			prevWasDigit = true;
		}

		if (!prevWasDigit)
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
		}

		value *= std::pow(10.0, exponentNegative ? -exponent : exponent);
	}

	if (i < s.length())
	{
		return std::unexpected(TomlError{TomlErrorType::InvalidNumberFormat, i});
	}

	return isNegative ? -value : value;
}

bool Marco::TomlReader::IsValueDelimiter(char c)
{
	return std::isspace(c) || c == '\n' || c == '#' || c == ']' || c == ',' || c == '}';
}

void Marco::TomlReader::MoveIndexUntilNotSpace(const std::string& tomlString, size_t& index)
{
	for (; index < tomlString.length(); index++)
	{
		if (! std::isspace(tomlString[index]))
		{
			break;
		}
	}
}