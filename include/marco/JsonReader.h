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
		static JsonError FormJsonFromString(JsonValue& jsonValue, const std::string& jsonString, size_t& index); 
		static JsonError FormJsonValue     (JsonValue& jsonValue, const std::string& jsonString, size_t& index); 
		static JsonError HandleNumber      (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleString      (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleBool        (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleNull        (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleArray       (JsonValue& jsonValue, const std::string& jsonString, size_t& index); // index is left pointing at the character after last processed
		static JsonError HandleEscape      (std::string& value,   const std::string& jsonString, size_t& index); // index is left pointing at the last processed character
		
		JsonError m_error;
		bool      m_valid;
	};
}