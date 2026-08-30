#pragma once

#include "marco/toml/Toml.h"
#include "marco/toml/TomlError.h"
#include <string>


namespace Marco
{
	class TomlReader
	{
	public:
		TomlReader();

		Toml Parse(const std::string& tomlString);
		Toml Parse(std::istream&      tomlFile);

		TomlError Error();
		bool      IsValid();

	private:
		static TomlError FormTomlFromString(TomlValue& tomlValue, const std::string& tomlString, size_t& index);
		static TomlError FormTomlValue     (TomlValue& tomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleNumber      (TomlValue& tomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleString      (TomlValue& tomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleBool        (TomlValue& tomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleNull        (TomlValue& tomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleArray       (TomlValue& tomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleEscape      (TomlValue& tomlValue, const std::string& tomlString, size_t& index);
		
		TomlError m_error;
		bool      m_valid;
	};
}