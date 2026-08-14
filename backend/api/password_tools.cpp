#include "api/password_tools.h"
#include "passwordTools/attacking-simulator/password_analysis.h"
#include "passwordTools/random-password-generator/password_generator.h"
#include "passwordTools/password-strength-and-breach-checker/password.h"

#include <algorithm>
#include <filesystem>
#include <sstream>

namespace {
const PasswordDictionary& password_dictionary() {
    static const PasswordDictionary dictionary(
        (std::filesystem::path(__FILE__).parent_path().parent_path() /
         "passwordTools/password-strength-and-breach-checker/passDictionary.txt").string());
    return dictionary;
}
}

// Adapts the existing password-generator module for the HTTP API.
std::string generate_password(int length, bool lower, bool upper, bool numbers, bool symbols)
{
    return PasswordGenerator(length, lower, upper, numbers, symbols).generate_password();
}
// Combines breach, strength, combination, and attack-time data into one JSON response.
std::string password_analysis_json(const std::string &password, double attempts)
{
    const PasswordDictionary& dictionary = password_dictionary();
    const auto position = dictionary.positionOf(password);
    StrengthAnalyzer checker(dictionary);
    checker.analyze(Password(password));
    const PasswordAnalysis analysis = analyze_password(password, attempts,
                                                       checker.isCommonPassword(), position);
    // The original checker score ranges from 0 to 14; convert it for the UI bar.
    const int score = static_cast<int>(checker.getScore() * 100.0 / 14.0 + 0.5);
    std::ostringstream o;
    o << "{\"strength\":\"" << checker.getStrength() << "\",\"score\":" << score
      << ",\"raw_score\":" << checker.getScore() << ",\"length\":" << password.size()
      << ",\"lowercase\":" << (checker.hasLowercase() ? "true" : "false")
      << ",\"uppercase\":" << (checker.hasUppercase() ? "true" : "false")
      << ",\"digit\":" << (checker.hasDigit() ? "true" : "false")
      << ",\"symbol\":" << (checker.hasSymbol() ? "true" : "false")
      << ",\"repeated\":" << (checker.hasRepeatedCharacters() ? "true" : "false")
      << ",\"common_password\":" << (checker.isCommonPassword() ? "true" : "false")
      << ",\"dictionary_position\":";
    if (position) o << *position; else o << "null";
    o << ",\"total_combinations\":\"" << analysis.total_combinations << "\""
      << ",\"attack_log10_seconds\":" << analysis.attack_log10_seconds
      << ",\"attack_seconds\":" << analysis.attack_seconds << '}';
    return o.str();
}
