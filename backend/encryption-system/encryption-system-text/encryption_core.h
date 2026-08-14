#pragma once

#include <string>

// Reusable form of the original text encryption program's XorCipher logic.
std::string xor_transform(const std::string& message, const std::string& password);
std::string encrypt_text_with_original_cipher(const std::string& message,
                                              const std::string& password);
