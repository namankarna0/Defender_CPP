#pragma once

#include <string>

class PasswordGenerator {
public:
    PasswordGenerator(int length, bool lower, bool upper, bool digits, bool symbols);
    std::string generate_password() const;

private:
    int length_;
    bool use_lower_;
    bool use_upper_;
    bool use_digits_;
    bool use_symbols_;
};
