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
		static TomlError HandleTables       (TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError HandleArrayOfTables(TomlValue& rootTomlValue, TomlValue& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
	
		static TomlError FormTomlValue      (TomlValue& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError HandleNumber       (TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleString       (TomlValue& currTomlValue, const std::string& tomlString, size_t& index, bool isMultiline);
		static TomlError HandleStringLiteral(TomlValue& currTomlValue, const std::string& tomlString, size_t& index, bool isMultiline);
		static TomlError HandleBool         (TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleNull         (TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleArray        (TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		static TomlError HandleInlineTables (TomlValue& currTomlValue, const std::string& tomlString, size_t& index);
		
		
		static std::expected<std::string, TomlError> HandleStringKey       (const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace
		static std::expected<std::string, TomlError> HandleStringLiteralKey(const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace
		static std::expected<std::string, TomlError> HandleBareKey         (const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace

		static TomlError HandleComment(const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError HandleEscape(std::string& value, const std::string& tomlString, size_t& index); // leaves index right after the 'X' as in \X

		static std::expected<int,    TomlError> ParseTomlInt  (std::string_view s);
		static std::expected<double, TomlError> ParseTomlFloat(std::string_view s);

		static bool IsValueDelimiter(char c);
		
		TomlError m_error;
		bool      m_valid;
	};
}