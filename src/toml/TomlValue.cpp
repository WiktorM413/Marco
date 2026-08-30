#include "marco/toml/TomlValue.h"
#include "marco/json/JsonValue.h"
#include "marco/toml/TomlError.h"
#include <cstddef>
#include <exception>
#include <expected>
#include <functional>
#include <variant>

Marco::TomlValue::TomlValue():                       m_value(nullptr)                {}
Marco::TomlValue::TomlValue(bool               b):   m_value(b)                      {}
Marco::TomlValue::TomlValue(double             d):   m_value(d)                      {}
Marco::TomlValue::TomlValue(int                i):   m_value(static_cast<double>(i)) {}
Marco::TomlValue::TomlValue(const std::string& s):   m_value(s)                      {}
Marco::TomlValue::TomlValue(const char*        c):   m_value(c)                      {}
Marco::TomlValue::TomlValue(TomlArray          arr): m_value(std::move(arr))         {}
Marco::TomlValue::TomlValue(TomlObject         obj): m_value(std::move(obj))         {}

bool Marco::TomlValue::IsEmpty() const
{
	return std::holds_alternative<std::nullptr_t>(this->m_value);
}

bool Marco::TomlValue::IsBool() const
{
	return std::holds_alternative<bool>(this->m_value);
}

bool Marco::TomlValue::IsNumber() const
{
	return std::holds_alternative<double>(this->m_value);
}

bool Marco::TomlValue::IsString() const
{
	return std::holds_alternative<std::string>(this->m_value);
}

bool Marco::TomlValue::IsObject() const
{
	return std::holds_alternative<TomlObject>(this->m_value);
}

bool Marco::TomlValue::IsArray() const
{
	return std::holds_alternative<TomlArray>(this->m_value);
}

std::expected<bool, Marco::TomlError> Marco::TomlValue::AsBool() const
{
	if (auto* p = std::get_if<bool>(&this->m_value))
	{
		return *p;
	}

	return std::unexpected(TomlError{TomlErrorType::WrongType, 0});
}

std::expected<double, Marco::TomlError> Marco::TomlValue::AsNumber() const
{
	if (auto* p = std::get_if<double>(&this->m_value))
	{
		return *p;
	}

	return std::unexpected(TomlError{TomlErrorType::WrongType, 0});
}

std::expected<std::reference_wrapper<const std::string>, Marco::TomlError> Marco::TomlValue::AsString() const
{
	if (auto* p = std::get_if<std::string>(&this->m_value))
	{
		return std::ref(*p);
	}

	return std::unexpected(TomlError{TomlErrorType::WrongType, 0});
}

std::expected<std::reference_wrapper<const Marco::TomlObject>, Marco::TomlError> Marco::TomlValue::AsObject() const
{
	if (auto* p = std::get_if<TomlObject>(&this->m_value))
	{
		return std::ref(*p);
	}

	return std::unexpected(TomlError{TomlErrorType::WrongType, 0});
}

std::expected<std::reference_wrapper<const Marco::TomlArray>, Marco::TomlError> Marco::TomlValue::AsArray() const
{
	if (auto* p = std::get_if<TomlArray>(&this->m_value))
	{
		return std::ref(*p);
	}

	return std::unexpected(TomlError{TomlErrorType::WrongType, 0});
}

Marco::TomlValue& Marco::TomlValue::PushBack(Marco::TomlValue value)
{
	if (! std::holds_alternative<TomlArray>(this->m_value))
	{
		this->m_value = TomlArray{};
	}

	return std::get<TomlArray>(this->m_value).emplace_back(std::move(value));
}

Marco::TomlValue& Marco::TomlValue::operator[](const std::string& key)
{
	if (! std::holds_alternative<TomlObject>(this->m_value))
	{
		this->m_value = TomlObject{};
	}

	return std::get<TomlObject>(this->m_value)[key];
}

std::expected<std::reference_wrapper<const Marco::TomlValue>, Marco::TomlError> Marco::TomlValue::operator[](const std::string& key) const
{
	if (auto* obj = std::get_if<TomlObject>(&this->m_value))
	{
		auto it = obj->find(key);
		if (it != obj->end())
		{
			return std::cref(it->second);
		}
	}

	return std::unexpected(TomlError{TomlErrorType::InvalidFormat, 0});
}

Marco::TomlValue& Marco::TomlValue::operator[](size_t index)
{
	if (! std::holds_alternative<TomlArray>(this->m_value))
	{
		this->m_value = TomlArray{};
	}

	auto& arr = std::get<TomlArray>(this->m_value);
	if (index >= arr.size())
	{
		arr.resize(index + 1);
	}

	return std::get<TomlArray>(this->m_value)[index];
}

std::expected<std::reference_wrapper<const Marco::TomlValue>, Marco::TomlError> Marco::TomlValue::operator[](size_t index) const
{
	if (auto* arr = std::get_if<TomlArray>(&this->m_value))
	{
		if (index > arr->size())
		{
			return std::cref((*arr)[index]);
		}
	}

	return std::unexpected(TomlError{TomlErrorType::InvalidFormat, 0});
}