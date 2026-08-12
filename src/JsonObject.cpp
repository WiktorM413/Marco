#include "marco/JsonObject.h"
#include "marco/JsonValue.h"
#include <expected>
#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>


Marco::JsonObject::JsonObject(std::initializer_list<Entry> init)
{
	for (const auto& entry : init)
	{
		auto it = this->index.find(entry.first);
		if (it != this->index.end())
		{
			it->second->second = entry.second;
			continue;
		}
		this->entries.emplace_back(entry.first, entry.second);
		this->index.emplace(entries.back().first, std::prev(entries.end()));
	}
}

Marco::JsonObject::JsonObject(const JsonObject& other)
{
	for (const auto& entry : other.entries)
	{
		this->entries.emplace_back(entry.first, entry.second);
		this->index.emplace(this->entries.back().first, std::prev(this->entries.end()));
	}
}

Marco::JsonObject& Marco::JsonObject::operator=(const JsonObject& other)
{
	if (this == &other)
	{
		return *this;
	}

	Storage newEntries{};
	std::unordered_map<std::string, Storage::iterator> newIndex{};

	for (const auto& entry : other.entries)
	{
		newEntries.emplace_back(entry.first, entry.second);
		newIndex.emplace(newEntries.back().first, std::prev(newEntries.end()));
	}

	this->entries = std::move(newEntries);
	this->index   = std::move(newIndex);

	return *this;
}

Marco::JsonObject::Iterator Marco::JsonObject::begin()
{
	return this->entries.begin();
}

Marco::JsonObject::Iterator Marco::JsonObject::end()
{
	return this->entries.end();
}

Marco::JsonObject::ConstIterator Marco::JsonObject::begin() const
{
	return this->entries.begin();
}

Marco::JsonObject::ConstIterator Marco::JsonObject::end() const
{
	return this->entries.end();
}

Marco::JsonValue& Marco::JsonObject::at(const std::string& key)
{
	auto it = this->index.find(key);

	if (it == this->index.end())
	{
		throw std::out_of_range("JsonObject::at key not found: " + key);
	}

	return it->second->second;
}

const Marco::JsonValue& Marco::JsonObject::at(const std::string& key) const
{
	auto it = this->index.find(key);

	if (it == this->index.end())
	{
		throw std::out_of_range("JsonObject::at key not found: " + key);
	}

	return it->second->second;
}

Marco::JsonObject::Iterator Marco::JsonObject::find(const std::string& key)
{
	auto it = this->index.find(key);
	
	if (it == this->index.end())
	{
		return this->entries.end();
	}
	
	return it->second;
}

Marco::JsonObject::ConstIterator Marco::JsonObject::find(const std::string& key) const
{
	auto it = this->index.find(key);
	
	if (it == this->index.end())
	{
		return this->entries.end();
	}
	
	return it->second;
}

size_t Marco::JsonObject::size() const
{
	return this->index.size();
}

Marco::JsonValue& Marco::JsonObject::operator[](const std::string& key)
{
	auto it = this->index.find(key);

	if (it == this->index.end())
	{
		this->entries.emplace_back(key, JsonValue{});
		it = this->index.emplace(key, std::prev(this->entries.end())).first;
	}

	return it->second->second;
}

std::expected<std::reference_wrapper<const Marco::JsonValue>, Marco::JsonErrorType> Marco::JsonObject::operator[](const std::string& key) const
{
	auto it = this->index.find(key);

	if (it == this->index.end())
	{
		return std::unexpected(JsonErrorType::KeyNotFound);
	}

	return it->second->second;
}

