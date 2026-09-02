#pragma once

#include "marco/json/JsonError.h"
#include <expected>
#include <functional>
#include <initializer_list>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>

namespace Marco
{
	class JsonValue;
	
	class JsonObject
	{
	public:
		using Entry         = std::pair<std::string, JsonValue>;
		using Storage       = std::list<Entry>;
		using Iterator      = std::list<Entry>::iterator;
		using ConstIterator = std::list<Entry>::const_iterator;

		JsonObject() = default;
		JsonObject(std::initializer_list<Entry> init);

		JsonObject(const JsonObject& other);
		JsonObject& operator=(const JsonObject& other);
	
		JsonObject(JsonObject&&) = default;
		JsonObject& operator=(JsonObject&&) = default;

		Iterator begin();
		Iterator end();
		
		ConstIterator begin() const;
		ConstIterator end() const;

		JsonValue&       at(const std::string& key);
		const JsonValue& at(const std::string& key) const;
		
		Iterator      find(const std::string& key);
		ConstIterator find(const std::string& key) const;

		size_t size() const;
		
		JsonValue& operator[](const std::string& key);
		std::expected<std::reference_wrapper<const JsonValue>, JsonErrorType> operator[](const std::string& key) const;
		
	private:
		Storage entries;
		std::unordered_map<std::string, Storage::iterator> index;
	};
}
