#pragma once

#include <string>
#include "marco/JsonError.h"
#include "marco/JsonValue.h"

namespace Marco
{
	class JsonReader
	{
	public:
		JsonReader();
		JsonReader(const std::string&  jsonString);
		JsonReader(const std::istream& jsonFile);

		JsonValue&                                                        operator[](const std::string& key);
		std::expected<std::reference_wrapper<const JsonValue>, JsonError> operator[](const std::string& key) const;
		JsonValue&                                                        operator[](size_t index);
		std::expected<std::reference_wrapper<const JsonValue>, JsonError> operator[](size_t index) const;
		
	private:
		static JsonError FormJsonFromString (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static JsonError FormJsonValue      (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleNumber(JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleString(JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleBool  (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleNull  (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleArray (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		
		JsonValue m_json;
		JsonError m_error;
		bool      m_valid;
	};
}