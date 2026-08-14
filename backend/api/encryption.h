#pragma once
#include <string>
std::string encrypt_text(const std::string& text, const std::string& password);
std::string encrypt_bytes(const std::string& bytes, const std::string& password);
