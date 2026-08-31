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

	TomlError error{};

	switch (tomlString[index])
	{
		case '#':
			error = HandleComment(rootTomlValue, currTomlValue, tomlString, index);
			break;
		case '[':
			error = HandleTables(rootTomlValue, currTomlValue, tomlString, index);
			return error;
		case '\"':
		{
			auto result = HandleStringKey(tomlString, index);

			if (! result.has_value())
			{
				error = result.error();
				break;
			}
			
			std::string key = result.value();
			error = FormTomlValue(rootTomlValue, currTomlValue[key], tomlString, index);
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
			error = FormTomlValue(rootTomlValue, currTomlValue[key], tomlString, index);
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
			error = FormTomlValue(rootTomlValue, currTomlValue[key], tomlString, index);
			break;
		}
	}

	return error;
}

Marco::TomlError Marco::TomlReader::FormTomlValue(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleComment(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
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

Marco::TomlError Marco::TomlReader::HandleNumber(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleString(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleBool(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleNull(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleTables(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	index++;
	
	TomlValue* currValue = &currTomlValue;
	std::string key{};

	if (tomlString[index] == '[')
	{
		TomlError error = HandleArrayOfTables(rootTomlValue, currTomlValue, tomlString, index);

		return error;
	}

	for (; index < tomlString.length(); index++)
	{
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
			currValue = &(*currValue)[key];
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

	currValue = &(*currValue)[key];

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

	TomlError error = FormTomlFromString(rootTomlValue, *currValue, tomlString, index);

	return error;
}

Marco::TomlError Marco::TomlReader::HandleArray(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleInlineTables(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleArrayOfTables(Marco::TomlValue& rootTomlValue, Marco::TomlValue& currTomlValue, const std::string& tomlString, size_t& index)
{
	index++;

	TomlValue* currValue = &currTomlValue;
	std::string key{};

	for (; index < tomlString.length(); index++)
	{
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
			currValue = &(*currValue)[key];
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

	currValue = &(*currValue)[key];

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

	TomlError error = FormTomlFromString(rootTomlValue, *currValue, tomlString, index);

	return error;
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

			continue;
		}
		
		key.push_back(tomlString[index]);
	}

	for (; index < tomlString.length(); index++)
	{
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
		
		if (!(tomlString[index] >= 'a' && tomlString[index] <= 'z') && !(tomlString[index] >= 'A' && tomlString[index] <= 'Z') && !(tomlString[index] >= '0' && tomlString[index] <= '9') && tomlString[index] != '_')
		{
			return std::unexpected(TomlError{TomlErrorType::InvalidFormat, index});
		}

		key.push_back(tomlString[index]);
	}

	for (; index < tomlString.length(); index++)
	{
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

Marco::TomlError Marco::TomlReader::HandleEscape(std::string& value, const std::string& tomlString, size_t& index)
{
	index++;

	if (index >= tomlString.length())
	{
		return TomlError{TomlErrorType::InvalidFormat, index};
	}

	switch (tomlString[index])
	{
		case '\"': value.push_back('\"'); break;
		case '\\': value.push_back('\\'); break;
		case '/': value.push_back ('/');  break;
		case 'b': value.push_back ('\b'); break;
		case 'f': value.push_back ('\f'); break;
		case 'n': value.push_back ('\n'); break;
		case 'r': value.push_back ('\r'); break;
		case 't': value.push_back ('\t'); break;
		default:
			return TomlError{TomlErrorType::InvalidFormat, index};
	}

	return TomlError{TomlErrorType::NoError, index};
}