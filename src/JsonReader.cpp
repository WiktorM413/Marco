#include "marco/JsonReader.h"
#include "marco/FileUtils.h"
#include "marco/Json.h"
#include "marco/JsonError.h"
#include "marco/JsonValue.h"
#include <cctype>
#include <stdexcept>


Marco::JsonReader::JsonReader(): m_error(JsonError{JsonErrorType::NoError, 0}), m_valid(false) { }

Marco::Json Marco::JsonReader::Parse(const std::string& jsonString)
{
	size_t start = jsonString.find_first_not_of(" \t\r\n");
	size_t end   = jsonString.find_last_not_of(" \t\r\n");

	JsonValue json{JsonObject{}};
	
	if (start == std::string::npos || jsonString[start] != '{' || jsonString[end] != '}')
	{
		this->m_error = JsonError{JsonErrorType::InvalidFormat, 0};
		this->m_valid = false;

		JsonValue emptyValue{};
		
		return Json{};
	}

	size_t index = start + 1;

	this->m_error = FormJsonFromString(json, jsonString, index);

	if (this->m_error.errorType == JsonErrorType::NoError)
	{
		this->m_valid = true;
	}

	return json.AsObject()->get();
}

Marco::Json Marco::JsonReader::Parse(std::istream& jsonFile)
{
	return this->Parse(ReadFile(jsonFile));
}

Marco::JsonError Marco::JsonReader::Error()
{
	return this->m_error;
}

bool Marco::JsonReader::IsValid()
{
	return this->m_valid;
}

Marco::JsonError Marco::JsonReader::FormJsonFromString(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	for (; index < jsonString.length(); index++)
	{
		if (! std::isspace(jsonString[index]))
		{
			break;
		}
	}

	if (index < jsonString.length() && jsonString[index] == '}')
	{
		index++;
		return JsonError{JsonErrorType::NoError, index};
	}

	while (true)
	{
		std::string key{};

		for (; index < jsonString.length(); index++)
		{
			if (! std::isspace(jsonString[index]))
			{
				break;
			}
		}

		if (index >= jsonString.length() || jsonString[index] != '\"')
		{
			return JsonError{JsonErrorType::InvalidFormat, index};
		}

		index++;

		for (; index < jsonString.length(); index++)
		{
			if (jsonString[index] == '\"')
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

		JsonError valueError = FormJsonValue(jsonValue[key], jsonString, index);
		if (valueError.errorType != JsonErrorType::NoError)
		{
			return valueError;
		}

		for (; index < jsonString.length(); index++)
		{
			if (! std::isspace(jsonString[index]))
			{
				break;
			}
		}

		if (index >= jsonString.length())
		{
			return JsonError{JsonErrorType::InvalidFormat, index};
		}

		if (jsonString[index] == '}')
		{
			index++;
			return JsonError{JsonErrorType::NoError, index};
		}

		if (jsonString[index] == ',')
		{
			index++;
			continue;
		}

		return JsonError{JsonErrorType::InvalidFormat, index};
	}
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

	JsonError error = JsonError{JsonErrorType::InvalidFormat, index};
	
	if ((jsonString[index] >= '0' && jsonString[index] <= '9') || jsonString[index] == '-') // Possibly number
	{
		error = HandleNumber(jsonValue, jsonString, index);
	}
	else if (jsonString[index] == '\"') // Possibly string
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
		error = JsonError{JsonErrorType::InvalidFormat, 1};
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
			try
			{	
				jsonValue = std::stod(value);
			}
			catch (const std::invalid_argument&)
			{
				return Marco::JsonError{JsonErrorType::NumberParseError, index};
			}
			catch (const std::out_of_range&)
			{
				return Marco::JsonError{JsonErrorType::NumberOutOfRange, index};
			}
			
			return Marco::JsonError{JsonErrorType::NoError, index};
		}
		
		value.push_back(jsonString[index]);
	}

	return Marco::JsonError{JsonErrorType::InvalidFormat, index};
}

Marco::JsonError Marco::JsonReader::HandleString(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	std::string value{};
	for (; index < jsonString.length(); index++)
	{
		if (jsonString[index] == '\n' || jsonString[index] == '\t' || jsonString[index] == '\b' || jsonString[index] == '\f' || jsonString[index] == '\r')
		{
			return Marco::JsonError{JsonErrorType::InvalidFormat, index};
		}

		if (jsonString[index] == '\\')
		{
			JsonError error = HandleEscape(value, jsonString, index);

			if (error.errorType != JsonErrorType::NoError)
			{
				return error;
			}

			continue;
		}
		
		if (jsonString[index] == '\"')
		{
			jsonValue = value;

			index++;
			
			return Marco::JsonError{JsonErrorType::NoError, index};
		}

		value.push_back(jsonString[index]);
	}

	return Marco::JsonError{JsonErrorType::InvalidFormat, index};
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
				
				return Marco::JsonError{JsonErrorType::NoError, index};
			}
			else if (value == "false")
			{
				jsonValue = false;

				return Marco::JsonError{JsonErrorType::NoError, index};
			}
			else
			{
				return Marco::JsonError{JsonErrorType::InvalidFormat, index};
			}
		}

		value.push_back(jsonString[index]);
	}

	return Marco::JsonError{JsonErrorType::InvalidFormat, index};
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

				return Marco::JsonError{JsonErrorType::NoError, index};
			}
			else
			{
				return Marco::JsonError{JsonErrorType::InvalidFormat, index};
			}
		}

		value.push_back(jsonString[index]);
	}

	return Marco::JsonError{JsonErrorType::InvalidFormat, index};
}

Marco::JsonError Marco::JsonReader::HandleArray(Marco::JsonValue& jsonValue, const std::string& jsonString, size_t& index)
{
	Marco::JsonError error = Marco::JsonError{JsonErrorType::NoError, index};

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

		jsonValue = JsonArray();

		return Marco::JsonError{JsonErrorType::NoError, index};
	}

	while (index < jsonString.length())
	{
		JsonValue value {};
		error = FormJsonValue(jsonValue.PushBack(value), jsonString, index);

		if (error.errorType != JsonErrorType::NoError)
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

Marco::JsonError Marco::JsonReader::HandleEscape(std::string& value,   const std::string& jsonString, size_t& index)
{
	index++;

	if (index >= jsonString.length())
	{
		return JsonError{JsonErrorType::InvalidFormat, index};
	}

	switch (jsonString[index])
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
			return JsonError{JsonErrorType::InvalidFormat, index};
	}

	return JsonError{JsonErrorType::NoError, index};
}