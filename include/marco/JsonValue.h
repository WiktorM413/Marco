#pragma once

#include <expected>
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
		std::expected<double, JsonError> AsNumber() const;
		JsonValue& operator[](const std::string& key);
		
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