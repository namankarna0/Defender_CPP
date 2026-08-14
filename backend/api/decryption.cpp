#include "api/decryption.h"
#include "api/encryption.h"
#include "encryption-system/encryption-system-text/base64.h"

std::string decrypt_bytes(const std::string& bytes, const std::string& password) {
    return encrypt_bytes(bytes, password);
}

std::string decrypt_text(const std::string& encrypted, const std::string& password) {
    return decrypt_bytes(base64_decode(encrypted), password);
}
