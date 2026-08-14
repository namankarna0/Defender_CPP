#pragma once
#include <string>
std::string decrypt_text(const std::string &encrypted, const std::string &password);
std::string decrypt_bytes(const std::string &bytes, const std::string &password);
