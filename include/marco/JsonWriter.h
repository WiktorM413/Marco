#pragma once

#include "marco/JsonError.h"
#include "marco/JsonValue.h"
#include <string>

namespace Marco
{
	class JsonWriter
	{
	public:

		std::string Write(const JsonValue& json);
		void        WriteTo(const JsonValue& json, const std::string& path);

	private:
		static std::string HandleJsonValue(const JsonValue&   value, size_t& indentLevel);
		static std::string HandleObject   (const JsonObject&  obj,   size_t& indentLevel);
		static std::string HandleArray    (const JsonArray&   arr,   size_t& indentLevel);
		static std::string HandleString   (const std::string& s);
		static std::string HandleNumber   (const double&      n);
		static std::string HandleBool     (const bool&        b);
		static std::string HandleNull();

		JsonErrorType m_error;
		bool          m_valid;
	};
}