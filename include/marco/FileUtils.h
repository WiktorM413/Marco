#pragma once

#include <string>
#include <iosfwd>


std::string ReadFile(const std::string&   filepath);
std::string ReadFile(std::istream&        file);
