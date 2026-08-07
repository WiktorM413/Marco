#pragma once

#include <cstddef>
#include <expected>
#include <functional>
#include <map>
#include <string>
#include <variant>
#include <vector>
#include "marco/JsonError.h"


namespace Marco
{
	class JsonValue;

	using JsonObject = std::map<std::string, JsonValue>;
	using JsonArray  = std::vector<JsonValue>;

	// TODO: Add preserving insertion order for a map
	class JsonValue
	{
	public:
		JsonValue();
		JsonValue(std::nullptr_t);
		JsonValue(bool b);
		JsonValue(double d);
		JsonValue(int i);
		JsonValue(const std::string& s);
		JsonValue(const char* c);
		JsonValue(JsonArray arr);
		JsonValue(JsonObject obj);

		bool IsNull()   const;
		bool IsBool()   const;
		bool IsNumber() const;
		bool IsString() const;
		bool IsObject() const;
		bool IsArray()  const;
		
		std::expected<bool,                                      JsonError> AsBool()   const;
		std::expected<double,                                    JsonError> AsNumber() const;
		std::expected<std::string,                               JsonError> AsString() const;
		std::expected<std::reference_wrapper<const JsonObject>,  JsonError> AsObject() const;
		std::expected<std::reference_wrapper<const JsonArray>,   JsonError> AsArray()  const;

		JsonValue& PushBack(JsonValue& value);
		
		JsonValue&                                                        operator[](const std::string& key);
		std::expected<std::reference_wrapper<const JsonValue>, JsonError> operator[](const std::string& key) const;
		JsonValue&                                                        operator[](size_t index);
		std::expected<std::reference_wrapper<const JsonValue>, JsonError> operator[](size_t index) const;
		
		using ValueType = std::variant
		<
			std::nullptr_t,
			bool,
			double,
			std::string,
			JsonObject,
			JsonArray
		>;

	private:
		ValueType m_value;
	};
}