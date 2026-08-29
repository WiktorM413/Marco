#pragma once

#include "marco/json/Json.h"
#include "marco/json/JsonValue.h"
#include <string>

namespace Marco
{
	class JsonWriter
	{
	public:

		std::string Write(const Json& json);
		void        WriteTo(const Json& json, const std::string& path);

	private:
		static std::string HandleJsonValue(const JsonValue&   value, size_t& indentLevel);
		static std::string HandleObject   (const JsonObject&  obj,   size_t& indentLevel);
		static std::string HandleArray    (const JsonArray&   arr,   size_t& indentLevel);
		static std::string HandleString   (const std::string& s);
		static std::string HandleNumber   (const double&      n);
		static std::string HandleBool     (const bool&        b);
		static std::string HandleNull();
	};
}