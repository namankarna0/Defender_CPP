#include "api/encryption.h"
#include "encryption-system/encryption-system-text/encryption_core.h"

std::string encrypt_text(const std::string& text, const std::string& password) {
    return encrypt_text_with_original_cipher(text, password);
}

std::string encrypt_bytes(const std::string& bytes, const std::string& password) {
    return xor_transform(bytes, password);
}
