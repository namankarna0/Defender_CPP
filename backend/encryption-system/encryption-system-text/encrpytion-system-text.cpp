#include "encryption_core.h"
#include "base64.h"

#include <stdexcept>

std::string xor_transform(const std::string& message, const std::string& password) {
    if (password.empty()) {
        throw std::invalid_argument("A password is required.");
    }

    std::string result = message;
    for (std::size_t i = 0; i < result.length(); ++i) {
        result[i] = result[i] ^ password[i % password.length()];
    }
    return result;
}

std::string encrypt_text_with_original_cipher(const std::string& message,
                                              const std::string& password) {
    return base64_encode(xor_transform(message, password));
}
