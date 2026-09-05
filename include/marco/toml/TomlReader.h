#pragma once

#include "marco/toml/Toml.h"
#include "marco/toml/TomlError.h"
#include "marco/toml/TomlValue.h"
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
		using KeyParser     = std::expected<std::string, TomlError> (*) (const std::string&, size_t&); // applies for HandleStringKey, HandleStringLiteralKey, HandleBareKey
		using NumericHandler = TomlError (*) (TomlValue*, const std::string&, size_t&); // applies for HandleNumber, HandleDate, HandleTime
		
		static TomlError FormTomlFromString (TomlValue& rootTomlValue, TomlValue* currTomlValue, const std::string& tomlString, size_t& index); // expects index to point to the first character of the line. Can be a whitespace
		static TomlError HandleTables       (TomlValue& rootTomlValue, TomlValue*& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError HandleArrayOfTables(TomlValue& rootTomlValue, TomlValue*& currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
	
		static TomlError FormTomlValue   (TomlValue* currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError FormKeyValuePair(KeyParser keyParser, TomlValue* currTomlValue, const std::string& tomlString, size_t& index);
		
		static TomlError HandleNumber       (TomlValue* currTomlValue, const std::string& tomlString, size_t& index); // leaves index at the first delimiter character after the number (not consumed)
		static TomlError HandleString       (TomlValue* currTomlValue, const std::string& tomlString, size_t& index, bool isMultiline); // leaves index right after the closing '"' (or closing '"""' if multiline)
		static TomlError HandleStringLiteral(TomlValue* currTomlValue, const std::string& tomlString, size_t& index, bool isMultiline); // leaves index right after the closing '\'' (or closing '\'\'\'' if multiline)
		static TomlError HandleBool         (TomlValue* currTomlValue, const std::string& tomlString, size_t& index); // leaves index at the first delimiter character after true/false (not consumed)
		static TomlError HandleArray        (TomlValue* currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the closing ']'
		static TomlError HandleInlineTables (TomlValue* currTomlValue, const std::string& tomlString, size_t& index); // leaves index right after the closing '}'
		static TomlError HandleDate         (TomlValue* currTomlValue, const std::string& tomlString, size_t& index); // leaves index at the whitespace character that ends the date (not consumed)
		static TomlError HandleTime         (TomlValue* currTomlValue, const std::string& tomlString, size_t& index); // leaves index at the whitespace character that ends the time (not consumed)
		
		static std::expected<std::string, TomlError> HandleStringKey       (const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace
		static std::expected<std::string, TomlError> HandleStringLiteralKey(const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace
		static std::expected<std::string, TomlError> HandleBareKey         (const std::string& tomlString, size_t& index); // leaves index at the closest character that either isnt a '=' or a whitespace

		static TomlError HandleComment(const std::string& tomlString, size_t& index); // leaves index right after the first \n character it encounters
		static TomlError HandleEscape(std::string& value, const std::string& tomlString, size_t& index); // leaves index right after the 'X' as in \X

		static std::expected<long,    TomlError> ParseTomlInt  (std::string_view s);
		static std::expected<double, TomlError> ParseTomlFloat(std::string_view s);

		static NumericHandler EvaluateNumericTomlValue(const std::string& tomlString, size_t& index);

		static bool IsValueDelimiter(char c);
		static void MoveIndexUntilNotSpace(const std::string& tomlString, size_t& index);
		
		TomlError m_error;
		bool      m_valid;
	};
}