#pragma once

#include "marco/toml/Toml.h"
#include "marco/toml/TomlError.h"
#include <expected>
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
		static TomlError FormTomlFromString (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index); // expects index to point to the first character of the line. Can be a whitespace
		static TomlError FormTomlValue      (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError HandleComment      (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError HandleNumber       (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleString       (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleBool         (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleNull         (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleTables       (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError HandleArray        (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleInlineTables (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleArrayOfTables(TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		
		static std::expected<std::string, TomlError> HandleStringKey       (const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace
		static std::expected<std::string, TomlError> HandleStringLiteralKey(const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace
		static std::expected<std::string, TomlError> HandleBareKey         (const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace

		static TomlError HandleEscape(std::string& value, const std::string& tomlString, size_t& index);
		
		TomlError m_error;
		bool      m_valid;
	};
}