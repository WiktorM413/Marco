#pragma once

#include "marco/toml/TomlDate.h"
#include "marco/toml/TomlError.h"
#include "marco/toml/TomlTime.h"
#include <cstddef>
#include <expected>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace Marco
{
	class TomlValue;

	using TomlObject = std::map<std::string, TomlValue>;
	using TomlArray  = std::vector<TomlValue>;
	
	class TomlValue
	{
	public:
		TomlValue();
		TomlValue(std::nullptr_t);
		TomlValue(bool b);
		TomlValue(double d);
		TomlValue(long l);
		TomlValue(const std::string& s);
		TomlValue(const char* c);
		TomlValue(TomlArray arr);
		TomlValue(TomlObject obj);
		TomlValue(TomlDate date);
		TomlValue(TomlTime time);

		bool IsEmpty()  const;
		bool IsBool()   const;
		bool IsNumber() const;
		bool IsString() const;
		bool IsObject() const;
		bool IsArray()  const;
		bool IsDate()   const;
		bool IsTime()   const;
		
		std::expected<bool,                                      TomlError> AsBool()   const;
		std::expected<double,                                    TomlError> AsNumber() const;
		std::expected<std::reference_wrapper<const std::string>, TomlError> AsString() const;
		std::expected<std::reference_wrapper<const TomlObject>,  TomlError> AsObject() const;
		std::expected<std::reference_wrapper<const TomlArray>,   TomlError> AsArray()  const;
		std::expected<TomlDate,                                  TomlError> AsDate()   const;
		std::expected<TomlTime,                                  TomlError> AsTime()   const;

		TomlValue& PushBack(TomlValue value);
		
		TomlValue&                                                        operator[](const std::string& key);
		std::expected<std::reference_wrapper<const TomlValue>, TomlError> operator[](const std::string& key) const;
		TomlValue&                                                        operator[](size_t index);
		std::expected<std::reference_wrapper<const TomlValue>, TomlError> operator[](size_t index) const;

		using ValueType = std::variant
		<
			std::nullptr_t,
			bool,
			double,
			std::string,
			TomlObject,
			TomlArray,
			TomlDate,
			TomlTime
		>;

	private:
		ValueType m_value;
	};
}
