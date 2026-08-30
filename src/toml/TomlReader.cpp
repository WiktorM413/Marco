#include "marco/toml/TomlReader.h"
#include "marco/toml/TomlValue.h"
#include "marco/utils/FileUtils.h"
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

	this->m_error = FormTomlFromString(toml, tomlString, index);

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

Marco::TomlError Marco::TomlReader::FormTomlFromString(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
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
			error = HandleComment(tomlValue, tomlString, index);
			break;
		case '[':
			error = HandleObject(tomlValue, tomlString, index);
			break;
		case '\"':
		{
			std::string key = HandleStringKey(tomlValue, tomlString, index);
			error = FormTomlValue(tomlValue[key], tomlString, index);
			break;
		}
		case '\'':
		{
			std::string key = HandleStringLiteralKey(tomlValue, tomlString, index);
			error = FormTomlValue(tomlValue[key], tomlString, index);
			break;
		}
		default:
		{
			std::string key = HandleBareKey(tomlValue, tomlString, index);
			error = FormTomlValue(tomlValue[key], tomlString, index);
			break;
		}
	}

	return error;
}

Marco::TomlError Marco::TomlReader::FormTomlValue(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleComment(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleNumber(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleString(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleBool(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleNull(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleObject(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

Marco::TomlError Marco::TomlReader::HandleArray(Marco::TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}


std::string Marco::TomlReader::HandleStringKey(TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

std::string Marco::TomlReader::HandleStringLiteralKey(TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}

std::string Marco::TomlReader::HandleBareKey(TomlValue& tomlValue, const std::string& tomlString, size_t& index)
{
	
}