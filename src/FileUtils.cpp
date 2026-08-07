#include "marco/FileUtils.h"

#include <fstream>
#include <filesystem>
#include <sstream>



std::string ReadFile(const std::string& filepath)
{
	std::ifstream file(filepath, std::ios::binary);
	
	auto size = std::filesystem::file_size(filepath);
	std::string content(size, '\0');

	file.read(content.data(), size);

	return content;
}

std::string ReadFile(const std::istream& file)
{
	std::ostringstream ss;
	ss << file.rdbuf();

	return ss.str();
}