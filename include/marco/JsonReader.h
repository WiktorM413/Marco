#pragma once

#include <optional>
#include <string>
#include <fstream>
#include "JsonError.h"

namespace Marco
{
	class JsonReader
	{
	public:
		JsonReader();
		JsonReader(const std::string&  jsonString);
		JsonReader(const std::fstream& jsonFile);

	private:
		std::string m_json;
		std::optional<JsonError> m_error;
		bool        m_valid;
	};
}