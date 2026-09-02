#include "marco/json/JsonValue.h"
#include "marco/json/JsonError.h"
#include <cstddef>
#include <expected>
#include <functional>
#include <variant>


Marco::JsonValue::JsonValue(): m_value(nullptr)                                      {}
Marco::JsonValue::JsonValue(std::nullptr_t): m_value(nullptr)                        {}
Marco::JsonValue::JsonValue(bool               b):   m_value(b)                      {}
Marco::JsonValue::JsonValue(double             d):   m_value(d)                      {}
Marco::JsonValue::JsonValue(int                i):   m_value(static_cast<double>(i)) {}
Marco::JsonValue::JsonValue(const std::string& s):   m_value(s)                      {}
Marco::JsonValue::JsonValue(const char*        c):   m_value(c)                      {}
Marco::JsonValue::JsonValue(JsonArray          arr): m_value(std::move(arr))         {}
Marco::JsonValue::JsonValue(JsonObject         obj): m_value(std::move(obj))         {}

bool Marco::JsonValue::IsNull() const
{
	return std::holds_alternative<std::nullptr_t>(this->m_value);
}

bool Marco::JsonValue::IsBool() const
{
	return std::holds_alternative<bool>(this->m_value);
}

bool Marco::JsonValue::IsNumber() const
{
	return std::holds_alternative<double>(this->m_value);
}

bool Marco::JsonValue::IsString() const
{
	return std::holds_alternative<std::string>(this->m_value);
}

bool Marco::JsonValue::IsObject() const
{
	return std::holds_alternative<JsonObject>(this->m_value);
}

bool Marco::JsonValue::IsArray() const
{
	return std::holds_alternative<JsonArray>(this->m_value);
}

std::expected<std::nullptr_t, Marco::JsonError> Marco::JsonValue::AsNull() const
{
	if (auto* p = std::get_if<std::nullptr_t>(&this->m_value))
	{
		return *p;
	}

	return std::unexpected(JsonError{JsonErrorType::WrongType, 0});
}

std::expected<bool, Marco::JsonError> Marco::JsonValue::AsBool() const
{
	if (auto* p = std::get_if<bool>(&this->m_value))
	{
		return *p;
	}

	return std::unexpected(JsonError{JsonErrorType::WrongType, 0});
}

std::expected<double, Marco::JsonError> Marco::JsonValue::AsNumber() const
{
	if (auto* p = std::get_if<double>(&this->m_value))
	{
		return *p;
	}

	return std::unexpected(JsonError{JsonErrorType::WrongType, 0});
}

std::expected<std::reference_wrapper<const std::string>, Marco::JsonError> Marco::JsonValue::AsString() const
{
	if (auto* p = std::get_if<std::string>(&this->m_value))
	{
		return std::cref(*p);
	}

	return std::unexpected(JsonError{JsonErrorType::WrongType, 0});
}

std::expected<std::reference_wrapper<const Marco::JsonObject>,  Marco::JsonError> Marco::JsonValue::AsObject() const
{
	if (auto* p = std::get_if<JsonObject>(&this->m_value))
	{
		return std::cref(*p);
	}

	return std::unexpected(JsonError{JsonErrorType::WrongType, 0});
}

std::expected<std::reference_wrapper<const Marco::JsonArray>, Marco::JsonError> Marco::JsonValue::AsArray() const
{
	if (auto* p = std::get_if<JsonArray>(&this->m_value))
	{
		return std::cref(*p);
	}

	return std::unexpected(JsonError{JsonErrorType::WrongType, 0});
}

Marco::JsonValue& Marco::JsonValue::PushBack(JsonValue value)
{
	if (! std::holds_alternative<JsonArray>(this->m_value))
	{
		this->m_value = JsonArray{};
	}
	
	return std::get<JsonArray>(this->m_value).emplace_back(std::move(value));
}

Marco::JsonValue& Marco::JsonValue::operator[](const std::string& key)
{
	if (! std::holds_alternative<JsonObject>(this->m_value))
	{
		this->m_value = JsonObject{};
	}

	return std::get<JsonObject>(this->m_value)[key];
}

std::expected<std::reference_wrapper<const Marco::JsonValue>, Marco::JsonError> Marco::JsonValue::operator[](const std::string& key) const
{
	if (auto* obj = std::get_if<JsonObject>(&this->m_value))
	{
		auto it = obj->find(key);
		if (it != obj->end())
		{
			return std::cref(it->second);
		}
	}

	return std::unexpected(JsonError{JsonErrorType::InvalidFormat, 0});
}

Marco::JsonValue& Marco::JsonValue::operator[](size_t index)
{
	if (! std::holds_alternative<JsonArray>(this->m_value))
	{
		this->m_value = JsonArray{};
	}

	auto& arr = std::get<JsonArray>(this->m_value);
	if (index >= arr.size())
	{
		arr.resize(index + 1);
	}
	
	return std::get<JsonArray>(this->m_value)[index];
}

std::expected<std::reference_wrapper<const Marco::JsonValue>, Marco::JsonError> Marco::JsonValue::operator[](size_t index) const
{
	if (auto* arr = std::get_if<JsonArray>(&this->m_value))
	{
		if (index < arr->size())
		{
			return std::cref((*arr)[index]);
		}
	}

	return std::unexpected(JsonError{JsonErrorType::InvalidFormat, 0});
}