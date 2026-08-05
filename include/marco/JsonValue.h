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
		
		std::expected<bool,                                      JsonError> AsBool()   const;
		std::expected<double,                                    JsonError> AsNumber() const;
		std::expected<std::string,                               JsonError> AsString() const;
		std::expected<std::reference_wrapper<const JsonObject>,  JsonError> AsObject() const;
		std::expected<std::reference_wrapper<const JsonArray>,   JsonError> AsArray()  const;
		
		JsonValue&                                 operator[](const std::string& key);
		std::expected<std::reference_wrapper<const JsonValue>, JsonError> operator[](const std::string& key) const;
		
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