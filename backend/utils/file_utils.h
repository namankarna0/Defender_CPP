#pragma once
#include <string>

bool read_binary_file(const std::string& path, std::string& contents);
std::string mime_type(const std::string& path);
std::string safe_filename(const std::string& name);
