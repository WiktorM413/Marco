#include "marco/JsonReader.h"
#include "marco/JsonError.h"
#include "marco/JsonValue.h"
#include <cctype>


Marco::JsonReader::JsonReader(): m_error(JsonError::NoError), m_valid(false) { }

// TODO: Finish implementing

Marco::JsonReader::JsonReader(const std::string& jsonString)
{
	size_t start = jsonString.find_first_not_of(" \t\r\n");
	size_t end   = jsonString.find_last_not_of(" \t\r\n");
	
	if (start == std::string::npos || jsonString[start] != '{' || jsonString[end] != '}')
	{
		this->m_error = JsonError::InvalidFormat;
		this->m_valid = false;

		return;
	}

	size_t index = start + 1;

	this->m_error = FormJsonFromString(this->m_json, jsonString, index);

	if (this->m_error == JsonError::NoError)
	{
		this->m_valid = true;
	}
}

Marco::JsonReader::JsonReader(const std::fstream& jsonFile)
{

}

Marco::JsonValue& Marco::JsonReader::operator[](const std::string& key)
{
	return this->m_json[key];
}

std::expected<std::reference_wrapper<const Marco::JsonValue>, Marco::JsonError> Marco::JsonReader::operator[](const std::string& key) const
{
	return this->m_json[key];
}

Marco::JsonValue& Marco::JsonReader::operator[](size_t index)
{
	return this->m_json[index];
}

std::expected<std::reference_wrapper<const Marco::JsonValue>, Marco::JsonError> Marco::JsonReader::operator[](size_t index) const
{
	return this->m_json[index];
}

Marco::JsonError Marco::JsonReader::FormJsonFromString(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	JsonError error = JsonError::InvalidFormat;
	while (index < jsonString.length())
	{
		std::string key{};
		
		for (; index < jsonString.length(); index++)
		{
			if (std::isspace(jsonString[index]))
			{
				continue;
			}
	
			if (jsonString[index] == '"')
			{
				index++;
				break;
			}
		}
	
		for (; index < jsonString.length(); index++)
		{
			if (jsonString[index] == '"')
			{
				index++;
				break;
			}
	
			key.push_back(jsonString[index]);
		}
	
		for (; index < jsonString.length(); index++)
		{
			if (jsonString[index] == ':')
			{
				index++;
				break;
			}
		}
	
		error = FormJsonValue(jsonValue[key], jsonString, index);

		bool EndOfObject = false;
		
		for (; index < jsonString.length(); index++)
		{
			if (jsonString[index] == '}')
			{
				EndOfObject = true;
				break;
			}
			
			if (jsonString[index] == ',')
			{
				break;
			}
		}

		if (EndOfObject)
		{
			break;
		}
	}
	
	
	return error;
}

Marco::JsonError Marco::JsonReader::FormJsonValue(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	for (; index < jsonString.length(); index++)
	{
		if (! std::isspace(jsonString[index]))
		{
			break;
		}
	}

	JsonError error = JsonError::InvalidFormat;
	
	if (jsonString[index] >= '0' && jsonString[index] <= '9') // Possibly number
	{
		error = HandleNumber(jsonValue, jsonString, index);
	}
	else if (jsonString[index] == '"') // Possibly string
	{
		index++;

		error = HandleString(jsonValue, jsonString, index);
	}
	else if (jsonString[index] == 't' || jsonString[index] == 'f') // Possibly boolean
	{
		error = HandleBool(jsonValue, jsonString, index);
	}
	else if (jsonString[index] == 'n') // Possibly null
	{
		error = HandleNull(jsonValue, jsonString, index);
	}
	else if (jsonString[index] == '[') // Possibly array
	{
		index++;
		
		error = HandleArray(jsonValue, jsonString, index);
	}
	else if (jsonString[index] == '{') // Possibly object
	{
		index++;
		
		error = FormJsonFromString(jsonValue, jsonString, index);
	}
	else
	{
		error = JsonError::InvalidFormat;
	}

	return error;
}

Marco::JsonError Marco::JsonReader::HandleNumber(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string value{};
	
	for (; index < jsonString.length(); index++)
	{
		if (std::isspace(jsonString[index]) || jsonString[index] == ',' || jsonString[index] == '}' || jsonString[index] == ']')
		{
			jsonValue = std::stod(value);
			
			return Marco::JsonError::NoError;
		}
		
		value.push_back(jsonString[index]);
	}

	return Marco::JsonError::InvalidFormat;
}

Marco::JsonError Marco::JsonReader::HandleString(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string value{};
	for (; index < jsonString.length(); index++)
	{
		if (jsonString[index] == '}')
		{
			return Marco::JsonError::InvalidFormat;
		}
		
		if (jsonString[index] == '"')
		{
			jsonValue = value;
			index++;
			
			return Marco::JsonError::NoError;
		}

		value.push_back(jsonString[index]);
	}

	return Marco::JsonError::InvalidFormat;
}

Marco::JsonError Marco::JsonReader::HandleBool(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string value{};
	
	for (; index < jsonString.length(); index++)
	{
		if (std::isspace(jsonString[index]) || jsonString[index] == ',' || jsonString[index] == '}' || jsonString[index] == ']')
		{
			if (value == "true")
			{
				jsonValue = true;
				
				return Marco::JsonError::NoError;
			}
			else if (value == "false")
			{
				jsonValue = false;

				return Marco::JsonError::NoError;
			}
			else
			{
				return Marco::JsonError::InvalidFormat;
			}
		}

		value.push_back(jsonString[index]);
	}

	return Marco::JsonError::InvalidFormat;
}

Marco::JsonError Marco::JsonReader::HandleNull(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string value{};
	
	for (; index < jsonString.length(); index++)
	{
		if (std::isspace(jsonString[index]) || jsonString[index] == ',' || jsonString[index] == '}' || jsonString[index] == ']')
		{
			if (value == "null")
			{
				jsonValue = nullptr;

				return Marco::JsonError::NoError;
			}
			else
			{
				return Marco::JsonError::InvalidFormat;
			}
		}

		value.push_back(jsonString[index]);
	}

	return Marco::JsonError::InvalidFormat;
}

Marco::JsonError Marco::JsonReader::HandleArray(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	Marco::JsonError error = Marco::JsonError::NoError;

	for (; index < jsonString.length(); index++)
	{
		if (! std::isspace(jsonString[index]))
		{
			break;
		}
	}

	if (index < jsonString.length() && jsonString[index] == ']')
	{
		index++;

		return Marco::JsonError::NoError;
	}

	while (index < jsonString.length())
	{
		JsonValue value {};
		error = FormJsonValue(jsonValue.PushBack(value), jsonString, index);

		if (error != JsonError::NoError)
		{
			return error;
		}

		bool endOfArray = false;

		for (; index < jsonString.length(); index++)
		{
			if (jsonString[index] == ']')
			{
				endOfArray = true;
				index++;
				break;
			}

			if (jsonString[index] == ',')
			{
				index++;
				break;
			}
		}

		if (endOfArray)
		{
			break;
		}
	}

	return error;
}