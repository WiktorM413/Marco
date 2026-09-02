#include "marco/json/JsonWriter.h"
#include "marco/utils/FileUtils.h"
#include "marco/json/Json.h"
#include "marco/json/JsonValue.h"
#include <cmath>
#include <format>
#include <stdexcept>


static std::string WriteIndentation(const size_t& indentLevel);

std::string Marco::JsonWriter::Write(const Json& json)
{
	size_t indentLevel = 0;
	
	std::string jsonString = HandleObject(json, indentLevel);
	
	return jsonString;
}

void Marco::JsonWriter::WriteTo(const Json& json, const std::string& path)
{
	std::string jsonString = this->Write(json);

	WriteFile(path, jsonString);
}

std::string Marco::JsonWriter::HandleJsonValue(const Marco::JsonValue& value, size_t& indentLevel)
{
	std::string jsonString{};
	
	if (value.IsObject())
	{
		jsonString += HandleObject(value.AsObject().value(), indentLevel);
	}
	else if (value.IsArray())
	{
		jsonString += HandleArray(value.AsArray().value(), indentLevel);
	}
	else if (value.IsNumber())
	{
		jsonString += HandleNumber(value.AsNumber().value());
	}
	else if (value.IsString())
	{
		jsonString += HandleString(value.AsString().value());
	}
	else if (value.IsBool())
	{
		jsonString += HandleBool(value.AsBool().value());
	}
	else if (value.IsNull())
	{
		jsonString += HandleNull();
	}
	else [[unlikely]]
	{
		throw std::runtime_error("JsonValue of unknown type. Unexpected error");
	}

	return jsonString;
}

std::string Marco::JsonWriter::HandleObject(const Marco::JsonObject& obj, size_t& indentLevel)
{
	if (obj.size() == 0)
	{
		return "{}";
	}
	
	std::string jsonString = "{\n";

	indentLevel += 4;
	
	for (const auto& property : obj)
	{
		const auto& key   = property.first;
		const auto& value = property.second;
		
		jsonString += WriteIndentation(indentLevel);
		jsonString += HandleString(key);
		jsonString += ": ";

		jsonString += HandleJsonValue(value, indentLevel);

		jsonString += ",\n";
	}

	indentLevel -= 4;

	jsonString.pop_back();
	jsonString.pop_back();

	jsonString += '\n';
	jsonString += WriteIndentation(indentLevel);
	jsonString += '}';

	return jsonString;
}

std::string Marco::JsonWriter::HandleArray(const Marco::JsonArray& arr, size_t& indentLevel)
{
	if (arr.size() == 0)
	{
		return "[]";
	}
	
	std::string jsonString = "[\n";

	indentLevel += 4;

	for (const auto& value : arr)
	{
		jsonString += std::format("{}{},\n", WriteIndentation(indentLevel), HandleJsonValue(value, indentLevel));
	}

	indentLevel -= 4;

	jsonString.pop_back();
	jsonString.pop_back();

	jsonString += '\n';
	jsonString += WriteIndentation(indentLevel);
	jsonString += ']';

	return jsonString;
}

std::string Marco::JsonWriter::HandleString(const std::string& s)
{
	std::string escapedS = "\"";

	for (size_t i = 0; i < s.length(); i++)
	{
		switch (s[i])
		{
			case '\"': escapedS += "\\\""; break;
			case '\\': escapedS += "\\\\"; break;
			case '\b': escapedS += "\\b";  break;
			case '\f': escapedS += "\\f";  break;
			case '\n': escapedS += "\\n";  break;
			case '\r': escapedS += "\\r";  break;
			case '\t': escapedS += "\\t";  break;
			default:
				if (s[i] < 0x20)
				{
					escapedS += std::format("\\u{:04x}", s[i]);
				}
				else
				{
					escapedS.push_back(static_cast<char>(s[i]));
				}
				break;
		}
	}

	escapedS.push_back('"');
	
	return escapedS;
}

std::string Marco::JsonWriter::HandleNumber(const double& n)
{
	if (! std::isfinite(n))
	{
		throw std::runtime_error("Json number must be finite (not NaN or infinite)");
	}
	
	return std::format(R"({})", n);
}

std::string Marco::JsonWriter::HandleBool(const bool& b)
{
	if (b)
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

std::string Marco::JsonWriter::HandleNull()
{
	return "null";
}

static std::string WriteIndentation(const size_t& indentLevel)
{
	std::string indentation{};

	for (size_t i = 0; i < indentLevel; i++)
	{
		indentation += " ";
	}

	return indentation;
}