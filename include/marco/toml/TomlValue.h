#pragma once

#include "marco/toml/TomlError.h"
#include <expected>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
namespace Marco
{
	class TomlValue;

	using TomlObject = std::unordered_map<std::string, TomlValue>;
	using TomlArray  = std::vector<TomlValue>;
	
	class TomlValue
	{
	public:
		TomlValue();
		TomlValue(bool b);
		TomlValue(double d);
		TomlValue(int i);
		TomlValue(const std::string& s);
		TomlValue(const char* c);
		TomlValue(TomlArray arr);
		TomlValue(TomlObject obj);

		bool IsEmpty()  const;
		bool IsBool()   const;
		bool IsNumber() const;
		bool IsString() const;
		bool IsObject() const;
		bool IsArray()  const;
		
		std::expected<bool,                                      TomlError> AsBool()   const;
		std::expected<double,                                    TomlError> AsNumber() const;
		std::expected<std::reference_wrapper<const std::string>, TomlError> AsString() const;
		std::expected<std::reference_wrapper<const TomlObject>,  TomlError> AsObject() const;
		std::expected<std::reference_wrapper<const TomlArray>,   TomlError> AsArray()  const;

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
			TomlArray
		>;

	private:
		ValueType m_value;
	};
}
