#include "../include/marco/JsonValue.h"
#include "marco/JsonError.h"
#include <expected>
#include <variant>

std::expected<double, Marco::JsonError> Marco::JsonValue::AsNumber() const
{
	if (auto* p = std::get_if<double>(&this->m_value))
	{
		return *p;
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