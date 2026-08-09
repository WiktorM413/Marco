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
		
		JsonValue Parse(const std::string&  jsonString);
		JsonValue Parse(std::istream& jsonFile);

		JsonError Error();
		bool      IsValid();
		
	private:
		static JsonError FormJsonFromString (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static JsonError FormJsonValue      (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleNumber(JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleString(JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleBool  (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleNull  (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		static Marco::JsonError HandleArray (JsonValue& jsonValue, const std::string& jsonString, size_t& index);
		
		JsonError m_error;
		bool      m_valid;
	};
}