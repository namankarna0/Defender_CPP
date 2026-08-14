#include "password.h"

#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;


// ===============================
// PasswordDictionary
// ===============================

PasswordDictionary::PasswordDictionary(const string &filename)
{
    ifstream file(filename);

    if (!file)
    {
        cout << "Warning: Could not open password dictionary.\n";
        return;
    }

    string word;

    size_t position = 0;
    while (getline(file, word))
    {
        if (word.empty()) continue;
        ++position;
        dictionary.insert(word);
        // Keep the earliest rank when a password appears more than once.
        positions.emplace(word, position);
    }

    file.close();
}


bool PasswordDictionary::contains(const string &password) const
{
    return dictionary.find(password) != dictionary.end();
}


std::optional<size_t> PasswordDictionary::positionOf(const string &password) const
{
    const auto found = positions.find(password);
    if (found == positions.end()) return std::nullopt;
    return found->second;
}


size_t PasswordDictionary::size() const
{
    return dictionary.size();
}


// ===============================
// Password
// ===============================

Password::Password(const string &password)
{
    value = password;
}


string Password::getValue() const
{
    return value;
}


size_t Password::length() const
{
    return value.length();
}


// ===============================
// StrengthAnalyzer
// ===============================

StrengthAnalyzer::StrengthAnalyzer(
    const PasswordDictionary &dict
)
    : dictionary(dict)
{
    reset();
}


void StrengthAnalyzer::reset()
{
    uppercase = false;
    lowercase = false;
    digit = false;
    symbol = false;
    repeated = false;
    commonpassword = false;

    score = 0;
}


void StrengthAnalyzer::analyze(const Password &password)
{
    reset();

    string value = password.getValue();

    for (size_t i = 0; i < value.length(); i++)
    {
        unsigned char current =
            static_cast<unsigned char>(value[i]);

        if (isupper(current))
        {
            uppercase = true;
        }
        else if (islower(current))
        {
            lowercase = true;
        }
        else if (isdigit(current))
        {
            digit = true;
        }
        else
        {
            symbol = true;
        }

        for (size_t j = i + 1; j < value.length(); j++)
        {
            if (value[i] == value[j])
            {
                repeated = true;
                break;
            }
        }
    }

    calculateScore(password);
}


void StrengthAnalyzer::calculateScore(
    const Password &password
)
{
    score = 0;

    if (password.length() >= 16)
    {
        score += 6;
    }
    else if (password.length() >= 12)
    {
        score += 4;
    }
    else if (password.length() >= 8)
    {
        score += 2;
    }

    if (uppercase)
        score += 1;

    if (lowercase)
        score += 1;

    if (digit)
        score += 1;

    if (symbol)
        score += 1;

    if (!repeated)
        score += 2;

    if (dictionary.contains(password.getValue()))
    {
        commonpassword = true;
    }
    else
    {
        score += 2;
    }
}


bool StrengthAnalyzer::hasUppercase() const
{
    return uppercase;
}


bool StrengthAnalyzer::hasLowercase() const
{
    return lowercase;
}


bool StrengthAnalyzer::hasDigit() const
{
    return digit;
}


bool StrengthAnalyzer::hasSymbol() const
{
    return symbol;
}


bool StrengthAnalyzer::hasRepeatedCharacters() const
{
    return repeated;
}

bool StrengthAnalyzer::isCommonPassword() const
{
    return commonpassword;
}


int StrengthAnalyzer::getScore() const
{
    return score;
}


string StrengthAnalyzer::getStrength() const
{
    if (score <= 4)
        return "Very Weak";

    if (score <= 7)
        return "Weak";

    if (score <= 10)
        return "Moderate";

    if (score <= 12)
        return "Strong";

    return "Very Strong";
}
