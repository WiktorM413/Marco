#include "../include/marco/JsonValue.h"
#include "marco/JsonError.h"
#include <expected>
#include <variant>


Marco::JsonValue::JsonValue(): m_value(nullptr) {}
Marco::JsonValue::JsonValue(bool               b):   m_value(b)              {}
Marco::JsonValue::JsonValue(double             d):   m_value(d)              {}
Marco::JsonValue::JsonValue(const std::string& s):   m_value(s)              {}
Marco::JsonValue::JsonValue(const char*        c):   m_value(c)              {}
Marco::JsonValue::JsonValue(JsonArray          arr): m_value(std::move(arr)) {}
Marco::JsonValue::JsonValue(JsonObject         obj): m_value(std::move(obj)) {}

std::expected<bool, Marco::JsonError> Marco::JsonValue::AsBool() const
{
	if (auto* p = std::get_if<bool>(&this->m_value))
	{
		return *p;
	}

	return std::unexpected(JsonError::InvalidFormat);
}

std::expected<double, Marco::JsonError> Marco::JsonValue::AsNumber() const
{
	if (auto* p = std::get_if<double>(&this->m_value))
	{
		return *p;
	}

	return std::unexpected(JsonError::InvalidFormat);
}

std::expected<std::string, Marco::JsonError> Marco::JsonValue::AsString() const
{
	if (auto* p = std::get_if<std::string>(&this->m_value))
	{
		return *p;
	}

	return std::unexpected(JsonError::InvalidFormat);
}

std::expected<std::reference_wrapper<const Marco::JsonObject>,  Marco::JsonError> Marco::JsonValue::AsObject() const
{
	if (auto* p = std::get_if<JsonObject>(&this->m_value))
	{
		return std::cref(*p);
	}

	return std::unexpected(JsonError::InvalidFormat);
}

std::expected<std::reference_wrapper<const Marco::JsonArray>, Marco::JsonError> Marco::JsonValue::AsArray() const
{
	if (auto* p = std::get_if<JsonArray>(&this->m_value))
	{
		return std::cref(*p);
	}

	return std::unexpected(JsonError::InvalidFormat);
}

Marco::JsonValue& Marco::JsonValue::operator[](const std::string& key)
{
	if (!std::holds_alternative<JsonObject>(this->m_value))
	{
		this->m_value = JsonObject{};
	}

	return std::get<JsonObject>(this->m_value)[key];
}