#pragma once

#include <string>
#include <optional>

struct PasswordAnalysis {
    bool lowercase{};
    bool uppercase{};
    bool digit{};
    bool symbol{};
    int character_pool{};
    bool common_password{};
    std::optional<std::size_t> dictionary_position;
    std::string strength;
    long double combination_log10{};
    std::string total_combinations;
    long double attack_log10_seconds{};
    long double attack_seconds{};
};

PasswordAnalysis analyze_password(const std::string& password,
                                  long double attempts_per_second,
                                  bool common_password = false,
                                  std::optional<std::size_t> dictionary_position = std::nullopt);
