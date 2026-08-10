#include "marco/JsonWriter.h"
#include "marco/FileUtils.h"
#include "marco/JsonError.h"
#include "marco/JsonValue.h"
#include <format>
#include <stdexcept>


static std::string WriteIndentation(const size_t& indentLevel);

std::string Marco::JsonWriter::Write(const JsonValue& json)
{

	if (! json.IsObject())
	{
		this->m_valid = false;
		this->m_error = JsonErrorType::NotObject;
		
		return "";
	}

	size_t indentLevel = 0;
	
	std::string jsonString = HandleObject(json.AsObject().value(), indentLevel);

	this->m_valid = true;
	this->m_error = JsonErrorType::NoError;
	
	return jsonString;
}

void Marco::JsonWriter::WriteTo(const JsonValue& json, const std::string& path)
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
	std::string jsonString = "{\n";

	indentLevel += 4;
	
	for (const auto& property : obj)
	{
		const auto& key   = property.first;
		const auto& value = property.second;
		
		jsonString += WriteIndentation(indentLevel);
		jsonString += std::format(R"("{}": )", key);

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
	return std::format(R"("{}")", s);
}

std::string Marco::JsonWriter::HandleNumber(const double& n)
{
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