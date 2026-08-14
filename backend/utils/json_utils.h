#pragma once
#include <string>
#include <unordered_map>

std::string json_escape(const std::string& value);
std::unordered_map<std::string, std::string> parse_json_object(const std::string& json);
std::string json_error(const std::string& message);
