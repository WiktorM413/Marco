#pragma once

#include <string>
#include <unordered_map>
#include <variant>
#include <vector>
namespace Marco
{
	class TomlValue;

	using TomlObject = std::unordered_map<std::string, TomlValue>;
	using TomlArray  = std::vector<TomlValue>;
	
	class TomlValue
	{
	public:
		TomlValue();
		TomlValue(bool b);
		TomlValue(double d);
		TomlValue(int i);
		TomlValue(const std::string& s);
		TomlValue(const char* c);
		TomlValue(TomlArray arr);
		TomlValue(TomlObject obj);

		bool IsBool()   const;
		bool IsNumber() const;
		bool IsString() const;
		bool IsObject() const;
		bool IsArray()  const;
		
		

		using ValueType = std::variant
		<
			std::nullptr_t,
			bool,
			double,
			std::string,
			TomlObject,
			TomlArray
		>;

	private:
		ValueType m_value;
	};
}
