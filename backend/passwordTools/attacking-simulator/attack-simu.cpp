#include "password_analysis.h"

#include <string>
#include <cmath>
#include <iomanip>
#include <sstream>

namespace {
// Formats the theoretical keyspace without overflowing a numeric response.
std::string format_combinations(long double log10_combinations) {
    if (log10_combinations < 18) {
        std::ostringstream output;
        output << std::fixed << std::setprecision(0) << std::pow(10.0L, log10_combinations);
        return output.str();
    }
    std::ostringstream output;
    output << "~" << std::fixed << std::setprecision(2)
           << std::pow(10.0L, log10_combinations - std::floor(log10_combinations))
           << "e+" << static_cast<long long>(std::floor(log10_combinations));
    return output.str();
}
}

// Calculates dictionary or brute-force attack metrics for the supplied password.
PasswordAnalysis analyze_password(const std::string& password,
                                  long double attempts_per_second,
                                  bool common_password,
                                  std::optional<std::size_t> dictionary_position) {
    PasswordAnalysis result;
    for (unsigned char character : password) {
        if (character >= 'a' && character <= 'z') result.lowercase = true;
        else if (character >= 'A' && character <= 'Z') result.uppercase = true;
        else if (character >= '0' && character <= '9') result.digit = true;
        else result.symbol = true;
    }
    result.character_pool = (result.lowercase ? 26 : 0) +
                            (result.uppercase ? 26 : 0) +
                            (result.digit ? 10 : 0) + (result.symbol ? 10 : 0);
    result.common_password = common_password;
    result.dictionary_position = dictionary_position;
    result.combination_log10 = result.character_pool
        ? password.length() * std::log10(static_cast<long double>(result.character_pool)) : 0;
    result.total_combinations = result.character_pool
        ? format_combinations(result.combination_log10) : "0";
    const int types = result.lowercase + result.uppercase + result.digit + result.symbol;
    if (common_password) result.strength = "Very Weak";
    else if (password.length() < 6) result.strength = "Weak";
    else if (password.length() < 8 || types < 3) result.strength = "Moderate";
    else if (password.length() >= 12 && types == 4) result.strength = "Very Strong";
    else result.strength = "Strong";

    if (dictionary_position && attempts_per_second > 0) {
        result.total_combinations = std::to_string(*dictionary_position) + " dictionary attempts";
        result.attack_seconds = static_cast<long double>(*dictionary_position) / attempts_per_second;
        result.attack_log10_seconds = result.attack_seconds > 0 ? std::log10(result.attack_seconds) : 0;
        return result;
    }

    result.attack_log10_seconds = result.character_pool && attempts_per_second > 0
        ? result.combination_log10 - std::log10(attempts_per_second)
        : 0;
    result.attack_seconds = result.attack_log10_seconds < 18
        ? std::pow(10.0L, result.attack_log10_seconds) : -1;
    return result;
}
