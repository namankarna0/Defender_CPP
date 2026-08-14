#include "password_generator.h"

#include <string>
#include <random>

PasswordGenerator::PasswordGenerator(int length, bool lower, bool upper,
                                     bool digits, bool symbols)
    : length_(length), use_lower_(lower), use_upper_(upper),
      use_digits_(digits), use_symbols_(symbols) {}

std::string PasswordGenerator::generate_password() const {
    std::string characters;
    if (use_lower_) characters += "abcdefghijklmnopqrstuvwxyz";
    if (use_upper_) characters += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (use_digits_) characters += "0123456789";
    if (use_symbols_) characters += "!@#$%^&*()-_=+[]{}<>?/|~";
    if (characters.empty() || length_ < 4 || length_ > 128) return "";

    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution<std::size_t> pick(0, characters.size() - 1);
    std::string password;
    password.reserve(static_cast<std::size_t>(length_));
    for (int i = 0; i < length_; ++i) password += characters[pick(generator)];
    return password;
}
