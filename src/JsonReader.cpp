#include "marco/JsonReader.h"
#include "marco/JsonError.h"
#include "marco/JsonValue.h"
#include "marco/StringUtils.h"
#include <cctype>


static Marco::JsonError HandleNumber(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index);
static Marco::JsonError HandleString(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index);
static Marco::JsonError HandleBool  (Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index);
static Marco::JsonError HandleNull  (Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index);
static Marco::JsonError HandleArray (Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index);


Marco::JsonReader::JsonReader(): m_error(JsonError::NoError), m_valid(false) { }

// TODO: Finish implementing

Marco::JsonReader::JsonReader(const std::string& jsonString)
{
	if (jsonString.empty() || jsonString.front() != '{' || jsonString.back() != '}')
	{
		this->m_error = JsonError::InvalidFormat;
		this->m_valid = false;

		return;
	}

	size_t index = 1;

	this->m_error = FormJsonFromString(this->m_json, jsonString, index);

	if (this->m_error == JsonError::NoError)
	{
		this->m_valid = true;
	}
}

Marco::JsonReader::JsonReader(const std::fstream& jsonFile)
{

}

Marco::JsonError Marco::JsonReader::FormJsonFromString(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string key{};
	JsonError error = JsonError::InvalidFormat;
	while (index < jsonString.length())
	{
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
		if (!std::isspace(jsonString[index]))
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

static Marco::JsonError HandleNumber(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string value{};
	
	for (; index < jsonString.length(); index++)
	{
		if (std::isspace(jsonString[index]) || jsonString[index] == ',' || jsonString[index] == '}')
		{
			jsonValue = std::stod(value);
			
			return Marco::JsonError::NoError;
		}
		
		value.push_back(jsonString[index]);
	}

	return Marco::JsonError::InvalidFormat;
}

static Marco::JsonError HandleString(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
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

static Marco::JsonError HandleBool(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string value{};
	
	for (; index < jsonString.length(); index++)
	{
		if (std::isspace(jsonString[index]) || jsonString[index] == ',' || jsonString[index] == '}')
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

static Marco::JsonError HandleNull(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string value{};
	
	for (; index < jsonString.length(); index++)
	{
		if (std::isspace(jsonString[index]) || jsonString[index] == ',' || jsonString[index] == '}')
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

static Marco::JsonError HandleArray(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	for (; index < jsonString.length(); index++)
	{
		if ()
		{
			
		}
	}
}