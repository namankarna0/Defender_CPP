#ifndef PASSWORD_H
#define PASSWORD_H

#include <string>
#include <optional>
#include <unordered_set>
#include <unordered_map>

using namespace std;


class PasswordDictionary
{
private:
     unordered_set< string> dictionary;
     unordered_map<string, size_t> positions;

public:
    PasswordDictionary(const  string &filename);

    bool contains(const  string &password) const;

    // Returns the one-based position in passDictionary.txt when present.
    std::optional<size_t> positionOf(const string &password) const;

    size_t size() const;
};


class Password
{
private:
     string value;

public:
    Password(const  string &password);

     string getValue() const;

    size_t length() const;
};


class PasswordAnalyzer
{
public:
    virtual void analyze(const Password &password) = 0;

    virtual ~PasswordAnalyzer() = default;
};


class StrengthAnalyzer : public PasswordAnalyzer
{
private:
    bool uppercase;
    bool lowercase;
    bool digit;
    bool symbol;
    bool repeated;
    bool commonpassword;

    int score;

    const PasswordDictionary &dictionary;

    void calculateScore(const Password &password);

public:
    StrengthAnalyzer(const PasswordDictionary &dict);

    void reset();

    void analyze(const Password &password) override;

    bool hasUppercase() const;

    bool hasLowercase() const;

    bool hasDigit() const;

    bool hasSymbol() const;

    bool hasRepeatedCharacters() const;

    bool isCommonPassword() const;

    int getScore() const;

     string getStrength() const;
};

#endif
